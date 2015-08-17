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

#import "NxAMarkerProperty+TraktorParsing.h"
#import "NxAGridMarkerProperty+TraktorParsing.h"
#import "NSDecimalNumber+NxAUtility.h"
#import "NSXMLElement+NxAUtility.h"
#import "Track Collection/Tags/Marker Properties/NxAGridMarkerProperty.h"
#import "Track Collection/Tags/Marker Properties/NxALoopMarkerProperty.h"
#import "Track Collection/Tags/Marker Properties/Internal/NxACueMarkerProperty.h"

#pragma mark Implementation

@implementation NxAMarkerProperty (TraktorParsing)

#pragma mark Class Methods

+ (BOOL)updateTraktorMarkers:(NSArray *)markers
                entryElement:(NSXMLElement *)entryElement
                         log:(NxAMessageLog *)log
{
    // -- First we delete any existing CUE_V2 and TEMPO elements.
    [entryElement removeChildrenWithName:@"CUE_V2"];
    [entryElement removeChildrenWithName:@"TEMPO"];

    // -- Then we add the current markers.
    for (NSInteger index = 0; index < markers.count; ++index) {
        NxAMarkerProperty *marker =  markers[index];

        if ([marker p_hasCustomFlag:NxAMarkerWasStoredAsAHotCue]) {
            NxAAssertTrue([marker isKindOfClass:[NxAGridMarkerProperty class]]);

            if (index != (markers.count - 1)) {
                NxAMarkerProperty *nextMarker =  markers[index + 1];

                if ([nextMarker isKindOfClass:[NxACueMarkerProperty class]] &&
                    [marker.p_pos isEqualToNumber:nextMarker.p_pos]) {
                    NxACueMarkerProperty *nextCueMarker = (NxACueMarkerProperty *)nextMarker;

                    if (nextCueMarker.p_hotCue) {
                        NxAGridMarkerProperty *gridMarker = (NxAGridMarkerProperty *)marker;
                        [gridMarker addToTraktorEntryElement:entryElement
                                                    withName:nextCueMarker.p_name
                                                   andHotCue:nextCueMarker.p_hotCue
                                                         log:log];

                        ++index;

                        continue;
                    }
                }
            }
        }

        [marker addToTraktorEntryElement:entryElement log:log];
    }

    return YES;
}

#pragma mark Instance Methods

- (void)addToTraktorEntryElement:(NSXMLElement *)entryElement
                             log:(NxAMessageLog *)log
{
    ALog(@"Invalid base class call.");
}

@end
