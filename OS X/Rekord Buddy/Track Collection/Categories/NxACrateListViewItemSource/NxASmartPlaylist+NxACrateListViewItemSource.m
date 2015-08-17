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

#import "NxASmartPlaylist+NxACrateListViewItemSource.h"
#import "NxACrate+NxACrateListViewItemSource.h"
#import "NxACrateListViewItem.h"
#import "Track Collection/Crates/Internal/NxASmartPlaylist.h"

#pragma mark Implementation

@implementation NxASmartPlaylist (NxACrateListViewItemSource)

#pragma mark NxACrateListViewItemSource Protocol

- (NxACrateListViewItem *)crateListViewItem
{
    NxACrateListViewItem *newItem = [super crateListViewItem];

    newItem.cellIdentifier = @"SmartPlaylistCell";

    return newItem;
}

- (NSArray *)trackListViewItems
{
    return self.tracks;
}

- (NSDragOperation)validateDropForItems:(NSArray *)items
                                  index:(NSInteger)index
{
    // -- We don't support dropping anything on a smart playlist.
    return NSDragOperationNone;
}

- (BOOL)acceptDropForItems:(NSArray *)items
                     index:(NSInteger)index
{
    // -- We don't support dropping anything on a smart playlist.
    return NO;
}

@end
