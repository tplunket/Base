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

#import "NxAWaveformView.h"

@class NxAWaveformViewMarker;

#pragma mark Public Interface

/*! A custom waveform view for Rekord Buddy. */
@interface NxARekordBuddyWaveformView : NxAWaveformView

#pragma mark Instance Methods

/*! Reset the list of markers stored by the view. */
- (void)resetMarkers;

/*!
 * Add a marker.
 * \param marker Marker to add.
 */
- (void)addMarker:(NxAWaveformViewMarker *)marker;

@end
