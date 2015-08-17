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

#import "NxAParsedMarkerValue+SeratoParsing.h"
#import "NSDecimalNumber+NxAUtility.h"
#import "NSXMLElement+NxAUtility.h"
#import "NxAMessageLog.h"

using namespace NxA;
using namespace NxA::Serato;
using namespace std;

#pragma mark Implementation

@implementation NxAParsedMarkerValue (SeratoParsing)

#pragma mark Factory Methods

+ (instancetype)markerValueWithSeratoCueMarker:(const CueMarker&)cueMarker
                             loggingMessagesIn:(NxAMessageLog *)log
{
    NSString *name = [NSString stringWithUTF8String:cueMarker.label().toUTF8()];
    if (!name.length) {
        name = nil;
    }

    NSDecimalNumber *posNumber = [NSDecimalNumber decimalNumberWithDouble:(double)cueMarker.positionInMilliseconds()];
    NSDecimalNumber *pos = [posNumber decimalNumberByMultiplyingByPowerOf10:-3];

    uinteger16 hotCueIndex = cueMarker.index();
    NSNumber *hotCueNumber = nil;
    if (hotCueIndex >= 0) {
        // -- In order to accomodate hotcues and loop cues in programs that only have 8 hotcues,
        // -- we split Serato's up with the first 4 hotcues then the first 4 loop cues.
        // -- The first four hotcues start at index 0.
        if (hotCueIndex >= 4) {
            // -- We shift the last 4 hotcues into the 8-11 slots to make room for loop cues.
            hotCueIndex += 4;
        }

        hotCueNumber = @(hotCueIndex);
    }

    NxAParsedMarkerValue *newValue = [NxAParsedMarkerValue parsedMarkerValueWithName:name
                                                                   positionInSeconds:pos
                                                                        hotCueNumber:hotCueNumber];
    return newValue;
}

+ (instancetype)markerValueWithSeratoLoopMarker:(const Serato::LoopMarker&)loopMarker
                              loggingMessagesIn:(NxAMessageLog *)log
{
    NSString *name = [NSString stringWithUTF8String:loopMarker.label().toUTF8()];
    if (!name.length) {
        name = nil;
    }

    NSDecimalNumber *startPosNumber = [NSDecimalNumber decimalNumberWithDouble:(double)loopMarker.startPositionInMilliseconds()];
    NSDecimalNumber *startPos = [startPosNumber decimalNumberByMultiplyingByPowerOf10:-3];
    NSDecimalNumber *endPosNumber = [NSDecimalNumber decimalNumberWithDouble:(double)loopMarker.endPositionInMilliseconds()];
    NSDecimalNumber *endPos = [endPosNumber decimalNumberByMultiplyingByPowerOf10:-3];
    NSDecimalNumber *length = [endPos decimalNumberBySubtracting:startPos];

    uinteger16 hotCueIndex = loopMarker.index();
    NSNumber *hotCueNumber = nil;
    if (hotCueIndex >= 0) {
        // -- In order to accomodate hotcues and loop cues in programs that only have 8 hotcues,
        // -- we split Serato's up with the first 4 hotcues then the first 4 loop cues.
        // -- The first four loop hotcues start at index 4.
        hotCueIndex += 4;
        if (hotCueIndex >= 8) {
            // -- We shift the last 4 loop hotcues into the 12-15 slots to make room for loop cues.
            hotCueIndex += 4;
        }

        hotCueNumber = @(hotCueIndex);
    }

    NxAParsedMarkerValue *newValue = [NxAParsedMarkerValue parsedLoopMarkerValueWithName:name
                                                                       positionInSeconds:startPos
                                                                         lengthInSeconds:length
                                                                            hotCueNumber:hotCueNumber];
    return newValue;
}

+ (instancetype)markerValueWithSeratoGridMarker:(const Serato::GridMarker&)gridMarker
                              loggingMessagesIn:(NxAMessageLog *)log;
{
    NSDecimalNumber *startPos = [NSDecimalNumber decimalNumberWithDouble:gridMarker.positionInSeconds()];
    NSDecimalNumber *bpm = [NSDecimalNumber decimalNumberWithDouble:gridMarker.beatsPerMinute()];

    NxAParsedMarkerValue *newValue = [NxAParsedMarkerValue parsedGridMarkerValueAtPositionInSeconds:startPos
                                                                                     beatsPerMinute:bpm];
    return newValue;
}

#pragma mark Class Methods

+ (NSArray *)parseAndSortMarkersValuesInSeratoTrack:(const Serato::Track&)track
                                  loggingMessagesIn:(NxAMessageLog *)log
{
    auto& cueMarkers = track.cueMarkers();
    auto& loopMarkers = track.loopMarkers();
    auto& gridMarkers = track.gridMarkers();
    NSMutableArray *newValues = [NSMutableArray arrayWithCapacity:cueMarkers.length() + loopMarkers.length() + gridMarkers.length()];

    for (auto& marker : cueMarkers) {
        NxAParsedMarkerValue *newValue = [NxAParsedMarkerValue markerValueWithSeratoCueMarker:marker
                                                                            loggingMessagesIn:log];

        // -- Even if something goes wrong in getting the marker element, we can skip this element without failing
        if (newValue) {
            [newValues addObject:newValue];
        }
    }

    for (auto& marker : loopMarkers) {
        NxAParsedMarkerValue *newValue = [NxAParsedMarkerValue markerValueWithSeratoLoopMarker:marker
                                                                             loggingMessagesIn:log];

        // -- Even if something goes wrong in getting the marker element, we can skip this element without failing
        if (newValue) {
            [newValues addObject:newValue];
        }
    }

    if (gridMarkers.length()) {
        for (auto& marker : gridMarkers) {
            NxAParsedMarkerValue *newValue = [NxAParsedMarkerValue markerValueWithSeratoGridMarker:marker
                                                                                 loggingMessagesIn:log];

            // -- Even if something goes wrong in getting the marker element, we can skip this element without failing
            if (newValue) {
                [newValues addObject:newValue];
            }
        }
    }
    else {
        auto& bpmString = track.bpm();
        if (bpmString.length()) {
            NxAParsedMarkerValue *newValue = [NxAParsedMarkerValue parsedGridMarkerValueAtPositionInSeconds:[NSDecimalNumber decimalNumberWithString:@"0.0"]
                                                                                             beatsPerMinute:[NSDecimalNumber decimalNumberWithString:[NSString stringWithUTF8String:bpmString.toUTF8()]]];
            [newValues addObject:newValue];
        }
    }

    if (newValues.count) {
        // -- Sort the markers by their position.
        newValues = [[newValues sortedArrayUsingSelector:@selector(compare:)] mutableCopy];
    }
    else {
        [newValues addObject:[NxAParsedMarkerValue parsedMarkerValueWithNoMarker]];
    }

    return newValues;
}

@end
