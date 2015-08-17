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
#import "NxATrack.h"

#pragma mark Private Interface

@interface NxAAudioFile ()

#pragma mark Properties

/*! Identifier for the device this file is from. */
@property (strong, nonatomic) NSString *p_deviceID;

/*! Track who's file this is. */
@property (strong, nonatomic) NxATrack *p_parentTrack;

@end
