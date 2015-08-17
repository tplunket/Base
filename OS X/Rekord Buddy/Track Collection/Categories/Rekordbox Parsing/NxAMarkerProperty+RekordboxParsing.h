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

#import "NxAMarkerProperty.h"
#import "NxAMessageLog.h"

#pragma mark Public Interface

@interface NxAMarkerProperty (RekordboxParsing_Private)

#pragma mark Class Methods

/*!
 * Find out the offset to use for a track with a given track element.
 * \param trackElement Trrack element for the track.
 * \return An offset to use for that track or nil if no offset.
 */
+ (NSDecimalNumber *)offsetForTrackElement:(NSXMLElement *)trackElement;

/*!
 * Update Rekordbox marker elements.
 * \param markers Markers to update the Rekordbox track.
 * \param trackElement Track element where to replace the markers.
 * \param log Log to use for errors and warnings.
 */
+ (void)updateRekordboxMarkers:(NSArray *)markers
                  trackElement:(NSXMLElement *)trackElement
                           log:(NxAMessageLog *)log;

@end
