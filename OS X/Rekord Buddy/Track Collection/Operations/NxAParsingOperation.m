//
//  Copyright (c) 2013-2015 Next Audio Labs, LLC. All rights reserved.
//
//  This file contains confidential and proprietary information of Next
//  Audio Labs, LLC ("Next"). No use is permitted without express written
//  permission of Next. If you are not a party to a Confidentiality/
//  Non-Disclosure Agreement with Next, please immediately delete this
//  file as well as all copies in your possession. For further information,
//  please email info@nextaudiolabs.com.
//

#import "Track Collection/Operations/Internal/NxAParsingOperation.h"
#import "NxATaggedObject+Parsing.h"
#import "NxATrack.h"
#import "NxATrack+NxAWarningContextObject.h"
#import "NxACrate+NxAWarningContextObject.h"
#import "NxACrate+Parsing.h"
#import "NxAMessageLog.h"
#import "Track Collection/Tags/Crate Properties/NxATrackProperty.h"
#import "NxATrackDateAdded.h"
#import "NxATrackDateModified.h"
#import "NxAProgressViewController.h"
#import "Track Collection/AudioFiles/OSX/NxAOSXAudioFile.h"
#import "NxAParsedTrackValue.h"
#import "Track Collection/Parsed Values/NxAParsedTrackEntryValue.h"
#import "Track Collection/Parsed Values/NxAParsedCrateEntryValue.h"
#import "NxATrack+NxAOSXAudioFile.h"

#pragma mark Implementation

@implementation NxAParsingOperation

#pragma mark Factory Methods

+ (instancetype)operationWithContext:(NSManagedObjectContext *)context
                 usingImplementation:(id<NxAParserImplementation>)implementation
                 showingProgressWith:(NxAProgressViewController *)progress
                   loggingMessagesIn:(NxAMessageLog *)log
{
    NxAParsingOperation *operation = [NxAParsingOperation operationWithContext:context
                                                                      progress:progress
                                                                           log:log];
    NxAAssertNotNil(operation);

    operation.implementation = implementation;

    return operation;
}

#pragma mark Accessors

- (NSMutableDictionary *)p_objectSourceByObjectID
{
    if (!_p_objectSourceByObjectID) {
        _p_objectSourceByObjectID = [NSMutableDictionary dictionary];
        NxAAssertNotNil(_p_objectSourceByObjectID);
    }

    return _p_objectSourceByObjectID;
}

- (NSMutableDictionary *)p_objectIDByObjectSource
{
    if (!_p_objectIDByObjectSource) {
        _p_objectIDByObjectSource = [NSMutableDictionary dictionary];
        NxAAssertNotNil(_p_objectIDByObjectSource);
    }

    return _p_objectIDByObjectSource;
}

- (NSMutableOrderedSet *)p_objectsUpdatedStoredInParsingOrder
{
    if (!_p_objectsUpdatedStoredInParsingOrder) {
        _p_objectsUpdatedStoredInParsingOrder = [NSMutableOrderedSet orderedSet];
        NxAAssertNotNil(_p_objectsUpdatedStoredInParsingOrder);
    }

    return _p_objectsUpdatedStoredInParsingOrder;
}

- (NSMutableOrderedSet *)p_objectsToUpdateStoredInParsingOrder
{
    if (!_p_objectsToUpdateStoredInParsingOrder) {
        _p_objectsToUpdateStoredInParsingOrder = [NSMutableOrderedSet orderedSet];
        NxAAssertNotNil(_p_objectsToUpdateStoredInParsingOrder);
    }

    return _p_objectsToUpdateStoredInParsingOrder;
}

- (id<NxAParserImplementation>)implementation
{
    NxAAssertNotNil(_implementation);
    return _implementation;
}

- (BOOL)hasChangesToSave
{
    return self.p_objectsToUpdateStoredInParsingOrder.count != 0;
}

- (BOOL)hasUpdatedInternalDatabase
{
    return self.p_objectsUpdatedStoredInParsingOrder.count != 0;
}

