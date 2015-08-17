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

#import "Track Collection/Tags/Track Properties/NxATrackKey.h"

#pragma mark Constants

extern NSString *const NxAKeyPropertyTraktorKeyName;

#pragma mark - Public Interface

@interface NxATrackKey (TraktorParsing)

#pragma mark Class Methods

/*!
 * Convert a Traktor key value to an internal key value.
 * \param value Traktor key value to convert.
 * \return A key value, if traktor key is supported or nil otherwise.
 */
+ (NSNumber *)keyValueForTraktorKeyValue:(NSUInteger)value;

#pragma mark Instance Methods

/*!
 * Update Traktor ENTRY element.
 * \param entryElement XML element for the entry to update in the Traktor collection file.
 */
- (void)updateTraktorKeyElementInEntry:(NSXMLElement *)entryElement;

/*!
 * Update Traktor INFO element.
 * \param infoElement XML element for the info to update in the Traktor track.
 */
- (void)updateTraktorKeyElementInInfoElement:(NSXMLElement *)infoElement;

@end
