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

#import "NSDate+NxAUtility.h"
#import "Track Collection/Parsers/NxARekordboxParser.h"
#import "NxATrackTests.h"
#import "NxACrate+Parsing.h"
#import "Track Collection/Tags/Crate Properties/NxATrackProperty.h"
#import "NxAParsedTrackValue.h"
#import "NxAParsedTrackEntryValueTests.h"
#import "Track Collection/Parsed Values/Internal/NxAParsedTrackEntryValue.h"
#import "NxAAudioFile.h"

#pragma mark Private Interface

@interface NxACrateParsingTests : NxATestCase

@property (strong, nonatomic) id<NxAParserImplementation> mockParser;

@end

#pragma mark Implementation

@implementation NxACrateParsingTests

#pragma mark Accessors

- (id<NxAParserImplementation>)mockParser
{
    if (!_mockParser) {
        _mockParser = [self mockParserWithClass:[NxARekordboxParser class]];
        NxAAssertNotNil(_mockParser);
    }

    return _mockParser;
}

#pragma mark Test Methods

- (void)testReplaceTracksIfOlderThan_CrateHasNoTracks_ReturnsCrateWithNewTracks
{
    // -- Given.
    NSDate *olderModificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-10-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-11-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    NxATrack *track1 = [NxATrackTests testTrack1InContext:self.context
                                                  keyName:@"NotImportant"
                                         modificationDate:modificationDate];
    NxATrackProperty *track1Property = [NxATrackProperty tagWithValue:track1 context:self.context];
    NxATrack *track2 = [NxATrackTests testTrack2InContext:self.context
                                                  keyName:@"NotImportant"
                                         modificationDate:modificationDate];
    NxATrackProperty *track2Property = [NxATrackProperty tagWithValue:track2 context:self.context];
    NxACrate *crate = [self playlistWithName:@"TestCrate"
              withEmptyContentLastModifiedOn:olderModificationDate];
    __block BOOL blockWasCalled = NO;

    // -- When.
    NxAParsingResultFlags result = [crate replaceTracksIfOlderThan:modificationDate
                                     andParsedTrackValuesReturnedBy:^() {
                                         blockWasCalled = YES;

                                         NxAParsedTrackValue *mockParsedTrack1Value = mock([NxAParsedTrackValue class]);
                                         [given([mockParsedTrack1Value tagForClass:[NxATrackProperty class] usingContext:self.context])
                                          willReturn:track1Property];
                                         NSString *comparisonValue = [NxAParsedTrackEntryValue p_comparisonValueForTrackWithPath:track1.fileForCurrentPlatform.path];
                                         [given([mockParsedTrack1Value comparisonValue]) willReturn:comparisonValue];

                                         NxAParsedTrackValue *mockParsedTrack2Value = mock([NxAParsedTrackValue class]);
                                         [given([mockParsedTrack2Value tagForClass:[NxATrackProperty class] usingContext:self.context])
                                          willReturn:track2Property];
                                         comparisonValue = [NxAParsedTrackEntryValue p_comparisonValueForTrackWithPath:track2.fileForCurrentPlatform.path];
                                         [given([mockParsedTrack2Value comparisonValue]) willReturn:comparisonValue];

                                         return @[ mockParsedTrack1Value, mockParsedTrack2Value ];
                                     }
                              areDifferentThanPreviouslyParsedWith:self.mockParser
                             thenCallThisEveryTimeATrackIsReplaced:nil];

    // -- Then.
    XCTAssertTrue(blockWasCalled);
    XCTAssertEqual(result, NxAParsedObjectCausedUpdating);
    NSArray *tracks = crate.tracks;
    XCTAssertEqual(tracks.count, 2);
    XCTAssertEqual(tracks[0], track1);
    XCTAssertEqual(tracks[1], track2);
}

