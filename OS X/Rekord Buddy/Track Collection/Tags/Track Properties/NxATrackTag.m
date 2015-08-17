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

#import "Track Collection/NxASortingCriteria.h"
#import "NxATrackBooleanTag.h"
#import "Track Collection/Tags/Track Properties/Internal/NxATrackTag.h"

#pragma mark Implementation

@implementation NxATrackTag

#pragma mark Overridden NxATag Factory Methods

+ (instancetype)p_tagWithValue:(id)value
                       context:(NSManagedObjectContext *)context
{
    NxAAssertTrue([value isKindOfClass:[NSString class]]);

    NxATrackTag *newTag = [super tagWithValue:value context:context];
    return newTag;
}

#pragma mark Overridden NxATag Factory Methods

+ (instancetype)tagWithValue:(id)value
                     context:(NSManagedObjectContext *)context
{
    NxATrackTag *newTag = nil;

    if (value) {
        NxAAssertTrue([value isKindOfClass:[NSString class]]);

        NSArray *components = [value componentsSeparatedByString:@":"];

        NxAAssertTrue(components.count <= 2);

        if (components.count == 1) {
            newTag = [NxATrackBooleanTag tagWithValue:value
                                              context:context];
        }
        else {
            newTag = [super tagWithName:components[0]
                                  value:components[1]
                                context:context];
        }
    }
    else {
        newTag = [super tagWithValue:nil
                             context:context];
    }

    NxAAssertNotNil(newTag);
    return newTag;
}

#pragma mark Overridden NxATag Class Methods

+ (NSString *)p_managedEntityName
{
    return @"TrackTag";
}

+ (BOOL)isATagTypeSortedByPropertyName
{
    return YES;
}

#pragma mark Overridden NxATag Accessors

- (NSString *)propertyDescription
{
    return self.p_tagName;
}

- (NSString *)stringValue
{
    return self.p_value;
}

#pragma mark Overridden NSObject Methods

- (NSString *)description
{
    if (self.p_tagName) {
        return [NSString stringWithFormat:@"%@:%@", self.p_tagName, self.p_value];
    }
    else {
        return self.p_value;
    }
}

@end
