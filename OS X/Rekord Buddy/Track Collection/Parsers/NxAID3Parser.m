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
#import "NxATrack+ID3Parsing.h"
#import "NSManagedObjectContext+NxAUtility.h"
#import "NxATrack.h"
#import "Track Collection/Crates/NxACrate.h"
#import "NxATagEntry.h"
#import "NxAID3Parser.h"
#import "NxAProgressViewController.h"
#import "NxAMessageLog.h"
#import "NSError+NxAUtility.h"

#pragma mark Public Interface

/*! Parser for a Traktor collection. */
@interface NxAID3TempParser : NSObject <NxAParserImplementation>

@end

#pragma mark Implementation

@implementation NxAID3TempParser

#pragma mark NxAParserImplementation Protocol

@synthesize parseOnlyToForceUpdateSavedValues;

+ (instancetype)implementationWithContext:(NSManagedObjectContext *)context
                        loggingMessagesIn:(NxAMessageLog *)log
{
    return nil;
}

+ (NSString *)nameOfTheApplication
{
    return @"ID3 Tags";
}

+ (NSString *)versionPrefix
{
    return @"ID3";
}

- (Class)parserClass
{
    return [self class];
}

- (NxAMessageLog *)log {
    return nil;
}

- (id)rootFolderSource {
    return nil;
}

- (id)collectionSource {
    return nil;
}

- (NSDate *)collectionModificationDate
{
    return nil;
}

- (NSDate *)rootCrateModificationDate
{
    return nil;
}

- (BOOL)openCollection;
{
    return NO;
}

- (NSString *)nameOfPlaylistSource:(id)source
{
    return nil;
}

- (NSString *)nameOfFolderEntrySource:(id)source
{
    return nil;
}

- (BOOL)folderEntrySourceIsAPlaylist:(id)source
{
    return NO;
}

- (BOOL)folderEntrySourceIsAFolder:(id)source
{
    return NO;
}

- (NSString *)locationPathForTrackSource:(id)source
{
    return nil;
}

- (void)trackForLocation:(NSString *)location
                      is:(NxATrack *)track
{

}

- (NSDate *)modificationDateForTrackSource:(id)source
{
    return nil;
}

- (NxAParsingResultFlags)parseTrackSource:(id)source
                       whichWasModifiedOn:(NSDate *)modificationDate
                                 forTrack:(NxATrack *)track
{
    return NxAParsedObjectWasUpToDate;
}

- (NxATrack *)trackForPlaylistEntrySource:(id)entry
{
    return nil;
}

- (NSArray *)childrenOfCollectionSource:(id)source
{
    return nil;
}

- (NSArray *)childrenOfPlaylistSource:(id)source
{
    return nil;
}

- (NSArray *)childrenOfFolderSource:(id)source
{
    return nil;
}

- (NSSet *)removeAndReturnValidChildrenFromCollectionSource:(id)source
{
    return nil;
}

- (NSSet *)removeAndReturnValidChildrenFromPlaylistSource:(id)source
{
    return nil;
}

- (NSSet *)removeAndReturnValidChildrenFromFolderSource:(id)source
{
    return nil;
}

- (NSUInteger)numberOfChildrenLeftInCollectionSource:(id)source
{
    return 0;
}

- (NSUInteger)numberOfChildrenLeftInFolderSource:(id)source
{
    return 0;
}

- (void)updateTrackSource:(id)source
                 forTrack:(NxATrack *)track
{

}

- (id)emptyTrackSourceFor:(NxATrack *)track
{
    return nil;
}

- (id)emptyPlaylistSourceFor:(NxACrate *)playlist
{
    return nil;
}

- (id)emptyFolderSourceFor:(NxACrate *)folder
{
    return nil;
}

- (id)playlistEntrySourceFor:(NxATrack *)track
{
    return nil;
}

- (void)addTrackSource:(id)source
             asChildOf:(id)collectionSource
{

}

- (void)addPlaylistEntrySource:(id)source
                     asChildOf:(id)playlistSource
{

}

- (void)addFolderEntrySource:(id)source
                   asChildOf:(id)folderSource
{

}

- (void)setEntriesCountForCollectionSource:(id)source
                                        to:(NSNumber *)count
{

}

- (void)setEntriesCountForPlaylistSource:(id)source
                                      to:(NSNumber *)count
{

}

- (void)setEntriesCountForFolderSource:(id)source
                                    to:(NSNumber *)count
{

}

- (void)deleteCollectionEntrySources:(NSSet *)objects
{

}

- (void)deletePlaylistEntrySources:(NSSet *)objects
{

}

- (void)deleteFolderEntrySources:(NSSet *)objects
{

}

- (void)saveCollection
{

}

@end

#pragma mark Implementation

@implementation NxAID3Parser

#pragma mark Class methods

+ (instancetype)operationWithContext:(NSManagedObjectContext *)context
                            progress:(NxAProgressViewController *)progress
                                 log:(NxAMessageLog *)log
{
    NxAID3Parser *operation = [super operationWithContext:context
                                                 progress:progress
                                                      log:log];
    NxAAssertNotNil(operation);

    id<NxAParserImplementation> implementation = [[NxAID3TempParser alloc] init];
    NxAAssertNotNil(implementation);

    operation.implementation = implementation;

    return operation;
}

#pragma mark Overridden NSOperation methods

- (void)main
{
    NSManagedObjectContext *context = self.context;

    __block BOOL returnCode = NO;

    [context performBlockAndWait:^() {
        @autoreleasepool {
            NxACrate *allTracksCrate = [NxACrate allTracksCrateIn:context];
            NSArray *allTracks = allTracksCrate.tracks;
            if (!allTracks.count) {
                returnCode = YES;
                return;
            }

            NxAProgressViewController *progress = self.progress;
            progress.status = @"Parsing ID3 tags.";
            progress.min = 0;
            progress.max = allTracks.count + 1;
            progress.value = 0;

            for (NxATrack *track in allTracks) {
                ++progress.value;

                NxAParsingResultFlags result = [track parseID3TagsInAudioFileWithParser:self.implementation
                                                                       forceToBeUpdated:self.forceParserToBeUpdatedByEverything];

                if (result & NxAParsedObjectCausedUpdating) {
                    [self hasUpdated:track];
                }

                if (result & NxAParsedObjectNeedsUpdating) {
                }
            }

            [self.log addMessage:[NSString stringWithFormat:@"ID3 parser updated %ld objects and has %ld that needs to be updated.",
                                  self.p_objectsUpdatedStoredInParsingOrder.count,
                                  self.p_objectsToUpdateStoredInParsingOrder.count]];

            returnCode = YES;
        }
    }];

    if (self.completionHandler) {
        self.completionHandler(returnCode);
    }
}

@end
