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
#import "Track Collection/Tags/Properties/Internal/NxAIntegerProperty.h"

#pragma mark Constants

/*!
 *  enum NxAKeyValue
 *  Musical keys for tracks.
 */
typedef NS_ENUM(NSUInteger, NxAKeyValue) {
    NxAMusicalKeyC,
    NxAMusicalKeyCsDb,
    NxAMusicalKeyD,
    NxAMusicalKeyDsEb,
    NxAMusicalKeyE,
    NxAMusicalKeyF,
    NxAMusicalKeyFsGb,
    NxAMusicalKeyG,
    NxAMusicalKeyGsAb,
    NxAMusicalKeyA,
    NxAMusicalKeyAsBb,
    NxAMusicalKeyB,
    NxAMusicalKeyCm,
    NxAMusicalKeyCsmDbm,
    NxAMusicalKeyDm,
    NxAMusicalKeyDsmEbm,
    NxAMusicalKeyEm,
    NxAMusicalKeyFm,
    NxAMusicalKeyFsmGbm,
    NxAMusicalKeyGm,
    NxAMusicalKeyGsmAbm,
    NxAMusicalKeyAm,
    NxAMusicalKeyAsmBbm,
    NxAMusicalKeyBm,
    NxAMusicalKeyMaxIndex = NxAMusicalKeyBm
};

#pragma mark - Private Interface

@interface NxATrackKey ()

#pragma mark Class Methods

/*!
 * A master translation table to go from supported key strings to key values.
 * \return A dictionary to translate key names into key values.
 */
+ (NSDictionary *)p_keyPreferenceNames;

@end