- (void)testReplaceTracksIfOlderThan_CrateHasSameModificationDate_MakesNoChanges
{
    // -- Given.
    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-11-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    NxATrack *track1 = [NxATrackTests testTrack1InContext:self.context
                                                  keyName:@"NotImportant"
                                         modificationDate:modificationDate];
    NxATrackProperty *track1Property = [NxATrackProperty tagWithValue:track1 context:self.context];
    NxATrack *track2 = [NxATrackTests testTrack2InContext:self.context
                                                  keyName:@"NotImportant"
                                         modificationDate:modificationDate];
    NxATrackProperty *track2Property = [NxATrackProperty tagWithValue:track2 context:self.context];
    NxACrate *crate = [self playlistWithName:@"TestCrate"
              withEmptyContentLastModifiedOn:modificationDate];
    [crate addTrack:track1 modificationDate:modificationDate];
    [crate addTrack:track2 modificationDate:modificationDate];

    __block BOOL blockWasCalled = NO;

    // -- When.
    NxAParsingResultFlags result = [crate replaceTracksIfOlderThan:modificationDate
                                        andParsedTrackValuesReturnedBy:^() {
                                            blockWasCalled = YES;

                                            NxAParsedTrackValue *mockParsedTrack1Value = mock([NxAParsedTrackValue class]);
                                            [given([mockParsedTrack1Value tagForClass:[NxATrackProperty class] usingContext:self.context])
                                             willReturn:track1Property];
                                            NSString *comparisonValue = [NxAParsedTrackEntryValue p_comparisonValueForTrackWithPath:track1.fileForCurrentPlatform.path];
                                            [given([mockParsedTrack1Value comparisonValue]) willReturn:comparisonValue];

                                            NxAParsedTrackValue *mockParsedTrack2Value = mock([NxAParsedTrackValue class]);
                                            [given([mockParsedTrack2Value tagForClass:[NxATrackProperty class] usingContext:self.context])
                                             willReturn:track2Property];
                                            comparisonValue = [NxAParsedTrackEntryValue p_comparisonValueForTrackWithPath:track2.fileForCurrentPlatform.path];
                                            [given([mockParsedTrack2Value comparisonValue]) willReturn:comparisonValue];

                                            return @[ mockParsedTrack1Value, mockParsedTrack2Value ];
                                          }
                              areDifferentThanPreviouslyParsedWith:self.mockParser
                             thenCallThisEveryTimeATrackIsReplaced:nil];

    // -- Then.
    XCTAssertFalse(blockWasCalled);
    XCTAssertEqual(result, NxAParsedObjectWasUpToDate);
    NSArray *tracks = crate.tracks;
    XCTAssertEqual(tracks.count, 2);
    XCTAssertEqual(tracks[0], track1);
    XCTAssertEqual(tracks[1], track2);
}

- (void)testReplaceTracksIfOlderThan_CrateHasNewerTrack_MakesNoChangesButFlagsTheParsedObjectAsNeedingUpdating
{
    // -- Given.
    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-11-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    NxATrack *track1 = [NxATrackTests testTrack1InContext:self.context
                                                  keyName:@"NotImportant"
                                         modificationDate:modificationDate];
    NxACrate *crate = [self playlistWithName:@"TestCrate"
              withEmptyContentLastModifiedOn:modificationDate];
    [crate addTrack:track1 modificationDate:modificationDate];

    modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-10-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    NxATrack *track2 = [NxATrackTests testTrack2InContext:self.context
                                                  keyName:@"NotImportant"
                                         modificationDate:modificationDate];
    NxATrackProperty *track2Property = [NxATrackProperty tagWithValue:track2 context:self.context];

    __block BOOL blockWasCalled = NO;

    // -- When.
    NxAParsingResultFlags result = [crate replaceTracksIfOlderThan:modificationDate
                                        andParsedTrackValuesReturnedBy:^() {
                                            blockWasCalled = YES;

                                            NxAParsedTrackValue *mockParsedTrack2Value = mock([NxAParsedTrackValue class]);
                                            [given([mockParsedTrack2Value tagForClass:[NxATrackProperty class] usingContext:self.context])
                                             willReturn:track2Property];
                                            NSString *comparisonValue = [NxAParsedTrackEntryValue p_comparisonValueForTrackWithPath:track2.fileForCurrentPlatform.path];
                                            [given([mockParsedTrack2Value comparisonValue]) willReturn:comparisonValue];

                                            return @[ mockParsedTrack2Value ];
                                        }
                              areDifferentThanPreviouslyParsedWith:self.mockParser
                             thenCallThisEveryTimeATrackIsReplaced:nil];

    // -- Then.
    XCTAssertFalse(blockWasCalled);
    XCTAssertEqual(result, NxAParsedObjectNeedsUpdating);
    NSArray *tracks = crate.tracks;
    XCTAssertEqual(tracks.count, 1);
    XCTAssertEqual(tracks[0], track1);
}

