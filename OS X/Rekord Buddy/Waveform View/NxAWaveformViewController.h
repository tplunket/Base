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

#import "NxATrack.h"

#pragma mark Public Interface

/*! A controller for the waveform view */
@interface NxAWaveformViewController : NSViewController

#pragma mark Instance Methods

- (void)resetMarkers;

- (void)addCueMarkerAt:(double)startPositionInSeconds
          hotCueNumber:(NSInteger)hotCueNumber;

- (void)addGridMarkerAt:(double)startPositionInSeconds
         beatsPerMinute:(double)beatsPerMinute;

- (void)addLoopMarkerAt:(double)startPositionInSeconds
        lengthInSeconds:(double)lengthInSeconds
           hotCueNumber:(NSInteger)hotCueNumber;

- (void)displaySamples:(NSData *)samples
       lengthInSeconds:(double)lengthInSeconds;

- (void)resetSamplesDisplayed;

- (void)displayTrack:(NxATrack *)track;

@end
