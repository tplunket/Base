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

@class NxACrateListViewItem;

#pragma mark Protocol

@protocol NxACrateListViewItemSource <NSObject>

#pragma mark Properties

@required

@property (readonly, nonatomic) NxACrateListViewItem *crateListViewItem;

@property (readonly, nonatomic) NSString *itemDescription;

@optional

@property (readonly, nonatomic) BOOL hasChildrenItems;

@property (readonly, nonatomic) NSArray *childrenItems;

@property (readonly, nonatomic) NSArray *trackListViewItems;

#pragma mark Instance Methods

@optional

- (void)attemptToSetItemDescription:(NSString *)description;

- (void)addURIToArray:(NSMutableArray *)uris;

- (NSDragOperation)validateDropForItems:(NSArray *)items
                                  index:(NSInteger)index;

- (BOOL)acceptDropForItems:(NSArray *)items
                     index:(NSInteger)index;

@end