#pragma mark Instance Methods

- (NxAParsingResultFlags)p_parseSourcePlaylist:(id)playlist
                                      forCrate:(NxACrate *)crate
 andReplaceContentIfDifferentThanPreviouslyParsedAndOlderThan:(NSDate *)date
{
    id<NxAParserImplementation> parser = self.implementation;

    NxAAssertNotNil(playlist);
    NxAAssertTrue(crate.isAPlaylist);

    if (![parser parseOnlyToForceUpdateSavedValues]) {
        [self sourceFor:crate is:playlist];
    }
    
    NxAParsingResultFlags result = NxAParsedObjectWasUpToDate;

#if DEBUG
    NSString *message = [NSString stringWithFormat:@"Playlist '%@'.", crate.description];
#endif

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result, [crate replaceTracksIfOlderThan:date
                                                  andParsedTrackValuesReturnedBy:^() {
                                                      NSArray *children = [parser childrenOfPlaylistSource:playlist];
                                                      NSMutableArray *newValues = [NSMutableArray arrayWithCapacity:children.count];

                                                      for (id entry in children) {
                                                          NxATrack *track = [self.implementation trackForPlaylistEntrySource:entry];
                                                          if (!track) {
                                                              continue;
                                                          }

                                                          NxAParsedTrackEntryValue *newValue = [NxAParsedTrackEntryValue parsedValueWithTrack:track];
                                                          [newValues addObject:newValue];
                                                      }

                                                      if (!newValues.count) {
                                                          [newValues addObject:[NxAParsedTrackEntryValue parsedValueWithNoTrackEntry]];
                                                      }
                                                      
                                                      return newValues;
                                                  }
                                            areDifferentThanPreviouslyParsedWith:parser
                                           thenCallThisEveryTimeATrackIsReplaced:nil],
                                         self.log, message);

    if (![parser parseOnlyToForceUpdateSavedValues]) {
        if (result == NxAParsedObjectCausedUpdating) {
            [self hasUpdated:crate];
        }

        if (result == NxAParsedObjectNeedsUpdating) {
            [self needsToUpdateSourceOf:crate];
        }
    }

    return result;
}

- (NxAParsingResultFlags)p_parseSourceFolder:(id)folder
                                    forCrate:(NxACrate *)crate
 andReplaceContentIfDifferentThanPreviouslyParsedAndOlderThan:(NSDate *)date
{
    id<NxAParserImplementation> parser = self.implementation;

    NxAAssertNotNil(folder);
    NxAAssertTrue(crate.isAFolder);

    NSArray *children = [parser childrenOfFolderSource:folder];

    if (![parser parseOnlyToForceUpdateSavedValues]) {
        [self sourceFor:crate is:folder];

        // -- In case we do need to do this, we save out spot in the list so that it's sorted in parsing order.
        [self hasUpdated:crate];
        [self needsToUpdateSourceOf:crate];

        for (id entry in children) {
            NSString *name = [parser nameOfFolderEntrySource:entry];
            if (!name.length) {
                // -- TODO: Warning message
                continue;
            }

            if ([crate hasAChildCrateNamed:name]) {
                [self parseSourceFolderEntry:entry
                              forParentCrate:crate
                andReplaceContentIfDifferentThanPreviouslyParsedAndOlderThan:date];
            }
        }
    }

    NxAParsingResultFlags result = NxAParsedObjectWasUpToDate;

#if DEBUG
    NSString *message = [NSString stringWithFormat:@"Folder '%@'.", crate.description];
#endif

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result, [crate replaceCratesIfOlderThan:date
                                                  andParsedCrateValuesReturnedBy:^() {
                                                      NSMutableArray *newValues = [NSMutableArray arrayWithCapacity:children.count];

                                                      for (id entry in children) {
                                                          NSString *name = [parser nameOfFolderEntrySource:entry];
                                                          if (!name.length) {
                                                              continue;
                                                          }

                                                          NxAParsedCrateEntryValue *newValue = [NxAParsedCrateEntryValue parsedValueWithSourceCrateEntry:entry
                                                                                                                                      whichWasModifiedOn:date
                                                                                                                                               hasParent:crate
                                                                                                                                      andCanBeParsedWith:self];
                                                          [newValues addObject:newValue];
                                                      }

                                                      if (!newValues.count) {
                                                          [newValues addObject:[NxAParsedCrateEntryValue parsedValueWithNoCrateEntry]];
                                                      }

                                                      return newValues;
                                                  }
                                            areDifferentThanPreviouslyParsedWith:parser
                                           thenCallThisEveryTimeATrackIsReplaced:nil],
                                         self.log, message);

    if (![parser parseOnlyToForceUpdateSavedValues]) {
        if (!(result & NxAParsedObjectCausedUpdating)) {
            [self hasNotUpdated:crate];
        }

        if (!(result & NxAParsedObjectNeedsUpdating)) {
            [self doesNotNeedToUpdateSourceOf:crate];
        }
    }

    return result;
}

