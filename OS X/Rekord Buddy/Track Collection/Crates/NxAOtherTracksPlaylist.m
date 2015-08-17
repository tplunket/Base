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

#import "NxAOtherTracksPlaylist.h"
#import "Track Collection/Tags/Crate Properties/NxACrateProperty.h"
#import "Track Collection/Crates/Internal/NxACrate.h"

#pragma mark Private Interface

@interface NxAOtherTracksPlaylist ()

#pragma mark Properties

@property (strong, nonatomic) NSArray *p_tracks;

@end

#pragma mark - Implementation

@implementation NxAOtherTracksPlaylist

#pragma mark Class Methods

+ (NxAOtherTracksPlaylist *)findOtherTrackPlaylistIn:(NxACrate *)crate
{
    NSArray *subCrates = crate.crates;
    for (NxACrate *subCrate in subCrates) {
        if ([subCrate isKindOfClass:[NxAOtherTracksPlaylist class]]) {
            return (NxAOtherTracksPlaylist *)subCrate;
        }
    }

    return nil;
}

#pragma mark NxACrate Class Methods

+ (NSString *)p_entityName
{
    return @"OtherTracksPlaylist";
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
    return NO;
}

- (void)updateSmartContent:(NxACrateUpdateSiblingsFlag)updateSiblings
{
    if (!self.hasParentCrate) {
        self.p_tracks = nil;
        return;
    }

    NxACrate *parentCrate = self.parentCrate;
    NSArray *siblingsCrates = parentCrate.crates;
    if (siblingsCrates.count == 1) {
        // -- If we're the only crate left then we can just delete outselves.
        [self destroy];
        return;
    }

    // -- Otherwise we find our what tracks, if any are not already in our siblings
    NSMutableSet *tracksLeft = [NSMutableSet setWithArray:parentCrate.p_tracks];

    for (NxACrate *subCrate in siblingsCrates) {
        if (subCrate == self) {
            continue;
        }

        NSArray *tracks = subCrate.tracks;
        for (NxATrack *track in tracks) {
            [tracksLeft removeObject:track];
        }
    }

    self.p_tracks = tracksLeft.allObjects;

    // -- If we're the main crate we update ourselves.
    [self p_updateSortingBuckets];

    // -- Then we update our clones too.
    for (NxACrate *clone in self.p_clones) {
        [clone p_updateSortingBuckets];
    }
}

- (void)moveTrackProperties:(NSArray *)propertiesToMove
                 toPosition:(NSInteger)index
{
    ALog(@"Can't modify an 'other tracks' crate.");
}

- (void)removeCrate:(NxACrate *)crate
{
    ALog(@"Can't modify an 'other tracks' crate.");
}

- (void)moveCrates:(NSArray *)cratesToMove
        toPosition:(NSInteger)index
{
    ALog(@"Can't modify an 'other tracks' crate.");
}

@end
