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

#import "Track Collection/Parsed Values/Internal/NxAParsedValue.h"
#import "NxAParsedMarkerValue.h"
#import "Track Collection/Tags/Marker Properties/NxACueMarkerProperty.h"
#import "Track Collection/Tags/Marker Properties/NxALoopMarkerProperty.h"
#import "Track Collection/Tags/Marker Properties/NxAGridMarkerProperty.h"

#pragma mark Private Interface

@interface NxAParsedMarkerValue ()

#pragma mark Properties

@property (assign, nonatomic) NxAParsedMarkerValueType type;

@property (strong, nonatomic) NSMutableSet *p_customFlags;

@end

#pragma mark - Implementation

@implementation NxAParsedMarkerValue

#pragma mark Factory Methods

+ (instancetype)parsedMarkerValueWithNoMarker
{
    NxAParsedMarkerValue *value = [[NxAParsedMarkerValue alloc] initPrivate];

    value.type = NxAParsedMarkerValueNilType;

    return value;
}

+ (instancetype)parsedMarkerValueWithName:(NSString *)name
                        positionInSeconds:(NSDecimalNumber *)positionInSeconds
                             hotCueNumber:(NSNumber *)hotCueNumber
{
    NxAAssertNotNil(positionInSeconds);
    NxAAssertTrue(hotCueNumber.integerValue <= NxAMarkerMaximumHotcueIndex);

    NxAParsedMarkerValue *value = [[NxAParsedMarkerValue alloc] initPrivate];

    value.type = NxAParsedMarkerValueCueType;
    value.name = name;
    value.positionInSeconds = positionInSeconds;
    value.hotCueNumber = hotCueNumber;

    return value;
}

+ (instancetype)parsedLoopMarkerValueWithName:(NSString *)name
                            positionInSeconds:(NSDecimalNumber *)positionInSeconds
                              lengthInSeconds:(NSDecimalNumber *)lengthInSeconds
                                 hotCueNumber:(NSNumber *)hotCueNumber
{
    NxAAssertNotNil(positionInSeconds);
    NxAAssertNotNil(lengthInSeconds);
    NxAAssertTrue(hotCueNumber.integerValue <= NxAMarkerMaximumHotcueIndex);

    NxAParsedMarkerValue *value = [[NxAParsedMarkerValue alloc] initPrivate];

    value.type = NxAParsedMarkerValueLoopType;
    value.name = name;
    value.positionInSeconds = positionInSeconds;
    value.lengthInSeconds = lengthInSeconds;
    value.hotCueNumber = hotCueNumber;

    return value;
}

+ (instancetype)parsedGridMarkerValueAtPositionInSeconds:(NSDecimalNumber *)positionInSeconds
                                          beatsPerMinute:(NSDecimalNumber *)beatsPerMinute
{
    NxAAssertNotNil(positionInSeconds);
    NxAAssertNotNil(beatsPerMinute);

    NxAParsedMarkerValue *value = [[NxAParsedMarkerValue alloc] initPrivate];

    value.type = NxAParsedMarkerValueGridType;
    value.positionInSeconds = positionInSeconds;
    value.beatsPerMinute = beatsPerMinute;

    return value;
}

#pragma mark Accessors

- (NSMutableSet *)p_customFlags
{
    if (!_p_customFlags) {
        _p_customFlags = [NSMutableSet set];
    }

    return _p_customFlags;
}

- (NSSet *)customFlags
{
    return self.p_customFlags;
}

#pragma mark Instance Methods

- (instancetype)initPrivate
{
    return [super initPrivate];
}

- (void)addCustomFlag:(NSString *)flag
{
    [self.p_customFlags addObject:flag];
}

- (void)removeCustomFlag:(NSString *)flag
{
    [self.p_customFlags removeObject:flag];
}

- (BOOL)hasCustomFlag:(NSString *)flag
{
    return [self.p_customFlags containsObject:flag];
}

#pragma mark Overridden NxAParsedValue Accessors

- (NSString *)comparisonValue
{
    return [NSString stringWithFormat:@"<%@>", self.description];
}

