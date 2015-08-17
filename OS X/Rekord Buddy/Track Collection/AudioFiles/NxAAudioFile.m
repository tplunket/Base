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

#import "NxAAudioFile.h"
#import "Track Collection/AudioFiles/Internal/NxAAudioFile.h"
#import "NxAAudioData.h"

#pragma mark Implementation

@implementation NxAAudioFile

#pragma mark Accessors

// -- Automatically generated for Core Data properties.
@dynamic p_deviceID;
@dynamic p_parentTrack;

- (NxAAudioData *)audioData
{
    // -- This should be overloaded by any derived class
    ALog(@"Illegal base class call from '%@'.", self.class);

    return nil;
}

- (NxATrack *)parentTrack
{
    return self.p_parentTrack;
}

- (BOOL)isLocal
{
    // -- This should be overloaded by any derived class
    ALog(@"Illegal base class call from '%@'.", self.class);

    return NO;
}

- (NSString *)path
{
    // -- This should be overloaded by any derived class
    ALog(@"Illegal base class call from '%@'.", self.class);

    return nil;
}

#pragma mark Instance Methods

- (void)destroy
{
    NxAAssertNil(self.parentTrack);

    [self.managedObjectContext deleteObject:self];
}

- (void)addURLToPasteboardArray:(NSArray *)array
{
    // -- This should be overloaded by any derived class
    ALog(@"Illegal base class call from '%@'.", self.class);
}

- (NxAAudioData *)audioDataWithSampleRate:(NSUInteger)sampleRate
                                 channels:(NSUInteger)channels
{
    // -- This should be overloaded by any derived class
    ALog(@"Illegal base class call from '%@'.", self.class);

    return nil;
}

@end
