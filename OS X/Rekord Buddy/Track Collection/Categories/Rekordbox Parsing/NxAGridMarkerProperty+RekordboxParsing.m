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

#import "NxAGridMarkerProperty+RekordboxParsing.h"
#import "Track Collection/Categories/Rekordbox Parsing/Internal/NxAMarkerProperty+RekordboxParsing.h"
#import "NSDecimalNumber+NxAUtility.h"
#import "NSXMLElement+NxAUtility.h"
#import "Track Collection/Tags/Marker Properties/Internal/NxAGridMarkerProperty.h"

#pragma mark Implementation

@implementation NxAGridMarkerProperty (RekordboxParsing)

#pragma mark Instance Methods

- (NSXMLElement *)p_rekordboxMarkerElementWithMarkerOffsetBy:(NSDecimalNumber *)offset
                                               logMessagesIn:(NxAMessageLog *)log
{
    NSDecimalNumber *start = self.p_pos;
    if (offset) {
        start = [start decimalNumberByAdding:offset];
    }

    NSXMLElement *markerElement = [NSXMLElement elementWithName:@"TEMPO"];
    [markerElement addAttributeWithName:@"Inizio" stringValue:[NSString stringWithFormat:@"%.3f", start.floatValue]];
    [markerElement addAttributeWithName:@"Bpm" stringValue:[NSString stringWithFormat:@"%.2f", self.p_bpm.floatValue]];
    [markerElement addAttributeWithName:@"Metro" stringValue:@"4/4"];
    [markerElement addAttributeWithName:@"Battito" stringValue:@"1"];

    return markerElement;
}

@end
