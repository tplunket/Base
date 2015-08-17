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

#import "Track Collection/Tags/Internal/NxATag.h"
#import "Track Collection/Tags/Crate Properties/NxATrackProperty.h"

#pragma mark Private Interface

@interface NxATrackProperty ()

#pragma mark Properties

/*! Track for this property. */
@property (strong, nonatomic) NxATrack *p_value;

@end
