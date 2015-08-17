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

#import "NxAParsedMarkerValue+TraktorParsing.h"
#import "NSDecimalNumber+NxAUtility.h"
#import "NSXMLElement+NxAUtility.h"
#import "NxAMarkerProperty+TraktorParsing.h"
#import "NxAMessageLog.h"

#pragma mark Implementation

@implementation NxAParsedMarkerValue (TraktorParsing)

#pragma mark Class Methods

+ (NSArray *)markerValuesWithTraktorCueV2Element:(NSXMLElement *)cueV2Element
                                           tempo:(NSDecimalNumber *)tempo
                                             log:(NxAMessageLog *)log
{
    NSDecimalNumber *pos = [NSDecimalNumber decimalNumberWithUnlocalizedAttribute:[cueV2Element attributeForName:@"START"]];
    if (!pos) {
        NxAWarning *warning = [NxAWarning warningWithDescription:NSLocalizedString(@"in your Traktor collection with missing or invalid 'START' attribute in its/their cue marker(s).",
                                                                                   @"")];
        [log addWarning:warning inContextWithPerObjectWarningInfo:@""];

        return @[ ];
    }

    // -- Convert the position from milliseconds to seconds.
    pos = [pos decimalNumberByMultiplyingByPowerOf10:-3];

    NSString *hotCueAttributeString = [[cueV2Element attributeForName:@"HOTCUE"] stringValue];
    NSNumber *hotCue = nil;
    if (hotCueAttributeString.length) {
        NSInteger hotCueValue = hotCueAttributeString.integerValue;

        if (hotCueValue >= 8) {
            NxAWarning *warning = [NxAWarning warningWithDescription:NSLocalizedString(@"in your Traktor collection with an invalid 'HOTCUE' attribute in its/their cue marker(s).",
                                                                                       @"")];
            [log addWarning:warning inContextWithPerObjectWarningInfo:@""];

            return @[ ];
        }
        else if (hotCueValue >= 0) {
            hotCue = @( hotCueValue );
        }
    }

    NSXMLNode *typeAttribute = [cueV2Element attributeForName:@"TYPE"];
    if (!typeAttribute) {
        NxAWarning *warning = [NxAWarning warningWithDescription:NSLocalizedString(@"in your Traktor collection with missing 'TYPE' attribute in its/their cue marker(s).",
                                                                                   @"")];
        [log addWarning:warning inContextWithPerObjectWarningInfo:@""];

        return @[ ];
    }

    NSXMLNode *nameAttribute = [cueV2Element attributeForName:@"NAME"];
    if (!nameAttribute) {
        NxAWarning *warning = [NxAWarning warningWithDescription:NSLocalizedString(@"in your Traktor collection with missing 'NAME' attribute in its/their cue marker(s).",
                                                                                   @"")];
        [log addWarning:warning inContextWithPerObjectWarningInfo:@""];

        return @[ ];
    }
    NSString *name = nameAttribute.stringValue;
    if (!name.length) {
        name = nil;
    }

    NSInteger type = typeAttribute.stringValue.intValue;
    switch (type) {
        case 0: {
            // -- "Cue"
            NxAParsedMarkerValue *newValue = [NxAParsedMarkerValue parsedMarkerValueWithName:name
                                                                           positionInSeconds:pos
                                                                                hotCueNumber:hotCue];
            return @[ newValue ];
        }
        case 3: {
            // -- "Load"
            NxAParsedMarkerValue *newValue = [NxAParsedMarkerValue parsedMarkerValueWithName:name
                                                                           positionInSeconds:pos
                                                                                hotCueNumber:hotCue];

            [newValue addCustomFlag:NxAMarkerIsLoadMarker];

            return @[ newValue ];
        }
        case 1: {
            // -- "Fade-in"
            NxAParsedMarkerValue *newValue = [NxAParsedMarkerValue parsedMarkerValueWithName:name
                                                                           positionInSeconds:pos
                                                                                hotCueNumber:hotCue];

            [newValue addCustomFlag:NxAMarkerIsFadeInMarker];

            return @[ newValue ];
        }
        case 2: {
            // -- "Fade-out"
            NxAParsedMarkerValue *newValue = [NxAParsedMarkerValue parsedMarkerValueWithName:name
                                                                           positionInSeconds:pos
                                                                                hotCueNumber:hotCue];

            [newValue addCustomFlag:NxAMarkerIsFadeOutMarker];

            return @[ newValue ];
        }
        case 4: {
            // -- "Grid"
            if (!tempo) {
                NxAWarning *warning = [NxAWarning warningWithDescription:NSLocalizedString(@"in your Traktor collection with grid markers but no tempo set for the track. Grid markers can only be used if the track has a valid tempo in Traktor.",
                                                                                           @"")];
                [log addWarning:warning inContextWithPerObjectWarningInfo:@""];

                return @[ ];
            }

            NxAParsedMarkerValue *newValue1 = [NxAParsedMarkerValue parsedGridMarkerValueAtPositionInSeconds:pos
                                                                                              beatsPerMinute:tempo];
            if (hotCue) {
                // -- If we found a grid maker with a hot cue, we split it into two markers and make a note of it.
                [newValue1 addCustomFlag:NxAMarkerWasStoredAsAHotCue];

                NxAParsedMarkerValue *newValue2 = [NxAParsedMarkerValue parsedMarkerValueWithName:name
                                                                                positionInSeconds:pos
                                                                                     hotCueNumber:hotCue];
                return @[ newValue1, newValue2 ];
            }

            return @[ newValue1 ];
        }
        case 5: {
            // -- "Loop"
            NSDecimalNumber *length = [NSDecimalNumber decimalNumberWithUnlocalizedAttribute:[cueV2Element attributeForName:@"LEN"]];
            if (!length) {
                NxAWarning *warning = [NxAWarning warningWithDescription:NSLocalizedString(@"in your Traktor collection with a missing or invalid 'LEN' attribute in its/their cue marker(s).",
                                                                                           @"")];
                [log addWarning:warning inContextWithPerObjectWarningInfo:@""];

                return @[ ];
            }

            // -- Convert the length from milliseconds to seconds.
            length = [length decimalNumberByMultiplyingByPowerOf10:-3];

            NxAParsedMarkerValue *newValue = [NxAParsedMarkerValue parsedLoopMarkerValueWithName:name
                                                                               positionInSeconds:pos
                                                                                 lengthInSeconds:length
                                                                                    hotCueNumber:hotCue];
            return @[ newValue ];
        }
        default: {
            NxAWarning *warning = [NxAWarning warningWithDescription:NSLocalizedString(@"in your Traktor collection with invalid 'TYPE' attribute in its/their cue marker(s).",
                                                                                       @"")];
            [log addWarning:warning inContextWithPerObjectWarningInfo:@""];

            return @[ ];
        }
    }
}

