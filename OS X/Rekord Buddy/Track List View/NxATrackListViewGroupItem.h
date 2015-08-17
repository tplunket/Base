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

#import "NxATrackListViewItem.h"

#pragma mark Public Interface

/*! A group item in the track list window. */
@interface NxATrackListViewGroupItem : NSObject <NxATrackListViewItem>

#pragma mark Factory Methods

/*!
 * Create a group item.
 * \param name Title of the item to display in the list.
 * \param children Children of the group item.
 * \return A track list group item with the given name and children.
 */
+ (instancetype)groupItemWithName:(NSString *)name
                         children:(NSArray *)children;
@end
