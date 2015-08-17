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

#import "Track Collection/Categories/Rekordbox Parsing/Internal/NxAMarkerProperty+RekordboxParsing.h"
#import "Track Collection/Tags/Marker Properties/Internal/NxACueMarkerProperty.h"
#import "NSXMLElement+NxAUtility.h"

#pragma mark Implementation

@implementation NxACueMarkerProperty (RekordboxParsing)

#pragma mark Instance Methods

- (NSXMLElement *)p_rekordboxMarkerElementWithMarkerOffsetBy:(NSDecimalNumber *)offset
                                               logMessagesIn:(NxAMessageLog *)log
{
    NSXMLElement *markerElement = [NSXMLElement elementWithName:@"POSITION_MARK"];

    [markerElement addAttributeWithName:@"Name" stringValue:self.p_name];

    NSString *markerType = @"0";
    if ([self p_hasCustomFlag:NxAMarkerIsLoadMarker]) {
        // -- Currently Rekordbox ignores load/fade marker types.
        markerType = @"3";
    }
    else if ([self p_hasCustomFlag:NxAMarkerIsFadeInMarker]) {
        markerType = @"1";
    }
    else if ([self p_hasCustomFlag:NxAMarkerIsFadeOutMarker]) {
        markerType = @"2";
    }
    [markerElement addAttributeWithName:@"Type" stringValue:markerType];

    NSDecimalNumber *start = self.p_pos;
    if (offset) {
        start = [start decimalNumberByAdding:offset];
    }
    [markerElement addAttributeWithName:@"Start" stringValue:[NSString stringWithFormat:@"%0.3f", start.floatValue]];

    NSInteger index = -1;
    if (self.p_hotCue) {
        index = self.p_hotCue.integerValue;
        if (index > 3) {
            index = -1;
        }
    }

    [markerElement addAttributeWithName:@"Num" stringValue:[NSString stringWithFormat:@"%ld", index]];

    return markerElement;
}

@end
