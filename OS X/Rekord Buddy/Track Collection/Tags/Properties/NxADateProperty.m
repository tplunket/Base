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

#import "NSManagedObjectContext+NxAUtility.h"
#import "NSDate+NxAUtility.h"
#import "Track Collection/Tags/Properties/Internal/NxADateProperty.h"

#pragma mark Constants

#define NXA_DATE_PROPERTY_DATE_FORMAT               @"yyyy/MM/dd HH:mm:ss"

#pragma mark - Implementation

@implementation NxADateProperty

#pragma mark Overidden Factory Methods

+ (instancetype)tagWithName:(NSString *)name value:(id)value context:(NSManagedObjectContext *)context
{
    if (!value) {
        return [super tagWithName:name value:nil context:context];
    }

    NxAAssertTrue([value isKindOfClass:[NSDate class]]);

    NSDate *date = (NSDate *)value;
    return [super tagWithName:name value:[date roundedDownToTheNearestSecond] context:context];
}

+ (instancetype)tagWithValue:(id)value context:(NSManagedObjectContext *)context
{
    if (!value) {
        return [super tagWithValue:nil context:context];
    }

    NxAAssertTrue([value isKindOfClass:[NSDate class]]);

    NSDate *date = (NSDate *)value;
    return [super tagWithValue:[date roundedDownToTheNearestSecond] context:context];
}

#pragma mark Class Methods

+ (NSString *)stringValueForDate:(NSDate *)date
{
    NxAAssertNotNil(date);
    
    return [date stringValueInLocalTimeZoneWithFormat:NXA_DATE_PROPERTY_DATE_FORMAT];
}

+ (NSDate *)dateFromStringValue:(NSString *)value
{
    NxAAssertNotNil(value);

    return [NSDate dateWithStringInLocalTimeZone:value format:NXA_DATE_PROPERTY_DATE_FORMAT];
}

#pragma mark Overridden NxATag Class Methods

+ (NSString *)p_managedEntityName
{
    return @"DateProperty";
}

+ (Class)p_valueClass
{
    return [NSDate class];
}

#pragma mark Accessors

// -- Automatically generated for Core Data properties.
@dynamic p_value;

- (NSInteger)numberOfSecondsSince1970InLocalTime
{
    return [self.p_value timeIntervalSince1970];
}

- (NSDate *)date
{
    return self.p_value;
}

#pragma mark Instance Methods

- (NSString *)stringValueWithTimeZone:(NSString *)timeZone
                               format:(NSString *)format
{
    return [self.p_value stringValueWithTimeZone:timeZone format:format];
}

- (NSString *)stringValueInLocalTimeZoneWithFormat:(NSString *)format
{
    return [self.p_value stringValueInLocalTimeZoneWithFormat:format];
}

#pragma mark Overridden NxATag Accessors

- (NSString *)stringValue
{
    return [NxADateProperty stringValueForDate:self.p_value];
}

@end
