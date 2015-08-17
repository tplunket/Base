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

#import "NxACrateListViewItemWithChild.h"
#import "NxATrack.h"
#import "NxATrackKey+NxACrateListViewItem.h"
#import "Crate List View/Internal/NxACrateListViewItem.h"

#pragma mark Private Interface

@interface NxACrateListViewItemWithChild ()

#pragma mark Properties

@property (strong, nonatomic) id <NxACrateListViewItemSource> p_childSource;

@end

#pragma mark Implementation

@implementation NxACrateListViewItemWithChild

#pragma mark Factory Methods

+ (instancetype)itemWithChild:(id <NxACrateListViewItemSource>)source
                  displayedAs:(NSString *)title;
{
    NxAAssertNotNil(source);

    NxACrateListViewItemWithChild *newItem = [super item];
    newItem.p_childSource = source;
    newItem.p_displayTitle = title;
    newItem.shouldBeExpandedAtStart = YES;
    newItem.childDisplayTitle = @"";
    newItem.isSelectable = NO;

    return newItem;
}

#pragma mark Accessors

- (void)setDisplayTitle:(NSString *)title
{
    self.p_displayTitle = title;
}

- (BOOL)hasChildrenItems
{
    return YES;
}

- (NSArray *)childrenItems
{
    if (!self.p_childrenItemsCache) {
        if (self.p_childSource) {
            NxACrateListViewItem *previousItem = self.p_previousChildrenItemsCache.firstObject;
            if (previousItem.sourceObject == self.p_childSource) {
                self.p_childrenItemsCache = @[ previousItem ];
            }
            else {
                NxACrateListViewItem *newItem = [NxACrateListViewItem itemWith:self.p_childSource
                                                                   displayedAs:self.childDisplayTitle];

                self.p_childrenItemsCache = @[ newItem ];
            }
        }
    }

    return self.p_childrenItemsCache;
}

- (NSArray *)trackListViewItems
{
    return nil;
}

@end
