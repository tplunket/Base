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

#import "Track Collection/Crates/NxACrate.h"

#pragma mark Public Interface

/*! Music crate, which contains all the tracks its sibbling tracks don't. */
@interface NxAOtherTracksPlaylist : NxACrate

#pragma mark Class Methods.

/*!
 * Find a child NxAOtherTracksPlaylist in a crate.
 * \return If found, the NxAOtherTracksPlaylist or nil otherwise.
 */
+ (NxAOtherTracksPlaylist *)findOtherTrackPlaylistIn:(NxACrate *)crate;

@end