#pragma mark Overridden NxAParsedValue Instance Methods

- (id)tagForClass:(Class)tagClass usingContext:(NSManagedObjectContext *)context;
{
    NxAAssertTrue(tagClass == [NxAMarkerProperty class]);
    NxAAssertNotNil(context);

    NSString* markerName = self.name;
    if (!markerName) {
        markerName = @"";
    }

    NxAMarkerProperty *newMarker = nil;

    switch (self.type) {
        case NxAParsedMarkerValueNilType: {
            newMarker = [NxACueMarkerProperty cueMarkerWithName:nil
                                                       position:[NSDecimalNumber decimalNumberWithString:@"0.0"]
                                                         hotCue:nil
                                                    customFlags:nil
                                                        context:context];
            break;
        }
        case NxAParsedMarkerValueCueType: {
            newMarker = [NxACueMarkerProperty cueMarkerWithName:markerName
                                                       position:self.positionInSeconds
                                                         hotCue:self.hotCueNumber
                                                    customFlags:self.customFlags.allObjects
                                                        context:context];
            break;
        }
        case NxAParsedMarkerValueGridType: {
            newMarker = [NxAGridMarkerProperty gridMarkerAtPosition:self.positionInSeconds
                                                                bpm:self.beatsPerMinute
                                                        customFlags:@[ ]
                                                            context:context];
            break;
        }
        case NxAParsedMarkerValueLoopType: {
            NxAAssertTrue(self.customFlags.count == 0);

            newMarker = [NxALoopMarkerProperty loopMarkerWithName:markerName
                                                         position:self.positionInSeconds
                                                           length:self.lengthInSeconds
                                                           hotCue:self.hotCueNumber
                                                          context:context];
            break;
        }
        default: {
            ALog(@"Invalid marker value type.");
            return nil;
        }
    }
    
    return newMarker;
}

#pragma mark Overridden NSObject Instance Methods

- (instancetype)init
{
    ALog(@"Illegal base class call from '%@'.", self.class);
    return nil;
}

- (NSString *)description
{
    NSString *stringValue;

    switch (self.type) {
        case NxAParsedMarkerValueNilType: {
            return @"nil";
        }
        case NxAParsedMarkerValueCueType: {
            stringValue = [NSString stringWithFormat:@"1P%@|N%@|H%@",
                           self.positionInSeconds,
                           self.name ? self.name : @"",
                           self.hotCueNumber ? self.hotCueNumber : @""];
            break;
        }
        case NxAParsedMarkerValueLoopType: {
            stringValue = [NSString stringWithFormat:@"2P%@|N%@|H%@|L%@",
                           self.positionInSeconds,
                           self.name ? self.name : @"",
                           self.hotCueNumber ? self.hotCueNumber : @"",
                           self.lengthInSeconds];
            break;
        }
        case NxAParsedMarkerValueGridType: {
            stringValue = [NSString stringWithFormat:@"0P%@|N%@|H%@|B%@",
                           self.positionInSeconds,
                           self.name ? self.name : @"",
                           self.hotCueNumber ? self.hotCueNumber : @"",
                           self.beatsPerMinute];
            break;
        }
    }

    NSSet *flags = self.p_customFlags;
    if (!flags) {
        return stringValue;
    }

    NSString *propertiesString = [flags.allObjects componentsJoinedByString:@","];
    return [NSString stringWithFormat:@"%@|%@", stringValue, propertiesString];
}

- (NSComparisonResult)compare:(NxAParsedMarkerValue *)otherMarkerValue
{
    // -- Comparing positions first should help weed out most comparaisons without constructing descriptions for all markers.
    if ([self.positionInSeconds isGreaterThan:otherMarkerValue.positionInSeconds]) {
        return NSOrderedDescending;
    }
    else if ([self.positionInSeconds isLessThan:otherMarkerValue.positionInSeconds]) {
        return NSOrderedAscending;
    }

    NSString *description = self.description;
    NSString *otherDescription = otherMarkerValue.description;
    return [description compare:otherDescription];
}

@end
