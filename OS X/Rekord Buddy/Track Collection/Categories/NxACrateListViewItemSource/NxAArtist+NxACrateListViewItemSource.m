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

#import "NxAArtist+NxACrateListViewItemSource.h"
#import "NxATrackListViewGroupItem.h"
#import "NxATrackArtistCredit.h"
#import "NxATrackProducerCredit.h"
#import "NxATrackRemixerCredit.h"
#import "NxATag+NxACrateListViewItemSource.h"
#import "NxACrateListViewItem.h"
#import "Track Collection/Internal/NxAArtist.h"

#pragma mark Implementation

@implementation NxAArtist (NxACrateListViewItemSource)

#pragma mark Class Methods

/*!
 * Get a track list view header with items.
 * \param tag Tag which track's make up the items.
 * \param headerName Name of the header item above all the track items.
 * \return A track list view header with the tag's tracks as child items.
 */
+ (NxATrackListViewGroupItem *)p_trackListViewItemsFromTag:(NxATag *)tag
                                                headerName:(NSString *)headerName
{
    NSArray *items = tag.trackListViewItems;
    if (!items.count) {
        return nil;
    }

    NxATrackListViewGroupItem *headerItem = [NxATrackListViewGroupItem groupItemWithName:headerName children:items];

    return headerItem;
}

#pragma mark NxACrateListViewItemSource Protocol

- (NxACrateListViewItem *)crateListViewItem
{
    NxACrateListViewItem *newItem = [NxACrateListViewItem item];

    newItem.sourceObject = self;
    newItem.cellIdentifier = @"ArtistCell";
    newItem.isSelectable = YES;
    newItem.shouldBeExpandedAtStart = YES;
    newItem.canBeRenamed = self.p_name ? YES : NO;

    return newItem;
}

- (NSString *)itemDescription
{
    return self.p_name ? self.p_name : @"No Artist";
}

- (NSArray *)trackListViewItems
{
    NSMutableArray *results = [NSMutableArray array];

    NxATrackArtistCredit *artistCredit = self.p_tracks;
    if (artistCredit) {
        NxATrackListViewGroupItem *headerItem = [NxAArtist p_trackListViewItemsFromTag:artistCredit
                                                                            headerName:@"Tracks"];
        if (headerItem) {
            [results addObject:headerItem];
        }
    }

    NxATrackRemixerCredit *remixerCredit = self.p_remixes;
    if (remixerCredit) {
        NxATrackListViewGroupItem *headerItem = [NxAArtist p_trackListViewItemsFromTag:remixerCredit
                                                                            headerName:@"Remixes"];
        if (headerItem) {
            [results addObject:headerItem];
        }
    }

    NxATrackProducerCredit *producerCredit = self.p_productions;
    if (producerCredit) {
        NxATrackListViewGroupItem *headerItem = [NxAArtist p_trackListViewItemsFromTag:producerCredit
                                                                            headerName:@"Productions"];
        if (headerItem) {
            [results addObject:headerItem];
        }
    }

    return results.count ? results : nil;
}

- (void)attemptToSetItemDescription:(NSString *)description
{
    NxAArtist *existing = [NxAArtist artistWithName:description
                                            context:self.managedObjectContext];
    if (existing) {
        // TODO: Need to merge the artist's tracks into the existing one's
        return;
    }

    self.p_name = description;
}

@end
