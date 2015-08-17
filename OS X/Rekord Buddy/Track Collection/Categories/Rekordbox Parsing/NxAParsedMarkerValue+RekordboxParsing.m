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

#import "NxAParsedMarkerValue+RekordboxParsing.h"
#import "NSDecimalNumber+NxAUtility.h"
#import "NSXMLElement+NxAUtility.h"
#import "Track Collection/Categories/Rekordbox Parsing/NxAMarkerProperty+RekordboxParsing.h"
#import "NxAMessageLog.h"

#pragma mark Implementation

@implementation NxAParsedMarkerValue (RekordboxParsing)

#pragma mark Factory Methods

+ (instancetype)markerValueWithRekordboxPositionElement:(NSXMLElement *)positionElement
                                                 offset:(NSDecimalNumber *)offset
                                                    log:(NxAMessageLog *)log
{
    NSXMLNode *nameAttribute = [positionElement attributeForName:@"Name"];
    if (!nameAttribute) {
        NxAWarning *warning = [NxAWarning warningWithDescription:NSLocalizedString(@"in your Rekordbox collection with missing 'Name' attribute in its/their cue marker(s).",
                                                                                   @"")];
        [log addWarning:warning inContextWithPerObjectWarningInfo:@""];
        
        return nil;
    }
    
    NSString *name = nameAttribute.stringValue;
    if (!name.length) {
        name = nil;
    }
    
    NSDecimalNumber *pos = [NSDecimalNumber decimalNumberWithUnlocalizedAttribute:[positionElement attributeForName:@"Start"]];
    if (!pos) {
        NxAWarning *warning = [NxAWarning warningWithDescription:NSLocalizedString(@"in your Rekordbox collection with missing or invalid 'Start' attribute in its/their cue marker(s).",
                                                                                   @"")];
        [log addWarning:warning inContextWithPerObjectWarningInfo:@""];

        return nil;
    }
    else if (offset) {
        pos = [pos decimalNumberByAdding:offset];
    }
    
    NSXMLNode *typeAttribute = [positionElement attributeForName:@"Type"];
    if (!typeAttribute) {
        NxAWarning *warning = [NxAWarning warningWithDescription:NSLocalizedString(@"in your Rekordbox collection with missing 'Type' attribute in its/their cue marker(s).",
                                                                                   @"")];
        [log addWarning:warning inContextWithPerObjectWarningInfo:@""];

        return nil;
    }
    
    NSXMLNode *hotCueAttribute = [positionElement attributeForName:@"Num"];
    NSNumber *hotCue = nil;
    if (hotCueAttribute) {
        NSInteger hotCueValue = hotCueAttribute.stringValue.integerValue;
        
        if (hotCueValue >= 8) {
            NxAWarning *warning = [NxAWarning warningWithDescription:NSLocalizedString(@"in your Rekordbox collection with an invalid 'Num' attribute in its/their cue marker(s).",
                                                                                       @"")];
            [log addWarning:warning inContextWithPerObjectWarningInfo:@""];

            return nil;
        }
        else if (hotCueValue >= 0) {
            hotCue = @( hotCueValue );
        }
    }
    
    NxAParsedMarkerValue *newValue = nil;

    NSInteger type = typeAttribute.stringValue.intValue;
    switch (type) {
        case 0: {
            // -- Cue
            newValue = [NxAParsedMarkerValue parsedMarkerValueWithName:name
                                                     positionInSeconds:pos
                                                          hotCueNumber:hotCue];
            break;
        }
        case 3: {
            // -- Load
            newValue = [NxAParsedMarkerValue parsedMarkerValueWithName:name
                                                     positionInSeconds:pos
                                                          hotCueNumber:hotCue];

            [newValue addCustomFlag:NxAMarkerIsLoadMarker];
            break;
        }
        case 1: {
            // -- Fade-in
            newValue = [NxAParsedMarkerValue parsedMarkerValueWithName:name
                                                     positionInSeconds:pos
                                                          hotCueNumber:hotCue];

            [newValue addCustomFlag:NxAMarkerIsFadeInMarker];
            break;
        }
        case 2: {
            // -- Fade-out
            newValue = [NxAParsedMarkerValue parsedMarkerValueWithName:name
                                                     positionInSeconds:pos
                                                          hotCueNumber:hotCue];

            [newValue addCustomFlag:NxAMarkerIsFadeOutMarker];
            break;
        }
        case 4: {
            // -- Loop
            NSDecimalNumber *end = [NSDecimalNumber decimalNumberWithUnlocalizedAttribute:[positionElement attributeForName:@"End"]];
            if (!end) {
                NxAWarning *warning = [NxAWarning warningWithDescription:NSLocalizedString(@"in your Rekordbox collection with a missing or invalid 'LEN' attribute in its/their cue marker(s).",
                                                                                           @"")];
                [log addWarning:warning inContextWithPerObjectWarningInfo:@""];
                
                return nil;
            }
            else if (offset) {
                end = [end decimalNumberByAdding:offset];
            }

            NSDecimalNumber *length = [end decimalNumberBySubtracting:pos];

            newValue = [NxAParsedMarkerValue parsedLoopMarkerValueWithName:name
                                                         positionInSeconds:pos
                                                           lengthInSeconds:length
                                                              hotCueNumber:hotCue];
            break;
        }
        default: {
            NxAWarning *warning = [NxAWarning warningWithDescription:NSLocalizedString(@"in your Rekordbox collection with an invalid position marker type. Try re-exporting your collection to a new XML file.",
                                                                                       @"")];
            [log addWarning:warning inContextWithPerObjectWarningInfo:@""];

            return nil;
        }
    }
    
    return newValue;
}

