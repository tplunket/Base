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

#import "NSDate+NxAUtility.h"

#pragma mark Constants

static NSString *p_localTimeZoneName = nil;

#pragma mark Implementation

@implementation NSDate (NxAUtility)

#pragma mark Class Methods

+ (void)p_setTestingLocalTimeZone:(NSString *)timeZone
{
    NSDateFormatter *dateFormatter = [NSDate p_dateFormatter];
    @synchronized(dateFormatter) {
        p_localTimeZoneName = timeZone;
    }
}

+ (NSDateFormatter *)p_dateFormatter
{
    static NSDateFormatter *dateFormatter = nil;
    static dispatch_once_t onceToken = 0;
    dispatch_once(&onceToken, ^{
        dateFormatter = [[NSDateFormatter alloc] init];
        NxAAssertNotNil(dateFormatter);
    });

    return dateFormatter;
}

/*!
 * Get a date from a string.
 * \param dateString String parsed to create the date.
 * \param timeZone Name of the timezone the string date is in or nil for local timezone.
 * \param format Format used for the string (for example "Y/M/D H:M:D").
 * \return A date object with the date from the string.
 */
+ (NSDate *)p_dateWithString:(NSString *)dateString
                  timeZone:(NSString *)timeZone
                    format:(NSString *)format
{
    NSDateFormatter *dateFormatter = [NSDate p_dateFormatter];
    @synchronized(dateFormatter) {
        NSTimeZone *timeZoneValue = timeZone ? [NSTimeZone timeZoneWithName:timeZone] : [NSTimeZone localTimeZone];
        NxAAssertNotNil(timeZoneValue);

        dateFormatter.timeZone = timeZoneValue;
        dateFormatter.dateFormat = format;

        return [dateFormatter dateFromString:dateString];
    }
}

+ (NSDate *)dateWithString:(NSString *)dateString
                  timeZone:(NSString *)timeZone
                    format:(NSString *)format
{
    NxAAssertNotNil(dateString);
    NxAAssertNotNil(timeZone);
    NxAAssertNotNil(format);

    return [NSDate p_dateWithString:dateString timeZone:timeZone format:format];
}

+ (NSDate *)dateWithStringInLocalTimeZone:(NSString *)dateString
                                   format:(NSString *)format
{
    return [NSDate p_dateWithString:dateString timeZone:p_localTimeZoneName format:format];
}

#pragma mark Instance Methods

/*!
 * Get a date string.
 * \param timeZone Name of the timezone the string should be in or nil for local timezone.
 * \param format Format used to output the string (for example "Y/M/D H:M:D").
 * \return A string with the representation of the date object.
 */
- (NSString *)p_stringValueWithTimeZone:(NSString *)timeZone
                                 format:(NSString *)format
{
    NSDateFormatter *dateFormatter = [NSDate p_dateFormatter];
    @synchronized(dateFormatter) {
        NSTimeZone *timeZoneValue = timeZone ? [NSTimeZone timeZoneWithName:timeZone] : [NSTimeZone localTimeZone];
        NxAAssertNotNil(timeZoneValue);

        dateFormatter.timeZone = timeZoneValue;
        dateFormatter.dateFormat = format;

        return [dateFormatter stringFromDate:self];
    }
}

- (NSString *)stringValueWithTimeZone:(NSString *)timeZone
                               format:(NSString *)format
{
    NxAAssertNotNil(timeZone);
    NxAAssertNotNil(format);

    return [self p_stringValueWithTimeZone:timeZone format:format];
}

- (NSString *)stringValueInLocalTimeZoneWithFormat:(NSString *)format
{
    return [self p_stringValueWithTimeZone:p_localTimeZoneName format:format];
}

- (NSDate *)roundedDownToTheNearestSecond
{
    NSTimeInterval interval = self.timeIntervalSince1970;
    return [NSDate dateWithTimeIntervalSince1970:floor(interval)];
}

@end
