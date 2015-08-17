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

#import "NxATrackSampleRate.h"
#import "Track Collection/Tags/Properties/Internal/NxAIntegerProperty.h"

#pragma mark Implementation

@implementation NxATrackSampleRate

#pragma mark Overridden NxATag Class Methods

+ (NSString *)p_managedEntityName
{
    return @"TrackSampleRate";
}

+ (BOOL)isASingleValueTagType
{
    // -- Only one sample rate at a time can be added to a tagged object.
    return YES;
}

+ (BOOL)valuesAreUserModifiable
{
    return NO;
}

#pragma mark Overridden NxATag Accessors

- (NSString *)propertyDescription
{
    return @"Sample Rate";
}

- (NSString *)description
{
    NSInteger rate = self.p_value.integerValue;

    NSString *rateString;
    if (rate > 1000) {
        rateString = [NSString stringWithFormat:@"%0.1f KHz", (float)self.p_value.integerValue / 1000.0];
    }
    else {
        rateString = [NSString stringWithFormat:@"%ld Hz", self.p_value.integerValue];
    }

    return rateString;
}

@end
