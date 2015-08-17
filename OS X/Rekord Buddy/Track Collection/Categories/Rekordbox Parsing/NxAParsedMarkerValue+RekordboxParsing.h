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

@class NxAMessageLog;

#import "NxAParsedMarkerValue.h"

#pragma mark Public Interface

@interface NxAParsedMarkerValue (RekordboxParsing)

#pragma mark Factory Methods

/*!
 * Create a marker value.
 * \param positionElement Position element read from Rekordbox.
 * \param offset Offset (if any) for the start position of this marker.
 * \param log Log for errors and warnings.
 * \return A marker value or nil if failed.
 */
+ (instancetype)markerValueWithRekordboxPositionElement:(NSXMLElement *)positionElement
                                                 offset:(NSDecimalNumber *)offset
                                                    log:(NxAMessageLog *)log;

/*!
 * Create a grid marker value.
 * \param tempoMarker Rekordbox tempo marker to use as the source for this marker.
 * \param offset Offset (if any) for the start position of this marker.
 * \param log Log for errors and warnings.
 * \return A grid marker value or nil if failed.
 */
+ (instancetype)gridMarkerValueWithRekordboxTempoElement:(NSXMLElement *)tempoMarker
                                                  offset:(NSDecimalNumber *)offset
                                                     log:(NxAMessageLog *)log;

#pragma mark Class Mathods

/*!
 * Parse Rekordbox markers elements.
 * \param trackElement Track element where read the markers from.
 * \return Track markers corresponding to the Rekordbox markers, or nil if failed.
 */
+ (NSArray *)parseAndSortRekordboxMarkerValuesInTrackElement:(NSXMLElement *)trackElement
                                                         log:(NxAMessageLog *)log;

@end
