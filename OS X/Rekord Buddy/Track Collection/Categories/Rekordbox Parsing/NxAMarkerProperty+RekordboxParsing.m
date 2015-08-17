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

#import "Track Collection/Categories/Rekordbox Parsing/NxAMarkerProperty+RekordboxParsing.h"
#import "NSDecimalNumber+NxAUtility.h"
#import "NSXMLElement+NxAUtility.h"
#import "Track Collection/Tags/Marker Properties/NxALoopMarkerProperty.h"
#import "Track Collection/Tags/Marker Properties/NxAGridMarkerProperty.h"
#import "NxAGridMarkerProperty+RekordboxParsing.h"
#import "Track Collection/Categories/Rekordbox Parsing/Internal/NxAMarkerProperty+RekordboxParsing.h"

#pragma mark Implementation

@implementation NxAMarkerProperty (RekordboxParsing)

#pragma mark Class Methods

+ (NSDecimalNumber *)offsetForTrackElement:(NSXMLElement *)trackElement
{
    NSString *location = [[trackElement attributeForName:@"Location"] stringValue];
    if ([location.pathExtension isEqualToString:@"m4a"]) {
        static NSDecimalNumber *m4aOffset = nil;
        static dispatch_once_t onceToken = 0;
        dispatch_once(&onceToken, ^{
            m4aOffset = [NSDecimalNumber decimalNumberWithDouble:0.0048];
        });

        // -- Rekordbox and the CDJs have a bug in their M4A code that causes about 50ms of silence to be inserted a the beginning of a track
        // -- because of this we need to offset the markers accordingly so they still match other programs
        return m4aOffset;
    }

    return nil;
}

+ (void)updateRekordboxMarkers:(NSArray *)markers
                  trackElement:(NSXMLElement *)trackElement
                           log:(NxAMessageLog *)log
{
    NSDecimalNumber *offset = [NxACueMarkerProperty offsetForTrackElement:trackElement];
    if (offset) {
        // -- Since we're writing the markers back, we need to make sure the offset is negative.
        offset = [offset decimalNumberByMultiplyingBy:[NSDecimalNumber decimalNumberWithString:@"-1"]];
    }

    // -- We remove all the old POSITION_MARK and TEMPO markers.
    [trackElement removeChildrenWithName:@"POSITION_MARK"];
    [trackElement removeChildrenWithName:@"TEMPO"];

    for (NxACueMarkerProperty *marker in markers) {
        NSXMLElement *rekordboxMarker = [marker p_rekordboxMarkerElementWithMarkerOffsetBy:offset logMessagesIn:log];
        [trackElement addChild:rekordboxMarker];
    }
}

#pragma mark Instance Methods

- (NSXMLElement *)p_rekordboxMarkerElementWithMarkerOffsetBy:(NSDecimalNumber *)offset
                                               logMessagesIn:(NxAMessageLog *)log
{
    ALog(@"Invalid base class call.");
    return nil;
}

@end
