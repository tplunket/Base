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

#import "NxATrackSortingBucket+NxATrackListViewItem.h"
#import "NxATrackListViewGroupItem.h"
#import "Track Collection/Tags/NxATag.h"
#import "Track Collection/Internal/NxATrackSortingBucket.h"

#pragma mark Implementation

@implementation NxATrackSortingBucket (NxATrackListViewItem)

#pragma mark Class Methods

+ (NSArray *)trackListViewGroupItemsWith:(NSArray *)sortingBuckets
{
    NSMutableArray *resultItems = [NSMutableArray array];

    for (NxATrackSortingBucket *bucket in sortingBuckets) {
        NSString *description = bucket.p_bucketDescription;
        id <NxATrackListViewItem> headerItem = [NxATrackListViewGroupItem groupItemWithName:description
                                                                                   children:bucket.p_tracks];
        [resultItems addObject:headerItem];
    }

    return resultItems;
}

@end
