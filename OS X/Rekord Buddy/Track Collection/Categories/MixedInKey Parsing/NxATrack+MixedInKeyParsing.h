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

@class FMResultSet;

#import "NxAParserImplementation.h"

#pragma mark Public Interface

@interface NxATrack (MixedInKeyParsing)

#pragma mark Instance Methods

/*!
 * Parse a track from MixedInKey.
 * \param sqlEntry Entry in the main sql database for the track.
 * \param modificationDate Date the track was last modified.
 * \param parser Parser currenlty used to parse this.
 */
- (NxAParsingResultFlags)parseMixedInKeyTrackFromEntry:(FMResultSet *)sqlEntry
                                      modificationDate:(NSDate *)modificationDate
                                                parser:(id<NxAParserImplementation>)parser;

@end
