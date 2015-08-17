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

#pragma mark Constants

/*!
 * enum NxAWaveformViewMarkerType
 * Type of audio marker supported by the waveform view.
 */
typedef NS_ENUM(NSInteger, NxAWaveformViewMarkerType) {
    NxAWaveformViewGridMarkerType,       /*!< Grid marker. */
    NxAWaveformViewCueMarkerType,        /*!< Cue or hot cue point. */
    NxAWaveformViewLoopMarkerType,       /*!< Loop marker. */
};

#pragma mark Public Interface

/* Audio marker for waveform views. */
@interface NxAWaveformViewMarker : NSObject

#pragma mark Factory Methods

+ (instancetype)cueMarkerAt:(double)startPositionInSeconds
               hotCueNumber:(NSInteger)hotCueNumber;

+ (instancetype)loopMarkerAt:(double)startPositionInSeconds
             lengthInSeconds:(double)lengthInSeconds
                hotCueNumber:(NSInteger)hotCueNumber;

+ (instancetype)gridMarkerAt:(double)startPositionInSeconds
              beatsPerMinute:(double)beatsPerMinute;

#pragma mark Properties

@property (assign, nonatomic) NxAWaveformViewMarkerType type;

@property (assign, nonatomic) double startPositionInSeconds;

@property (assign, nonatomic) double lengthInSeconds;

@property (assign, nonatomic) double beatsPerMinute;

@property (assign, nonatomic) NSInteger hotCueNumber;

@end
