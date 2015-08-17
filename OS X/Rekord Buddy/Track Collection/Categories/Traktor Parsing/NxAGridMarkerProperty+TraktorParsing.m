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

#import "NxAGridMarkerProperty+TraktorParsing.h"
#import "NSXMLElement+NxAUtility.h"
#import "Track Collection/Tags/Marker Properties/Internal/NxAGridMarkerProperty.h"

#pragma mark Implementation

@implementation NxAGridMarkerProperty (TraktorParsing)

#pragma mark Instance Methods

- (void)p_addToTraktorEntryElement:(NSXMLElement *)entryElement
                          withName:(NSString *)name
                         andHotCue:(NSNumber *)hotCueIndex
                               log:(NxAMessageLog *)log
{
    NxAAssertNotNil(name);

    NSXMLElement *newElement = [NSXMLElement elementWithName:@"CUE_V2"];

    [newElement addAttributeWithName:@"NAME" stringValue:name];
    [newElement addAttributeWithName:@"DISPL_ORDER" stringValue:@"0"];
    [newElement addAttributeWithName:@"TYPE" stringValue:@"4"];

    // -- Convert the position from seconds to milliseconds.
    NSDecimalNumber *pos = [self.p_pos decimalNumberByMultiplyingByPowerOf10:3];

    NSString *posString = [NSString stringWithFormat:@"%.4f", pos.floatValue];
    [newElement addAttributeWithName:@"START" stringValue:posString];

    [newElement addAttributeWithName:@"LEN" stringValue:@"0"];
    [newElement addAttributeWithName:@"REPEATS" stringValue:@"-1"];

    NSString *hotCue = hotCueIndex ? hotCueIndex.stringValue : @"-1";
    [newElement addAttributeWithName:@"HOTCUE" stringValue:hotCue];

    NSString *bpmString = [NSString stringWithFormat:@"%3.3f", self.p_bpm.floatValue];

    NSArray *existingTempos = [entryElement elementsForName:@"TEMPO"];
    NxAAssertTrue(existingTempos.count <= 1);

    NSXMLElement *existingTempo = existingTempos.firstObject;
    if (existingTempo) {
        NSString *existingBpmString = [[existingTempo attributeForName:@"BPM"] stringValue];
        if (![existingBpmString isEqualToString:bpmString]) {
            NxAWarning *warning = [NxAWarning warningWithDescription:NSLocalizedString(@"in your Rekord Buddy collection with multiple BPM value. Traktor only supports a constant BPM throughout the track.",
                                                                                       @"")];
            [log addWarning:warning inContextWithPerObjectWarningInfo:@""];
        }
    }
    else {
        NSXMLElement *tempoElement = [NSXMLElement elementWithName:@"TEMPO"];
        [tempoElement addAttributeWithName:@"BPM" stringValue:bpmString];
        [tempoElement addAttributeWithName:@"BPM_QUALITY" stringValue:@"100"];

        NSXMLNode *bpmAttribute = [tempoElement attributeForName:@"BPM"];
        bpmAttribute.stringValue = bpmString;

        [entryElement addChild:tempoElement];
    }

    [entryElement addChild:newElement];
}

- (void)addToTraktorEntryElement:(NSXMLElement *)entryElement
                        withName:(NSString *)name
                       andHotCue:(NSNumber *)hotCueIndex
                             log:(NxAMessageLog *)log
{
    [self p_addToTraktorEntryElement:entryElement
                            withName:name ? name : @""
                           andHotCue:hotCueIndex log:log];
}

- (void)addToTraktorEntryElement:(NSXMLElement *)entryElement
                             log:(NxAMessageLog *)log
{
    [self p_addToTraktorEntryElement:entryElement
                            withName:@""
                           andHotCue:nil
                                 log:log];
}

@end
