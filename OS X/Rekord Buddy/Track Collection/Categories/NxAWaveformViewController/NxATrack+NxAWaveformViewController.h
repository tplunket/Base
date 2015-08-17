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

@class NxAWaveformViewController;

#pragma mark Public Interface

@interface NxATrack (NxAWaveformViewController)

#pragma mark Instance Methods

/*!
 * Add the track to a waveform view controller.
 * \param controller Controller to add the track to.
 */
- (void)addToWaveformViewController:(NxAWaveformViewController *)controller;

@end
