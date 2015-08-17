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

#import "NxACrateClone.h"
#import "Track Collection/NxASortingCriteria.h"
#import "Track Collection/Crates/Internal/NxACrateClone.h"

#pragma mark Implementation

@implementation NxACrateClone

#pragma mark Factory Methods

+ (instancetype)crateAsACloneOf:(NxACrate *)crate
{
    NxAAssertNotNil(crate);

    if ([crate isKindOfClass:[NxACrateClone class]]) {
        NxACrate *clone = [(NxACrateClone *)crate p_cloneOf];
        return [self crateAsACloneOf:clone];
    }

    NSString *crateName = crate.isASystemCrate ? [crate.p_name substringFromIndex:[NxACrateSystemNamePrefix length]] : crate.p_name;

    // -- First we create a new crate with a descrptive name.
    NxACrateClone *newCrate = [self p_emptyCrateWithName:[NSString stringWithFormat:@"Clone of %@", crateName]
                                               inContext:crate.managedObjectContext];
    NxAAssertNotNil(newCrate);

    // -- We set the new crate to be a clone of this one.
    newCrate.p_cloneOf = crate;
    newCrate.p_type = [NSNumber numberWithInteger:NxACrateIsAClone];

    return newCrate;
}

#pragma mark Overridden NxACrate Class Methods

+ (NSString *)p_entityName
{
    return @"CrateClone";
}

#pragma mark Accessors

// -- Automatically generated for Core Data properties.
@dynamic p_cloneOf;

#pragma mark Overridden NxACrate Accessors

- (NSArray *)p_tracks
{
    return self.p_cloneOf.tracks;
}

- (BOOL)p_canReceiveCrate:(NxACrate *)crate
{
    return [self.p_cloneOf p_canReceiveCrate:crate];
}

- (BOOL)isAPlaylist
{
    return self.p_cloneOf.isAPlaylist;
}

- (BOOL)isAFolder
{
    if (self.p_cloneOf.containsFilters) {
        return [super isAFolder];
    }
    else {
        return self.p_cloneOf.isAFolder;
    }
}

- (BOOL)isASystemCrate
{
    return self.p_cloneOf.isASystemCrate;
}

- (BOOL)isASmartPlaylist
{
    return self.p_cloneOf.isASmartPlaylist;
}

- (NSArray *)tracks
{
    return self.p_cloneOf.tracks;
}

- (NSArray *)crates
{
    if (self.p_cloneOf.containsFilters) {
        return [super crates];
    }
    else {
        return self.p_cloneOf.crates;
    }
}

- (NxACrate *)originalCrate
{
    return self.p_cloneOf;
}

#pragma mark Overridden NxACrate Instance Methods

- (void)p_addCrateWithoutUpdatingSmartContent:(NxACrate *)crate
                               lastModifiedOn:(NSDate *)modificationDate
{
    if (self.p_cloneOf.containsFilters) {
        [super p_addCrateWithoutUpdatingSmartContent:crate
                                      lastModifiedOn:modificationDate];
    }
    else {
        [self.p_cloneOf p_addCrateWithoutUpdatingSmartContent:crate
                                               lastModifiedOn:modificationDate];
    }
}

- (void)addCrate:(NxACrate *)crate
    lastModifiedOn:(NSDate *)modificationDate
{
    if (self.p_cloneOf.containsFilters) {
        [super addCrate:crate modificationDate:modificationDate];
    }
    else {
        [self.p_cloneOf addCrate:crate modificationDate:modificationDate];
    }
}

- (void) addTrack:(NxATrack *)track
 modificationDate:(NSDate *)modificationDate
{
    [self.p_cloneOf addTrack:track modificationDate:modificationDate];
}

- (BOOL)containsTrack:(NxATrack *)track
{
    return [self.p_cloneOf containsTrack:track];
}

- (void)removeTrackProperty:(NxATrackProperty *)property;
{
    [self.p_cloneOf removeTrackProperty:property];
}

- (void)moveTrackProperties:(NSArray *)propertiesToMove
                 toPosition:(NSInteger)index
{
    [self.p_cloneOf moveTrackProperties:propertiesToMove toPosition:index];
}

- (void) addCrate:(NxACrate *)crate
 modificationDate:(NSDate *)modificationDate
{
    if (self.p_cloneOf.containsFilters) {
        [super addCrate:crate modificationDate:modificationDate];
    }
    else {
        [self.p_cloneOf addCrate:crate modificationDate:modificationDate];
    }
}

- (void)removeCrate:(NxACrate *)crate
{
    if (self.p_cloneOf.containsFilters) {
        [super removeCrate:crate];
    }
    else {
        [self.p_cloneOf removeCrate:crate];
    }
}

- (void)moveCrates:(NSArray *)cratesToMove
        toPosition:(NSInteger)index
{
    if (self.p_cloneOf.containsFilters) {
        [super moveCrates:cratesToMove toPosition:index];
    }
    else {
        [self.p_cloneOf moveCrates:cratesToMove toPosition:index];
    }
}

- (NSInteger)indexOfSubCrate:(NxACrate *)crate
{
    if (self.p_cloneOf.containsFilters) {
        return [super indexOfSubCrate:crate];
    }
    else {
        return [self.p_cloneOf indexOfSubCrate:crate];
    }
}

#pragma mark Overridden NxATaggedObject Instance Methods

- (void)destroy
{
    self.p_cloneOf = nil;

    [super destroy];
}

#pragma mark Overridden NSObject Methods

- (NSString *)description
{
    return [NSString stringWithFormat:@"%@ (Clone of %@)", self.p_fullName, self.p_cloneOf.p_fullName];
}

@end