- (void)p_updateCollectionSourceFor:(NxACrate *)crate
{
    id<NxAParserImplementation> implementation = self.implementation;

    id collectionSource = [self sourceFor:crate];

    NSMutableSet *existingChildrenToDelete = [[implementation removeAndReturnValidChildrenFromCollectionSource:collectionSource] mutableCopy];

    NSUInteger count = [implementation numberOfChildrenLeftInCollectionSource:collectionSource];

    NSArray *tracks = crate.tracks;
    for (NxATrack *track in tracks) {
        id trackSource = nil;

        if ([self hasSourceFor:track]) {
            trackSource = [self sourceFor:track];
            [existingChildrenToDelete removeObject:trackSource];
        }
        else {
            trackSource = [implementation emptyTrackSourceFor:track];
            [self sourceFor:track is:trackSource];

            [self p_updateSourceFor:track];
        }
        NxAAssertNotNil(trackSource);

        [implementation addTrackSource:trackSource asChildOf:collectionSource];
        ++count;
    }

    [implementation setEntriesCountForCollectionSource:collectionSource to:@(count)];

    [implementation deleteCollectionEntrySources:existingChildrenToDelete];
}

- (void)p_updatePlaylistSourceFor:(NxACrate *)crate
{
    id<NxAParserImplementation> implementation = self.implementation;

    if (![self hasSourceFor:crate]) {
        NxACrate *parentCrate = crate.parentCrate;
        id parentCrateSource = [self sourceFor:parentCrate];
        id newCrateSource = [implementation emptyPlaylistSourceFor:crate];
        [self sourceFor:crate is:newCrateSource];
        [implementation addPlaylistEntrySource:newCrateSource asChildOf:parentCrateSource];
    }

    id playlistSource = [self sourceFor:crate];
    NxAAssertTrue([[implementation nameOfPlaylistSource:playlistSource] isEqualToString:crate.name]);
    NxAAssertTrue([implementation folderEntrySourceIsAPlaylist:playlistSource]);

    NSMutableSet *existingChildrenToDelete = [[implementation removeAndReturnValidChildrenFromPlaylistSource:playlistSource] mutableCopy];
    NSMutableDictionary *existingChildrenByFilePath = [NSMutableDictionary dictionaryWithCapacity:existingChildrenToDelete.count];
    for (id children in existingChildrenToDelete) {
        NxATrack *track = [implementation trackForPlaylistEntrySource:children];
        if (track) {
            existingChildrenByFilePath[track.fileForCurrentPlatform.path] = children;
        }
    }

    NSUInteger count = 0;

    [self.log startWarningContextWith:crate];

    NSArray *tracks = crate.tracks;
    for (NxATrack *track in tracks) {
        id playlistEntrySource = existingChildrenByFilePath[track.fileForCurrentPlatform.path];
        if (!playlistEntrySource) {
            playlistEntrySource = [implementation playlistEntrySourceFor:track];
        }
        else {
            [existingChildrenToDelete removeObject:playlistEntrySource];
        }

        if (playlistEntrySource) {
            [implementation addPlaylistEntrySource:playlistEntrySource asChildOf:playlistSource];
            ++count;
        }
    }

    [self.log endWarningContext];

    [implementation setEntriesCountForPlaylistSource:playlistSource to:@(count)];

    [implementation deletePlaylistEntrySources:existingChildrenToDelete];

    [implementation setParseOnlyToForceUpdateSavedValues:YES];
    NxAParsingResultFlags result = [self p_parseSourcePlaylist:playlistSource forCrate:crate andReplaceContentIfDifferentThanPreviouslyParsedAndOlderThan:[NSDate date]];
    NxAAssertTrue(result == NxAParsedObjectWasUpToDate);
    [implementation setParseOnlyToForceUpdateSavedValues:NO];
}

