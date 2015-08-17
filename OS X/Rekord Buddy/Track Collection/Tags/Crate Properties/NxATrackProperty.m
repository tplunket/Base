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

#import "NxATrack.h"
#import "Track Collection/Crates/NxACrate.h"
#import "NxATagEntry.h"
#import "NSString+NxAUtility.h"
#import "NSManagedObjectContext+NxAUtility.h"
#import "Track Collection/Tags/Crate Properties/Internal/NxATrackProperty.h"

#import "NxAAppProtection.h"

#pragma mark Implementation

@implementation NxATrackProperty

#pragma mark Class Methods

NXA_APP_PROTECTION_INJECTED_METHOD

+ (NSArray *)arrayOfTracksFromProperties:(NSArray *)properties
{
    NSMutableArray *results = [NSMutableArray arrayWithCapacity:properties.count];

    for (NxATrackProperty *property in properties) {
        NxAAssertNotNil(property.p_value);
        [results addObject:property.p_value];
    }

    return results;
}

+ (NSSet *)parentCratesForTrackProperties:(NSSet *)properties
{
    NSMutableSet *allParentCrates = [NSMutableSet set];

    for (NxATrackProperty *trackProperty in properties) {
        NxAAssertTrue([trackProperty isKindOfClass:[NxATrackProperty class]]);

        NSSet *parentTagEntries = trackProperty.p_parentTagEntries;
        NSArray *parentCrates = [NxATagEntry parentCratesForTagEntries:parentTagEntries
                                                    ignoreSystemCrates:YES];
        if (parentCrates.count) {
            NxAAssertTrue(parentCrates.count == 1);

            [allParentCrates addObject:parentCrates.firstObject];
        }
    }

    return allParentCrates;
}

+ (NSArray *)trackPropertiesFromPasteboard:(NSPasteboard *)pasteboard
                                   context:(NSManagedObjectContext *)context
{
    NSArray *urls = [pasteboard readObjectsForClasses:@[ [NSURL class] ] options:nil ];
    NSMutableArray *properties = [NSMutableArray array];

    for (NSURL *url in urls) {
        NSString *desc = url.description;

        // -- If this is not a core data URI then it can't be a Track Property, therefore we skip it.
        if (![desc hasPrefix:@"x-coredata://"]) {
            continue;
        }

        NSManagedObject *object = [context objectWithURI:url];
        if ([object isKindOfClass:[NxATrackProperty class]]) {
            NxATrackProperty *property = (NxATrackProperty *)object;
            [properties addObject:property];
        }
    }
    
    return properties;
}

#pragma mark Overridden NxATag Class Methods

+ (NSString *)p_managedEntityName
{
    return @"TrackProperty";
}

+ (Class)p_valueClass
{
    return [NxATrack class];
}

+ (BOOL)sharesTags
{
    // -- Track properties are not shared between different tagged objects,
    // -- each object gets an individual one.
    return NO;
}

#pragma mark Accessors

// -- Automatically generated for Core Data properties.
@dynamic p_value;

- (NxATrack *)track
{
    return self.p_value;
}

#pragma mark Overridden NxATag Accessors

- (NSString *)stringValue
{
    return self.p_value.stringValue;
}

#pragma mark Overridden NxATag Instance Methods

- (void)destroy
{
    NxATrack *track = self.p_value;
    self.p_value = nil;
    
    if (track.isAnOrphan) {
        [track destroy];
    }

    [super destroy];
}

#pragma mark Overridden NSObject methods

- (NSString *)description
{
    return [NSString stringWithFormat:@"Track property for '%@'", self.stringValue];
}

@end