+ (instancetype)gridMarkerValueWithRekordboxTempoElement:(NSXMLElement *)tempoMarker
                                                  offset:(NSDecimalNumber *)offset
                                                     log:(NxAMessageLog *)log
{
    NSDecimalNumber *pos = [NSDecimalNumber decimalNumberWithUnlocalizedAttribute:[tempoMarker attributeForName:@"Inizio"]];
    if (!pos) {
        NxAWarning *warning = [NxAWarning warningWithDescription:NSLocalizedString(@"in your Rekordbox collection with missing or invalid 'Inizio' attribute in its/their grid marker(s).",
                                                                                   @"")];
        [log addWarning:warning inContextWithPerObjectWarningInfo:@""];

        return nil;
    }
    else if (offset) {
        pos = [pos decimalNumberByAdding:offset];
    }

    NSDecimalNumber *tempo = [NSDecimalNumber decimalNumberWithUnlocalizedAttribute:[tempoMarker attributeForName:@"Bpm"]];
    if (!tempo) {
        NxAWarning *warning = [NxAWarning warningWithDescription:NSLocalizedString(@"in your Rekordbox collection with missing or invalid 'Tempo' attribute in its/their grid marker(s).",
                                                                                   @"")];
        [log addWarning:warning inContextWithPerObjectWarningInfo:@""];

        return nil;
    }

    NSInteger firstBeatValue = [[[tempoMarker attributeForName:@"Battito"] stringValue] floatValue];
    if (firstBeatValue > 1.0) {
        if (firstBeatValue > 4.0) {
            NxAWarning *warning = [NxAWarning warningWithDescription:NSLocalizedString(@"in your Rekordbox collection with an illegal first beat. Try re-exporting your collection to a new XML file.",
                                                                                       @"")];
            [log addWarning:warning inContextWithPerObjectWarningInfo:@""];

            return nil;
        }

        NSDecimalNumber *firstBeatOffset = [NSDecimalNumber decimalNumberWithDouble:(5.0f - firstBeatValue) * (60000.0f / tempo.floatValue)];
        pos = [pos decimalNumberByAdding:firstBeatOffset];
    }

    NxAParsedMarkerValue *newValue = [NxAParsedMarkerValue parsedGridMarkerValueAtPositionInSeconds:pos
                                                                                     beatsPerMinute:tempo];
    return newValue;
}

#pragma mark Class Methods

+ (NSArray *)parseAndSortRekordboxMarkerValuesInTrackElement:(NSXMLElement *)trackElement
                                                         log:(NxAMessageLog *)log
{
    NSArray *markerElements = [trackElement elementsForName:@"POSITION_MARK"];
    NSArray *tempoElements = [trackElement elementsForName:@"TEMPO"];
    NSMutableArray *newValues = [NSMutableArray arrayWithCapacity:markerElements.count + tempoElements.count];
    
    NSDecimalNumber *offset = [NxACueMarkerProperty offsetForTrackElement:trackElement];

    for (NSXMLElement *positionMarker in markerElements) {
        NxAParsedMarkerValue *newValue = [NxAParsedMarkerValue markerValueWithRekordboxPositionElement:positionMarker
                                                                                                offset:offset
                                                                                                   log:log];

        // -- Even if something goes wrong in getting the marker element, we can skip this element without failing
        if (newValue) {
            [newValues addObject:newValue];
        }
    }

    if (tempoElements.count) {
        for (NSXMLElement *tempoMarker in tempoElements) {
            NxAParsedMarkerValue *newValue = [NxAParsedMarkerValue gridMarkerValueWithRekordboxTempoElement:tempoMarker
                                                                                                     offset:offset
                                                                                                        log:log];

            // -- Even if something goes wrong in getting the marker element, we can skip this element without failing
            if (newValue) {
                [newValues addObject:newValue];
            }
        }
    }
    else {
        NSString *averageBPMString = [[trackElement attributeForName:@"AverageBpm"] stringValue];
        if (averageBPMString) {
            NSDecimalNumber *tempo = [NSDecimalNumber decimalNumberWithString:averageBPMString];
            NxAParsedMarkerValue *newValue = [NxAParsedMarkerValue parsedGridMarkerValueAtPositionInSeconds:[NSDecimalNumber decimalNumberWithString:@"0.0"]
                                                                                             beatsPerMinute:tempo];
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
