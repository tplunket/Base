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

#import "Track Collection/AudioFiles/OSX/NxAOSXAudioFile.h"

#pragma mark Private Interface

@interface NxAOSXAudioFile ()

#pragma mark Class Methods

+ (void)p_setTestingBootVolumeName:(NSString *)name;

+ (void)p_setTestingOtherVolumeNames:(NSSet *)names;

+ (void)p_setTestingFileManager:(NSFileManager *)manager;

#pragma mark Properties

/*! Bookmark to this audio file. */
@property (strong, nonatomic) NSData *p_bookmark;

/*! String version of the path to the file. */
@property (strong, nonatomic) NSString *p_path;

@end
