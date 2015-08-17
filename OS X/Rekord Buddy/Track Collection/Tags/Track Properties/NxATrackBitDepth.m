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

#import "NxATrackBitDepth.h"
#import "Track Collection/Tags/Properties/Internal/NxAIntegerProperty.h"

#pragma mark Implementation

@implementation NxATrackBitDepth

#pragma mark Overridden NxATag Class Methods

+ (NSString *)p_managedEntityName
{
    return @"TrackBitDepth";
}

+ (BOOL)isASingleValueTagType
{
    // -- Only one bit depth at a time can be added to a tagged object.
    return YES;
}

+ (BOOL)valuesAreUserModifiable
{
    return NO;
}

#pragma mark Overridden NxATag Accessors

- (NSString *)propertyDescription
{
    return @"Bit Depth";
}

- (NSString *)description
{
    return [NSString stringWithFormat:@"%ld bits", self.p_value.integerValue];
}

@end
