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

#import "Track Collection/Crates/Internal/NxACrate.h"
#import "Track Collection/Tags/Crate Properties/NxATrackProperty.h"
#import "Track Collection/Tags/Crate Properties/NxACrateProperty.h"
#import "NxATrack.h"
#import "Track Collection/NxASortingCriteria.h"
#import "NxATrackSortingBucket.h"
#import "NSManagedObjectContext+NxAUtility.h"
#import "NxATrackTitle.h"
#import "NxATrackGenre.h"
#import "NxATrackArtistCredit.h"
#import "NxATrackRemixerCredit.h"
#import "NxATrackProducerCredit.h"
#import "Track Collection/NxAArtist.h"
#import "NxATrackDateAdded.h"
#import "NxATrackDateModified.h"
#import "NxATrackDateReleased.h"
#import "Track Collection/Parsed Values/NxAParsedCrateEntryValue.h"
#import "Track Collection/Parsed Values/NxAParsedTrackEntryValue.h"

#pragma mark Constants

/*! Prefix used for all system crate names. */
NSString *const NxACrateSystemNamePrefix = @"__NxA_";

/*! Name of the crate containing all tracks for the collection. */
NSString *const NxAAllTracksSystemCrateName = @"All Tracks";

/*! Name of the crate containing all the crates for the collection. */
NSString *const NxARootSystemCrateName = @"ROOT";

#pragma mark - Implementation

@implementation NxACrate

#pragma mark Factory Methods

+ (instancetype)p_emptyCrateWithName:(NSString *)name
                           inContext:(NSManagedObjectContext *)context;
{
    NxAAssertNotNil(name);
    NxAAssertNotNil(context);

    NSString *entityName = [self p_entityName];
    NxACrate *newCrate = [context insertNewObjectForEntity:entityName];

    newCrate.p_name = name;
    newCrate.p_type = [NSNumber numberWithInteger:NxACrateIsAPlaylist];

    return newCrate;
}

+ (instancetype)folderWithName:(NSString *)name
 withEmptyContentLastModifiedOn:(NSDate *)modificationDate
                     inContext:(NSManagedObjectContext *)context
{
    NxAAssertNotNil(modificationDate);

    NxACrate *newCrate = [self p_emptyCrateWithName:name
                                          inContext:context];
    newCrate.p_type = [NSNumber numberWithInteger:NxACrateIsAFolder];

    [newCrate p_replaceTagClass:[NxACrateProperty class]
               withParsedValues:@[ [NxAParsedCrateEntryValue parsedValueWithNoCrateEntry] ]
               modificationDate:modificationDate];

    return newCrate;
}

+ (instancetype)playlistWithName:(NSString *)name
  withEmptyContentLastModifiedOn:(NSDate *)modificationDate
                       inContext:(NSManagedObjectContext *)context
{
    NxAAssertNotNil(modificationDate);

    NxACrate *newCrate = [self p_emptyCrateWithName:name
                                          inContext:context];
    newCrate.p_type = [NSNumber numberWithInteger:NxACrateIsAPlaylist];

    [newCrate p_replaceTagClass:[NxATrackProperty class]
               withParsedValues:@[ [NxAParsedTrackEntryValue parsedValueWithNoTrackEntry] ]
               modificationDate:modificationDate];

    return newCrate;
}

+ (instancetype)existingOrNewSystemFolderWithName:(NSString *)name
              ifNewWithEmptyContentLastModifiedOn:(NSDate *)modificationDate
                                        inContext:(NSManagedObjectContext *)context
{
    NxAAssertNotNil(name);
    NxAAssertNotNil(context);

    name = [NSString stringWithFormat:@"%@%@", NxACrateSystemNamePrefix, name];

    NSArray *results;
    // -- Look for an existing crate.
    results = [context fetchFromTemplate:@"CrateWithName"
                   substitutionVariables:@{ @"NAME" : name }];
    if (!results) {
        return nil;
    }

    if (results.count == 0) {
        return [NxACrate folderWithName:name
         withEmptyContentLastModifiedOn:modificationDate
                              inContext:context];
    }

    NxAAssertTrue(results.count == 1);

    NxACrate *result = results[0];
    NxAAssertTrue(result.isAFolder);

    return result;
}

