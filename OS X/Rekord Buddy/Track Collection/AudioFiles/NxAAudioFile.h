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

@class NxATrack;
@class NxAAudioData;

#pragma mark Public Interface

/*!
 * File containing the audio data for a track.
 * \attention This is an abstract class which should never be instantiated directly.
 */
@interface NxAAudioFile : NSManagedObject

#pragma mark Properties

/*! Audio data for this file. */
@property (readonly, nonatomic) NxAAudioData *audioData;

/*! Track who's file this is. */
@property (readonly, nonatomic) NxATrack *parentTrack;

/*! Indicate if this file is found on the current device. */
@property (readonly, nonatomic) BOOL isLocal;

@property (readonly, nonatomic) NSString *path;

#pragma mark Instance Methods

- (void)destroy;

/*!
 * Add the file's pasteboard URL representation to an array.
 * \param array Where to store the pasteboard representation of the file.
 */
- (void)addURLToPasteboardArray:(NSMutableArray *)array;

@end
