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

#import "NxALoopMarkerProperty+SeratoParsing.h"
#import "Track Collection/Tags/Marker Properties/Internal/NxALoopMarkerProperty.h"

using namespace NxA;
using namespace NxA::Serato;
using namespace std;

#pragma mark Implementation

@implementation NxALoopMarkerProperty (SeratoParsing)

- (LoopMarker::Pointer)seratoLoopMarker
{
    // -- We shifted the last 4 hotcues up when reading them in, we shift them back down.
    NSInteger hotCueIndex = self.p_hotCue.integerValue - 4;
    if (hotCueIndex > 7) {
        hotCueIndex -= 4;
    }
    NxAAssertTrue(hotCueIndex <= NxAMarkerMaximumHotcueIndex);

    NSDecimalNumber *startPosInMilliseconds = [self.p_pos decimalNumberByMultiplyingByPowerOf10:3];
    NSDecimalNumber *endPosInMilliseconds = [[self.p_pos decimalNumberByAdding:self.p_length] decimalNumberByMultiplyingByPowerOf10:3];

    auto marker = LoopMarker::markerWithLabelStartEndPositionsAndIndex(String::stringWith(self.p_name.UTF8String),
                                                                       static_cast<uinteger32>(startPosInMilliseconds.integerValue),
                                                                       static_cast<uinteger32>(endPosInMilliseconds.integerValue),
                                                                       hotCueIndex);
    return marker;
}

@end