- (void)p_updateFolderSourceFor:(NxACrate *)crate
{
    id<NxAParserImplementation> implementation = self.implementation;

    id folderSource = [self sourceFor:crate];
    if (!crate.isASystemCrate) {
        NxAAssertTrue([[implementation nameOfFolderEntrySource:folderSource] isEqualToString:crate.name]);
    }
    NxAAssertTrue([implementation folderEntrySourceIsAFolder:folderSource]);

    NSMutableSet *existingChildrenToDelete = [[implementation removeAndReturnValidChildrenFromFolderSource:folderSource] mutableCopy];

    NSUInteger count = [implementation numberOfChildrenLeftInFolderSource:folderSource];

    NSArray *crates = crate.crates;
    for (NxACrate *childCrate in crates) {
        id childCrateSource = nil;

        if ([self hasSourceFor:childCrate]) {
            childCrateSource = [self sourceFor:childCrate];
            [existingChildrenToDelete removeObject:childCrateSource];
        }
        else {
            if (childCrate.isAPlaylist) {
                childCrateSource = [implementation emptyPlaylistSourceFor:childCrate];
            }
            else {
                childCrateSource = [implementation emptyFolderSourceFor:childCrate];
            }

            [self sourceFor:childCrate is:childCrateSource];

            if (![self.p_objectsToUpdateStoredInParsingOrder containsObject:childCrate]) {
                [self p_updateSourceFor:childCrate];
            }
        }

        if (childCrateSource) {
            [implementation addFolderEntrySource:childCrateSource asChildOf:folderSource];

            ++count;
        }
    }

    [implementation setEntriesCountForFolderSource:folderSource to:@(count)];

    [implementation deleteFolderEntrySources:existingChildrenToDelete];

    [implementation setParseOnlyToForceUpdateSavedValues:YES];
    NxAParsingResultFlags result = [self p_parseSourceFolder:folderSource forCrate:crate andReplaceContentIfDifferentThanPreviouslyParsedAndOlderThan:[NSDate date]];
    NxAAssertTrue(result == NxAParsedObjectWasUpToDate);
    [implementation setParseOnlyToForceUpdateSavedValues:NO];
}

