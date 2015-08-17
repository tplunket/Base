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

#import "NxAArtwork+NxATrackListViewItem.h"
#import "Track Collection/Internal/NxAArtwork.h"

#pragma mark Implementation

@implementation NxAArtwork (NxATrackListViewItem)

#pragma mark NxATrackListViewItem Protocol

- (BOOL)trackListViewItemIsGroupItem
{
    return NO;
}

- (NSArray *)trackListViewItemChildren
{
    return nil;
}

- (NSImage *)trackListViewItemArtwork
{
    return self.p_image;
}

@end
