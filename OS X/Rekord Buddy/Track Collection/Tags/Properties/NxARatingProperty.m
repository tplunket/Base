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

#import "Track Collection/Tags/Properties/NxAIntegerProperty.h"
#import "NxARatingProperty.h"
#import "Track Collection/Tags/Properties/Internal/NxAIntegerProperty.h"

#pragma mark Implementation

@implementation NxARatingProperty

#pragma mark Overridden NxATag Class Methods

+ (NSString *)p_classEntityName
{
    return @"RatingProperty";
}

+ (Class)p_valueClass
{
    return [NSNumber class];
}

#pragma mark Overridden NxATag Accessors

- (NSString *)description
{
    NSInteger value = self.p_value.integerValue;
    switch (value) {
        case 0: {
            return @"0 stars";
        }
        case 1: {
            return @"1 star";
        }
        case 2: {
            return @"2 stars";
        }
        case 3: {
            return @"3 stars";
        }
        case 4: {
            return @"4 stars";
        }
        case 5: {
            return @"5 stars";
        }
    }

    ALog(@"Illegal rating value '%ld'.", value);

    return nil;
}

@end
