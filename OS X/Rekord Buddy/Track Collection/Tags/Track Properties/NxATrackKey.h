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

#import "Track Collection/Tags/Properties/NxAIntegerProperty.h"

/*!
 *  enum NxAMusicalNotation
 *  Different musical notations supported by the key property.
 */
typedef NS_ENUM(NSInteger, NxAMusicalNotation) {
    NxAMusicalNotationStandard,          /*!< Standard musical notation. */
    NxAMusicalNotationStandardAllSharps, /*!< Standard musical notation with only sharps. */
    NxAMusicalNotationCamelot,           /*!< Camelot musical notation (used by Mixed-in-key). */
    NxAMusicalNotationOpenKey,           /*!< OpenKey musical notation. */
};

#pragma mark Public Interface

/*! Key property for a track. */
@interface NxATrackKey : NxAIntegerProperty

#pragma mark Class Methods

+ (BOOL)stringIsAValidKeyValue:(NSString *)value;

+ (NSNumber *)keyValueForString:(NSString *)value;

+ (NSString *)stringValueForKeyValue:(NSNumber *)value
                       usingNotation:(NxAMusicalNotation)notation;

#pragma mark Instance Methods

/*!
 * Get the string value.
 * \param notation Notation to use for the string value.
 * \return The string value of the key using the given notation.
 */
- (NSString *)stringValueForNotation:(NxAMusicalNotation)notation;

@end
