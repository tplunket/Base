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

#import "NSManagedObjectContext+NxAUtility.h"
#import "NxATrack+NxAOSXAudioFile.h"
#import "Track Collection/AudioFiles/OSX/NxAOSXAudioFile.h"
#import "Track Collection/Internal/NxATrack.h"

#pragma mark Implementation

@implementation NxATrack (NxAOSXAudioFile)

#pragma mark Factory Methods

+ (NxATrack *)trackFromOSXAudioFile:(NxAOSXAudioFile *)file
                            context:(NSManagedObjectContext *)context
{
    NxAAssertNotNil(file);
    NxAAssertNotNil(context);

    // -- Create a new track.
    NxATrack *track = [context insertNewObjectForEntity:@"Track"];

    NSMutableSet *files = [track mutableSetValueForKey:@"p_files"];
    [files addObject:file];

    return track;
}

@end
