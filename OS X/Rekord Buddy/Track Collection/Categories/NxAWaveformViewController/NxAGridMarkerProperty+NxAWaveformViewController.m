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

#import "NxACueMarkerProperty+NxAWaveformViewController.h"
#import "NxAWaveformViewController.h"
#import "Track Collection/Tags/Marker Properties/Internal/NxAGridMarkerProperty.h"

#pragma mark Implementation

@implementation NxAGridMarkerProperty (NxAWaveformViewController)

- (void)addToWaveformViewController:(NxAWaveformViewController *)controller
{
    [controller addGridMarkerAt:self.p_pos.doubleValue
                 beatsPerMinute:self.p_bpm.doubleValue];
}

@end
