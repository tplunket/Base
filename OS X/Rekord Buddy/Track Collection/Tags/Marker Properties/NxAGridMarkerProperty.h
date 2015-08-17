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

#import "NxAMarkerProperty.h"

#pragma mark Public Interface

/*! Marker used to indicate a downbeat and a bpm for the grid in a track. */
@interface NxAGridMarkerProperty : NxAMarkerProperty

#pragma mark Factory Methods

+ (instancetype)gridMarkerAtPosition:(NSDecimalNumber *)position
                                 bpm:(NSDecimalNumber *)bpm
                         customFlags:(NSArray *)flags
                             context:(NSManagedObjectContext *)context;

#pragma mark Class Methods

/*!
 * Calculate the average BPM of a group of markers.
 * \param markers Markers to average the BPM from.
 * \return The average BPM for all the markers.
 */
+ (NSDecimalNumber *)averageBpmForMarkers:(NSArray *)markers;

@end
