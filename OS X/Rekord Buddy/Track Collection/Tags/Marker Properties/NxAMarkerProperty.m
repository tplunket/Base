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

#import "Track Collection/Tags/Marker Properties/Internal/NxAMarkerProperty.h"
#import "NSManagedObjectContext+NxAUtility.h"

#pragma mark Implementation

@implementation NxAMarkerProperty

#pragma mark Accessors

// -- Automatically generated for Core Data properties.
@dynamic p_pos;
@dynamic p_customFlagStrings;

#pragma mark Factory Methods

+ (instancetype)p_markerAtPosition:(NSDecimalNumber *)position
                         inContext:(NSManagedObjectContext *)context
{
    NxAAssertNotNil(position);

    NxAMarkerProperty *newMarker = [context insertNewObjectForEntity:self.p_managedEntityName];
    NxAAssertNotNil(newMarker);

    newMarker.p_pos = position;

    return newMarker;
}

#pragma mark Overridden NxATag Class Methods

+ (NSString *)tagType
{
    return @"MarkerProperty";
}

#pragma mark Instance Methods

- (BOOL)p_hasCustomFlag:(NSString *)flag
{
    return [self.p_customFlagStrings containsObject:flag];
}

- (void)p_addCustomFlag:(NSString *)flag
{
    if ([self p_hasCustomFlag:flag]) {
        return;
    }

    NSMutableSet *currentFlags = self.p_customFlagStrings.mutableCopy;
    if (!currentFlags) {
        currentFlags = [NSMutableSet set];
    }

    [currentFlags addObject:flag];
    self.p_customFlagStrings = currentFlags;
}

@end
