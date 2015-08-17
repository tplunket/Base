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

#import "NxATag+NxACrateListViewItemSource.h"
#import "NxATagEntry+NxATrackListViewItem.h"
#import "NxATaggedObject.h"
#import "NxACrateListViewItem.h"
#import "Track Collection/Tags/Internal/NxATag.h"

#pragma mark Implementation

@implementation NxATag (NxATrackListViewItemSource)

#pragma mark NxACrateListViewItemSource Protocol

- (NxACrateListViewItem *)crateListViewItem
{
    NxACrateListViewItem *newItem = [NxACrateListViewItem item];
    newItem.sourceObject = self;
    newItem.cellIdentifier = @"TagCell";
    newItem.isSelectable = YES;
    newItem.shouldBeExpandedAtStart = NO;

    return newItem;
}

- (NSString *)itemDescription
{
    return self.isNil ? @"No Tag" : self.stringValue;
}

- (NSArray *)trackListViewItems
{
    NSSet *tagEntries = self.p_parentTagEntries;
    NSSet *results = [NxATagEntry parentTracksForEntries:tagEntries];
    return results.count ? results.allObjects : nil;
}

- (NSDragOperation)validateDropForItems:(NSArray *)items
                                  index:(NSInteger)index
{
    if (index >= 0) {
        return NSDragOperationNone;
    }

    for (id item in items) {
        if (![item isKindOfClass:[NxATaggedObject class]]) {
            return NSDragOperationNone;
        }
    }

    return NSDragOperationMove;
}

- (BOOL)acceptDropForItems:(NSArray *)items
                     index:(NSInteger)index
{
    NSDate *now = [NSDate date];

    if (self.isNil) {
        Class tagClass = self.class;

        if (self.p_tagName) {
            for (NxATaggedObject *object in items) {
                [object removeTagsWithClass:tagClass andName:self.p_tagName];
            }
        }
        else {
            for (NxATaggedObject *object in items) {
                [object removeTagsWithClass:tagClass];
            }
        }
    }
    else {
        for (NxATaggedObject *object in items) {
            [object addTag:self lastModifiedOn:now];
        }
    }

    return YES;
}

@end
