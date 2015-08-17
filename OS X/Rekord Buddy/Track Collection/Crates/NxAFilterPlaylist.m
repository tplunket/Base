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

#import "NxAFilterPlaylist.h"
#import "Track Collection/Crates/Internal/NxASmartPlaylist.h"
#import "NxAOtherTracksPlaylist.h"

#pragma mark Constants

NSString *NxAFilterPlaylistOtherTracksPlaylistName = @"Other Tracks";

@implementation NxAFilterPlaylist

#pragma mark Factory Methods

+ (instancetype)crateWithName:(NSString *)name
                    predicate:(NSString *)predicate
                      context:(NSManagedObjectContext *)context
{
    NxAAssertNotNil(name);
    NxAAssertNotNil(predicate);

    NxAFilterPlaylist *newCrate = [self p_crateWithName:name
                                              predicate:predicate
                                                context:context];
    NxAAssertNotNil(newCrate);

    newCrate.p_type = [NSNumber numberWithInteger:NxACrateIsAFilterPlaylist];

    return newCrate;
}

#pragma mark Overridden NxACrate Class Methods

+ (NSString *)p_entityName
{
    return @"FilterPlaylist";
}

#pragma mark Overridden NxACrate Instance Methods

- (void)updateSmartContent:(NxACrateUpdateSiblingsFlag)updateSiblings
{
    if (!self.hasParentCrate) {
        self.p_tracks = nil;
        return;
    }

    NxACrate *parentCrate = self.parentCrate;
    NSManagedObjectContext *context = self.managedObjectContext;
    NxAAssertNotNil(self.p_predicate);

    NSArray *tracks = [[NxACrate p_tracksMatchingPredicate:self.p_predicate
                                                   inCrate:parentCrate
                                                   context:context]
                       allObjects];

    self.p_tracks = tracks;

    // -- If we're the main crate we update ourselves.
    [self p_updateSortingBuckets];

    // -- Then we update our clones too.
    for (NxACrate *clone in self.p_clones) {
        [clone p_updateSortingBuckets];
    }

    NxAOtherTracksPlaylist *otherTracksPlaylist = [NxAOtherTracksPlaylist findOtherTrackPlaylistIn:parentCrate];
    if (!otherTracksPlaylist) {
        // -- If we don't have an 'Other Tracks' crate, we add one now.
        NSDate *now = [NSDate date];

        otherTracksPlaylist = [NxAOtherTracksPlaylist p_emptyCrateWithName:NxAFilterPlaylistOtherTracksPlaylistName inContext:context];
        [parentCrate p_addCrateWithoutUpdatingSmartContent:otherTracksPlaylist
                                            lastModifiedOn:now];
    }
    else {
        // -- If we do have an 'Other Tracks' crate, we make sure it stays at the end of the list of sibbling crates.
        NSArray *parentCrateEntries = [NxACrateProperty tagsIn:parentCrate];

        // -- Then move it into position;
        [parentCrate moveCrates:@[ otherTracksPlaylist ] toPosition:parentCrateEntries.count - 1];
    }

    // -- And we update its content.
    [otherTracksPlaylist updateSmartContent:NxACrateShouldNotUpdateSiblings];
}

@end
