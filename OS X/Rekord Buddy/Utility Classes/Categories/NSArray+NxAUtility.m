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

#import "NSArray+NxAUtility.h"

#pragma mark Implementation

@implementation NSArray (NxAUtility)

#pragma mark Factory Methods

+ (instancetype)arrayWithStringValuesFromOrderedSet:(NSOrderedSet *)set
{
    NSMutableArray *newArray = [NSMutableArray arrayWithCapacity:set.count];

    for (id object in set) {
        NSString *stringValue = [object stringValue];
        if (stringValue) {
            [newArray addObject:stringValue];
        }
    }

    return newArray;
}

+ (instancetype)arrayWithStringValuesFromArray:(NSOrderedSet *)array
{
    NSMutableArray *newArray = [NSMutableArray arrayWithCapacity:array.count];

    for (id object in array) {
        NSString *stringValue = [object stringValue];
        if (stringValue) {
            [newArray addObject:stringValue];
        }
    }

    return newArray;
}

+ (instancetype)arrayWithDescriptionsFromOrderedSet:(NSSet *)set
{
    NSMutableArray *newArray = [NSMutableArray arrayWithCapacity:set.count];

    for (id object in set) {
        NSString *description = [object description];
        if (description) {
            [newArray addObject:description];
        }
    }

    return newArray;
}

+ (instancetype)arrayWithDescriptionsFromSet:(NSSet *)set
{
    NSMutableArray *newArray = [NSMutableArray arrayWithCapacity:set.count];

    for (id object in set) {
        NSString *description = [object description];
        if (description) {
            [newArray addObject:[object description]];
        }
    }

    return newArray;
}

+ (instancetype)arrayWithDescriptionsFromArray:(NSArray *)array
{
    NSMutableArray *newArray = [NSMutableArray arrayWithCapacity:array.count];

    for (id object in array) {
        NSString *description = [object description];
        if (description) {
            [newArray addObject:description];
        }
    }

    return newArray;
}

@end
