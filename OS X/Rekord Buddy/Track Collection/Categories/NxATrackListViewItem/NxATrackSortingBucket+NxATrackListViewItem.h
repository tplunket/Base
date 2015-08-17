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

#import "NxATrackSortingBucket.h"

#pragma mark Public Interface

@interface NxATrackSortingBucket (NxATrackListViewItem)

#pragma mark Class Methods

/*!
 * Get an array of track list view items.
 * \param sortingBuckets Array of sorting buckets with tracks, sorted by tag value.
 * \return Array of track list view items for this sorting buckets.
 */
+ (NSArray *)trackListViewGroupItemsWith:(NSArray *)sortingBuckets;

@end