- (void)p_parseTracksFromCollectionSource:(id)collectionSource
             andIfTheyAreNewerUpdateCrate:(NxACrate *)crate
{
    id<NxAParserImplementation> implementation = self.implementation;

    if (![implementation parseOnlyToForceUpdateSavedValues]) {
        [self sourceFor:crate is:collectionSource];
    }

    NSArray *trackSources = [implementation childrenOfCollectionSource:collectionSource];

    NxAProgressViewController *progress = self.progress;
    progress.status = @"Parsing Tracks...";
    progress.min = 0;
    progress.max = trackSources.count + 1;
    progress.value = 0;

    NSDate *collectionModificationDate = self.forceParserToBeUpdatedByEverything ? [NSDate distantPast] : implementation.collectionModificationDate;
    NxAParsingResultFlags crateParsingResult = [crate replaceTracksIfOlderThan:collectionModificationDate
                                                andParsedTrackValuesReturnedBy:^() {
                                                    NSMutableArray *newValues = [NSMutableArray arrayWithCapacity:trackSources.count];

                                                    for (id trackSource in trackSources) {
                                                        NSString *path = [self.implementation locationPathForTrackSource:trackSource];
                                                        if (!path) {
                                                            continue;
                                                        }

                                                        NxAParsedTrackValue *newValue = [NxAParsedTrackValue parsedValueWithSourceTrack:trackSource
                                                                                                                   whichHasTheLocalPath:path
                                                                                                                     andCanBeParsedWith:self.implementation
                                                                                                                    andParsingOperation:self
                                                                                                                      loggingMessagesIn:self.log];
                                                        [newValues addObject:newValue];
                                                    }

                                                    if (!newValues.count) {
                                                        [newValues addObject:[NxAParsedTrackEntryValue parsedValueWithNoTrackEntry]];
                                                    }

                                                    return newValues;
                                                }
                                          areDifferentThanPreviouslyParsedWith:self.implementation
                                         thenCallThisEveryTimeATrackIsReplaced:^() {
                                             progress.value += 1;
                                         }];

    if (![implementation parseOnlyToForceUpdateSavedValues]) {
        if (crateParsingResult == NxAParsedObjectNeedsUpdating) {
            [self needsToUpdateSourceOf:crate];
        }

        if (crateParsingResult == NxAParsedObjectCausedUpdating) {
            [self hasUpdated:crate];
        }
        else {
            // -- If our list of tracks is newer, we still parse any tracks that are both in our collection and the parsed collection
            // -- in case some of the track data itself is newer in the parsed collection.
            NSMutableDictionary *trackSourcesForPaths = [NSMutableDictionary dictionaryWithCapacity:trackSources.count];
            for (id source in trackSources) {
                NSString *path = [implementation locationPathForTrackSource:source];
                if (path) {
                    trackSourcesForPaths[path] = source;
                }
            }

            NSArray *tracks = crate.tracks;

            for (NxATrack *track in tracks) {
                progress.value += 1;

                NSString *path = track.fileForCurrentPlatform.path;
                id trackSource = trackSourcesForPaths[path];
                if (trackSource) {
                    progress.value += 1;

                    [implementation trackForLocation:path is:track];
                    
                    [self parseTrackSource:trackSource forTrack:track];
                }
            }
        }
    }
}

- (void)p_updateSourceFor:(NxATaggedObject *)object
{
    id<NxAParserImplementation> implementation = self.implementation;
    NSString *nameOfTheapplication = [implementation.parserClass nameOfTheApplication];

    if ([object isKindOfClass:[NxATrack class]]) {
        NxATrack *track = (NxATrack *)object;

        id source = [self sourceFor:track];
        NxAAssertNotNil(source);

        [self.log addMessage:[NSString stringWithFormat:@"Updating %@ track '%@'.", nameOfTheapplication, track]];

        [implementation updateTrackSource:source forTrack:track];

        [implementation setParseOnlyToForceUpdateSavedValues:YES];
        NxAParsingResultFlags result = [implementation parseTrackSource:source whichWasModifiedOn:[NSDate date] forTrack:track];
        NxAAssertTrue(result == NxAParsedObjectWasUpToDate);
        [implementation setParseOnlyToForceUpdateSavedValues:NO];
    }
    else if ([object isKindOfClass:[NxACrate class]]) {
        NxACrate *crate = (NxACrate *)object;
        if (crate.isAPlaylist) {
            if (crate.isASystemCrate) {
                [self.log addMessage:[NSString stringWithFormat:@"Updating %@ collection in '%@'.", nameOfTheapplication, crate]];
                [self p_updateCollectionSourceFor:crate];
            }
            else {
                [self.log addMessage:[NSString stringWithFormat:@"Updating %@ playlist '%@'.", nameOfTheapplication, crate]];
                [self p_updatePlaylistSourceFor:crate];
            }
        }
        else {
            [self.log addMessage:[NSString stringWithFormat:@"Updating %@ folder '%@'.", nameOfTheapplication, crate]];
            [self p_updateFolderSourceFor:crate];
        }
    }
    else {
        ALog(@"Unknown object to update.");
    }
}