+ (instancetype)existingOrNewSystemPlaylistWithName:(NSString *)name
                ifNewWithEmptyContentLastModifiedOn:(NSDate *)modificationDate
                                          inContext:(NSManagedObjectContext *)context
{
    NxAAssertNotNil(name);
    NxAAssertNotNil(context);

    name = [NSString stringWithFormat:@"%@%@", NxACrateSystemNamePrefix, name];

    NSArray *results;
    // -- Look for an existing crate.
    results = [context fetchFromTemplate:@"CrateWithName"
                   substitutionVariables:@{ @"NAME" : name }];
    if (!results) {
        return nil;
    }

    if (results.count == 0) {
        return [NxACrate playlistWithName:name
           withEmptyContentLastModifiedOn:modificationDate
                                inContext:context];
    }

    NxAAssertTrue(results.count == 1);

    NxACrate *result = results[0];
    NxAAssertTrue(result.isAPlaylist);

    return result;
}

#pragma mark Class Methods

+ (NSMutableSet *)p_tracksMatchingPredicate:(NSString *)predicateFormat
                                    inCrate:(NxACrate *)inCrate
                                    context:(NSManagedObjectContext *)context
{
    NxAAssertTrue(inCrate.isAPlaylist);

    NSPredicate *predicate = [NSPredicate predicateWithFormat:predicateFormat];

    if ([predicate isKindOfClass:[NSCompoundPredicate class]]) {
        NSCompoundPredicate *compoundPredicate = (NSCompoundPredicate *)predicate;

        NSMutableSet *tracks = nil;

        for (NSPredicate *subPredicate in compoundPredicate.subpredicates) {
            NSMutableSet *newTracks = [NxACrate p_tracksMatchingPredicate:subPredicate.predicateFormat
                                                                  inCrate:inCrate
                                                                  context:context];

            if (!tracks) {
                tracks = newTracks;
            }
            else {
                if (compoundPredicate.compoundPredicateType == NSAndPredicateType) {
                    [tracks intersectSet:newTracks];
                }
                else if (compoundPredicate.compoundPredicateType == NSOrPredicateType) {
                    [tracks unionSet:newTracks];
                }
                else {
                    ALog(@"Invalid compound predicate type.");
                }
            }
        }

        return tracks;
    }
    else if ([predicate isKindOfClass:[NSComparisonPredicate class]]) {
        NSMutableSet *tracks = nil;

        if ([predicateFormat hasPrefix:@"Title"]) {
            tracks = [[NxATrackTitle tracksWithTagMatchingPredicate:[predicateFormat substringFromIndex:[@"Title" length]]
                                                            context:context] mutableCopy];
        }
        else if ([predicateFormat hasPrefix:@"Artist"]) {
            tracks = [[NxATrackArtistCredit tracksWithTagMatchingPredicate:[predicateFormat substringFromIndex:[@"Artist" length]]
                                                                   context:context] mutableCopy];
        }
        else if ([predicateFormat hasPrefix:@"Remixer"]) {
            tracks = [[NxATrackRemixerCredit tracksWithTagMatchingPredicate:[predicateFormat substringFromIndex:[@"Remixer" length]]
                                                                    context:context] mutableCopy];
        }
        else if ([predicateFormat hasPrefix:@"Producer"]) {
            tracks = [[NxATrackProducerCredit tracksWithTagMatchingPredicate:[predicateFormat substringFromIndex:[@"Producer" length]]
                                                                     context:context] mutableCopy];
        }
        else if ([predicateFormat hasPrefix:@"Genre"]) {
            tracks = [[NxATrackGenre tracksWithTagMatchingPredicate:[predicateFormat substringFromIndex:[@"Genre" length]]
                                                            context:context] mutableCopy];
        }
        else if ([predicateFormat hasPrefix:@"DateAdded"]) {
            tracks = [[NxATrackDateAdded tracksWithTagMatchingPredicate:[predicateFormat substringFromIndex:[@"DateAdded" length]]
                                                                context:context] mutableCopy];
        }
        else if ([predicateFormat hasPrefix:@"DateModified"]) {
            tracks = [[NxATrackDateModified tracksWithTagMatchingPredicate:[predicateFormat substringFromIndex:[@"DateModified" length]]
                                                                   context:context] mutableCopy];
        }
        else if ([predicateFormat hasPrefix:@"DateReleased"]) {
            tracks = [[NxATrackDateReleased tracksWithTagMatchingPredicate:[predicateFormat substringFromIndex:[@"DateReleased" length]]
                                                                   context:context] mutableCopy];
        }

        if (inCrate && !inCrate.isASystemCrate) {
            NSSet *allTracks = [NSSet setWithArray:inCrate.tracks];
            [tracks intersectSet:allTracks];
        }

        return tracks;
    }
    else {
        ALog(@"Invalid predicate type.");
        return nil;
    }
}

