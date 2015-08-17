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

#import "NxATrackListViewItem.h"
#import "NxATrackProperty+NxATrackListViewItem.h"
#import "NxATrack+NxATrackListViewItem.h"
#import "Track Collection/Tags/Crate Properties/Internal/NxATrackProperty.h"

#pragma mark Implementation

@implementation NxATrackProperty (NxATrackListViewItem)

#pragma mark NxATrackListViewItem Protocol

- (BOOL)trackListViewItemIsGroupItem
{
    return NO;
}

- (NSString *)trackListViewItemCellIdentifier
{
    return @"TrackCell";
}

- (NSArray *)trackListViewItemChildren
{
    return nil;
}

- (NSString *)trackListViewItemTitle
{
    return self.p_value.trackListViewItemTitle;
}

- (void)setTrackListViewItemTitle:(NSString *)title
{
    self.p_value.trackListViewItemTitle = title;
}

- (NSArray *)trackListViewItemArtists
{
    return self.p_value.trackListViewItemArtists;
}

- (void)setTrackListViewItemArtists:(NSArray *)artists
{
    self.p_value.trackListViewItemArtists = artists;
}

- (NSArray *)trackListViewItemRemixers
{
    return self.p_value.trackListViewItemRemixers;
}

- (void)setTrackListViewItemRemixers:(NSArray *)remixers
{
    self.p_value.trackListViewItemRemixers = remixers;
}

- (NSArray *)trackListViewItemProducers
{
    return self.p_value.trackListViewItemProducers;
}

- (void)setTrackListViewItemProducers:(NSArray *)producers
{
    self.p_value.trackListViewItemProducers = producers;
}

- (NSString *)trackListViewItemComments
{
    return self.p_value.trackListViewItemComments;
}

- (void)setTrackListViewItemComments:(NSString *)comments
{
    self.p_value.trackListViewItemComments = comments;
}

- (NSImage *)trackListViewItemArtwork
{
    return self.p_value.trackListViewItemArtwork;
}

- (void)setTrackListViewItemArtwork:(NSImage *)artwork
{
    self.p_value.trackListViewItemArtwork = artwork;
}

- (NSArray *)trackListViewItemTags
{
    return self.p_value.trackListViewItemTags;
}

- (void)setTrackListViewItemTags:(NSArray *)tags
{
    self.p_value.trackListViewItemTags = tags;
}

- (NSArray *)trackListViewItemKeys
{
    return self.p_value.trackListViewItemKeys;
}

- (void)setTrackListViewItemKeys:(NSArray *)keys
{
    self.p_value.trackListViewItemKeys = keys;
}

- (NSNumber *)trackListViewItemRating
{
    return self.p_value.trackListViewItemRating;
}

- (void)setTrackListViewItemRating:(NSNumber *)rating
{
    self.p_value.trackListViewItemRating = rating;
}

- (NSArray *)trackListViewItemGenres
{
    return self.p_value.trackListViewItemGenres;
}

- (void)setTrackListViewItemGenres:(NSArray *)genres
{
    self.p_value.trackListViewItemGenres = genres;
}

- (NSString *)trackListViewItemRecordLabel
{
    return self.p_value.trackListViewItemRecordLabel;
}

- (void)setTrackListViewItemRecordLabel:(NSString *)recordLabel
{
    self.p_value.trackListViewItemRecordLabel = recordLabel;
}

- (NSDate *)trackListViewItemDateReleased
{
    return self.p_value.trackListViewItemDateReleased;
}

- (void)setTrackListViewItemDateReleased:(NSDate *)dateReleased
{
    self.p_value.trackListViewItemDateReleased = dateReleased;
}

- (NSDate *)trackListViewItemDateAdded
{
    return self.p_value.trackListViewItemDateAdded;
}

- (void)setTrackListViewItemDateAdded:(NSDate *)dateAdded
{
    self.p_value.trackListViewItemDateAdded = dateAdded;
}

- (NSDate *)trackListViewItemDateModified
{
    return self.p_value.trackListViewItemDateModified;
}

- (NSString *)trackListViewItemMixName
{
    return self.p_value.trackListViewItemMixName;
}

- (void)setTrackListViewItemMixName:(NSString *)mixName
{
    self.p_value.trackListViewItemMixName = mixName;
}

- (NSNumber *)trackListViewItemPlayCount
{
    return self.p_value.trackListViewItemPlayCount;
}

- (void)setTrackListViewItemPlayCount:(NSNumber *)playCount
{
    self.p_value.trackListViewItemPlayCount = playCount;
}

- (NSString *)trackListViewItemAlbum
{
    return self.p_value.trackListViewItemAlbum;
}

- (NSNumber *)trackListViewItemTrackCount
{
    return self.p_value.trackListViewItemTrackCount;
}

- (NSNumber *)trackListViewItemTrackNumber
{
    return self.p_value.trackListViewItemTrackNumber;
}

- (void)setTrackListViewItemTrackNumber:(NSNumber *)trackNumber
{
    self.p_value.trackListViewItemTrackNumber = trackNumber;
}

- (NSSet *)trackListViewItemParentCrates
{
    return self.p_value.trackListViewItemParentCrates;
}

- (NSString *)trackListViewItemSampleRate
{
    return self.p_value.trackListViewItemSampleRate;
}

- (NSString *)trackListViewItemBitRate
{
    return self.p_value.trackListViewItemBitRate;
}

- (NSString *)trackListViewItemBitDepth
{
    return self.p_value.trackListViewItemBitDepth;
}

- (NSString *)trackListViewItemFileSize
{
    return self.p_value.trackListViewItemFileSize;
}

- (NSString *)trackListViewItemLength
{
    return self.p_value.trackListViewItemLength;
}

- (void)displayWaveformWithController:(NxAWaveformViewController *)controller
{
    NxATrack *track = self.p_value;

    if([track respondsToSelector:sel_registerName("displayWaveformWithController:")]) {
        [track displayWaveformWithController:controller];
    }
}

- (void)trackListViewItemAddURIToArray:(NSMutableArray *)uris
{
    NSURL *entryURI = [[self objectID] URIRepresentation];
    [uris addObject:entryURI];

    NxATrack *track = self.p_value;
    if([track respondsToSelector:sel_registerName("trackListViewItemAddURIToArray:")]) {
        [track trackListViewItemAddURIToArray:uris];
    }
}

- (void)trackListViewItemAddURLToArray:(NSMutableArray *)urls
{
    NxATrack *track = self.p_value;
    if([track respondsToSelector:sel_registerName("trackListViewItemAddURLToArray:")]) {
        [track trackListViewItemAddURLToArray:urls];
    }
}

@end
