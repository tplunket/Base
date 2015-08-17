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

#import "NxACrateClone+NxACrateListViewItemSource.h"
#import "NxACrate+NxACrateListViewItemSource.h"
#import "NxATrackSortingBucket+NxATrackListViewItem.h"
#import "Track Collection/Tags/Crate Properties/NxATrackProperty.h"
#import "NxACrateListViewItem.h"
#import "Track Collection/Crates/Internal/NxACrateClone.h"

#pragma mark Implementation

@implementation NxACrateClone (NxACrateListViewItemSource)

#pragma mark NxACrateListViewItemSource Protocol

- (NxACrateListViewItem *)crateListViewItem
{
    NxACrateListViewItem *newItem = [super crateListViewItem];

    if (self.isAPlaylist) {
        newItem.cellIdentifier = @"PlaylistCloneCell";
    }
    else {
        newItem.cellIdentifier = @"FolderCloneCell";
    }

    return newItem;
}

- (NSArray *)trackListViewItems
{
    // -- TODO: This is a lot of duplicated code from the same category in NxACrate.
    if (self.p_sortingBuckets.count) {
        NSArray *items = [NxATrackSortingBucket trackListViewGroupItemsWith:self.p_sortingBuckets];
        return items;
    }

    if (self.isAPlaylist) {
        // -- If we have tracks, we return the property items to that the user can redorder the tracks if they want to.
        NSArray *items = [NxATrackProperty tagsIn:self.p_cloneOf];
        if (!items.count) {
            return nil;
        }

        return items;
    }
    else {
        // -- Otherwise we just recurse into all our sub crates and return all the tracks we find.
        NSArray *tracks = self.p_cloneOf.tracks;
        if (!tracks.count) {
            return nil;
        }

        return tracks;
    }
}

@end
