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

#import "NxAParsedMarkerValue.h"
#import "NSManagedObjectContext+NxAUtility.h"
#import "NSNumber+NxAUtility.h"
#import "NSString+NxAUtility.h"
#import "Track Collection/Tags/Marker Properties/Internal/NxALoopMarkerProperty.h"

#pragma mark Implementation

@implementation NxALoopMarkerProperty

#pragma mark Factory Methods

+ (instancetype)p_loopMarkerWithName:(NSString *)name
                            position:(NSDecimalNumber *)position
                              length:(NSDecimalNumber *)length
                              hotCue:(NSNumber *)hotCue
                             context:(NSManagedObjectContext *)context
{
    NxAAssertNotNil(length);

    NxALoopMarkerProperty *marker;
    if (hotCue) {
        marker = [self cueMarkerWithName:name
                                position:position
                                  hotCue:hotCue
                             customFlags:@[ ]
                                 context:context];
    }
    else {
        marker = [self cueMarkerWithName:name
                                position:position
                             customFlags:@[ ]
                                 context:context];
    }

    NxAAssertNotNil(marker);

    marker.p_length = length;

    return marker;
}

+ (instancetype)loopMarkerWithName:(NSString *)name
                          position:(NSDecimalNumber *)position
                            length:(NSDecimalNumber *)length
                            hotCue:(NSNumber *)hotCue
                           context:(NSManagedObjectContext *)context
{
    return [self p_loopMarkerWithName:name position:position length:length hotCue:hotCue context:context];
}

+ (instancetype)loopMarkerWithName:(NSString *)name
                          position:(NSDecimalNumber *)position
                            length:(NSDecimalNumber *)length
                           context:(NSManagedObjectContext *)context
{
    return [self p_loopMarkerWithName:name position:position length:length hotCue:nil context:context];
}

#pragma mark Overridden NxATag Class Methods

+ (NSString *)p_managedEntityName
{
    return @"LoopMarkerProperty";
}

+ (BOOL)sharesTags
{
    // -- Marker properties are not shared between different tagged objects,
    // -- each object gets an individual one.
    return NO;
}

#pragma mark Accessors

// -- Automatically generated for Core Data properties.
@dynamic p_length;

#pragma mark Overridden NxATag Accessors

- (NSString *)stringValue
{
    NxAParsedMarkerValue *value = [NxAParsedMarkerValue parsedLoopMarkerValueWithName:self.p_name
                                                                    positionInSeconds:self.p_pos
                                                                      lengthInSeconds:self.p_length
                                                                         hotCueNumber:self.p_hotCue];

    for (NSString *flagName in self.p_customFlagStrings) {
        [value addCustomFlag:flagName];
    }

    return value.description;
}

#pragma mark Overridden NSObject Methods

- (NSString *)description
{
    if (self.p_hotCue) {
        return [NSString stringWithFormat:@"LoopMarker '%@' (Hotcue %@) at %@-%@",
                self.p_name,
                self.p_hotCue,
                self.p_pos.stringValueAsTimeRepresentation,
                [[self.p_pos decimalNumberByAdding:self.p_length] stringValueAsTimeRepresentation]];
    }
    else {
        return [NSString stringWithFormat:@"LoopMarker '%@' t %@-%@",
                self.p_name,
                self.p_pos.stringValueAsTimeRepresentation,
                [[self.p_pos decimalNumberByAdding:self.p_length] stringValueAsTimeRepresentation]];
    }
}

@end
