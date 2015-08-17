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

#import "NxATrack+NxAWaveformViewController.h"
#import "Track Collection/Internal/NxATrack.h"
#import "NxAWaveformViewController.h"
#import "NxAAudioData+NxAWaveformViewController.h"
#import "NxACueMarkerProperty+NxAWaveformViewController.h"

#pragma mark Implementation

@implementation NxATrack (NxAWaveformViewController)

- (void)addToWaveformViewController:(NxAWaveformViewController *)controller
{
    NxAAudioData *audioData = self.p_audioData;

    if (audioData) {
        // -- For displaying the audio waveform, we down sample it and convert if to mono.
        audioData = [NxAAudioData dataWithAudioData:audioData downSample:6 mono:YES];

        // -- Add the audio data to the waveform controller.
        [audioData addToWaveformViewController:controller];
    }
    else {
        [controller resetSamplesDisplayed];
    }

    // -- Clear any previous markers the view might have.
    [controller resetMarkers];

    // -- Add the markers for this track.
    NSArray *markers = [NxAMarkerProperty tagsIn:self];
    for (NxACueMarkerProperty *marker in markers) {
        [marker addToWaveformViewController:controller];
    }
}

@end