- (instancetype)init
{
    ALog(@"We only create this type of objects via our factory methods.");
    return nil;
}

- (void)sourceFor:(NxATaggedObject *)object
               is:(id)source;
{
    NxAAssertNotNil(source);
    NxAAssertNotNil(object);

    NSManagedObjectID *objectID = object.objectID;
    NxAAssertNilDebug(self.p_objectSourceByObjectID[objectID]);

    self.p_objectSourceByObjectID[objectID] = source;
    self.p_objectIDByObjectSource[source] = objectID;
}

- (BOOL)hasSourceFor:(NxATaggedObject *)object
{
    NxAAssertNotNil(object);
    return (self.p_objectSourceByObjectID[object.objectID] != nil);
}

- (id)sourceFor:(NxATaggedObject *)object
{
    NxAAssertNotNil(object);
    id source = self.p_objectSourceByObjectID[object.objectID];
    NxAAssertNotNil(source);

    return source;
}

- (BOOL)hasAlreadyParsedSource:(id)source
{
    NxAAssertNotNil(source);
    return (self.p_objectIDByObjectSource[source] != nil);
}

- (NxATaggedObject *)objectForSource:(id)source
{
    NxAAssertNotNil(source);
    NSManagedObjectID *objectID = self.p_objectIDByObjectSource[source];
    NxAAssertNotNil(objectID);

    NSManagedObject *object = [self.context objectWithID:objectID];
    NxAAssertTrue([object isKindOfClass:[NxATaggedObject class]]);

    return (NxATaggedObject *)object;
}

- (void)hasNotUpdated:(NxATaggedObject *)object
{
    NxAAssertNotNil(object);
    NxAAssertTrueDebug([self.p_objectsUpdatedStoredInParsingOrder containsObject:object]);

    [self.p_objectsUpdatedStoredInParsingOrder removeObject:object];
}

- (void)hasUpdated:(NxATaggedObject *)object
{
    NxAAssertNotNil(object);
    NxAAssertFalseDebug([self.p_objectsUpdatedStoredInParsingOrder containsObject:object]);

    [self.p_objectsUpdatedStoredInParsingOrder addObject:object];
}

- (void)doesNotNeedToUpdateSourceOf:(NxATaggedObject *)object
{
    NxAAssertNotNil(object);
    NxAAssertTrueDebug([self.p_objectsToUpdateStoredInParsingOrder containsObject:object]);

    [self.p_objectsToUpdateStoredInParsingOrder removeObject:object];
}

- (void)needsToUpdateSourceOf:(NxATaggedObject *)object
{
    NxAAssertNotNil(object);
    NxAAssertFalseDebug([self.p_objectsToUpdateStoredInParsingOrder containsObject:object]);

    [self.p_objectsToUpdateStoredInParsingOrder addObject:object];
}

- (void)needsToUpdateSourceOfObjectsUpdatedIn:(NxAParsingOperation *)parsingOperation
{
    NxAAssertNotNil(parsingOperation);

#if DEBUG
    for (NSObject* object in parsingOperation.p_objectsUpdatedStoredInParsingOrder) {
        [self.log addMessage:[NSString stringWithFormat:@"%@ needs to update %@",
                              [self.implementation.parserClass nameOfTheApplication],
                              object.description]];
    }
#endif

    [self.p_objectsToUpdateStoredInParsingOrder unionOrderedSet:parsingOperation.p_objectsUpdatedStoredInParsingOrder];
}