+ (NSString *)p_entityName
{
    return @"Crate";
}

+ (BOOL)canFindARootCrateInContext:(NSManagedObjectContext *)context
{
    NxAAssertNotNil(context);

    NSString *name = [NSString stringWithFormat:@"%@%@", NxACrateSystemNamePrefix, NxARootSystemCrateName];

    NSArray *results = [context fetchFromTemplate:@"CrateWithName"
                            substitutionVariables:@{ @"NAME" : name }];
    return results.count != 0;
}

+ (BOOL)canFindAnAllTracksCrateInContext:(NSManagedObjectContext *)context
{
    NxAAssertNotNil(context);

    NSString *name = [NSString stringWithFormat:@"%@%@", NxACrateSystemNamePrefix, NxAAllTracksSystemCrateName];

    NSArray *results = [context fetchFromTemplate:@"CrateWithName"
                            substitutionVariables:@{ @"NAME" : name }];
    return results.count != 0;
}

+ (NSArray *)cratesFromPasteboard:(NSPasteboard *)pasteboard
                          context:(NSManagedObjectContext *)context
{
    NSArray *urls = [pasteboard readObjectsForClasses:@[ [NSURL class] ] options:nil ];
    NSMutableArray *crates = [NSMutableArray array];

    // -- First we look for core-data URIs to see if we have any track objects on the pasteboard.
    for (NSURL *url in urls) {
        NSString *desc = url.description;

        // -- If this is not a core data URI then it can't be a Track, therefore we skip it.
        if (![desc hasPrefix:@"x-coredata://"]) {
            continue;
        }

        NSManagedObject *object = [context objectWithURI:url];
        if ([object isKindOfClass:[NxACrate class]]) {
            NxACrate *crate = (NxACrate *)object;
            [crates addObject:crate];
        }
    }

    if (!crates.count) {
        return nil;
    }

    return crates;
}

+ (BOOL)isAValidName:(NSString *)name
{
    return ![name hasPrefix:NxACrateSystemNamePrefix];
}

+ (NxACrate *)allTracksCrateIn:(NSManagedObjectContext *)context
{
    return [NxACrate existingOrNewSystemPlaylistWithName:NxAAllTracksSystemCrateName
                     ifNewWithEmptyContentLastModifiedOn:[NSDate distantPast]
                                               inContext:context];
}

+ (NxACrate *)rootCrateIn:(NSManagedObjectContext *)context
{
    return [NxACrate existingOrNewSystemFolderWithName:NxARootSystemCrateName
                   ifNewWithEmptyContentLastModifiedOn:[NSDate distantPast]
                                             inContext:context];
}

+ (NSArray *)allCratesIn:(NSManagedObjectContext *)context
{
    NSArray *allCrates = [context allObjectsWithEntityType:@"Crate" error:nil];
    return allCrates;
}

#pragma mark Accessors

// -- Automatically generated for Core Data properties.
@dynamic p_name;
@dynamic p_type;
@dynamic p_parentCrateProperty;
@dynamic p_sortedBy;
@dynamic p_clones;

// -- Automatically generated for properties.
@synthesize p_sortingBuckets = _p_sortingBuckets;

- (NSArray *)p_sortingBuckets
{
    if (!_p_sortingBuckets) {
        [self updateSmartContent:NxACrateShouldNotUpdateSiblings];
    }

    return _p_sortingBuckets;
}

