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

#import <AVFoundation/AVFoundation.h>

#import "NxAWaveformViewController.h"
#import "NxARekordBuddyWaveformView.h"
#import "NxAWaveformViewMarker.h"
#import "NxATrack+NxAWaveformViewController.h"

#pragma mark Private Interface

@interface NxAWaveformViewController ()

/*! Track currently displayed in the waveform view. */
@property (strong, nonatomic) NxATrack *displayedTrack;

@end

#pragma mark - Implementation

@implementation NxAWaveformViewController

#pragma mark Instance Methods

- (void)resetMarkers
{
    NxARekordBuddyWaveformView *view = (NxARekordBuddyWaveformView *)self.view;
    [view resetMarkers];
}

- (void)addCueMarkerAt:(double)startPositionInSeconds
          hotCueNumber:(NSInteger)hotCueNumber
{
    NxAAssertTrue(startPositionInSeconds >= 0.0);

    NxAWaveformViewMarker *marker = [NxAWaveformViewMarker cueMarkerAt:startPositionInSeconds hotCueNumber:hotCueNumber];
    NxARekordBuddyWaveformView *view = (NxARekordBuddyWaveformView *)self.view;

    [view addMarker:marker];
}

- (void)addGridMarkerAt:(double)startPositionInSeconds
         beatsPerMinute:(double)beatsPerMinute
{
    NxAAssertTrue(startPositionInSeconds >= 0.0);
    NxAAssertTrue(beatsPerMinute > 40.0 && beatsPerMinute < 300.0);

    NxAWaveformViewMarker *marker = [NxAWaveformViewMarker gridMarkerAt:startPositionInSeconds beatsPerMinute:beatsPerMinute];
    NxARekordBuddyWaveformView *view = (NxARekordBuddyWaveformView *)self.view;

    [view addMarker:marker];
}

- (void)addLoopMarkerAt:(double)startPositionInSeconds
        lengthInSeconds:(double)lengthInSeconds
           hotCueNumber:(NSInteger)hotCueNumber
{
    NxAAssertTrue(startPositionInSeconds >= 0.0);
    NxAAssertTrue(lengthInSeconds > 0.0);
    
    NxAWaveformViewMarker *marker = [NxAWaveformViewMarker loopMarkerAt:startPositionInSeconds
                                                        lengthInSeconds:lengthInSeconds
                                                           hotCueNumber:hotCueNumber];
    NxARekordBuddyWaveformView *view = (NxARekordBuddyWaveformView *)self.view;

    [view addMarker:marker];
}

- (void)displaySamples:(NSData *)samples
       lengthInSeconds:(double)lengthInSeconds
{
    NxAWaveformView *waveformView = (NxAWaveformView *)self.view;
    [waveformView displaySamples:samples length:lengthInSeconds];
}

- (void)resetSamplesDisplayed
{
    NxARekordBuddyWaveformView *view = (NxARekordBuddyWaveformView *)self.view;
    [view resetSamplesDisplayed];
}

- (void)displayTrack:(NxATrack *)track
{
    if (self.displayedTrack) {
        // -- If we have a current track displayed, we clear its caches since we're unloading it.
        [self.displayedTrack clearCaches];
    }

    // -- We keep track of the currently displayed track.
    self.displayedTrack = track;

    // -- Then we get the track to add itself to this controller.
    [track addToWaveformViewController:self];
}

@end
