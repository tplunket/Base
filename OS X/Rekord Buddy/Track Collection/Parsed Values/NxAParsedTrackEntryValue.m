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

#import "Track Collection/Parsed Values/Internal/NxAParsedTrackEntryValue.h"
#import "Track Collection/Tags/Crate Properties/NxATrackProperty.h"
#import "NxATrack.h"
#import "NxAAudioFile.h"

#pragma mark Implementation

@implementation NxAParsedTrackEntryValue

#pragma mark Factory Methods

+ (instancetype)parsedValueWithNoTrackEntry
{
    NxAParsedTrackEntryValue *newValue = [[NxAParsedTrackEntryValue alloc] initPrivate];

    newValue.p_track = nil;

    return newValue;
}

+ (instancetype)parsedValueWithTrack:(NxATrack *)track
{
    NxAAssertNotNil(track);

    NxAParsedTrackEntryValue *newValue = [[NxAParsedTrackEntryValue alloc] initPrivate];

    newValue.p_track = track;

    return newValue;
}

#pragma mark Class Methods

+ (NSString *)p_comparisonValueForTrackWithPath:(NSString *)path
{
    return path ? [NSString stringWithFormat:@"<%@>", path] : @"nil";
}

#pragma mark Overridden NxAParsedValue Accessors

- (NSString *)comparisonValue
{
    return [NxAParsedTrackEntryValue p_comparisonValueForTrackWithPath:self.p_track.fileForCurrentPlatform.path];
}

#pragma mark Instance Methods

- (instancetype)initPrivate
{
    return [super initPrivate];
}

#pragma mark Overridden NxAParsedValue Instance Methods

- (NxATag *)tagForClass:(Class)tagClass usingContext:(NSManagedObjectContext *)context;
{
    NxAAssertTrue(tagClass == [NxATrackProperty class]);
    NxAAssertNotNil(context);

    return [tagClass tagWithValue:self.p_track context:context];
}

#pragma mark Overridden NSObject Instance Methods

- (instancetype)init
{
    ALog(@"Illegal base class call from '%@'.", self.class);
    return nil;
}

@end