- (NSArray *)p_tracks
{
    NSArray *trackEntries = [NxATrackProperty tagsIn:self];
    NSArray *tracks = [NxATrackProperty arrayOfTracksFromProperties:trackEntries];
    return tracks;
}

- (NSString *)p_fullName
{
    if (self.hasParentCrate) {
        NxACrate *parentCrate = self.parentCrate;
        if (!parentCrate.isASystemCrate) {
            return [NSString stringWithFormat:@"%@->%@", parentCrate.p_fullName, self.name];
        }
    }

    return self.name;
}

- (NSString *)name
{
    NSString *name = self.p_name;
    NxAAssertNotNil(name);
    return name;
}

- (BOOL)isEmpty
{
    if (self.isAPlaylist) {
        return self.tracks.count == 0;
    }
    else {
        return self.crates.count == 0;
    }
}

- (BOOL)isAPlaylist
{
    return self.p_type.integerValue == NxACrateIsAPlaylist;
}

- (BOOL)isAFolder
{
    return self.p_type.integerValue == NxACrateIsAFolder;
}

- (BOOL)containsFilters
{
    NxAAssertTrue(self.isAPlaylist);
    NSArray *crates = self.filterCrates;
    return crates.count != 0;
}

- (BOOL)isASystemCrate
{
    return [self.p_name hasPrefix:NxACrateSystemNamePrefix];
}

- (BOOL)isASmartPlaylist
{
    return self.p_type.integerValue == NxACrateIsASmartPlaylist;
}

- (BOOL)isAFilter
{
    return self.p_type.integerValue == NxACrateIsAFilterPlaylist;
}

- (BOOL)hasSortingCriterias
{
    NxAAssertTrue(self.isAPlaylist);

    return self.p_sortedBy.count != 0;
}

- (BOOL)hasParentCrate
{
    NxACrateProperty *property = self.p_parentCrateProperty;
    NxACrate *parentCrate = property.parentCrate;
    return parentCrate != nil;
}

- (NxACrate *)parentCrate
{
    NxACrateProperty *property = self.p_parentCrateProperty;
    NxAAssertNotNil(property);

    NxACrate *parentCrate = property.parentCrate;
    NxAAssertNotNil(parentCrate);

    return parentCrate;
}

- (NSArray *)tagTypesForSorting
{
    if (!self.isAPlaylist) {
        return @[ ];
    }

    NSArray *tracks = self.tracks;

    NSMutableDictionary *tagTypes = [NSMutableDictionary dictionary];

    for (NxATrack *track in tracks) {
        NSArray *trackTags = track.tagTypesForSorting;
        for (NxATag *tag in trackTags) {
            NSString *description = tag.propertyDescription;
            NxAAssertNotNil(description);
            tagTypes[description] = tag;
        }
    }

    return tagTypes.allValues;
}

- (NSArray *)tracks
{
    if (self.isAPlaylist) {
        NSArray *tracks = self.p_tracks;
        return tracks;
    }
    else {
        NSMutableSet *results = [NSMutableSet set];

        NSArray *crates = self.crates;
        for (NxACrate *subCrate in crates) {
            if (subCrate.isASmartPlaylist) {
                continue;
            }

            NSArray *containedTracks = subCrate.tracks;
            for (NxATrack *track in containedTracks) {
                [results addObject:track];
            }
        }

        return results.allObjects;
    }
}

- (NSArray *)crates
{
    NxAAssertTrue(self.isAFolder);

    NSArray *crateProperties = [NxACrateProperty tagsIn:self];
    NSArray *crates = [NxACrateProperty arrayOfCratesFromProperties:crateProperties];
    return crates;
}

- (NSArray *)filterCrates
{
    NxAAssertTrue(self.isAPlaylist);

    NSArray *crateProperties = [NxACrateProperty tagsIn:self];
    NSArray *crates = [NxACrateProperty arrayOfCratesFromProperties:crateProperties];
    NSMutableArray *filterCrates = [NSMutableArray arrayWithCapacity:crates.count];
    for (NxACrate *crate in crates) {
        if (crate.isAFilter) {
            [filterCrates addObject:crate];
        }
    }

    return filterCrates;
}