#pragma mark Class Methods

+ (NSArray *)parseAndSortTraktorMarkerValuesInEntryElement:(NSXMLElement *)entryElement
                                                       log:(NxAMessageLog *)log
{
    NSArray *cueV2Elements = [entryElement elementsForName:@"CUE_V2"];

    // -- Then we need to make sure that the track's tempo is read before the markers because it is used later for grid markers.
    NSXMLElement *tempoElement = [entryElement elementForName:@"TEMPO"];
    NSDecimalNumber *tempo = nil;
    if (tempoElement) {
        NSXMLNode *attribute = [tempoElement attributeForName:@"BPM"];
        if (attribute) {
            NSString *tempoString = attribute.stringValue;
            if (tempoString.length) {
                tempo = [NSDecimalNumber decimalNumberWithUnlocalizedString:tempoString];
            }
        }
    }

    NSMutableArray *newValues = [NSMutableArray arrayWithCapacity:cueV2Elements.count];
    BOOL foundGridMarker = NO;
    for (NSXMLElement *cueV2Element in cueV2Elements) {
        NSArray *newMarkers = [NxAParsedMarkerValue markerValuesWithTraktorCueV2Element:cueV2Element
                                                                                  tempo:tempo
                                                                                    log:log];

        for (NxAParsedMarkerValue *newValue in newMarkers) {
            if (newValue.beatsPerMinute != nil) {
                foundGridMarker = YES;
            }

            [newValues addObject:newValue];
        }
    }

    if (!foundGridMarker && tempo) {
        NxAParsedMarkerValue *newValue = [NxAParsedMarkerValue parsedGridMarkerValueAtPositionInSeconds:[NSDecimalNumber decimalNumberWithString:@"0.0"]
                                                                                         beatsPerMinute:tempo];
        [newValues addObject:newValue];
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
