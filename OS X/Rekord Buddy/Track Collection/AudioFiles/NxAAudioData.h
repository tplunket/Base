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

#pragma mark Public Interface

/*!
 * Audio data for a track.
 * \attention This is an abstract class which should never be instantiated directly.
 */
@interface NxAAudioData : NSObject

#pragma mark Factory Methods

/*!
 * Create audio data from the URL of an audio file.
 * \param url URL of a file to read for this audio data.
 * \param sampleRate Sample rate the audio should be converted to.
 * \param channels Number of channels the audio should be converted to.
 * \return The data is successful or nil otherwise.
 */
+ (instancetype)dataWithContentsOfURL:(NSURL *)url
                           sampleRate:(NSUInteger)sampleRate
                             channels:(NSUInteger)channels;

/*!
 * Create audio data by down sampling existing data.
 * \param data Audio data to use as source material.
 * \param downSample Divider by which to down sample the data. For example a value
 *                   of 4 would return one sample for each 4 that the source data contains.
 * \param mono If YES, all the channels in the audio data are averaged to one mono sample.
 * \return The new audio data if succesful or nil otherwise.
 */
+ (instancetype)dataWithAudioData:(NxAAudioData *)data
                       downSample:(NSUInteger)downSample
                             mono:(BOOL)mono;

@end
