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

#import "NxACrateListViewItemSource.h"

#pragma mark Public Interface

@interface NxACrateListViewItem : NSObject

#pragma mark Factory Methods

+ (instancetype)item;

+ (instancetype)itemWith:(id <NxACrateListViewItemSource>)source
             displayedAs:(NSString *)title;

#pragma mark Properties

@property (strong, nonatomic) id <NxACrateListViewItemSource> sourceObject;

@property (strong, nonatomic) NSString *displayTitle;

@property (strong, nonatomic) NSString *cellIdentifier;

@property (assign, nonatomic) BOOL isSelectable;

@property (assign, nonatomic) BOOL shouldBeExpandedAtStart;

@property (assign, nonatomic) BOOL canBeRenamed;

@property (assign, nonatomic) BOOL hasChildrenItems;

@property (readonly, nonatomic) NSArray *childrenItems;

@property (readonly, nonatomic) NSArray *trackListViewItems;

#pragma mark Instance Methods

- (void)clearCache;

- (NSDragOperation)validateDropForItems:(NSArray *)items
                                  index:(NSInteger)index;

- (BOOL)acceptDropForItems:(NSArray *)items
                     index:(NSInteger)index;

@end
