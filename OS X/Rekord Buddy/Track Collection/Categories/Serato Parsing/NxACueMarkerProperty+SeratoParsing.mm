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

#import "NxAMarkerProperty+SeratoParsing.h"
#import "Track Collection/Tags/Marker Properties/Internal/NxACueMarkerProperty.h"

using namespace NxA;
using namespace NxA::Serato;
using namespace std;

#pragma mark Implementation

@implementation NxACueMarkerProperty (SeratoParsing)

#pragma mark Accessors

- (Serato::CueMarker::Pointer)seratoCueMarker
{
    NSInteger hotCueIndex = self.p_hotCue.integerValue;
    if (hotCueIndex > 7) {
        // -- We shifted the last 4 hotcues up when reading them in, we shift them back down.
        hotCueIndex -= 4;
    }
    NxAAssertTrue(hotCueIndex <= NxAMarkerMaximumHotcueIndex);

    NSDecimalNumber *posInMilliseconds = [self.p_pos decimalNumberByMultiplyingByPowerOf10:3];

    auto marker = CueMarker::markerWithLabelPositionAndIndex(String::stringWith(self.p_name.UTF8String),
                                                             static_cast<uinteger32>(posInMilliseconds.integerValue),
                                                             hotCueIndex);
    return marker;
}

@end
