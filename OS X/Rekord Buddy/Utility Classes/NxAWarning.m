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

#import "Utility Classes/Internal/NxAWarning.h"

#pragma mark Implementation

@implementation NxAWarning

#pragma mark Factory Methods

+ (instancetype)warningWithDescription:(NSString *)description
{
    NxAAssertNotNil(description);

    return [NxAWarning warningWithDescription:description additionalUrl:nil];
}

+ (instancetype)warningWithDescription:(NSString *)description
                         additionalUrl:(NSString *)url
{
    NxAAssertNotNil(description);
    
    NxAWarning *warning = [[NxAWarning alloc] init];
    if (warning) {
        warning.p_description = description;
        warning.additionalUrl = url;
    }
    
    return warning;
}

#pragma mark Instance Methods

- (BOOL)isEqual:(id)anObject
{
    NxAWarning *other = (NxAWarning *)anObject;
    
    return [self.description isEqualTo:other.description] &&
           [self.additionalUrl isEqualTo:other.additionalUrl];
}

- (NSUInteger)hash
{
    NSString *combined = [NSString stringWithFormat:@"%@%@",
                          self.p_description,
                          self.additionalUrl];
    
    return combined.hash;
}

#pragma mark Overridden NSObject Methods

- (NSString *)description
{
    return self.p_description;
}

#pragma mark NSCopying delegate methods

- (id)copyWithZone:(NSZone *)zone
{
    NxAWarning *copy = [[[self class] allocWithZone:zone] init];
    
    copy.p_description = self.p_description;
    copy.additionalUrl = self.additionalUrl;
    
    return copy;
}

@end
