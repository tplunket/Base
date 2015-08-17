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

@class NxAParsedMarkerValue;

#import "Track Collection/Tags/NxATag.h"

#pragma mark Constants

#define NxAMarkerIsLoadMarker          @"Load"
#define NxAMarkerIsFadeInMarker        @"FadeIn"
#define NxAMarkerIsFadeOutMarker       @"FadeOut"
#define NxAMarkerWasStoredAsAHotCue    @"WasStoredAsHotcue"

#pragma mark Public Interface

/*! Marker used to indicate a certain position in a track. */
@interface NxAMarkerProperty : NxATag

@end
