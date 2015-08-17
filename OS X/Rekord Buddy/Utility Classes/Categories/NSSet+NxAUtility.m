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

#import "NSSet+NxAUtility.h"

#pragma mark Implementation

@implementation NSSet (NxAUtility)

#pragma mark Factory Methods

+ (instancetype)setWithStringValuesFromSet:(NSSet *)set
{
    NSMutableSet *newSet = [NSMutableSet setWithCapacity:set.count];

    for (id object in set) {
        [newSet addObject:[object stringValue]];
    }

    return newSet;
}

+ (instancetype)setWithDescriptionsFromSet:(NSSet *)set
{
    NSMutableSet *newSet = [NSMutableSet setWithCapacity:set.count];

    for (id object in set) {
        [newSet addObject:[object description]];
    }

    return newSet;
}

@end
