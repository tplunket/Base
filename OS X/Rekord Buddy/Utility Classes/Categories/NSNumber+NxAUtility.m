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

#import "NSNumber+NxAUtility.h"

#pragma mark Implementation

@implementation NSNumber (NxAUtility)

+ (BOOL)numbersAreEqualOrBothNil:(NSNumber *)firstNumber
                    secondNumber:(NSNumber *)secondNumber
{
    if (!firstNumber) {
        return secondNumber == nil;
    }
    else if (!secondNumber) {
        return NO;
    }

    return [firstNumber isEqualToNumber:secondNumber];
}

#pragma mark Accessors

- (NSString *)stringValueAsTimeRepresentation
{
    NSInteger milliseconds = self.floatValue;
    NSInteger seconds = milliseconds / 1000;
    NSInteger minutes = seconds / 60;
    NSInteger hours = minutes / 60;

    milliseconds %= 1000;
    seconds %= 60;

    if (hours) {
        minutes %= 60;

        return [NSString stringWithFormat:@"%02ld:%02ld:%02ld:%03ld", hours, minutes, seconds, milliseconds];
    }
    else {
        return [NSString stringWithFormat:@"%02ld:%02ld:%03ld", minutes, seconds, milliseconds];
    }
}

@end