- (NxACrate *)originalCrate
{
    return self;
}

- (NSString *)stringValue
{
    return self.p_name;
}

#pragma mark Instance Methods

- (void)p_updateSortingBuckets
{
    NxAAssertTrue(self.isAPlaylist);

    NSOrderedSet *criterias = self.p_sortedBy;

    if (!criterias.count) {
        self.p_sortingBuckets = nil;

        return;
    }

    NSArray *tracks = self.tracks;
    self.p_sortingBuckets = [NxATrackSortingBucket bucketsWithTracks:tracks
                                                          sortedWith:criterias
                                                             atIndex:0
                                                         parentCrate:self];
}

- (BOOL)p_canReceiveCrate:(NxACrate *)crate
{
    if (crate.hasParentCrate) {
        if ((crate.parentCrate != self) &&
            self.isAFolder &&
            [self hasAChildCrateNamed:crate.name]) {
            return NO;
        }
    }

    if (crate.isAFilter) {
        return self.isAPlaylist;
    }
    else {
        return !self.isAPlaylist;
    }
}

- (void)p_addCrateWithoutUpdatingSmartContent:(NxACrate *)crate
                               lastModifiedOn:(NSDate *)modificationDate
{
    NxAAssertNotNil(crate);
    NxAAssertTrue([self p_canReceiveCrate:crate]);

    NxACrateProperty *newProperty = [NxACrateProperty tagWithValue:crate
                                                           context:self.managedObjectContext];

    BOOL emptyCrate = self.isEmpty;
    BOOL tryingToAddAFilter = crate.isAFilter;
    NxAAssertTrue(emptyCrate ||
                  (!tryingToAddAFilter && self.isAFolder) ||
                  (tryingToAddAFilter && (self.isAPlaylist || self.isAFilter)));

    if (emptyCrate) {
        NSString *trackPropertyTagType = [NxATrackProperty tagType];
        if ([self hasTagsWithType:trackPropertyTagType]) {
            NSArray *crateProperties = [self allTagsWithType:trackPropertyTagType];
            for (NxACrateProperty *property in crateProperties) {
                [property destroy];
            }
        }

        [self p_replaceTagClass:[NxACrateProperty class] withTags:@[ newProperty ] modificationDate:modificationDate];
    }
    else {
        [self addTag:newProperty lastModifiedOn:modificationDate];
    }
}

- (void)destroy
{
    if (self.hasParentCrate) {
        [self.parentCrate removeCrate:self];
    }

    if (self.p_clones.count) {
        NSOrderedSet *clonesCopy = self.p_clones.copy;
        [[self mutableSetValueForKey:@"p_clones"] removeAllObjects];

        for (NxACrate *crate in clonesCopy) {
            [crate destroy];
        }
    }

    if (self.p_sortedBy.count) {
        NSOrderedSet *sortedByCopy = self.p_sortedBy.copy;
        [[self mutableOrderedSetValueForKey:@"p_sortedBy"] removeAllObjects];

        for (NxASortingCriteria *criteria in sortedByCopy) {
            [criteria destroy];
        }
    }

    NSArray *childrenCrates = self.isAFolder ? self.crates : nil;

    [super destroy];

    for (NxACrate *crate in childrenCrates) {
        [crate destroy];
    }
}

- (void)switchTypeTo:(NxACrateType)type
{
    NxACrate *original = self.originalCrate;

    NxAAssertTrue(original.isEmpty);
    NxAAssertTrue(original.isAPlaylist || original.isAFolder);
    NxAAssertTrue((type == NxACrateIsAFolder) || (type == NxACrateIsAPlaylist));

    original.p_type = [NSNumber numberWithInteger:type];
}

- (BOOL)hasAChildCrateNamed:(NSString *)name
{
    NxAAssertTrue(self.isAFolder);

    NSArray *crates = self.crates;
    for (NxACrate *crate in crates) {
        if ([crate.p_name isEqualToString:name]) {
            return YES;
        }
    }

    return NO;
}

