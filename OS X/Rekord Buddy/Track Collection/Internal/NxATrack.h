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

@class NxAAudioData;
@class NxAAudioFile;

#import "Track Collection/NxATrack.h"
#import "Track Collection/Tags/Internal/NxATaggedObject.h"

#pragma mark Private Interface

@interface NxATrack ()

#pragma mark Properties

/*!
 * Files for the audio data of this track.
 * \details Audio can be in different locations and format based on the device the track is accessed on.
 */
@property (strong, nonatomic) NSSet *p_files;

/*! Track property tags which point to this track. */
@property (strong, nonatomic) NSSet *p_parentTrackProperties;

/*! Audio data for this track (Transient). */
@property (strong, nonatomic) NxAAudioData *p_audioData;

@end
