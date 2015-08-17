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

#import "NxAParsedTrackEntryValueTests.h"
#import "Track Collection/Parsed Values/Internal/NxAParsedTrackEntryValue.h"

#import "Track Collection/Tags/Crate Properties/NxATrackProperty.h"
#import "NxATrack.h"
#import "Track Collection/Crates/NxACrate.h"
#import "NxAAudioFile.h"

#pragma mark Implementation

@implementation NxAParsedTrackEntryValueTests

#pragma mark Class Methods

+ (NxAParsedTrackEntryValue *)p_mockTrackEntryValueForTrack:(NxATrack *)track
{
    NxAParsedTrackEntryValue *mockValue = mock([NxAParsedTrackEntryValue class]);

    [given([mockValue comparisonValue]) willReturn:[NxAParsedTrackEntryValue p_comparisonValueForTrackWithPath:track.fileForCurrentPlatform.path]];

    return mockValue;
}

+ (void)saveTrackEntryValuesForTracks:(NSArray *)tracks
                              inCrate:(NxACrate *)crate
                   usingVersionPrefix:(NSString *)versionPrefix
{
    NSArray *parsedValues = nil;

    if (tracks.count) {
        NSMutableArray *newValues = [NSMutableArray arrayWithCapacity:tracks.count];
        for (NxATrack *track in tracks) {
            NxAParsedTrackEntryValue *mockValue = [NxAParsedTrackEntryValueTests p_mockTrackEntryValueForTrack:track];
            [newValues addObject:mockValue];
        }

        parsedValues = newValues;
    }
    else {
        parsedValues = @[ [NxAParsedTrackEntryValue parsedValueWithNoTrackEntry] ];
    }

    [crate saveValues:parsedValues forTagClass:[NxATrackProperty class] andVersion:versionPrefix];
}

@end
