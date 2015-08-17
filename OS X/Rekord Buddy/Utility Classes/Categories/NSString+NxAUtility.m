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

#import "NSString+NxAUtility.h"
#import "NSSet+NxAUtility.h"
#import "NSOrderedSet+NxAUtility.h"

#pragma mark Implementation

@implementation NSString (NxAUtility)

#pragma mark Factory Methods

+ (instancetype)stringSeparatedBy:(NSString *)seperator
          withStringValuesFromSet:(NSSet *)set
{
    NSString *result = [[[NSSet setWithStringValuesFromSet:set] allObjects] componentsJoinedByString:seperator];
    return result;
}

+ (instancetype)stringSeparatedBy:(NSString *)seperator
   withStringValuesFromOrderedSet:(NSOrderedSet *)set
{
    NSString *result = [[[NSOrderedSet orderedSetWithStringValuesFromOrderedSet:set] array] componentsJoinedByString:seperator];
    return result;
}

+ (instancetype)stringSeparatedBy:(NSString *)seperator
        withStringValuesFromArray:(NSArray *)array
{
    NSMutableArray *stringValuesArray = [NSMutableArray arrayWithCapacity:array.count];

    for (id object in array) {
        [stringValuesArray addObject:[object stringValue]];
    }

    NSString *result = [stringValuesArray componentsJoinedByString:seperator];
    return result;
}

#pragma mark Class Methods

+ (BOOL)stringsAreEqualOrBothNil:(NSString *)firstString
                    secondString:(NSString *)secondString
{
    if (!firstString) {
        return secondString == nil;
    }
    else if (!secondString) {
        return NO;
    }

    return [firstString isEqualToString:secondString];
}

#pragma mark Instance Methods

- (NSString *)stringByCenteringInAStringOfAtLeastWidth:(NSUInteger)width
{
    NSString *result = self;

    NSInteger sidePadding = (width - result.length - 1) / 2;
    if (sidePadding > 0) {
        for (NSUInteger index = 0; index < sidePadding; ++index) {
            result = [NSString stringWithFormat:@" %@ ", result];
        }
    }

    return result;
}

@end
