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

#import "NxALoopMarkerProperty+RekordboxParsing.h"
#import "NSDecimalNumber+NxAUtility.h"
#import "NSXMLElement+NxAUtility.h"
#import "Track Collection/Tags/Marker Properties/Internal/NxALoopMarkerProperty.h"

#pragma mark Implementation

@implementation NxALoopMarkerProperty (RekordboxParsing)

#pragma mark Instance Methods

- (NSXMLElement *)p_rekordboxMarkerElementWithMarkerOffsetBy:(NSDecimalNumber *)offset
                                               logMessagesIn:(NxAMessageLog *)log
{
    NSXMLElement *markerElement = [NSXMLElement elementWithName:@"POSITION_MARK"];

    [markerElement addAttributeWithName:@"Name" stringValue:self.p_name];

    NSDecimalNumber *start = self.p_pos;
    NSDecimalNumber *end = [start decimalNumberByAdding:self.p_length];
    if (offset) {
        start = [start decimalNumberByAdding:offset];
        end = [end decimalNumberByAdding:offset];
    }
    [markerElement addAttributeWithName:@"Start" stringValue:[NSString stringWithFormat:@"%.3f", start.floatValue]];
    [markerElement addAttributeWithName:@"Type" stringValue:@"4"];
    [markerElement addAttributeWithName:@"End" stringValue:[NSString stringWithFormat:@"%.3f",  end.floatValue]];

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
