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

#import "NxATagEntry+NxATrackListViewItem.h"
#import "NxATrack+NxATrackListViewItem.h"
#import "Track Collection/Tags/Internal/NxATagEntry.h"

#pragma mark Implementation

@implementation NxATagEntry (NxATrackListViewItem)

#pragma mark Accessors

- (id <NxATrackListViewItem>)trackListViewItem
{
    NxATaggedObject *object = self.p_parent;

    if ([object isKindOfClass:[NxATrack class]]) {
        return (NxATrack *)object;
    }
    else {
        ALog(@"Invalid tagged object type.");

        return nil;
    }
}

@end
