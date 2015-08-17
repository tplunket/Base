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

#import "Track Collection/Tags/Marker Properties/NxAGridMarkerProperty.h"
#import "NxAParsedMarkerValue.h"
#import "NSManagedObjectContext+NxAUtility.h"
#import "NSNumber+NxAUtility.h"
#import "NSString+NxAUtility.h"
#import "Track Collection/Tags/Marker Properties/Internal/NxAGridMarkerProperty.h"

#pragma mark Implementation

@implementation NxAGridMarkerProperty

#pragma mark Factory Methods

+ (instancetype)gridMarkerAtPosition:(NSDecimalNumber *)position
                                 bpm:(NSDecimalNumber *)bpm
                         customFlags:(NSArray *)flags
                             context:(NSManagedObjectContext *)context;
{
    NxAAssertNotNil(position);
    NxAAssertNotNil(bpm);

    NxAGridMarkerProperty *newMarker = [self p_markerAtPosition:position inContext:context];
    NxAAssertNotNil(newMarker);

    newMarker.p_bpm = bpm;

    for (NSString *flag in flags) {
        [newMarker p_addCustomFlag:flag];
    }

    return newMarker;
}

#pragma mark Class Methods

+ (NSDecimalNumber *)averageBpmForMarkers:(NSArray *)markers
{
    NSDecimalNumber *average = [NSDecimalNumber zero];
    NSInteger count = 0;

    for (NxAMarkerProperty *marker in markers) {
        if (![marker isKindOfClass:[NxAGridMarkerProperty class]]) {
            continue;
        }

        NxAGridMarkerProperty *gridMarker = (NxAGridMarkerProperty *)marker;

        average = [average decimalNumberByAdding:gridMarker.p_bpm];
        ++count;
    }

    if (count) {
        NSDecimalNumber *divider = [NSDecimalNumber decimalNumberWithString:[NSString stringWithFormat:@"%ld", count]];
        average = [average decimalNumberByDividingBy:divider];
    }

    return average;
}

#pragma mark Overridden NxATag Class Methods

+ (NSString *)p_managedEntityName
{
    return @"GridMarkerProperty";
}

+ (BOOL)sharesTags
{
    // -- Marker properties are not shared between different tagged objects,
    // -- each object gets an individual one.
    return NO;
}

#pragma mark Accessors

// -- Automatically generated for Core Data properties.
@dynamic p_bpm;

- (NSString *)p_value
{
    return self.p_pos.stringValue;
}

- (void)setP_value:(NSString *)p_value
{
    self.p_pos = [NSDecimalNumber decimalNumberWithString:p_value];
}

#pragma mark Overridden NxATag Accessors

- (NSString *)stringValue
{
    NxAParsedMarkerValue *value = [NxAParsedMarkerValue parsedGridMarkerValueAtPositionInSeconds:self.p_pos
                                                                                  beatsPerMinute:self.p_bpm];

    for (NSString *flagName in self.p_customFlagStrings) {
        [value addCustomFlag:flagName];
    }

    return value.description;
}

#pragma mark Overridden NSObject Methods

- (NSString *)description
{
    return [NSString stringWithFormat:@"GridMarker at %@ (BPM %@).",
            self.p_pos.stringValueAsTimeRepresentation,
            self.p_bpm];
}

@end
