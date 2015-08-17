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

extern NSString *const NxAKeyPropertyRekordboxKeyName;

#pragma mark - Public Interface

@interface NxATrackKey (RekordboxParsing)

#pragma mark Class Methods

/*!
 * Update Rekordbox ENTRY element.
 * \param key Key to update.
 * \param element XML element for the entry to update in the Rekordbox collection file.
 */
+ (void)updateRekordboxKeyElementForKey:(NxATrackKey *)key
                              inElement:(NSXMLElement *)element;

@end