- (void) addTrack:(NxATrack *)track
 modificationDate:(NSDate *)modificationDate
{
    NxAAssertTrue(self.isAPlaylist);

    NxAAssertNotNil(track);
    NxAAssertNotNil(modificationDate);

    NxATrackProperty *newProperty = [NxATrackProperty tagWithValue:track
                                                           context:self.managedObjectContext];

    BOOL emptyCrate = self.isEmpty;
    NxAAssertTrue(self.isAPlaylist || emptyCrate);

    if (emptyCrate) {
        NSString *cratePropertyTagType = [NxACrateProperty tagType];
        if ([self hasTagsWithType:[NxACrateProperty tagType]]) {
            NSArray *crateProperties = [self allTagsWithType:cratePropertyTagType];
            for (NxACrateProperty *property in crateProperties) {
                [property destroy];
            }
        }

        [self p_replaceTagClass:[NxATrackProperty class] withTags:@[ newProperty ] modificationDate:modificationDate];
    }
    else {
        [self addTag:newProperty lastModifiedOn:modificationDate];
    }

    // -- Since we added a track, we need to update our sorting buckets.
    [self updateSmartContent:NxACrateShouldUpdateSiblings];
}

- (BOOL)containsTrack:(NxATrack *)track
{
    NxAAssertTrue(self.isAPlaylist);
    NxAAssertNotNil(track);

    NSArray *trackProperties = [NxATrackProperty tagsIn:self];
    NSArray *tracks = [NxATrackProperty arrayOfTracksFromProperties:trackProperties];
    for (NxATrack *existingTrack in tracks) {
        if (track == existingTrack) {
            return YES;
        }
    }

    return NO;
}

- (void)removeTrackProperty:(NxATrackProperty *)property;
{
    NxAAssertTrue(self.isAPlaylist);
    NxAAssertNotNil(property);

    [property destroy];

    // -- We mark all the properties as being modified now.
    NSDate *now = [NSDate date];
    NSArray *entries = [self p_entriesForType:[NxATrackProperty tagType]];
    [NxATagEntry markEntries:entries asModified:now];

    [self updateSmartContent:NxACrateShouldUpdateSiblings];
}

- (void)moveTrackProperties:(NSArray *)propertiesToMove
                 toPosition:(NSInteger)index
{
    NxAAssertTrue(self.isAPlaylist);
    NxAAssertNotNil(propertiesToMove);

    NSString *type = [NxATrackProperty tagType];
    [self p_moveTagType:type tags:propertiesToMove toPosition:index];

    // -- We mark all the properties as being modified now.
    NSDate *now = [NSDate date];
    NSArray *entries = [self p_entriesForType:[NxATrackProperty tagType]];
    [NxATagEntry markEntries:entries asModified:now];
}

- (void) addCrate:(NxACrate *)crate
 modificationDate:(NSDate *)modificationDate
{
    if (self.isEmpty && self.isAPlaylist) {
        [self switchTypeTo:NxACrateIsAFolder];
    }

    NxAAssertTrue(self.isAFolder);
    NxAAssertNotNil(crate);
    NxAAssertNotNil(modificationDate);
    NxAAssertNil(crate.p_parentCrateProperty);

    // -- Add the crate
    [self p_addCrateWithoutUpdatingSmartContent:crate
                                 lastModifiedOn:modificationDate];

    // -- And update any smart content for itself or it siblings.
    //[crate updateSmartContent:NxACrateShouldUpdateSiblings];
}

- (void)removeCrate:(NxACrate *)crate
{
    NxAAssertTrue(self.isAFolder);
    NxAAssertNotNil(crate);

    NxATagEntry *entryToRemove = nil;

    NSArray *entries = [self p_entriesForType:[NxACrateProperty tagType]];
    for (NxATagEntry *entry in entries) {
        NxACrateProperty *property = (NxACrateProperty *)entry.tag;
        if (property.crate == crate) {
            entryToRemove = entry;
            break;
        }
    }

    NxAAssertNotNil(entryToRemove);
    [entryToRemove destroyButPreserveTag];

    crate.p_parentCrateProperty = nil;

    // -- We mark all the properties as being modified now.
    NSDate *now = [NSDate date];
    entries = [self p_entriesForType:[NxACrateProperty tagType]];
    [NxATagEntry markEntries:entries asModified:now];

/*    if (self.hasParentCrate) {
        NxACrate *parentCrate = self.parentCrate;

        // -- Update any smart content for itself or it siblings.
        [parentCrate updateSmartContent:NxACrateShouldNotUpdateSiblings];
    }*/
}

