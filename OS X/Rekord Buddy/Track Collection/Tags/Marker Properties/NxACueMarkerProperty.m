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

#import "Track Collection/Tags/Marker Properties/NxACueMarkerProperty.h"
#import "Track Collection/Tags/Marker Properties/NxAGridMarkerProperty.h"
#import "Track Collection/Tags/Marker Properties/NxALoopMarkerProperty.h"
#import "NxAParsedMarkerValue.h"
#import "NSManagedObjectContext+NxAUtility.h"
#import "NSNumber+NxAUtility.h"
#import "NSString+NxAUtility.h"
#import "NSNumber+NxAUtility.h"
#import "Track Collection/Tags/Marker Properties/Internal/NxACueMarkerProperty.h"
#import "Track Collection/Tags/Marker Properties/Internal/NxAGridMarkerProperty.h"
#import "Track Collection/Tags/Marker Properties/Internal/NxALoopMarkerProperty.h"

#pragma mark Implementation

@implementation NxACueMarkerProperty

#pragma mark Factory Methods

+ (instancetype)p_cueMarkerWithName:(NSString *)name
                        position:(NSDecimalNumber *)position
                          hotCue:(NSNumber *)hotCue
                     customFlags:(NSArray *)flags
                         context:(NSManagedObjectContext *)context
{
    NxAAssertNotNil(position);

    NxACueMarkerProperty *newMarker = [context insertNewObjectForEntity:self.p_managedEntityName];
    NxAAssertNotNil(newMarker);

    newMarker.p_name = name;
    newMarker.p_pos = position;

    if (hotCue && (hotCue.integerValue <= NxAMarkerMaximumHotcueIndex)) {
        newMarker.p_hotCue = hotCue;
    }

    for (NSString *flag in flags) {
        [newMarker p_addCustomFlag:flag];
    }

    return newMarker;
}

+ (instancetype)cueMarkerWithName:(NSString *)name
                         position:(NSDecimalNumber *)position
                           hotCue:(NSNumber *)hotCue
                      customFlags:(NSArray *)flags
                          context:(NSManagedObjectContext *)context
{
    return [self p_cueMarkerWithName:name position:position hotCue:hotCue customFlags:flags context:context];
}

+ (instancetype)cueMarkerWithName:(NSString *)name
                         position:(NSDecimalNumber *)position
                      customFlags:(NSArray *)flags
                          context:(NSManagedObjectContext *)context
{
    return [self p_cueMarkerWithName:name position:position hotCue:nil customFlags:flags context:context];
}

#pragma mark Overridden NxATag Class Methods

+ (NSString *)p_managedEntityName
{
    return @"CueMarkerProperty";
}

+ (BOOL)sharesTags
{
    // -- Marker properties are not shared between different tagged objects,
    // -- each object gets an individual one.
    return NO;
}

#pragma mark Accessors

// -- Automatically generated for Core Data properties.
@dynamic p_name;
@dynamic p_hotCue;

- (NSString *)p_value
{
    return self.p_name;
}

- (void)setP_value:(NSString *)p_value
{
    self.p_name = p_value;
}

#pragma mark Overridden NxATag Accessors

- (NSString *)stringValue
{
    NxAParsedMarkerValue *value = [NxAParsedMarkerValue parsedMarkerValueWithName:self.p_name
                                                                positionInSeconds:self.p_pos
                                                                     hotCueNumber:self.p_hotCue];

    for (NSString *flagName in self.p_customFlagStrings) {
        [value addCustomFlag:flagName];
    }

    return value.description;
}

#pragma mark Overridden NSObject Methods

- (NSString *)description
{
    if  (self.p_hotCue) {
        return [NSString stringWithFormat:@"CueMarker '%@' (Hotcue %@) at %@",
                self.p_value,
                self.p_hotCue,
                self.p_pos.stringValueAsTimeRepresentation];
    }
    else {
        return [NSString stringWithFormat:@"CueMarker '%@' at %@",
                self.p_value,
                self.p_pos.stringValueAsTimeRepresentation];
    }
}

@end
