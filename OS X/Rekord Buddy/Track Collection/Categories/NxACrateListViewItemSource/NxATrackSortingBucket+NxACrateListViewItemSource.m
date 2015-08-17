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

#import "NxATrackSortingBucket+NxACrateListViewItemSource.h"
#import "NxATrackSortingBucket+NxATrackListViewItem.h"
#import "NxACrateListViewItem.h"
#import "NxATrack.h"
#import "Track Collection/Crates/NxACrate.h"
#import "Track Collection/Internal/NxATrackSortingBucket.h"

#pragma mark Implementation

@implementation NxATrackSortingBucket (NxACrateListViewItemSource)

#pragma mark NxACrateListViewItemSource Protocol

- (NxACrateListViewItem *)crateListViewItem
{
    NxACrateListViewItem *newItem = [NxACrateListViewItem item];
    newItem.sourceObject = self;
    newItem.cellIdentifier = @"SortingBucketCell";
    newItem.isSelectable = YES;
    newItem.shouldBeExpandedAtStart = NO;

    return newItem;
}

- (NSString *)itemDescription
{
    return self.p_bucketDescription;
}

- (BOOL)hasChildrenItems
{
    return self.childBuckets.count != 0;
}

- (NSArray *)childrenItems
{
    NSArray *childBuckets = self.childBuckets;
    NSMutableArray *children = [NSMutableArray arrayWithCapacity:childBuckets.count];

    for (NxATrackSortingBucket *bucket in childBuckets) {
        NxACrateListViewItem *item = bucket.crateListViewItem;
        [children addObject:item];
    }

    return children;
}

- (NSArray *)trackListViewItems
{
    if (self.childBuckets) {
        return [NxATrackSortingBucket trackListViewGroupItemsWith:self.childBuckets];
    }
    else {
        if (!self.p_tracks.count) {
            return nil;
        }

        return self.p_tracks;
    }
}

- (NSDragOperation)validateDropForItems:(NSArray *)items
                                  index:(NSInteger)index
{
    id firstObject = items.firstObject;

    // -- Find out what type of items we're dragging.
    if (![firstObject isKindOfClass:[NxATrack class]]) {
        // -- We don't support dropping anything but tracks on sorting buckets.
        return NSDragOperationNone;
    }

    if (index != -1) {
        // -- We don't support dropping tracks anywhere but right on the sorting buckets.
        return NSDragOperationNone;
    }

    NxATag *tag = self.p_tag;

    Class tagClass;
    if (tag) {
        tagClass = tag.class;
    }
    else {
        tagClass = NSClassFromString(self.p_tagClassName);
    }

    if (![tagClass valuesAreUserModifiable]) {
        // -- If we don't support modifying this tag type, we just say no.
        return NSDragOperationNone;
    }

    return NSDragOperationMove;
}

- (BOOL)acceptDropForItems:(NSArray *)items
                     index:(NSInteger)index
{
    NxACrate *parentCrate = self.p_parentCrate;
    NSDate *now = [NSDate date];
    NxATag *tag = self.p_tag;

    for (NxATrack *track in items) {
        if (![parentCrate containsTrack:track]) {
            // -- If this track is not already in the parent crate, we add it.
            [parentCrate addTrack:track modificationDate:now];
        }

        Class tagClass = NSClassFromString(self.p_tagClassName);

        if (tag) {
            [track addTag:tag lastModifiedOn:now];
        }
        else if ([tagClass isATagTypeSortedByPropertyName]) {
            [track removeTagsWithClass:tagClass andName:self.p_tagDescription];
        }
        else {
            [track removeTagsWithClass:tagClass];
        }
    }

    NxATrackSortingBucket *parentBucket = self.p_parentBucket;
    if (parentBucket) {
        if ([parentBucket validateDropForItems:items index:index] != NSDragOperationNone) {
            // -- If we have a parent bucket that accepts it, we drop the items on it too.
            [parentBucket acceptDropForItems:items index:index];
        }
    }

    [parentCrate updateSmartContent:NxACrateShouldNotUpdateSiblings];

    return YES;
}

@end
