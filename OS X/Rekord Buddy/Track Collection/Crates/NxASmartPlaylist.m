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

#import "Track Collection/Crates/Internal/NxASmartPlaylist.h"

#pragma mark Implementation

@implementation NxASmartPlaylist

#pragma mark Factory Methods

+ (instancetype)p_crateWithName:(NSString *)name
                      predicate:(NSString *)predicate
                        context:(NSManagedObjectContext *)context
{
    NxAAssertNotNil(predicate);

    NxASmartPlaylist *newCrate = [self p_emptyCrateWithName:name inContext:context];
    NxAAssertNotNil(newCrate);

    newCrate.predicate = predicate;
    newCrate.p_type = [NSNumber numberWithInteger:NxACrateIsASmartPlaylist];

    return newCrate;
}

+ (instancetype)crateWithName:(NSString *)name
                    predicate:(NSString *)predicate
                      context:(NSManagedObjectContext *)context
{
    return [self p_crateWithName:name
                       predicate:predicate
                         context:context];
}

#pragma mark Overidden NxACrate Class Methods

+ (NSString *)p_entityName
{
    return @"SmartPlaylist";
}

#pragma mark Accessors

// -- Automatically generated for Core Data properties.
@dynamic p_predicate;

// -- Automatically generated for properties.
@synthesize predicate;

- (void)setPredicate:(NSString *)newPredicate
{
    self.p_predicate = newPredicate;

    [self updateSmartContent:NxACrateShouldUpdateSiblings];
}

- (NSString *)predicate
{
    return self.p_predicate;
}

#pragma mark Overridden NxACrate Accessors

// -- Automatically generated for properties.
@synthesize p_tracks;

- (NSArray *)tracks
{
    return self.p_tracks;
}

#pragma mark Overridden NxACrate Instance Methods

- (BOOL)p_canReceiveCrate:(NxACrate *)crate
{
    return crate.isAFilter;
}

- (void)moveTrackProperties:(NSArray *)propertiesToMove
                 toPosition:(NSInteger)index
{
    ALog(@"Can't move tracks in a smart playlist.");
}

- (void)updateSmartContent:(NxACrateUpdateSiblingsFlag)updateSiblings
{
    [super updateSmartContent:updateSiblings];

    NSManagedObjectContext *context = self.managedObjectContext;
    NxACrate *allTracksCrate = [NxACrate allTracksCrateIn:context];

    NSArray *tracks = [[NxACrate p_tracksMatchingPredicate:self.p_predicate
                                                   inCrate:allTracksCrate
                                                   context:context]
                       allObjects];

    self.p_tracks = tracks;
}

@end