- (void)testReplaceTracksIfOlderThan_CrateHasOlderTrackButParsedContentIsSameAsBefore_MakesNoChanges
{
    // -- Given.
    NSDate *moreRecentModificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-12-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    NxATrack *track1 = [NxATrackTests testTrack1InContext:self.context
                                                  keyName:@"NotImportant"
                                         modificationDate:moreRecentModificationDate];
    NxATrack *track2 = [NxATrackTests testTrack2InContext:self.context
                                                  keyName:@"NotImportant"
                                         modificationDate:moreRecentModificationDate];
    NxATrackProperty *track2Property = [NxATrackProperty tagWithValue:track2 context:self.context];
    NxACrate *crate = [self playlistWithName:@"TestCrate"
              withEmptyContentLastModifiedOn:moreRecentModificationDate];
    [crate addTrack:track1 modificationDate:moreRecentModificationDate];
    [crate addTrack:track2 modificationDate:moreRecentModificationDate];

    NSString *versionPrefix = [self.mockParser.parserClass versionPrefix];
    [NxAParsedTrackEntryValueTests saveTrackEntryValuesForTracks:@[ track2 ] inCrate:crate usingVersionPrefix:versionPrefix];

    NSDate *evenMoreRecentModificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-12-20 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];

    __block BOOL blockWasCalled = NO;

    // -- When.
    NxAParsingResultFlags result = [crate replaceTracksIfOlderThan:evenMoreRecentModificationDate
                                        andParsedTrackValuesReturnedBy:^() {
                                            blockWasCalled = YES;

                                            NxAParsedTrackValue *mockParsedTrack2Value = mock([NxAParsedTrackValue class]);
                                            [given([mockParsedTrack2Value tagForClass:[NxATrackProperty class] usingContext:self.context])
                                             willReturn:track2Property];
                                            NSString *comparisonValue = [NxAParsedTrackEntryValue p_comparisonValueForTrackWithPath:track2.fileForCurrentPlatform.path];
                                            [given([mockParsedTrack2Value comparisonValue]) willReturn:comparisonValue];

                                            return @[ mockParsedTrack2Value ];
                                        }
                              areDifferentThanPreviouslyParsedWith:self.mockParser
                             thenCallThisEveryTimeATrackIsReplaced:nil];

    // -- Then.
    XCTAssertTrue(blockWasCalled);
    XCTAssertEqual(result, NxAParsedObjectWasUpToDate);
    NSArray *tracks = crate.tracks;
    XCTAssertEqual(tracks.count, 2);
    XCTAssertEqual(tracks[0], track1);
    XCTAssertEqual(tracks[1], track2);
}

- (void)testReplaceTracksIfOlderThan_CrateHasOlderTrackAndDifferentContentThanParsed_CrateContentIsUpdated
{
    // -- Given.
    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-11-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    NxATrack *track1 = [NxATrackTests testTrack1InContext:self.context
                                                  keyName:@"NotImportant"
                                         modificationDate:modificationDate];
    NxACrate *crate = [self playlistWithName:@"TestCrate"
              withEmptyContentLastModifiedOn:modificationDate];
    [crate addTrack:track1 modificationDate:modificationDate];
    [crate addTrack:track1 modificationDate:modificationDate];
    NSString *versionPrefix = [self.mockParser.parserClass versionPrefix];
    [NxAParsedTrackEntryValueTests saveTrackEntryValuesForTracks:@[ track1 ] inCrate:crate usingVersionPrefix:versionPrefix];

    modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-12-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    NxATrack *track2 = [NxATrackTests testTrack2InContext:self.context
                                                  keyName:@"NotImportant"
                                         modificationDate:modificationDate];
    NxATrackProperty *track2Property = [NxATrackProperty tagWithValue:track2 context:self.context];

    __block BOOL blockWasCalled = NO;

    // -- When.
    NxAParsingResultFlags result = [crate replaceTracksIfOlderThan:modificationDate
                                        andParsedTrackValuesReturnedBy:^() {
                                            blockWasCalled = YES;

                                            NxAParsedTrackValue *mockParsedTrack2Value = mock([NxAParsedTrackValue class]);
                                            [given([mockParsedTrack2Value tagForClass:[NxATrackProperty class] usingContext:self.context])
                                             willReturn:track2Property];
                                            NSString *comparisonValue = [NxAParsedTrackEntryValue p_comparisonValueForTrackWithPath:track2.fileForCurrentPlatform.path];
                                            [given([mockParsedTrack2Value comparisonValue]) willReturn:comparisonValue];

                                            return @[ mockParsedTrack2Value ];
                                        }
                              areDifferentThanPreviouslyParsedWith:self.mockParser
                             thenCallThisEveryTimeATrackIsReplaced:nil];

    // -- Then.
    XCTAssertTrue(blockWasCalled);
    XCTAssertEqual(result, NxAParsedObjectCausedUpdating);
    NSArray *tracks = crate.tracks;
    XCTAssertEqual(tracks.count, 1);
    XCTAssertEqual(tracks[0], track2);
}

@end
