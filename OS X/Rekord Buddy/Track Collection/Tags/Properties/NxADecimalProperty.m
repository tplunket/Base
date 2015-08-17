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

#import "NxADecimalProperty.h"
#import "NSManagedObjectContext+NxAUtility.h"
#import "Track Collection/Tags/Internal/NxATag.h"

#pragma mark Private Interface

@interface NxADecimalProperty ()

#pragma mark Properties

/*! Value of the property. */
@property (strong, nonatomic) NSDecimalNumber *p_value;

@end

#pragma mark - Implementation

@implementation NxADecimalProperty

#pragma mark Overridden NxATag Class Methods

+ (NSString *)p_managedEntityName
{
    return @"DecimalProperty";
}

+ (Class)p_valueClass
{
    return [NSDecimalNumber class];
}

#pragma mark Accessors

// -- Automatically generated for Core Data properties.
@dynamic p_value;

- (NSString *)timeStringValue
{
    NSDecimalNumberHandler *handler = [NSDecimalNumberHandler decimalNumberHandlerWithRoundingMode:NSRoundDown
                                                                                             scale:0
                                                                                  raiseOnExactness:NO
                                                                                   raiseOnOverflow:NO
                                                                                  raiseOnUnderflow:NO
                                                                               raiseOnDivideByZero:NO];

    NSDecimalNumber *sixty = [NSDecimalNumber decimalNumberWithString:@"60"];

    NSDecimalNumber *seconds = [self.value decimalNumberByRoundingAccordingToBehavior:handler];
    NSDecimalNumber *minutes = [[seconds decimalNumberByDividingBy:sixty] decimalNumberByRoundingAccordingToBehavior:handler];
    NSDecimalNumber *hours = [[minutes decimalNumberByDividingBy:sixty] decimalNumberByRoundingAccordingToBehavior:handler];

    seconds = [seconds decimalNumberBySubtracting:[minutes decimalNumberByMultiplyingBy:sixty]];
    minutes = [minutes decimalNumberBySubtracting:[hours decimalNumberByMultiplyingBy:sixty]];

    NSString *lengthString = nil;
    if (hours.doubleValue != 0.0) {
        lengthString = [NSString stringWithFormat:@"%02ld:", hours.stringValue.integerValue];
    }
    else {
        lengthString = @"";
    }

    lengthString = [lengthString stringByAppendingString:[NSString stringWithFormat:@"%02ld:%02ld",
                                                          minutes.stringValue.integerValue,
                                                          seconds.stringValue.integerValue]];

    return lengthString;
}

- (NSString *)stringValueWithFractionDigits:(NSUInteger)fractionDigits
{
    static NSNumberFormatter *formatter = nil;
    static dispatch_once_t onceToken = 0;
    dispatch_once(&onceToken, ^{
        formatter = [[NSNumberFormatter alloc] init];
        formatter.minimumFractionDigits = 0;
        formatter.locale = [[NSLocale alloc] initWithLocaleIdentifier:@"en_US"];
    });

    formatter.maximumFractionDigits = fractionDigits;

    return [formatter stringFromNumber:self.p_value];
}

#pragma mark Overridden NxATag Accessors

- (NSString *)stringValue
{
    return self.p_value.stringValue;
}

@end
