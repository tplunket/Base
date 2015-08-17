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

#import "NxAAudioData.h"

@class NxAWaveformViewController;

#pragma mark Public Interface

@interface NxAAudioData (NxAWaveformViewController)

#pragma mark Instance Methods

/*!
 * Add the audio data to a waveform view controller.
 * \param controller Controller to add the data to.
 */
- (void)addToWaveformViewController:(NxAWaveformViewController *)controller;

@end
