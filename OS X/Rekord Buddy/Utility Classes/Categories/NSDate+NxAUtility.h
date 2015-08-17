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

#pragma mark Public Interface

@interface NSDate (NxAUtility)

#pragma mark Class Methods

/*!
 * Get a date from a string.
 * \param dateString String parsed to create the date.
 * \param timeZone Name of the timezone the string date is in.
 * \param format Format used for the string (for example "Y/M/D H:M:D").
 * \return A date object with the date from the string.
 */
+ (NSDate *)dateWithString:(NSString *)dateString
                  timeZone:(NSString *)timeZone
                    format:(NSString *)format;

/*!
 * Get a date from a string using the local timeone.
 * \param dateString String parsed to create the date.
 * \param format Format used for the string (for example "Y/M/D H:M:D").
 * \return A date object with the date from the string.
 */
+ (NSDate *)dateWithStringInLocalTimeZone:(NSString *)dateString
                                   format:(NSString *)format;


#pragma mark Instance Methods

/*!
 * Get a date string.
 * \param timeZone Name of the timezone the string should be in.
 * \param format Format used to output the string (for example "Y/M/D H:M:D").
 * \return A string with the representation of the date object.
 */
- (NSString *)stringValueWithTimeZone:(NSString *)timeZone
                               format:(NSString *)format;

/*!
 * Get a string in the local timezone.
 * \param format Format used to output the string (for example "Y/M/D H:M:D").
 * \return A string with the representation of the date object.
 */
- (NSString *)stringValueInLocalTimeZoneWithFormat:(NSString *)format;

#pragma mark Instance Methods

- (NSDate *)roundedDownToTheNearestSecond;

@end
