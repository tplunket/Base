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

@import ObjectiveC;

#import "NxATrackBooleanTag.h"
#import "Track Collection/Tags/Track Properties/Internal/NxATrackTag.h"

#pragma mark Implementation

@implementation NxATrackBooleanTag

#pragma mark Overridden NxATag Factory Methods

+ (instancetype)tagWithValue:(id)value
                     context:(NSManagedObjectContext *)context
{
    if (!value) {
        // -- It makes no sense to have a boolean tag with no value,
        // -- so we swap it with a regular track tag instead.
        return [super tagWithValue:nil context:context];
    }

    NxAAssertNotNil(value);
    NxAAssertTrue([value isKindOfClass:[NSString class]]);

    NxATrackBooleanTag *newTag = [super p_tagWithValue:value
                                               context:context];
    NxAAssertNotNil(newTag);

    return newTag;
}

+ (instancetype)tagWithName:(NSString *)name
                      value:(id)value
                    context:(NSManagedObjectContext *)context
{
    if (!value) {
        // -- It makes no sense to have a boolean tag with no value,
        // -- so we swap it with a regular track tag instead.
        return [super tagWithValue:nil context:context];
    }

    NxAAssertNotNil(value);
    NxAAssertTrue([value isKindOfClass:[NSString class]]);

    NxATrackBooleanTag *newTag = [super p_tagWithValue:[NSString stringWithFormat:@"%@:%@", name, value]

                                               context:context];
    NxAAssertNotNil(newTag);

    return newTag;
}

#pragma mark Overridden NxATag Class Methods

+ (NSString *)p_managedEntityName
{
    return @"TrackBooleanTag";
}

+ (BOOL)isABooleanTagType
{
    return YES;
}

+ (BOOL)isASingleValueTagType
{
    return NO;
}

#pragma mark Overridden NxATag Accessors

- (NSString *)propertyDescription
{
    return self.p_value;
}

+ (NSString *)tagType
{
    Class parentClass = class_getSuperclass([self class]);
    return NSStringFromClass(parentClass);
}

#pragma mark Overridden NSObject Methods

- (NSString *)description
{
    return self.p_value;
}

@end
