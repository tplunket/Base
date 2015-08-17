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

#import "NxACueMarkerProperty+TraktorParsing.h"
#import "NSDecimalNumber+NxAUtility.h"
#import "NSXMLElement+NxAUtility.h"
#import "Track Collection/Tags/Marker Properties/NxAGridMarkerProperty.h"
#import "Track Collection/Tags/Marker Properties/NxALoopMarkerProperty.h"
#import "Track Collection/Tags/Marker Properties/Internal/NxACueMarkerProperty.h"

#pragma mark Implementation

@implementation NxACueMarkerProperty (TraktorParsing)

#pragma mark Instance Methods

- (void)addToTraktorEntryElement:(NSXMLElement *)entryElement
                             log:(NxAMessageLog *)log
{
    NSXMLElement *newElement = [NSXMLElement elementWithName:@"CUE_V2"];

    [newElement addAttributeWithName:@"NAME" stringValue:self.p_name];
    [newElement addAttributeWithName:@"DISPL_ORDER" stringValue:@"0"];

    NSString *type = nil;
    if ([self p_hasCustomFlag:NxAMarkerIsLoadMarker]) {
        // -- Load Marker.
        type = @"3";
    }
    else if ([self p_hasCustomFlag:NxAMarkerIsFadeInMarker]) {
        // -- Fade In Marker.
        type = @"1";
    }
    else if ([self p_hasCustomFlag:NxAMarkerIsFadeOutMarker]) {
        // -- Fade Out Marker.
        type = @"2";
    }
    else {
        // -- Cue
        type = @"0";
    }

    [newElement addAttributeWithName:@"TYPE" stringValue:type];

    // -- Convert the position from seconds to milliseconds.
    NSDecimalNumber *pos = [self.p_pos decimalNumberByMultiplyingByPowerOf10:3];

    NSString *posString = [NSString stringWithFormat:@"%.4f", pos.floatValue];
    [newElement addAttributeWithName:@"START" stringValue:posString];

    [newElement addAttributeWithName:@"LEN" stringValue:@"0"];
    [newElement addAttributeWithName:@"REPEATS" stringValue:@"-1"];

    NSString *index = @"-1";
    if (self.p_hotCue && self.p_hotCue.integerValue < 8) {
        index = self.p_hotCue.stringValue;
    }

    [newElement addAttributeWithName:@"HOTCUE" stringValue:index];

    return [entryElement addChild:newElement];
}

@end
