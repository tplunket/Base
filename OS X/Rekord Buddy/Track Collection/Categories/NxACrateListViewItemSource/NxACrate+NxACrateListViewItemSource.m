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

#import "NxACrate+NxACrateListViewItemSource.h"
#import "Track Collection/Tags/Crate Properties/NxATrackProperty.h"
#import "NxACrateClone.h"
#import "NxATrack.h"
#import "NxACrateListViewItem.h"
#import "NxATrackSortingBucket+NxACrateListViewItemSource.h"
#import "NxATrackSortingBucket+NxATrackListViewItem.h"
#import "Track Collection/Crates/Internal/NxACrate.h"

#pragma mark Implementation

@implementation NxACrate (NxACrateListViewItemSource)

#pragma mark NxACrateListViewItemSource Protocol

- (NxACrateListViewItem *)crateListViewItem
{
    NxACrateListViewItem *newItem = [NxACrateListViewItem item];
    newItem.sourceObject = self;
    newItem.canBeRenamed = self.isASystemCrate;

    if (self.isAPlaylist) {
        newItem.cellIdentifier = self.isASystemCrate ? @"AllTracksCell" : @"PlaylistCell";
    }
    else {
        newItem.cellIdentifier = self.isASystemCrate ? @"HeaderCell" : @"FolderCell";
    }

    newItem.isSelectable = YES;
    newItem.shouldBeExpandedAtStart = NO;

    return newItem;
}

- (NSString *)itemDescription
{
    return self.p_name;
}

- (BOOL)hasChildrenItems
{
    if (self.isAPlaylist && !(self.p_sortedBy.count || self.containsFilters)) {
        return NO;
    }
    else if (self.isEmpty) {
        return NO;
    }
    else {
        return YES;
    }
}

- (NSArray *)childrenItems
{
    if (self.isAPlaylist) {
        NSArray *sortingBuckets = self.p_sortingBuckets;
        NSMutableArray *children = [NSMutableArray arrayWithCapacity:sortingBuckets.count];

        for (NxATrackSortingBucket *bucket in sortingBuckets) {
            NxACrateListViewItem *item = bucket.crateListViewItem;
            [children addObject:item];
        }

        return children;
    }
    else {
        NSArray *subCrates = self.crates;
        NSMutableArray *children = [NSMutableArray arrayWithCapacity:subCrates.count];

        for (NxACrate *crate in subCrates) {
            NxACrateListViewItem *item = crate.crateListViewItem;
            [children addObject:item];
        }
        
        return children;
    }
}

- (NSArray *)trackListViewItems
{
    if (self.isAPlaylist && self.p_sortingBuckets.count) {
        NSArray *items = [NxATrackSortingBucket trackListViewGroupItemsWith:self.p_sortingBuckets];
        return items;
    }

    if (self.isAPlaylist) {
        // -- If we have tracks, we return the property items to that the user can redorder the tracks if they want to.
        NSArray *items = [NxATrackProperty tagsIn:self];
        if (!items.count) {
            return nil;
        }

        return items;
    }
    else {
        // -- Otherwise we just recurse into all our sub crates and return all the tracks we find.
        NSArray *tracks = self.tracks;
        if (!tracks.count) {
            return nil;
        }

        return tracks;
    }
}

- (void)attemptToSetItemDescription:(NSString *)description
{
    if (!self.isASystemCrate) {
        return;
    }

    self.p_name = description;

    NxAAssertTrue(self.hasParentCrate);

    NSDate *now = [NSDate date];
    NSArray *entries = [self.parentCrate p_entriesForType:[NxACrateProperty tagType]];
    [NxATagEntry markEntries:entries asModified:now];
}

- (void)addURIToArray:(NSMutableArray *)uris
{
    // -- We don't support dragging system crates but their clones are ok.
    if (self.isASystemCrate && ![self isKindOfClass:[NxACrateClone class]]) {
        return;
    }

    NSURL *entryURI = [[self objectID] URIRepresentation];
    [uris addObject:entryURI];
}

- (NSDragOperation)validateDropForItems:(NSArray *)items
                                  index:(NSInteger)index
{
    // -- All the items are the same type, so we test the first one.
    id firstObject = items.firstObject;

    // -- Find out what type of items we're dragging.
    if ([firstObject isKindOfClass:[NxATrack class]]) {
        if ((self.isAFolder && !self.isEmpty) || (index != -1)) {
            // -- We don't support dragging tracks on non-empty folder crate or as child of crates either.
            return NSDragOperationNone;
        }

        if (self.isASystemCrate) {
            // -- We don't support dragging tracks on system crates, smart playlists or if the crate is not a leaf.
            return NSDragOperationNone;
        }

        // -- We've found at least one item to drop.
        return NSDragOperationMove;
    }
    else if ([firstObject isKindOfClass:[NxACrate class]]) {
        for (id item in items) {
            if ([self p_canReceiveCrate:item]) {
                // -- We've found at least one item to drop.
                return NSDragOperationMove;
            }
        }
   }

    // -- Seems like the drop items are not something we support.
    return NSDragOperationNone;
}

- (BOOL)acceptDropForItems:(NSArray *)items
                     index:(NSInteger)index
{
    NSDate *now = [NSDate date];

    // -- All the items are the same type, so we test the first one.
    id firstObject = items.firstObject;

    // -- Find out what type of items we're dragging.
    if ([firstObject isKindOfClass:[NxATrack class]]) {
        // -- Add the tracks to the crate.
        for (NxATrack *track in items) {
            [self addTrack:track modificationDate:now];
        }
    }
    else if ([firstObject isKindOfClass:[NxACrate class]]) {
        if (index == -1) {
            // -- If there is no index, we're just moving the crates inside the target crate.
            index = self.crates.count;
        }

        for (NxACrate *crate in items) {
            NSUInteger indexToAddAt = index;
            ++index;

            if (![self p_canReceiveCrate:crate]) {
                continue;
            }

            if (crate.hasParentCrate) {
                NxACrate *parentCrate = crate.parentCrate;
                if (parentCrate != self) {
                    [parentCrate removeCrate:crate];
                    [self addCrate:crate modificationDate:now];
                }
            }

            [self moveCrates:@[ crate ] toPosition:indexToAddAt];
        }
    }

    return YES;
}

@end
