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

#import <AudioToolbox/AudioToolbox.h>

#import "NxAOSXAudioData.h"
#import "Track Collection/AudioFiles/Internal/NxAAudioData.h"

#pragma mark Implementation

@implementation NxAOSXAudioData

#pragma mark Factory Methods

+ (instancetype)dataWithContentsOfURL:(NSURL *)url
                           sampleRate:(NSUInteger)sampleRate
                             channels:(NSUInteger)channels
{
    NxAAssertNotNil(url);
    NxAAssertTrue(sampleRate > 0);
    NxAAssertTrue(channels > 0);

    ExtAudioFileRef audioFile;

    OSStatus err = ExtAudioFileOpenURL((__bridge CFURLRef)url, &audioFile);
	if(err != noErr) {
        NSError *error = [NSError errorWithDomain:NSOSStatusErrorDomain code:err userInfo:nil];
        NSLog(@"Error reading '%@'.\n%@ (%@)",
              url.path,
              error,
              [NxAOSXAudioData OSStatusToStr:err]);

		return nil;
	}

	AudioStreamBasicDescription fileFormat;
    memset(&fileFormat, 0, sizeof(AudioStreamBasicDescription));

    UInt32 structSize = sizeof(fileFormat);
    err = ExtAudioFileGetProperty(audioFile, kExtAudioFileProperty_FileDataFormat, &structSize, &fileFormat);
	if(err != noErr) {
        // -- Cannot get audio file properties.
		return nil;
	}

    SInt64 srcFramesCount = 0;
    structSize = sizeof(SInt64);
    err = ExtAudioFileGetProperty(audioFile, kExtAudioFileProperty_FileLengthFrames, &structSize, &srcFramesCount);
	if(err != noErr) {
        // -- Cannot get frames count.
		return nil;
	}

    AudioStreamBasicDescription clientFormat;
    structSize = sizeof(clientFormat);
    memset(&clientFormat, 0, sizeof(AudioStreamBasicDescription));

    clientFormat.mFormatID = kAudioFormatLinearPCM;
    clientFormat.mFormatFlags = kAudioFormatFlagIsFloat;
    clientFormat.mSampleRate = sampleRate;
    clientFormat.mChannelsPerFrame = (UInt32)channels;
    clientFormat.mBitsPerChannel = sizeof(float) * 8;
    clientFormat.mFramesPerPacket = 1;
    clientFormat.mBytesPerFrame = (UInt32)channels * sizeof(float);
    clientFormat.mBytesPerPacket = (UInt32)channels * sizeof(float);

    err = ExtAudioFileSetProperty(audioFile, kExtAudioFileProperty_ClientDataFormat, structSize, &clientFormat);
	if(err != noErr) {
        // -- Cannot convert audio file to PCM format.
		return nil;
	}

	UInt32 framesToReadCount = (UInt32)srcFramesCount;
    UInt32 totalSamplesCount = framesToReadCount * (UInt32)channels;
    UInt32 segmentSize = totalSamplesCount * sizeof(float);

    NSMutableData *readAudioData = [NSMutableData dataWithLength:segmentSize];
    if (!readAudioData) {
		return nil;
    }

    AudioBufferList bufferList;
    bufferList.mNumberBuffers = 1;
    bufferList.mBuffers[0].mNumberChannels = (UInt32)channels;
    bufferList.mBuffers[0].mData = (float *)readAudioData.mutableBytes;
    bufferList.mBuffers[0].mDataByteSize = (UInt32)segmentSize;

    if (ExtAudioFileRead(audioFile, &framesToReadCount, &bufferList) != noErr) {
        return nil;
    }

	err = ExtAudioFileDispose(audioFile);
	if(err != noErr) {
        // -- Error closing audio file.
		return nil;
	}

    NxAOSXAudioData *newAudioData = [[NxAOSXAudioData alloc] init];

    newAudioData.p_data = readAudioData;
    newAudioData.p_length = (double)readAudioData.length / sizeof(float) / channels / sampleRate;
    newAudioData.p_sampleRate = sampleRate;
    newAudioData.p_channels = channels;
    
    return newAudioData;
}

#pragma mark Class Methods

+ (NSString *)OSStatusToStr:(OSStatus)st
{
    switch (st) {
        case kAudioFileUnspecifiedError:
            return @"kAudioFileUnspecifiedError";

        case kAudioFileUnsupportedFileTypeError:
            return @"kAudioFileUnsupportedFileTypeError";

        case kAudioFileUnsupportedDataFormatError:
            return @"kAudioFileUnsupportedDataFormatError";

        case kAudioFileUnsupportedPropertyError:
            return @"kAudioFileUnsupportedPropertyError";

        case kAudioFileBadPropertySizeError:
            return @"kAudioFileBadPropertySizeError";

        case kAudioFilePermissionsError:
            return @"kAudioFilePermissionsError";

        case kAudioFileNotOptimizedError:
            return @"kAudioFileNotOptimizedError";

        case kAudioFileInvalidChunkError:
            return @"kAudioFileInvalidChunkError";

        case kAudioFileDoesNotAllow64BitDataSizeError:
            return @"kAudioFileDoesNotAllow64BitDataSizeError";

        case kAudioFileInvalidPacketOffsetError:
            return @"kAudioFileInvalidPacketOffsetError";

        case kAudioFileInvalidFileError:
            return @"kAudioFileInvalidFileError";

        case kAudioFileOperationNotSupportedError:
            return @"kAudioFileOperationNotSupportedError";

        case kAudioFileNotOpenError:
            return @"kAudioFileNotOpenError";

        case kAudioFileEndOfFileError:
            return @"kAudioFileEndOfFileError";

        case kAudioFilePositionError:
            return @"kAudioFilePositionError";

        case kAudioFileFileNotFoundError:
            return @"kAudioFileFileNotFoundError";
            
        default:
            return @"Unknown error";
    }
}

@end