- (void)setAnyMissingDateAddedOnUpdatedTracksTo:(NSDate *)date
{
    NSOrderedSet *objectsUpdated = self.p_objectsUpdatedStoredInParsingOrder;
    for (id object in objectsUpdated) {
        if (![object isKindOfClass:[NxATrack class]]) {
            // -- If this is not a track we move on.
            continue;
        }

        NxATrack *track = object;
        if (![track hasNonNilTagsWithType:[NxATrackDateAdded tagType]]) {
            NxATag *newTag = [NxATrackDateAdded tagWithValue:date context:self.context];
            [track addTag:newTag lastModifiedOn:date];

            // -- We've changed something in this track so we mark it as needing an update.
            [self needsToUpdateSourceOf:track];
        }
    }
}

- (void)parseTrackSource:(id)source
                forTrack:(NxATrack *)track
{
    NxAAssertNotNil(source);
    NxAAssertNotNil(track);

    [self sourceFor:track is:source];

#if DEBUG
    [self.log addMessage:[NSString stringWithFormat:@"Parsing track '%@' in %@.", track.description, [self.implementation.parserClass nameOfTheApplication]]];
#endif

    // -- Let the warning system know this is the current track.
    [self.log startWarningContextWith:track];

    NSDate *trackModificationDate = self.forceParserToBeUpdatedByEverything ? [NSDate distantPast] : [self.implementation modificationDateForTrackSource:source];

    NxAParsingResultFlags result = [self.implementation parseTrackSource:source
                                                      whichWasModifiedOn:trackModificationDate
                                                                forTrack:track];

    if (result & NxAParsedObjectCausedUpdating) {
        if (![track hasNonNilTagsWithType:[NxATrackDateModified tagType]] ||
            ([track.dateModified laterDate:trackModificationDate] == trackModificationDate)) {
            [track markAsModifiedOn:trackModificationDate];
        }

        [self hasUpdated:track];
    }

    if (result & NxAParsedObjectNeedsUpdating) {
        [self needsToUpdateSourceOf:track];
    }

    [self.log endWarningContext];
}

- (void)parseSourceFolderEntry:(id)entry
                forParentCrate:(NxACrate *)crate
 andReplaceContentIfDifferentThanPreviouslyParsedAndOlderThan:(NSDate *)date
{
    id<NxAParserImplementation> parser = self.implementation;

    NxAAssertNotNil(crate);
    NxAAssertTrue(crate.isAFolder);

    NSString *name = [parser nameOfFolderEntrySource:entry];
    NxAAssertFalse(name.length == 0);

    BOOL isAPlaylist = [parser folderEntrySourceIsAPlaylist:entry];
    BOOL isAFolder = [parser folderEntrySourceIsAFolder:entry];

    NxACrate *childCrate;
    if (![crate hasAChildCrateNamed:name]) {
        NSDate *pastDate = [NSDate distantPast];
        if (isAFolder) {
            childCrate = [NxACrate folderWithName:name withEmptyContentLastModifiedOn:pastDate inContext:self.context];
        }
        else {
            childCrate = [NxACrate playlistWithName:name withEmptyContentLastModifiedOn:pastDate inContext:self.context];
        }
    }
    else {
        childCrate = [crate childCrateWithName:name];
    }

    if (isAFolder && isAPlaylist) {
        [self.log startWarningContextWith:childCrate];

        NxAWarning *warning = [NxAWarning warningWithDescription:[NSString stringWithFormat:NSLocalizedString(@"in %@ that contains both tracks and crates. This is not supported by other programs and the tracks will be ignored.",
                                                                                                              @""),
                                                                  [parser.parserClass nameOfTheApplication]]];
        [self.log addWarning:warning inContextWithPerObjectWarningInfo:@""];

        [self.log endWarningContext];
    }
    
    if (isAFolder) {
        [self p_parseSourceFolder:entry
                         forCrate:childCrate
            andReplaceContentIfDifferentThanPreviouslyParsedAndOlderThan:date];
    }
    else {
        [self p_parseSourcePlaylist:entry
                           forCrate:childCrate
            andReplaceContentIfDifferentThanPreviouslyParsedAndOlderThan:date];
    }
}

