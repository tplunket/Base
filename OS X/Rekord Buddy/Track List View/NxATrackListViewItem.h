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

@class NxAWaveformViewController;

#pragma mark Protocol

/*! An item displayed in the main document window's track list */
@protocol NxATrackListViewItem <NSObject>

@required

#pragma mark Properties

/*! Whether the item is a group item or not. */
@property (readonly, nonatomic) BOOL trackListViewItemIsGroupItem;

/*! Children, if any, of this item. */
@property (readonly, nonatomic) NSArray *trackListViewItemChildren;

@optional

/*! Name of the identifier to use for this item's cell. */
@property (readonly, nonatomic) NSString *trackListViewItemCellIdentifier;

/*! Title of the track. */
@property (strong, nonatomic) NSString *trackListViewItemTitle;

/*! Comments for the track. */
@property (strong, nonatomic) NSString *trackListViewItemComments;

/*! Artwork for the track. */
@property (strong, nonatomic) NSImage *trackListViewItemArtwork;

/*!
 * Artists for the track.
 * \details This is an array of NSString.
 */
@property (strong, nonatomic) NSArray *trackListViewItemArtists;

/*!
 * Remixers for the track.
 * \details This is an array of NSString.
 */
@property (strong, nonatomic) NSArray *trackListViewItemRemixers;

/*!
 * Producers for the track.
 * \details This is an array of NSString.
 */
@property (strong, nonatomic) NSArray *trackListViewItemProducers;

/*!
 * Tags for the track.
 * \details This is an array of NSString.
 */
@property (strong, nonatomic) NSArray *trackListViewItemTags;

/*!
 * Musical keys for the track.
 * \details This is an array of NSString.
 */
@property (strong, nonatomic) NSArray *trackListViewItemKeys;

/*! Rating for the track. */
@property (strong, nonatomic) NSNumber *trackListViewItemRating;

/*!
 * Musical genres for the track.
 * \details This is an array of NSString.
 */
@property (strong, nonatomic) NSArray *trackListViewItemGenres;

/*! Record Label for the track. */
@property (strong, nonatomic) NSString *trackListViewItemRecordLabel;

/*! Date the track was added. */
@property (strong, nonatomic) NSDate *trackListViewItemDateAdded;

/*! Date the track was last modified. */
@property (readonly, nonatomic) NSDate *trackListViewItemDateModified;

/*! Date the track was released. */
@property (strong, nonatomic) NSDate *trackListViewItemDateReleased;

/*! Name of the mix for the track. */
@property (strong, nonatomic) NSString *trackListViewItemMixName;

/*! Number of plays for the track. */
@property (strong, nonatomic) NSNumber *trackListViewItemPlayCount;

/*! Bit rate for the track. */
@property (readonly, nonatomic) NSString *trackListViewItemBitRate;

/*! Bit depth for the track. */
@property (readonly, nonatomic) NSString *trackListViewItemBitDepth;

/*! Sample rate for the track. */
@property (readonly, nonatomic) NSString *trackListViewItemSampleRate;

/*! File size for the track. */
@property (readonly, nonatomic) NSString *trackListViewItemFileSize;

/*! Album for the track. */
@property (readonly, nonatomic) NSString *trackListViewItemAlbum;

/*! Track number for the track inside its album. */
@property (strong, nonatomic) NSNumber *trackListViewItemTrackNumber;

/*! Number of tracks inside its album. */
@property (readonly, nonatomic) NSNumber *trackListViewItemTrackCount;

/*! Length of the track in minutes and seconds. */
@property (readonly, nonatomic) NSString *trackListViewItemLength;

/*!
 * Parent crates names for this track.
 * \details This is an ordered set of NSString.
 */
@property (readonly, nonatomic) NSSet *trackListViewItemParentCrates;

#pragma mark Instance Methods

@optional

/*!
 * Display the track list view item with a waveform controller.
 * \param controller Waveform controller to add the item to.
 */
- (void)displayWaveformWithController:(NxAWaveformViewController *)controller;

/*!
 * Add the URI for this item to an array.
 * \param uris Array to add the URI to.
 */
- (void)trackListViewItemAddURIToArray:(NSMutableArray *)uris;

/*!
 * Add the URL for this item to an array.
 * \param urls Array to add the URL to.
 */
- (void)trackListViewItemAddURLToArray:(NSMutableArray *)urls;

@end