- (NxACrate *)childCrateWithName:(NSString *)name;
{
    NxAAssertTrue(self.isAFolder);
    NxACrate *result = nil;

    NSArray *crates = self.crates;
    for (NxACrate *crate in crates) {
        if ([crate.p_name isEqualToString:name]) {
            result = crate;
            break;
        }
    }

    NxAAssertNotNil(result);
    return result;
}

- (void)moveCrates:(NSArray *)cratesToMove
        toPosition:(NSInteger)index
{
    NxAAssertTrue(self.isAFolder);
    NxAAssertNotNil(cratesToMove);

    NSString *type = [NxACrateProperty tagType];
    NSArray *allProperties = [NxACrateProperty tagsIn:self];
    NSMutableArray *propertiesToMove = [NSMutableArray arrayWithCapacity:cratesToMove.count];
    for (NxACrate *crate in cratesToMove) {
        for (NxACrateProperty *property in allProperties) {
            NxACrate *crateForProperty = property.crate;
            if (crateForProperty == crate) {
                [propertiesToMove addObject:property];
                break;
            }
        }
    }

    NxAAssertTrue(cratesToMove.count == propertiesToMove.count);
    
    [self p_moveTagType:type tags:propertiesToMove toPosition:index];
}

- (NSInteger)indexOfSubCrate:(NxACrate *)crate
{
    NxAAssertTrue(self.isAFolder);
    NxAAssertNotNil(crate);

    NSArray *subCrates = self.crates;
    NSUInteger index = [subCrates indexOfObject:crate];

    return index;
}

- (void)addSortingCriteria:(NxASortingCriteria *)criteria
{
    NxAAssertNotNil(criteria);
    NxAAssertTrue(self.isAPlaylist);

    NSMutableOrderedSet *criterias = [self mutableOrderedSetValueForKey:@"p_sortedBy"];
    [criterias addObject:criteria];

    [self updateSmartContent:NxACrateShouldNotUpdateSiblings];
}

- (void)removeSortingCriteria:(NxASortingCriteria *)criteria
{
    NxAAssertNotNil(criteria);
    NxAAssertTrue(self.isAPlaylist);

    NSMutableOrderedSet *criterias = [self mutableOrderedSetValueForKey:@"p_sortedBy"];
    [criterias removeObject:criteria];

    [self updateSmartContent:NxACrateShouldNotUpdateSiblings];
}

- (BOOL)hasSortingCriteriaForTag:(NxATag *)tag
{
    NxAAssertTrue(self.isAPlaylist);

    return [NxASortingCriteria isCriteriaForTag:tag foundIn:self.p_sortedBy];
}

- (void)updateSmartContent:(NxACrateUpdateSiblingsFlag)updateSiblings
{
    NxAAssertTrue(self.isAPlaylist);

    // -- If we're the main crate we update ourselves.
    [self p_updateSortingBuckets];

    // -- Then we update our clones too.
    for (NxACrate *clone in self.p_clones) {
        [clone p_updateSortingBuckets];
    }

    if (updateSiblings == NxACrateShouldUpdateSiblings) {
        if (!self.hasParentCrate) {
            return;
        }
        
        // -- And if we have siblings, we update them also.
        NxACrate *parentCrate = self.parentCrate;
        if (parentCrate.isAFolder) {
            NSArray *subCrates = parentCrate.crates;
            for (NxACrate *subCrate in subCrates) {
                if (subCrate == self) {
                    continue;
                }

                if (subCrate.isAPlaylist) {
                    [subCrate updateSmartContent:NxACrateShouldNotUpdateSiblings];
                }
            }
        }
    }
}

#pragma mark Overridden NSObject Methods

- (NSString *)description
{
    return self.p_fullName;
}

@end
