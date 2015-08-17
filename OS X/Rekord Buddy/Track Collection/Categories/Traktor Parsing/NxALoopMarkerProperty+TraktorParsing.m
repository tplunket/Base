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

#import "NxALoopMarkerProperty+TraktorParsing.h"
#import "NSXMLElement+NxAUtility.h"
#import "Track Collection/Tags/Marker Properties/Internal/NxALoopMarkerProperty.h"

#pragma mark Implementation

@implementation NxALoopMarkerProperty (TraktorParsing)

#pragma mark Instance Methods

- (void)addToTraktorEntryElement:(NSXMLElement *)entryElement
                             log:(NxAMessageLog *)log
{
    NSXMLElement *newElement = [NSXMLElement elementWithName:@"CUE_V2"];

    [newElement addAttributeWithName:@"NAME" stringValue:self.p_name];
    [newElement addAttributeWithName:@"DISPL_ORDER" stringValue:@"0"];
    [newElement addAttributeWithName:@"TYPE" stringValue:@"5"];

    // -- Convert the position from seconds to milliseconds.
    NSDecimalNumber *pos = [self.p_pos decimalNumberByMultiplyingByPowerOf10:3];

    NSString *posString = [NSString stringWithFormat:@"%.4f", pos.floatValue];
    [newElement addAttributeWithName:@"START" stringValue:posString];

    // -- Convert the length from seconds to milliseconds.
    NSDecimalNumber *len = [self.p_length decimalNumberByMultiplyingByPowerOf10:3];

    NSString *lenString = [NSString stringWithFormat:@"%.4f", len.floatValue];
    [newElement addAttributeWithName:@"LEN" stringValue:lenString];

    [newElement addAttributeWithName:@"REPEATS" stringValue:@"-1"];

    NSString *index = @"-1";
    if (self.p_hotCue && self.p_hotCue.integerValue < 8) {
        index = self.p_hotCue.stringValue;
    }

    [newElement addAttributeWithName:@"HOTCUE" stringValue:index];

    [entryElement addChild:newElement];
}

@end
