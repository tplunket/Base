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

#import "Track Collection/NxAArtwork.h"

#pragma mark Private Interface

@interface NxAArtwork ()

#pragma mark Properties

/*!
 * Checksum of the image data used in searches and indexing.
 * \details This is a 128bit hash value stored as a string.
 */
@property (strong, nonatomic) NSString *p_hash;

/*! Image used for this artwork. */
@property (strong, nonatomic) NSImage *p_image;

/*! Tracks using this artwork. */
@property (strong, nonatomic) NSSet *p_tracks;

/*! Artists using this artwork. */
@property (strong, nonatomic) NSSet *p_artists;

/*! Record labels using this artwork. */
@property (strong, nonatomic) NSSet *p_recordLabels;

@end