- (void)updateCollection
{
    if ((self.p_objectsToUpdateStoredInParsingOrder.count == 0) &&
        (self.p_objectsUpdatedStoredInParsingOrder.count == 0)){
        return;
    }

    if (self.p_objectsToUpdateStoredInParsingOrder.count) {
        NSMutableOrderedSet *objectsToUpdate = [self.p_objectsToUpdateStoredInParsingOrder mutableCopy];

        // -- If needed, we update the all tracks crate first to create new source
        // -- entries needed by any new tracks.
        NxACrate *allTracksCrate = [NxACrate allTracksCrateIn:self.context];
        if ([objectsToUpdate containsObject:allTracksCrate]) {
            [self p_updateSourceFor:allTracksCrate];
            [objectsToUpdate removeObject:allTracksCrate];
        }

        // -- Then we update all the tracks.
        NSMutableArray *tracksUpdated = [NSMutableArray array];
        for (NxATaggedObject *object in objectsToUpdate) {
            if (![object isKindOfClass:[NxATrack class]]) {
                continue;
            }

            [self p_updateSourceFor:object];

            [tracksUpdated addObject:object];
        }

        [objectsToUpdate removeObjectsInArray:tracksUpdated];

        // -- Then we update everything else (folders and playlists in parsing order).
        for (NxATaggedObject *object in objectsToUpdate) {
            [self p_updateSourceFor:object];
        }
    }
}

- (void)saveCollection
{
    [self.implementation saveCollection];
}

#pragma mark Overridden NSOperation methods

- (void)main
{
    __block BOOL returnCode = NO;

    NSManagedObjectContext *context = self.context;
    [context performBlockAndWait:^() {
        @autoreleasepool {
            id<NxAParserImplementation> implementation = self.implementation;
            if (![implementation openCollection]) {
                return;
            }

            NxACrate *allTracksCrate = [NxACrate allTracksCrateIn:self.context];
            NxAAssertNotNil(allTracksCrate);

            id collectionSource = implementation.collectionSource;
            [self p_parseTracksFromCollectionSource:collectionSource andIfTheyAreNewerUpdateCrate:allTracksCrate];

            NSString *nameOfTheApplication = [implementation.parserClass nameOfTheApplication];
#if DEBUG
            [self.log addMessage:[NSString stringWithFormat:@"Parsing Folders and Playlists in %@", nameOfTheApplication]];
#endif

            NxACrate *rootCrate = [NxACrate rootCrateIn:context];
            NxAAssertNotNil(rootCrate);

            id rootFolder = implementation.rootFolderSource;
            NSDate *rootFolderModificationDate = self.forceParserToBeUpdatedByEverything ? [NSDate distantPast] : implementation.rootCrateModificationDate;

            [self p_parseSourceFolder:rootFolder
                             forCrate:rootCrate
                andReplaceContentIfDifferentThanPreviouslyParsedAndOlderThan:rootFolderModificationDate];

            [self.log addMessage:[NSString stringWithFormat:@"%@ parser caused %ld object(s) to be updated and has %ld that need updating.",
                                  nameOfTheApplication,
                                  self.p_objectsUpdatedStoredInParsingOrder.count,
                                  self.p_objectsToUpdateStoredInParsingOrder.count]];
            for (id object in self.p_objectsUpdatedStoredInParsingOrder) {
                [self.log addMessage:[NSString stringWithFormat:@"   Updated '%@' from %@.", object, nameOfTheApplication]];
            }
            for (id object in self.p_objectsToUpdateStoredInParsingOrder) {
                [self.log addMessage:[NSString stringWithFormat:@"   Need to update '%@' in %@.", object, nameOfTheApplication]];
            }

            // -- Make sure we don't have any tag entries left dangling after parsing.
            [NxATagEntry destroyOrphansIn:context andAddAnyMessagesToLog:self.log];

            returnCode = YES;
        }
    }];

    if (self.completionHandler) {
        self.completionHandler(returnCode);
    }
}

@end
