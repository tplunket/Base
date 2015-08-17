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

#import "NxATestCase.h"
#import "Track Collection/AudioFiles/OSX/NxAOSXAudioFile.h"
#import "Track Collection/AudioFiles/OSX/Internal/NxAOSXAudioFile.h"

#pragma mark Public Interface

@interface NxAOSXAudioFileTests : NxATestCase

#pragma mark Class Methods

/*!
 * Create a stubbed audio file.
 * \param path Path to use for the stubbed audio file.
 * \param context Context where to create the file.
 * \return A stubbed audio file for the given path.
 */
+ (NxAOSXAudioFile *)stubbedOSXAudioFileWithPath:(NSString *)path
                                         context:(NSManagedObjectContext *)context;

@end
