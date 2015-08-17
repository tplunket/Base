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

#import "Track Collection/Tags/NxATag.h"

#pragma mark Public Interface

/*! Date property for a track. */
@interface NxADateProperty : NxATag

#pragma mark Class Methods

+ (NSString *)stringValueForDate:(NSDate *)date;

+ (NSDate *)dateFromStringValue:(NSString *)value;

#pragma mark Properties

@property (readonly, nonatomic) NSInteger numberOfSecondsSince1970InLocalTime;

@property (readonly, nonatomic) NSDate *date;

#pragma mark Instance Methods

/*!
 * Get a string representation of the date.
 * \param timeZone Name of the timezone the string should be in.
 * \param format Format used to output the string (for example "Y/M/D H:M:D").
 * \return A string with the representation.
 */
- (NSString *)stringValueWithTimeZone:(NSString *)timeZone
                               format:(NSString *)format;

/*!
 * Get a string representation of the date in the local timezone.
 * \param format Format used to output the string (for example "Y/M/D H:M:D").
 * \return A string with the representation.
 */
- (NSString *)stringValueInLocalTimeZoneWithFormat:(NSString *)format;

@end
