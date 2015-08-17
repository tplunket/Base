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

#import "NxATrackTag+NxACrateListViewItemSource.h"
#import "NxATag+NxACrateListViewItemSource.h"
#import "NxATrackBooleanTag.h"
#import "Track Collection/Tags/Track Properties/Internal/NxATrackTag.h"
#import "NxATagEntry.h"

#pragma mark Implementation

@implementation NxATrackTag (NxACrateListViewItemSource)

#pragma mark NxACrateListViewItemSource Protocol

- (void)attemptToSetItemDescription:(NSString *)description
{
    NxATrackTag *existing = nil;

    if ([self isKindOfClass:[NxATrackBooleanTag class]]) {
        existing = [NxATrackBooleanTag existingTagWithValue:description
                                                    context:self.managedObjectContext];
    }
    else if ([self isKindOfClass:[NxATrackTag class]]) {
        existing = [NxATrackTag existingTagWithName:self.p_tagName
                                              value:description
                                            context:self.managedObjectContext];
    }
    else {
        ALog(@"Invalid track tag.");
    }

    if (existing) {
        // TODO: Need to merge the tag's tracks into the existing one's
        return;
    }

    self.p_value = description;
}

@end
