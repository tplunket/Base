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

#import "NxACrateListViewTagClassItem.h"
#import "NxATrack.h"
#import "Track Collection/NxAArtist.h"
#import "NxATrackKey+NxACrateListViewItem.h"
#import "Crate List View/Internal/NxACrateListViewItem.h"

#pragma mark Private Interface

@interface NxACrateListViewTagClassItem ()

#pragma mark Properties

@property (strong, nonatomic) Class p_itemsClass;

@property (strong, nonatomic) NSManagedObjectContext *p_context;

@end

#pragma mark Implementation

@implementation NxACrateListViewTagClassItem

#pragma mark Factory Methods

+ (instancetype)itemWithTagClass:(Class)itemsClass
                     displayedAs:(NSString *)title
                    usingContext:(NSManagedObjectContext *)context;
{
    NxAAssertNotNil(itemsClass);
    NxAAssertNotNil(title);
    NxAAssertNotNil(context);

    NxACrateListViewTagClassItem *newItem = [super item];
    newItem.p_itemsClass = itemsClass;
    newItem.cellIdentifier = @"HeaderCell";
    newItem.p_displayTitle = title;
    newItem.p_context = context;
    newItem.tagNameFilter = @"";

    return newItem;
}

#pragma mark NxACrateListViewItem Protocol

- (void)setDisplayTitle:(NSString *)title
{
    self.p_displayTitle = title;
}

- (BOOL)hasChildrenItems
{
    if (!self.p_childrenItemsCache) {
        NSArray *existingItems = self.p_previousChildrenItemsCache;
        if (!existingItems) {
            existingItems = @[ ];
        }

        self.p_childrenItemsCache = [self.p_itemsClass crateListViewItemsForAllItemsInContext:self.p_context
                                                                                  withTagName:self.tagNameFilter
                                                                                existingItems:existingItems];
    }
    
    return self.p_childrenItemsCache.count != 0;
}

- (NSArray *)childrenItems
{
    return self.p_childrenItemsCache;
}

- (NSArray *)trackListViewItems
{
    return nil;
}

@end
