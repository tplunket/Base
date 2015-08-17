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

@class NxAOSXAudioFile;

#import "NxATrack.h"

#pragma mark Public Interface

@interface NxATrack (NxAOSXAudioFile)

#pragma mark Factory Methods

/*!
 * Create new track.
 * \param file Audio file for this track.
 * \param context Managed object context to create the new object in.
 * \return A new track based on the audio file location.
 */
+ (NxATrack *)trackFromOSXAudioFile:(NxAOSXAudioFile *)file
                            context:(NSManagedObjectContext *)context;

@end
