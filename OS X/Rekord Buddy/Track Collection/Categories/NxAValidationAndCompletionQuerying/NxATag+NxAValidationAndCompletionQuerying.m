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

#import "NSCharacterSet+NxAUtility.h"
#import "NxATag+NxAValidationAndCompletionQuerying.h"

#pragma mark Implementation

@implementation NxATag (NxAValidationAndCompletionQuerying)

#pragma mark NxAValidationAndCompletionQuerying Protocol

+ (NSArray *)itemsWithNamesBeginningBy:(NSString *)subString
                               context:(NSManagedObjectContext *)context
{
    ALog(@"Invalid base class method call.");

    return nil;
}

+ (NSCharacterSet *)validCharactersForDescription
{
    return nil;
}

+ (BOOL)isAValidStringDescription:(NSString *)description
{
    return NO;
}

+ (BOOL)isAValidNumberDescription:(NSNumber *)description
{
    return NO;
}

+ (BOOL)canContainMultipleValues
{
    return NO;
}

@end
