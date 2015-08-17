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

#import "NxATrackListViewGroupItem.h"

#pragma mark Private Interface

@interface NxATrackListViewGroupItem ()

#pragma mark Properties

/*! The name displayed by the group item. */
@property (strong, nonatomic) NSString *p_name;

/*! Children of this group item. */
@property (strong, nonatomic) NSArray *p_children;

@end

#pragma mark Implementation

@implementation NxATrackListViewGroupItem

#pragma mark Factory Methods

+ (instancetype)groupItemWithName:(NSString *)name
                         children:(NSArray *)children
{
    NxATrackListViewGroupItem *item = [[NxATrackListViewGroupItem alloc] init];
    if (!item) {
        return nil;
    }

    item.p_name = name;
    item.p_children = children;

    return item;
}

#pragma mark NxATrackListViewItem Protocol

- (BOOL)trackListViewItemIsGroupItem
{
    return YES;
}

- (NSString *)trackListViewItemCellIdentifier
{
    return @"HeaderCell";
}

- (NSArray *)trackListViewItemChildren
{
    return self.p_children;
}

- (NSString *)trackListViewItemTitle
{
    return self.p_name;
}

@end
