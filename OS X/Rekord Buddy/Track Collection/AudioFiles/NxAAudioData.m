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

#import "NxAAudioData.h"
#import "Track Collection/AudioFiles/Internal/NxAAudioData.h"

#pragma mark Implementation

@implementation NxAAudioData

#pragma mark Factory Methods

+ (instancetype)dataWithContentsOfURL:(NSURL *)url
                           sampleRate:(NSUInteger)sampleRate
                             channels:(NSUInteger)channels
{
    // -- This should be overloaded by any derived class
    ALog(@"Illegal base class call from '%@'.", self.class);

    return nil;
}

+ (instancetype)dataWithAudioData:(NxAAudioData *)data
                       downSample:(NSUInteger)downSample
                             mono:(BOOL)mono
{
    NxAAssertNotNil(data);
    NxAAssertTrue(downSample > 0);

    const float *srcData = data.p_data.bytes;
    NSUInteger sampleCount = data.p_data.length / sizeof(float);
    NSUInteger averagedSampleCount = downSample * (mono ? data.p_channels : 1);

    NSMutableData *results = [NSMutableData dataWithLength:sizeof(float) * (sampleCount / averagedSampleCount)];
    float *resultsData = results.mutableBytes;
    NSInteger resultsIndex = 0;

    for (NSUInteger index = 0; index < sampleCount;) {
        if (mono) {
            float avg = 0.0;
            NSInteger valueCount = 0;
                
            for (NSInteger avgIndex = 0; avgIndex < averagedSampleCount; ++avgIndex) {
                if (index < sampleCount) {
                    avg += srcData[index];

                    ++valueCount;
                }

                ++index;
            }

            if (valueCount) {
                avg /= valueCount;
            }
            
            resultsData[resultsIndex] = avg;
            ++resultsIndex;
        }
        else {
            for (NSInteger channelIndex = 0; channelIndex < data.p_channels; ++channelIndex) {
                float avg = 0.0;
                NSInteger valueCount = 0;

                for (NSInteger avgIndex = 0; avgIndex < averagedSampleCount; ++avgIndex) {
                    NSUInteger lookUpIndex = index + avgIndex + channelIndex;

                    if (lookUpIndex < sampleCount) {
                        avg += srcData[lookUpIndex];

                        ++valueCount;
                    }
                }

                if (valueCount) {
                    avg /= valueCount;
                }

                resultsData[resultsIndex] = avg;
                ++resultsIndex;

                index += averagedSampleCount * data.p_channels;
            }
        }
    }
    
    NxAAudioData *newAudioData = [[NxAAudioData alloc] init];

    newAudioData.p_data = results;
    newAudioData.p_sampleRate = data.p_sampleRate / downSample;
    newAudioData.p_channels = mono ? 1 : data.p_channels;
    newAudioData.p_length = data.p_length;

    return newAudioData;
}

@end
