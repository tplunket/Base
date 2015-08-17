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

#import "NxAParsedValue.h"

#pragma mark Public Interface

@interface NxAParsedTrackEntryValue : NxAParsedValue

#pragma mark Factory Methods

+ (instancetype)parsedValueWithNoTrackEntry;

+ (instancetype)parsedValueWithTrack:(NxATrack *)track;

@end
