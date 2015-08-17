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

#import "Crate List View/Internal/NxACrateListViewItem.h"
#import "NxATrack.h"

#pragma mark Implementation

@implementation NxACrateListViewItem

#pragma mark Factory Methods

+ (instancetype)item
{
    NxACrateListViewItem *newItem = [[self alloc] init];
    NxAAssertNotNil(newItem);

    newItem.p_displayTitle = @"";
    newItem.cellIdentifier = @"HeaderCell";
    newItem.isSelectable = NO;
    newItem.shouldBeExpandedAtStart = NO;
    newItem.canBeRenamed = NO;

    return newItem;
}

+ (instancetype)itemWith:(id <NxACrateListViewItemSource>)source
             displayedAs:(NSString *)title
{
    NxAAssertNotNil(title);

    NxACrateListViewItem *newItem = source.crateListViewItem;
    newItem.p_displayTitle = title;
    newItem.canBeRenamed = NO;

    return newItem;
}

#pragma mark Accessors

- (void)setSourceObject:(id<NxACrateListViewItemSource>)sourceObject
{
    NxAAssertNotNil(sourceObject);
    
    _sourceObject = sourceObject;

    self.p_displayTitle = sourceObject.itemDescription;
}

- (void)setDisplayTitle:(NSString *)title
{
    NxAAssertTrue(self.canBeRenamed);

    if (!self.sourceObject) {
        return;
    }

    if ([self.sourceObject respondsToSelector:@selector(attemptToSetItemDescription:)]) {
        [self.sourceObject attemptToSetItemDescription:title];

        NxAAssertTrue([self.sourceObject respondsToSelector:@selector(itemDescription)]);
        self.p_displayTitle = self.sourceObject.itemDescription;
    }
}

- (NSString *)displayTitle
{
    return self.p_displayTitle;
}

- (BOOL)hasChildrenItems
{
    id <NxACrateListViewItemSource> sourceObject = self.sourceObject;
    if([sourceObject respondsToSelector:@selector(hasChildrenItems)]) {
        return sourceObject.hasChildrenItems;
    }

    return NO;
}

- (NSArray *)childrenItems
{
    if (!self.p_childrenItemsCache) {
        if (self.sourceObject) {
            NSMutableDictionary *previousItemsByObjectAddress = [NSMutableDictionary dictionaryWithCapacity:self.p_previousChildrenItemsCache.count];
            for (NxACrateListViewItem *previousItem in self.p_previousChildrenItemsCache) {
                NSString *key = [NSString stringWithFormat:@"%08lx", (NSUInteger)previousItem.sourceObject];
                previousItemsByObjectAddress[key] = previousItem;
            }

            NSArray *newChildren = self.sourceObject.childrenItems;
            NSMutableArray *newItems = [NSMutableArray arrayWithCapacity:newChildren.count];

            for (NxACrateListViewItem *newItem in newChildren) {
                NSString *key = [NSString stringWithFormat:@"%08lx", (NSUInteger)newItem.sourceObject];
                NxACrateListViewItem *previousItem = previousItemsByObjectAddress[key];
                if (previousItem) {
                    [newItems addObject:previousItem];
                }
                else {
                    [newItems addObject:newItem];
                }
            }

            self.p_childrenItemsCache = newItems;
        }
    }

    return self.p_childrenItemsCache;
}

- (NSArray *)trackListViewItems
{
    if (self.sourceObject) {
        return self.sourceObject.trackListViewItems;
    }
    else {
        return nil;
    }
}

#pragma mark Instance Methods

- (void)clearCache
{
    self.p_previousChildrenItemsCache = self.p_childrenItemsCache;

    for (NxACrateListViewItem *item in self.p_childrenItemsCache) {
        [item clearCache];
    }

    self.p_childrenItemsCache = nil;
}

- (NSDragOperation)validateDropForItems:(NSArray *)items
                                  index:(NSInteger)index
{
    if (!self.sourceObject) {
        return NSDragOperationNone;
    }

    if (![self.sourceObject respondsToSelector:@selector(validateDropForItems:index:)]) {
        return NSDragOperationNone;
    }

    return [self.sourceObject validateDropForItems:items index:index];
}

- (BOOL)acceptDropForItems:(NSArray *)items
                     index:(NSInteger)index
{
    if (!self.sourceObject) {
        return NO;
    }

    if (![self.sourceObject respondsToSelector:@selector(acceptDropForItems:index:)]) {
        return NO;
    }

    return [self.sourceObject acceptDropForItems:items index:index];
}

@end
