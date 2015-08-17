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

#import "NxAFilterPLaylist+NxACrateListViewItemSource.h"
#import "NxACrate+NxACrateListViewItemSource.h"
#import "NxATrackSortingBucket+NxATrackListViewItem.h"
#import "NxACrateListViewItem.h"
#import "Track Collection/Crates/Internal/NxASmartPlaylist.h"

#pragma mark Implementation

@implementation NxAFilterPlaylist (NxACrateListViewItemSource)

#pragma mark NxACrateListViewItemSource Protocol

- (NxACrateListViewItem *)crateListViewItem
{
    NxACrateListViewItem *newItem = [super crateListViewItem];

    newItem.cellIdentifier = @"FilterCell";

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
    // -- We don't support dropping anything on a filter playlist.
    return NSDragOperationNone;
}

- (BOOL)acceptDropForItems:(NSArray *)items
                     index:(NSInteger)index
{
    // -- We don't support dropping anything on a filter playlist.
    return NO;
}

@end
