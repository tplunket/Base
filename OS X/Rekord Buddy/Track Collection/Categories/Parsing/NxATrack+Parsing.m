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

#import "NxATrackDateReleased.h"
#import "NxATrack+Parsing.h"
#import "Track Collection/Tags/NxATag.h"
#import "Track Collection/Tags/Internal/NxATaggedObject.h"

#pragma mark Implementation

@implementation NxATrack (Parsing)

#pragma mark Instance Methods

- (NxAParsingResultFlags)ifNotAlreadyPresentAddNilTagsForTagClass:(Class)tagClass
                                                    createdOnDate:(NSDate *)date;
{
    if ([self hasTagsWithType:[tagClass tagType]]) {
        return NxAParsedObjectWasUpToDate;
    }

    [self addNilTagForTagClass:tagClass modifiedOn:date];

    return NxAParsedObjectCausedUpdating;
}

@end
