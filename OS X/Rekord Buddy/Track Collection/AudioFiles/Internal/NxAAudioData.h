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

#pragma mark Private Interface

@interface NxAAudioData ()

#pragma mark Properties

/*! Samples stored as floats. */
@property (strong, nonatomic) NSData *p_data;

/*! Length in seconds. */
@property (assign, nonatomic) double p_length;

/*! Sample rate in samples per seconds. */
@property (assign, nonatomic) NSUInteger p_sampleRate;

/*! Number of audio channels per sample. */
@property (assign, nonatomic) NSUInteger p_channels;

@end
