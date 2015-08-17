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

#import "NxATag+NxAValidationAndCompletionQuerying.h"
#import "NxAIntegerProperty+NxAValidationAndCompletionQuerying.h"

#pragma mark Implementation

@implementation NxAIntegerProperty (NxAValidationAndCompletionQuerying)

#pragma mark NxAValidationAndCompletionQuerying Protocol

+ (NSCharacterSet *)validCharactersForDescription
{
    static NSCharacterSet *validCharacters = nil;
    static dispatch_once_t onceToken = 0;
    dispatch_once(&onceToken, ^{
        validCharacters = [NSCharacterSet decimalDigitCharacterSet];
    });

    return validCharacters;
}

+ (BOOL)isAValidNumberDescription:(NSNumber *)description
{
    return description.integerValue >= 0;
}

@end
