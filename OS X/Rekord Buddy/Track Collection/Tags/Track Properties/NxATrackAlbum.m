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

#import "NxATrack.h"
#import "Track Collection/Tags/Track Properties/Internal/NxATrackAlbum.h"

#pragma mark Implementation

@implementation NxATrackAlbum

#pragma mark Overridden NxATag Class Methods

+ (NSString *)p_managedEntityName
{
    return @"TrackAlbum";
}

+ (BOOL)isASingleValueTagType
{
    // -- Only one album at a time can be added to a tagged object.
    return YES;
}

#pragma mark Accessors

// -- Automatically generated for Core Data properties.
@dynamic p_numberOfTracks;

#pragma mark Overridden NxATag Accessors

- (NSString *)propertyDescription
{
    return @"Album";
}

#pragma mark Instance Methods

- (void)addTrack:(NxATrack *)track
{
    NSMutableOrderedSet *tracks = [self mutableOrderedSetValueForKey:@"p_tracks"];
    [tracks addObject:track];
}

- (void)removeTrack:(NxATrack *)track
{
    NSMutableOrderedSet *tracks = [self mutableOrderedSetValueForKey:@"p_tracks"];
    [tracks removeObject:track];
}

@end
