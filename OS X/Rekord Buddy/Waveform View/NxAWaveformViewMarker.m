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

#import "NxAWaveformViewMarker.h"

#pragma mark Implementation

@implementation NxAWaveformViewMarker

#pragma mark Factory Methods

+ (instancetype)cueMarkerAt:(double)startPositionInSeconds
               hotCueNumber:(NSInteger)hotCueNumber
{
    NxAWaveformViewMarker *marker = [[NxAWaveformViewMarker alloc] init];
    if (!marker) {
        return nil;
    }

    marker.type = NxAWaveformViewCueMarkerType;
    marker.startPositionInSeconds = startPositionInSeconds;
    marker.hotCueNumber = hotCueNumber;

    return marker;
}

+ (instancetype)loopMarkerAt:(double)startPositionInSeconds
             lengthInSeconds:(double)lengthInSeconds
                hotCueNumber:(NSInteger)hotCueNumber
{
    NxAWaveformViewMarker *marker = [[NxAWaveformViewMarker alloc] init];
    if (!marker) {
        return nil;
    }

    marker.type = NxAWaveformViewLoopMarkerType;
    marker.startPositionInSeconds = startPositionInSeconds;
    marker.lengthInSeconds = lengthInSeconds;
    marker.hotCueNumber = hotCueNumber;

    return marker;
}

+ (instancetype)gridMarkerAt:(double)startPositionInSeconds
              beatsPerMinute:(double)beatsPerMinute;

{
    NxAWaveformViewMarker *marker = [[NxAWaveformViewMarker alloc] init];
    if (!marker) {
        return nil;
    }

    marker.type = NxAWaveformViewGridMarkerType;
    marker.startPositionInSeconds = startPositionInSeconds;
    marker.beatsPerMinute = beatsPerMinute;

    return marker;
}

#pragma mark Overridden NSObject Methods

- (NSComparisonResult)compare:(NxAWaveformViewMarker *)otherMarker
{
    if (self.startPositionInSeconds > otherMarker.startPositionInSeconds) {
        return NSOrderedDescending;
    }
    else if (self.startPositionInSeconds < otherMarker.startPositionInSeconds) {
        return NSOrderedAscending;
    }

    return NSOrderedSame;
}

@end
