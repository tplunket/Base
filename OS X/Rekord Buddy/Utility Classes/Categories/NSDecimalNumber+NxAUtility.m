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

#import "NSDecimalNumber+NxAUtility.h"

#pragma mark Implementation

@implementation NSDecimalNumber (NxAUtility)

#pragma mark Factory Methods

+ (NSDecimalNumber *)decimalNumberWithDouble:(double)doubleNumber
{
    NSString *string = [NSString stringWithFormat:@"%f", doubleNumber];

    NSDecimalNumber *number = [NSDecimalNumber decimalNumberWithString:string];
    
    return number;
}

+ (NSDecimalNumber *)decimalNumberWithUnlocalizedString:(NSString *)numberString
{
    // -- First we make sure that the decimal seperator is a dot.
    NSString *localizedNumber = [numberString stringByReplacingOccurrencesOfString:@"," withString:@"."];
    
    // -- Then we convert the localized number.
    NSDecimalNumber *number = [NSDecimalNumber decimalNumberWithString:localizedNumber locale:@{ NSLocaleDecimalSeparator : @"." }];

    return number;
}

+ (NSDecimalNumber *)decimalNumberWithUnlocalizedAttribute:(NSXMLNode *)attribute;
{
    if (!attribute) {
        return nil;
    }
    
    return [NSDecimalNumber decimalNumberWithUnlocalizedString:attribute.stringValue];
}

@end
