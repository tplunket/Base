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

#import "NSManagedObjectContext+NxAUtility.h"
#import "Track Collection/Crates/NxACrate.h"
#import "NxATagEntry.h"
#import "Track Collection/Tags/Crate Properties/Internal/NxACrateProperty.h"

#pragma mark Implementation

@implementation NxACrateProperty

#pragma mark Class Methods

+ (NSArray *)arrayOfCratesFromProperties:(NSArray *)properties
{
    NSMutableArray *results = [NSMutableArray arrayWithCapacity:properties.count];

    for (NxACrateProperty *property in properties) {
        NxAAssertNotNil(property.p_value);
        [results addObject:property.p_value];
    }

    return results;
}

#pragma mark Overridden NxATag Class Methods

+ (NSString *)p_managedEntityName
{
    return @"CrateProperty";
}

+ (Class)p_valueClass
{
    return [NxACrate class];
}

+ (BOOL)sharesTags
{
    // -- Crate properties are not shared between different tagged objects,
    // -- each object gets an individual one.
    return NO;
}

#pragma mark Overridden NxATag Accessors

- (NSString *)stringValue
{
    return self.p_value.stringValue;
}

#pragma mark Accessors

// -- Automatically generated for Core Data properties.
@dynamic p_value;

- (NxACrate *)parentCrate
{
    NSSet *parentTagEntries = self.p_parentTagEntries;
    NSArray *parentCrates = [NxATagEntry parentCratesForTagEntries:parentTagEntries
                                                ignoreSystemCrates:NO];
    NxAAssertTrue(parentCrates.count <= 1);

    return parentCrates.firstObject;
}

- (NxACrate *)crate
{
    return self.p_value;
}

#pragma mark Overridden NxATag Instance Methods

- (void)destroy
{
    NxACrate *crate = self.p_value;
    if (crate) {
        self.p_value = nil;
        [crate destroy];
    }

    [super destroy];
}

#pragma mark Overridden NSObject methods

- (NSString *)description
{
    return [NSString stringWithFormat:@"Crate property for '%@'", self.stringValue];
}

@end
