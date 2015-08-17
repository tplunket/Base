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

#import "NxAOtherTracksPlaylist+NxACrateListViewItemSource.h"
#import "NxATrackSortingBucket+NxATrackListViewItem.h"
#import "NxACrate+NxACrateListViewItemSource.h"
#import "NxACrateListViewItem.h"
#import "Track Collection/Crates/Internal/NxASmartPlaylist.h"

#pragma mark Implementation

@implementation NxAOtherTracksPlaylist (NxACrateListViewItemSource)

#pragma mark NxACrateListViewItemSource Protocol

- (NxACrateListViewItem *)crateListViewItem
{
    NxACrateListViewItem *newItem = [super crateListViewItem];

    newItem.cellIdentifier = @"OtherTracksCell";
    newItem.canBeRenamed = NO;

    return newItem;
}

- (NSArray *)trackListViewItems
{
    if (self.p_sortingBuckets.count) {
        NSArray *items = [NxATrackSortingBucket trackListViewGroupItemsWith:self.p_sortingBuckets];
        return items;
    }

    return self.tracks;
}

- (void)addURIToArray:(NSMutableArray *)uris
{
    // -- We don't support dragging 'Other tracks' crates.
}

- (NSDragOperation)validateDropForItems:(NSArray *)items
                                  index:(NSInteger)index
{
    // -- We don't support dropping anything on an 'Other Tracks' crate.
    return NSDragOperationNone;
}

- (BOOL)acceptDropForItems:(NSArray *)items
                     index:(NSInteger)index
{
    // -- We don't support dropping anything on an 'Other Tracks' crate.
    return NO;
}

@end
