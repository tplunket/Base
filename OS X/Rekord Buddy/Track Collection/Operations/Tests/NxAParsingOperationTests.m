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

#import "NxATestCase.h"
#import "NxAProgressViewController.h"
#import "NSDate+NxAUtility.h"
#import "NxATrackTests.h"
#import "NxAParsedTrackEntryValueTests.h"
#import "NxAParsedCrateEntryValueTests.h"
#import "Track Collection/Crates/Internal/NxACrate.h"
#import "Track Collection/AudioFiles/OSX/NxAOSXAudioFile.h"
#import "NxAParserImplementation.h"
#import "Track Collection/Parsers/NxARekordboxParser.h"
#import "Track Collection/Operations/Internal/NxAParsingOperation.h"


@interface NxAParsingOperationTests : NxATestCase

@property (strong, nonatomic) NxAParsingOperation *operation;

@property (strong, nonatomic) id<NxAParserImplementation> mockParser;

@end

@implementation NxAParsingOperationTests

#pragma mark Accessors

- (NxAParsingOperation *)operation
{
    if (!_operation) {
        _operation = [NxAParsingOperation operationWithContext:self.context
                                           usingImplementation:self.mockParser
                                           showingProgressWith:mock([NxAProgressViewController class])
                                             loggingMessagesIn:self.mockLog];
        NxAAssertNotNil(_operation);
    }

    return _operation;
}

- (id<NxAParserImplementation>)mockParser
{
    if (!_mockParser) {
        _mockParser = [self mockParserWithClass:[NxARekordboxParser class]];
        NxAAssertNotNil(_mockParser);
    }

    return _mockParser;
}

#pragma mark Test methods

- (void)testp_parseSourcePlaylist_CrateIsEmpty_ReturnsCrateWithNewContent
{
    // -- Given.
    NSString *playlistSource = @"Test Playlist";
    NSString *playlistEntry1Source = @"Test Playlist Entry 1";
    NSString *playlistEntry2Source = @"Test Playlist Entry 2";

    NSDate *earlierModificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-10-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    NxACrate *crate = [self playlistWithName:@"Test Playlist"
              withEmptyContentLastModifiedOn:earlierModificationDate];
    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-11-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    NxATrack *track1 = [NxATrackTests testTrack1InContext:self.context keyName:@"DoesNotMatter" modificationDate:modificationDate];
    NxATrack *track2 = [NxATrackTests testTrack2InContext:self.context keyName:@"DoesNotMatter" modificationDate:modificationDate];

    [given([self.mockParser nameOfPlaylistSource:playlistSource]) willReturn:@"Test Playlist"];
    [given([self.mockParser folderEntrySourceIsAPlaylist:playlistSource]) willReturn:@(YES)];
    [given([self.mockParser childrenOfPlaylistSource:playlistSource]) willReturn:@[ playlistEntry1Source, playlistEntry2Source ]];
    [given([self.mockParser trackForPlaylistEntrySource:playlistEntry1Source]) willReturn:track1];
    [given([self.mockParser trackForPlaylistEntrySource:playlistEntry2Source]) willReturn:track2];
    [given([self.mockParser locationPathForTrackSource:playlistEntry1Source]) willReturn:[[(NxAOSXAudioFile *)track1.fileForCurrentPlatform url] path]];
    [given([self.mockParser locationPathForTrackSource:playlistEntry2Source]) willReturn:[[(NxAOSXAudioFile *)track2.fileForCurrentPlatform url] path]];

    // -- When.
    NxAParsingResultFlags result = [self.operation p_parseSourcePlaylist:playlistSource
                                                                forCrate:crate
            andReplaceContentIfDifferentThanPreviouslyParsedAndOlderThan:modificationDate];

    // -- Then.
    XCTAssertEqual(result, NxAParsedObjectCausedUpdating);
    NSArray *children = crate.tracks;
    XCTAssertEqual(children.count, 2);
    XCTAssertEqual(children[0], track1);
    XCTAssertEqual(children[1], track2);
    XCTAssertEqual(self.operation.p_objectSourceByObjectID.count, 1);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[crate.objectID], playlistSource);
    XCTAssertEqual(self.operation.p_objectsUpdatedStoredInParsingOrder.count, 1);
    XCTAssertTrue([self.operation.p_objectsUpdatedStoredInParsingOrder containsObject:crate]);
    XCTAssertEqual(self.operation.p_objectsToUpdateStoredInParsingOrder.count, 0);

    assertThatNothingWasLogged();
}

- (void)testp_parseSourcePlaylist_CrateHasNewerContent_MakesNoChangesButFlagsTheParsedObjectAsNeedingUpdating
{
    // -- Given.
    NSString *playlistSource = @"Test Playlist";
    NSString *playlistEntry1Source = @"Test Playlist Entry 1";
    NSString *playlistEntry2Source = @"Test Playlist Entry 2";

    NSDate *moreRecentModificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-12-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    NxACrate *crate = [self playlistWithName:@"Test Playlist"
              withEmptyContentLastModifiedOn:moreRecentModificationDate];
    NxATrack *track1 = [NxATrackTests testTrack1InContext:self.context keyName:@"DoesNotMatter" modificationDate:moreRecentModificationDate];
    NxATrack *track2 = [NxATrackTests testTrack2InContext:self.context keyName:@"DoesNotMatter" modificationDate:moreRecentModificationDate];
    [crate addTrack:track2 modificationDate:moreRecentModificationDate];

    [given([self.mockParser nameOfPlaylistSource:playlistSource]) willReturn:@"Test Playlist"];
    [given([self.mockParser folderEntrySourceIsAPlaylist:playlistSource]) willReturn:@(YES)];
    [given([self.mockParser childrenOfPlaylistSource:playlistSource]) willReturn:@[ playlistEntry1Source ]];
    [given([self.mockParser trackForPlaylistEntrySource:playlistEntry1Source]) willReturn:track1];
    [given([self.mockParser trackForPlaylistEntrySource:playlistEntry2Source]) willReturn:track2];
    [given([self.mockParser locationPathForTrackSource:playlistEntry1Source]) willReturn:[[(NxAOSXAudioFile *)track1.fileForCurrentPlatform url] path]];
    [given([self.mockParser locationPathForTrackSource:playlistEntry2Source]) willReturn:[[(NxAOSXAudioFile *)track2.fileForCurrentPlatform url] path]];

    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-11-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];

    // -- When.
    NxAParsingResultFlags result = [self.operation p_parseSourcePlaylist:playlistSource
                                                                forCrate:crate
            andReplaceContentIfDifferentThanPreviouslyParsedAndOlderThan:modificationDate];

    // -- Then.
    XCTAssertEqual(result, NxAParsedObjectNeedsUpdating);
    NSArray *children = crate.tracks;
    XCTAssertEqual(children.count, 1);
    XCTAssertEqual(children[0], track2);
    XCTAssertEqual(self.operation.p_objectSourceByObjectID.count, 1);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[crate.objectID], playlistSource);
    XCTAssertEqual(self.operation.p_objectsUpdatedStoredInParsingOrder.count, 0);
    XCTAssertEqual(self.operation.p_objectsToUpdateStoredInParsingOrder.count, 1);
    XCTAssertTrue([self.operation.p_objectsToUpdateStoredInParsingOrder containsObject:crate]);

    assertThatNothingWasLogged();
}

- (void)testp_parseSourcePlaylist_CrateHasOlderTrackButParsedContentIsSameAsBefore_MakesNoChanges
{
    // -- Given.
    NSString *playlistSource = @"Test Playlist";
    NSString *playlistEntry1Source = @"Test Playlist Entry 1";
    NSString *playlistEntry2Source = @"Test Playlist Entry 2";

    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-11-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    NxACrate *crate = [self playlistWithName:@"Test Playlist"
              withEmptyContentLastModifiedOn:modificationDate];
    NxATrack *track1 = [NxATrackTests testTrack1InContext:self.context keyName:@"DoesNotMatter" modificationDate:modificationDate];
    NxATrack *track2 = [NxATrackTests testTrack2InContext:self.context keyName:@"DoesNotMatter" modificationDate:modificationDate];
    [crate addTrack:track2 modificationDate:modificationDate];
    NSString *versionPrefix = [self.mockParser.parserClass versionPrefix];
    [NxAParsedTrackEntryValueTests saveTrackEntryValuesForTracks:@[ track1 ] inCrate:crate usingVersionPrefix:versionPrefix];

    [given([self.mockParser nameOfPlaylistSource:playlistSource]) willReturn:@"Test Playlist"];
    [given([self.mockParser folderEntrySourceIsAPlaylist:playlistSource]) willReturn:@(YES)];
    [given([self.mockParser childrenOfPlaylistSource:playlistSource]) willReturn:@[ playlistEntry1Source ]];
    [given([self.mockParser trackForPlaylistEntrySource:playlistEntry1Source]) willReturn:track1];
    [given([self.mockParser trackForPlaylistEntrySource:playlistEntry2Source]) willReturn:track2];
    [given([self.mockParser locationPathForTrackSource:playlistEntry1Source]) willReturn:[[(NxAOSXAudioFile *)track1.fileForCurrentPlatform url] path]];
    [given([self.mockParser locationPathForTrackSource:playlistEntry2Source]) willReturn:[[(NxAOSXAudioFile *)track2.fileForCurrentPlatform url] path]];

    NSDate *moreRecentModificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-12-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];

    // -- When.
    NxAParsingResultFlags result = [self.operation p_parseSourcePlaylist:playlistSource
                                                                forCrate:crate
            andReplaceContentIfDifferentThanPreviouslyParsedAndOlderThan:moreRecentModificationDate];

    // -- Then.
    XCTAssertEqual(result, NxAParsedObjectWasUpToDate);
    NSArray *children = crate.tracks;
    XCTAssertEqual(children.count, 1);
    XCTAssertEqual(children[0], track2);
    XCTAssertEqual(self.operation.p_objectSourceByObjectID.count, 1);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[crate.objectID], playlistSource);
    XCTAssertEqual(self.operation.p_objectsUpdatedStoredInParsingOrder.count, 0);
    XCTAssertEqual(self.operation.p_objectsToUpdateStoredInParsingOrder.count, 0);

    assertThatNothingWasLogged();
}

- (void)testp_parseSourcePlaylist_CrateHasSameModificationDate_DoesNotChangeAnything
{
    // -- Given.
    NSString *playlistSource = @"Test Playlist";
    NSString *playlistEntry1Source = @"Test Playlist Entry 1";
    NSString *playlistEntry2Source = @"Test Playlist Entry 2";

    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-11-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    NxACrate *crate = [self playlistWithName:@"Test Playlist"
              withEmptyContentLastModifiedOn:modificationDate];
    NxATrack *track1 = [NxATrackTests testTrack1InContext:self.context keyName:@"DoesNotMatter" modificationDate:modificationDate];
    NxATrack *track2 = [NxATrackTests testTrack2InContext:self.context keyName:@"DoesNotMatter" modificationDate:modificationDate];
    [crate addTrack:track1 modificationDate:modificationDate];
    [crate addTrack:track2 modificationDate:modificationDate];

    [given([self.mockParser nameOfPlaylistSource:playlistSource]) willReturn:@"Test Playlist"];
    [given([self.mockParser folderEntrySourceIsAPlaylist:playlistSource]) willReturn:@(YES)];
    [given([self.mockParser childrenOfPlaylistSource:playlistSource]) willReturn:@[ playlistEntry1Source, playlistEntry2Source ]];
    [given([self.mockParser trackForPlaylistEntrySource:playlistEntry1Source]) willReturn:track1];
    [given([self.mockParser trackForPlaylistEntrySource:playlistEntry2Source]) willReturn:track2];
    [given([self.mockParser locationPathForTrackSource:playlistEntry1Source]) willReturn:[[(NxAOSXAudioFile *)track1.fileForCurrentPlatform url] path]];
    [given([self.mockParser locationPathForTrackSource:playlistEntry2Source]) willReturn:[[(NxAOSXAudioFile *)track2.fileForCurrentPlatform url] path]];

    // -- When.
    NxAParsingResultFlags result = [self.operation p_parseSourcePlaylist:playlistSource
                                                                forCrate:crate
            andReplaceContentIfDifferentThanPreviouslyParsedAndOlderThan:modificationDate];

    // -- Then.
    XCTAssertEqual(result, NxAParsedObjectWasUpToDate);
    NSArray *children = crate.tracks;
    XCTAssertEqual(children.count, 2);
    XCTAssertEqual(children[0], track1);
    XCTAssertEqual(children[1], track2);
    XCTAssertEqual(self.operation.p_objectSourceByObjectID.count, 1);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[crate.objectID], playlistSource);
    XCTAssertEqual(self.operation.p_objectsUpdatedStoredInParsingOrder.count, 0);
    XCTAssertEqual(self.operation.p_objectsToUpdateStoredInParsingOrder.count, 0);

    assertThatNothingWasLogged();
}

- (void)testp_parseSourcePlaylist_CrateHasOlderContent_ReturnsCrateWithNewContent
{
    // -- Given.
    NSString *playlistSource = @"Test Playlist";
    NSString *playlistEntry1Source = @"Test Playlist Entry 1";
    NSString *playlistEntry2Source = @"Test Playlist Entry 2";

    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-11-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    NxACrate *crate = [self playlistWithName:@"Test Playlist"
              withEmptyContentLastModifiedOn:modificationDate];
    NxATrack *track1 = [NxATrackTests testTrack1InContext:self.context keyName:@"DoesNotMatter" modificationDate:modificationDate];
    NxATrack *track2 = [NxATrackTests testTrack2InContext:self.context keyName:@"DoesNotMatter" modificationDate:modificationDate];
    [crate addTrack:track2 modificationDate:modificationDate];

    [given([self.mockParser nameOfPlaylistSource:playlistSource]) willReturn:@"Test Playlist"];
    [given([self.mockParser folderEntrySourceIsAPlaylist:playlistSource]) willReturn:@(YES)];
    [given([self.mockParser childrenOfPlaylistSource:playlistSource]) willReturn:@[ playlistEntry1Source ]];
    [given([self.mockParser trackForPlaylistEntrySource:playlistEntry1Source]) willReturn:track1];
    [given([self.mockParser trackForPlaylistEntrySource:playlistEntry2Source]) willReturn:track2];
    [given([self.mockParser locationPathForTrackSource:playlistEntry1Source]) willReturn:[[(NxAOSXAudioFile *)track1.fileForCurrentPlatform url] path]];
    [given([self.mockParser locationPathForTrackSource:playlistEntry2Source]) willReturn:[[(NxAOSXAudioFile *)track2.fileForCurrentPlatform url] path]];

    NSDate *moreRecentModificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-12-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];

    // -- When.
    NxAParsingResultFlags result = [self.operation p_parseSourcePlaylist:playlistSource
                                                                forCrate:crate
            andReplaceContentIfDifferentThanPreviouslyParsedAndOlderThan:moreRecentModificationDate];

    // -- Then.
    XCTAssertEqual(result, NxAParsedObjectCausedUpdating);
    NSArray *children = crate.tracks;
    XCTAssertEqual(children.count, 1);
    XCTAssertEqual(children[0], track1);
    XCTAssertEqual(self.operation.p_objectSourceByObjectID.count, 1);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[crate.objectID], playlistSource);
    XCTAssertEqual(self.operation.p_objectsUpdatedStoredInParsingOrder.count, 1);
    XCTAssertTrue([self.operation.p_objectsUpdatedStoredInParsingOrder containsObject:crate]);
    XCTAssertEqual(self.operation.p_objectsToUpdateStoredInParsingOrder.count, 0);

    assertThatNothingWasLogged();
}

- (void)testParseSourceFolderEntry_ParseASourceFolderWithAKnownCrate_ReturnsKnownCrate
{
    // -- Given.
    NSString *folderSource = @"SubFolder";
    NSString *subFolderSource = @"SubFolder2";
    NSString *subPlaylistSource = @"SubPlaylist";

    [given([self.mockParser nameOfFolderEntrySource:folderSource]) willReturn:@"SubFolder"];
    [given([self.mockParser folderEntrySourceIsAFolder:folderSource]) willReturn:@(YES)];
    [given([self.mockParser nameOfFolderEntrySource:subFolderSource]) willReturn:@"SubFolder2"];
    [given([self.mockParser folderEntrySourceIsAFolder:subFolderSource]) willReturn:@(YES)];
    [given([self.mockParser nameOfFolderEntrySource:subPlaylistSource]) willReturn:@"SubPlaylist"];
    [given([self.mockParser nameOfPlaylistSource:subPlaylistSource]) willReturn:@"SubPlaylist"];
    [given([self.mockParser folderEntrySourceIsAPlaylist:subPlaylistSource]) willReturn:@(YES)];
    [given([self.mockParser childrenOfFolderSource:folderSource]) willReturn:@[ subFolderSource, subPlaylistSource ]];

    NSDate *earlierModificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-10-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    NxACrate *crate = [self folderWithName:@"TestFolder"
            withEmptyContentLastModifiedOn:earlierModificationDate];
    NxACrate *subFolder = [self folderWithName:@"SubFolder"
                withEmptyContentLastModifiedOn:earlierModificationDate];
    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-11-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    [crate addCrate:subFolder modificationDate:modificationDate];

    NSDate *laterModificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-12-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];

    // -- When.
    [self.operation parseSourceFolderEntry:folderSource
                            forParentCrate:crate
        andReplaceContentIfDifferentThanPreviouslyParsedAndOlderThan:laterModificationDate];

    // -- Then.
    XCTAssertEqual([self.operation objectForSource:folderSource], subFolder);
    NSArray *children = subFolder.crates;
    XCTAssertEqual(children.count, 2);
    NxACrate *subFolder2 = children[0];
    NxACrate *subPlaylist = children[1];
    XCTAssertEqual(self.operation.p_objectSourceByObjectID.count, 3);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[subFolder.objectID], folderSource);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[subFolder2.objectID], subFolderSource);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[subPlaylist.objectID], subPlaylistSource);
    XCTAssertEqual(self.operation.p_objectsUpdatedStoredInParsingOrder.count, 3);
    XCTAssertTrue([self.operation.p_objectsUpdatedStoredInParsingOrder containsObject:subFolder]);
    XCTAssertTrue([self.operation.p_objectsUpdatedStoredInParsingOrder containsObject:subFolder2]);
    XCTAssertTrue([self.operation.p_objectsUpdatedStoredInParsingOrder containsObject:subPlaylist]);
    XCTAssertEqual(self.operation.p_objectsToUpdateStoredInParsingOrder.count, 0);

    assertThatNothingWasLogged();
}

- (void)testParseSourceFolderEntry_ParseASourceFolderWithAnUnknownCrate_ReturnsNewContent
{
    // -- Given.
    NSString *folderSource = @"SubFolder";
    NSString *subFolderSource = @"SubFolder2";
    NSString *subPlaylistSource = @"SubPlaylist";

    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-11-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    [given([self.mockParser nameOfFolderEntrySource:folderSource]) willReturn:@"SubFolder"];
    [given([self.mockParser folderEntrySourceIsAFolder:folderSource]) willReturn:@(YES)];
    [given([self.mockParser nameOfFolderEntrySource:subFolderSource]) willReturn:@"SubFolder2"];
    [given([self.mockParser folderEntrySourceIsAFolder:subFolderSource]) willReturn:@(YES)];
    [given([self.mockParser nameOfFolderEntrySource:subPlaylistSource]) willReturn:@"SubPlaylist"];
    [given([self.mockParser nameOfPlaylistSource:subPlaylistSource]) willReturn:@"SubPlaylist"];
    [given([self.mockParser folderEntrySourceIsAPlaylist:subPlaylistSource]) willReturn:@(YES)];
    [given([self.mockParser childrenOfFolderSource:folderSource]) willReturn:@[ subFolderSource, subPlaylistSource ]];

    NxACrate *crate = [self folderWithName:@"TestFolder"
            withEmptyContentLastModifiedOn:modificationDate];

    // -- When.
    [self.operation parseSourceFolderEntry:folderSource
                            forParentCrate:crate
        andReplaceContentIfDifferentThanPreviouslyParsedAndOlderThan:modificationDate];

    // -- Then.
    NxACrate *subFolder = (NxACrate *)[self.operation objectForSource:folderSource];
    XCTAssertNotNil(subFolder);
    XCTAssertTrue([subFolder isKindOfClass:[NxACrate class]]);
    NSArray *children = subFolder.crates;
    XCTAssertEqual(children.count, 2);
    NxACrate *subFolder2 = children[0];
    NxACrate *subPlaylist = children[1];
    XCTAssertEqual(self.operation.p_objectSourceByObjectID.count, 3);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[subFolder.objectID], folderSource);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[subFolder2.objectID], subFolderSource);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[subPlaylist.objectID], subPlaylistSource);
    XCTAssertEqual(self.operation.p_objectsUpdatedStoredInParsingOrder.count, 3);
    XCTAssertTrue([self.operation.p_objectsUpdatedStoredInParsingOrder containsObject:subFolder]);
    XCTAssertTrue([self.operation.p_objectsUpdatedStoredInParsingOrder containsObject:subFolder2]);
    XCTAssertTrue([self.operation.p_objectsUpdatedStoredInParsingOrder containsObject:subPlaylist]);
    XCTAssertEqual(self.operation.p_objectsToUpdateStoredInParsingOrder.count, 0);

    assertThatNothingWasLogged();
}

- (void)testp_ParseSourceFolder_ParseIntoAnEmptyCrate_ReturnTheCrateWithNewContent
{
    // -- Given.
    NSString *folderSource = @"TestFolder";
    NSString *playlist1Source = @"Playlist1";
    NSString *playlist2Source = @"Playlist2";

    [given([self.mockParser nameOfFolderEntrySource:folderSource]) willReturn:@"TestFolder"];
    [given([self.mockParser nameOfFolderEntrySource:playlist1Source]) willReturn:@"Playlist1"];
    [given([self.mockParser nameOfFolderEntrySource:playlist2Source]) willReturn:@"Playlist2"];
    [given([self.mockParser nameOfPlaylistSource:playlist1Source]) willReturn:@"Playlist1"];
    [given([self.mockParser nameOfPlaylistSource:playlist2Source]) willReturn:@"Playlist2"];
    [given([self.mockParser childrenOfFolderSource:folderSource]) willReturn:@[ playlist1Source, playlist2Source ]];
    [given([self.mockParser folderEntrySourceIsAPlaylist:playlist1Source]) willReturn:@(YES)];
    [given([self.mockParser folderEntrySourceIsAPlaylist:playlist2Source]) willReturn:@(YES)];

    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-11-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];

    NxACrate *crate = [self folderWithName:@"TestFolder"
            withEmptyContentLastModifiedOn:modificationDate];

    NSDate *laterModificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-12-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];

    // -- When.
    NxAParsingResultFlags result = [self.operation p_parseSourceFolder:folderSource
                                                              forCrate:crate
          andReplaceContentIfDifferentThanPreviouslyParsedAndOlderThan:laterModificationDate];

    // -- Then.
    XCTAssertEqual(result, NxAParsedObjectCausedUpdating);
    NSArray *crates = crate.crates;
    XCTAssertEqual(crates.count, 2);
    NxACrate *playlist1 = crates[0];
    XCTAssertEqualObjects(playlist1.p_name, @"Playlist1");
    XCTAssertFalse(playlist1.isAFolder);
    NxACrate *playlist2 = crates[1];
    XCTAssertEqualObjects(playlist2.p_name, @"Playlist2");
    XCTAssertFalse(playlist2.isAFolder);
    XCTAssertEqual(self.operation.p_objectSourceByObjectID.count, 3);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[crate.objectID], folderSource);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[playlist1.objectID], playlist1Source);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[playlist2.objectID], playlist2Source);
    XCTAssertEqual(self.operation.p_objectsUpdatedStoredInParsingOrder.count, 3);
    XCTAssertTrue([self.operation.p_objectsUpdatedStoredInParsingOrder containsObject:crate]);
    XCTAssertTrue([self.operation.p_objectsUpdatedStoredInParsingOrder containsObject:playlist1]);
    XCTAssertTrue([self.operation.p_objectsUpdatedStoredInParsingOrder containsObject:playlist2]);
    XCTAssertEqual(self.operation.p_objectsToUpdateStoredInParsingOrder.count, 0);

    assertThatNothingWasLogged();
}

- (void)testp_ParseSourceFolder_ParseIntoACrateWithSameModificationDate_ReturnUnchangedCrate
{
    // -- Given.
    NSString *folderSource = @"TestFolder";
    NSString *playlist1Source = @"Playlist1";
    NSString *playlist2Source = @"Playlist2";

    [given([self.mockParser nameOfFolderEntrySource:folderSource]) willReturn:@"TestFolder"];
    [given([self.mockParser nameOfFolderEntrySource:playlist1Source]) willReturn:@"Playlist1"];
    [given([self.mockParser nameOfFolderEntrySource:playlist2Source]) willReturn:@"Playlist2"];
    [given([self.mockParser nameOfPlaylistSource:playlist1Source]) willReturn:@"Playlist1"];
    [given([self.mockParser nameOfPlaylistSource:playlist2Source]) willReturn:@"Playlist2"];
    [given([self.mockParser childrenOfFolderSource:folderSource]) willReturn:@[ playlist1Source, playlist2Source ]];
    [given([self.mockParser folderEntrySourceIsAPlaylist:playlist1Source]) willReturn:@(YES)];
    [given([self.mockParser folderEntrySourceIsAPlaylist:playlist2Source]) willReturn:@(YES)];

    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-11-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];

    NxACrate *crate = [self folderWithName:@"TestFolder"
            withEmptyContentLastModifiedOn:modificationDate];
    NxACrate *playlist1 = [self playlistWithName:@"Playlist1"
                  withEmptyContentLastModifiedOn:modificationDate];
    NxACrate *playlist2 = [self playlistWithName:@"Playlist2"
                  withEmptyContentLastModifiedOn:modificationDate];
    [crate addCrate:playlist1 modificationDate:modificationDate];
    [crate addCrate:playlist2 modificationDate:modificationDate];

    // -- When.
    NxAParsingResultFlags result = [self.operation p_parseSourceFolder:folderSource
                                                              forCrate:crate
          andReplaceContentIfDifferentThanPreviouslyParsedAndOlderThan:modificationDate];

    // -- Then.
    XCTAssertEqual(result, NxAParsedObjectWasUpToDate);
    NSArray *crates = crate.crates;
    XCTAssertEqual(crates.count, 2);
    XCTAssertEqual(crates[0], playlist1);
    XCTAssertEqual(crates[1], playlist2);
    XCTAssertEqual(self.operation.p_objectSourceByObjectID.count, 3);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[crate.objectID], folderSource);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[playlist1.objectID], playlist1Source);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[playlist2.objectID], playlist2Source);
    XCTAssertEqual(self.operation.p_objectsUpdatedStoredInParsingOrder.count, 0);
    XCTAssertEqual(self.operation.p_objectsToUpdateStoredInParsingOrder.count, 0);

    assertThatNothingWasLogged();
}

- (void)testp_ParseSourceFolder_ParseIntoACrateWithNewerContent_ReturnUnchangedCrateButFlagsTheParsedObjectAsNeedingUpdating
{
    // -- Given.
    NSString *folderSource = @"TestFolder";
    NSString *playlist1Source = @"Playlist1";
    NSString *playlist2Source = @"Playlist2";

    [given([self.mockParser nameOfFolderEntrySource:folderSource]) willReturn:@"TestFolder"];
    [given([self.mockParser nameOfFolderEntrySource:playlist1Source]) willReturn:@"Playlist1"];
    [given([self.mockParser nameOfFolderEntrySource:playlist2Source]) willReturn:@"Playlist2"];
    [given([self.mockParser nameOfPlaylistSource:playlist1Source]) willReturn:@"Playlist1"];
    [given([self.mockParser nameOfPlaylistSource:playlist2Source]) willReturn:@"Playlist2"];
    [given([self.mockParser childrenOfFolderSource:folderSource]) willReturn:@[ playlist1Source, playlist2Source ]];
    [given([self.mockParser folderEntrySourceIsAPlaylist:playlist1Source]) willReturn:@(YES)];
    [given([self.mockParser folderEntrySourceIsAPlaylist:playlist2Source]) willReturn:@(YES)];

    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-12-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    NxACrate *crate = [self folderWithName:@"TestFolder"
            withEmptyContentLastModifiedOn:modificationDate];
    NxACrate *playlist1 = [self playlistWithName:@"Playlist1"
                  withEmptyContentLastModifiedOn:modificationDate];
    NxACrate *playlist2 = [self playlistWithName:@"Playlist2"
                  withEmptyContentLastModifiedOn:modificationDate];
    [crate addCrate:playlist1 modificationDate:modificationDate];
    [crate addCrate:playlist2 modificationDate:modificationDate];

    NSDate *olderModificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-11-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];

    // -- When.
    NxAParsingResultFlags result = [self.operation p_parseSourceFolder:folderSource
                                                              forCrate:crate
          andReplaceContentIfDifferentThanPreviouslyParsedAndOlderThan:olderModificationDate];

    // -- Then.
    XCTAssertEqual(result, NxAParsedObjectNeedsUpdating);
    NSArray *crates = crate.crates;
    XCTAssertEqual(crates.count, 2);
    XCTAssertEqual(crates[0], playlist1);
    XCTAssertEqual(crates[1], playlist2);
    XCTAssertEqual(self.operation.p_objectSourceByObjectID.count, 3);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[crate.objectID], folderSource);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[playlist1.objectID], playlist1Source);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[playlist2.objectID], playlist2Source);
    XCTAssertEqual(self.operation.p_objectsUpdatedStoredInParsingOrder.count, 0);
    XCTAssertEqual(self.operation.p_objectsToUpdateStoredInParsingOrder.count, 3);
    XCTAssertTrue([self.operation.p_objectsToUpdateStoredInParsingOrder containsObject:crate]);
    XCTAssertTrue([self.operation.p_objectsToUpdateStoredInParsingOrder containsObject:playlist1]);
    XCTAssertTrue([self.operation.p_objectsToUpdateStoredInParsingOrder containsObject:playlist2]);

    assertThatNothingWasLogged();
}

- (void)testp_ParseSourceFolder_ParseIntoACrateWithOlderContentButParsedDataHasNotChangedSinceLastParse_ReturnUnchangedCrate
{
    // -- Given.
    NSString *folderSource = @"TestFolder";
    NSString *playlist1Source = @"Playlist1";
    NSString *playlist2Source = @"Playlist2";

    [given([self.mockParser nameOfFolderEntrySource:folderSource]) willReturn:@"TestFolder"];
    [given([self.mockParser nameOfFolderEntrySource:playlist1Source]) willReturn:@"Playlist1"];
    [given([self.mockParser nameOfFolderEntrySource:playlist2Source]) willReturn:@"Playlist2"];
    [given([self.mockParser nameOfPlaylistSource:playlist1Source]) willReturn:@"Playlist1"];
    [given([self.mockParser nameOfPlaylistSource:playlist2Source]) willReturn:@"Playlist2"];
    [given([self.mockParser childrenOfFolderSource:folderSource]) willReturn:@[ playlist1Source, playlist2Source ]];
    [given([self.mockParser folderEntrySourceIsAPlaylist:playlist1Source]) willReturn:@(YES)];
    [given([self.mockParser folderEntrySourceIsAPlaylist:playlist2Source]) willReturn:@(YES)];

    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-11-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    NxACrate *crate = [self folderWithName:@"TestFolder"
            withEmptyContentLastModifiedOn:modificationDate];
    NxACrate *playlist1 = [self playlistWithName:@"Playlist1"
                  withEmptyContentLastModifiedOn:modificationDate];
    NxACrate *playlist2 = [self playlistWithName:@"Playlist2"
                  withEmptyContentLastModifiedOn:modificationDate];

    NSString *versionPrefix = [self.mockParser.parserClass versionPrefix];
    [NxAParsedCrateEntryValueTests saveCrateEntryValuesForCrates:@[ playlist1, playlist2 ] inCrate:crate usingVersionPrefix:versionPrefix];
    [NxAParsedTrackEntryValueTests saveTrackEntryValuesForTracks:@[ ] inCrate:playlist1 usingVersionPrefix:versionPrefix];
    [NxAParsedTrackEntryValueTests saveTrackEntryValuesForTracks:@[ ] inCrate:playlist2 usingVersionPrefix:versionPrefix];

    NSDate *moreRecentModificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-12-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    [crate addCrate:playlist1 modificationDate:moreRecentModificationDate];
    [crate addCrate:playlist2 modificationDate:moreRecentModificationDate];

    NSDate *evenMoreRecentModificationDate = [NSDate dateWithStringInLocalTimeZone:@"2001-12-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];

    // -- When.
    NxAParsingResultFlags result = [self.operation p_parseSourceFolder:folderSource
                                                              forCrate:crate
          andReplaceContentIfDifferentThanPreviouslyParsedAndOlderThan:evenMoreRecentModificationDate];

    // -- Then.
    XCTAssertEqual(result, NxAParsedObjectWasUpToDate);
    NSArray *crates = crate.crates;
    XCTAssertEqual(crates.count, 2);
    XCTAssertEqual(crates[0], playlist1);
    XCTAssertEqual(crates[1], playlist2);
    XCTAssertEqual(self.operation.p_objectSourceByObjectID.count, 3);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[crate.objectID], folderSource);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[playlist1.objectID], playlist1Source);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[playlist2.objectID], playlist2Source);
    XCTAssertEqual(self.operation.p_objectsUpdatedStoredInParsingOrder.count, 0);
    XCTAssertEqual(self.operation.p_objectsToUpdateStoredInParsingOrder.count, 0);

    assertThatNothingWasLogged();
}

- (void)testp_ParseSourceFolder_CrateWithOlderContentParsedDataHasNotChangedSinceLastParseButSubContentIsNewer_ReturnUnchangedCrateButUpdatesSubContent
{
    // -- Given.
    NSString *folderSource = @"TestFolder";
    NSString *playlist1Source = @"Playlist1";
    NSString *playlist1EntrySource = @"Playlist1Entry";
    NSString *playlist2Source = @"Playlist2";

    NSDate *moreRecentModificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-12-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    NxACrate *crate = [self folderWithName:@"TestFolder"
            withEmptyContentLastModifiedOn:moreRecentModificationDate];
    NxACrate *playlist1 = [self playlistWithName:@"Playlist1"
                  withEmptyContentLastModifiedOn:moreRecentModificationDate];
    NxACrate *playlist2 = [self playlistWithName:@"Playlist2"
                  withEmptyContentLastModifiedOn:moreRecentModificationDate];
    NxATrack *track1 = [NxATrackTests testTrack1InContext:self.context keyName:@"DoesNotMatter" modificationDate:moreRecentModificationDate];
    NxATrack *track2 = [NxATrackTests testTrack2InContext:self.context keyName:@"DoesNotMatter" modificationDate:moreRecentModificationDate];
    [crate addCrate:playlist1 modificationDate:moreRecentModificationDate];
    [crate addCrate:playlist2 modificationDate:moreRecentModificationDate];
    [playlist1 addTrack:track2 modificationDate:moreRecentModificationDate];
    NSString *versionPrefix = [self.mockParser.parserClass versionPrefix];
    [NxAParsedTrackEntryValueTests saveTrackEntryValuesForTracks:@[ ] inCrate:playlist2 usingVersionPrefix:versionPrefix];
    [NxAParsedCrateEntryValueTests saveCrateEntryValuesForCrates:@[ playlist1, playlist2 ] inCrate:crate usingVersionPrefix:versionPrefix];

    [given([self.mockParser nameOfFolderEntrySource:folderSource]) willReturn:@"TestFolder"];
    [given([self.mockParser nameOfFolderEntrySource:playlist1Source]) willReturn:@"Playlist1"];
    [given([self.mockParser nameOfFolderEntrySource:playlist2Source]) willReturn:@"Playlist2"];
    [given([self.mockParser nameOfPlaylistSource:playlist1Source]) willReturn:@"Playlist1"];
    [given([self.mockParser nameOfPlaylistSource:playlist2Source]) willReturn:@"Playlist2"];
    [given([self.mockParser childrenOfFolderSource:folderSource]) willReturn:@[ playlist1Source, playlist2Source ]];
    [given([self.mockParser childrenOfPlaylistSource:playlist1Source]) willReturn:@[ playlist1EntrySource ]];
    [given([self.mockParser trackForPlaylistEntrySource:playlist1EntrySource]) willReturn:track1];
    [given([self.mockParser locationPathForTrackSource:playlist1EntrySource]) willReturn:[[(NxAOSXAudioFile *)track1.fileForCurrentPlatform url] path]];
    [given([self.mockParser folderEntrySourceIsAPlaylist:playlist1Source]) willReturn:@(YES)];
    [given([self.mockParser folderEntrySourceIsAPlaylist:playlist2Source]) willReturn:@(YES)];

    NSDate *evenMoreRecentModificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-12-15 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];

    // -- When.
    NxAParsingResultFlags result = [self.operation p_parseSourceFolder:folderSource
                                                              forCrate:crate
          andReplaceContentIfDifferentThanPreviouslyParsedAndOlderThan:evenMoreRecentModificationDate];

    // -- Then.
    XCTAssertEqual(result, NxAParsedObjectWasUpToDate);
    NSArray *crates = crate.crates;
    XCTAssertEqual(crates.count, 2);
    XCTAssertEqual(crates[0], playlist1);
    XCTAssertEqual(crates[1], playlist2);
    NSArray *tracks = playlist1.tracks;
    XCTAssertEqual(tracks.count, 1);
    XCTAssertEqual(tracks[0], track1);
    XCTAssertEqual(self.operation.p_objectSourceByObjectID.count, 3);
    XCTAssertEqual(self.operation.p_objectSourceByObjectID.count, 3);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[crate.objectID], folderSource);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[playlist1.objectID], playlist1Source);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[playlist2.objectID], playlist2Source);
    XCTAssertEqual(self.operation.p_objectsUpdatedStoredInParsingOrder.count, 1);
    XCTAssertTrue([self.operation.p_objectsUpdatedStoredInParsingOrder containsObject:playlist1]);
    XCTAssertEqual(self.operation.p_objectsToUpdateStoredInParsingOrder.count, 0);

    assertThatNothingWasLogged();
}

- (void)testp_ParseSourceFolder_ParseIntoACrateWithOlderContentButNoPrevisoulyParsedDataAndWithNewParsedData_ReturnTheCrateWithNewContent
{
    // -- Given.
    NSString *folderSource = @"TestFolder";
    NSString *playlist1Source = @"Playlist1";
    NSString *playlist2Source = @"Playlist2";

    [given([self.mockParser nameOfFolderEntrySource:folderSource]) willReturn:@"TestFolder"];
    [given([self.mockParser nameOfFolderEntrySource:playlist1Source]) willReturn:@"Playlist1"];
    [given([self.mockParser nameOfFolderEntrySource:playlist2Source]) willReturn:@"Playlist2"];
    [given([self.mockParser nameOfPlaylistSource:playlist1Source]) willReturn:@"Playlist1"];
    [given([self.mockParser nameOfPlaylistSource:playlist2Source]) willReturn:@"Playlist2"];
    [given([self.mockParser childrenOfFolderSource:folderSource]) willReturn:@[ playlist1Source, playlist2Source ]];
    [given([self.mockParser folderEntrySourceIsAPlaylist:playlist1Source]) willReturn:@(YES)];
    [given([self.mockParser folderEntrySourceIsAPlaylist:playlist2Source]) willReturn:@(YES)];

    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-11-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    NxACrate *crate = [self folderWithName:@"TestFolder"
            withEmptyContentLastModifiedOn:modificationDate];
    NxACrate *playlist1 = [self playlistWithName:@"Playlist1"
                  withEmptyContentLastModifiedOn:modificationDate];
    NxACrate *playlist2 = [self playlistWithName:@"Playlist2"
                  withEmptyContentLastModifiedOn:modificationDate];
    [crate addCrate:playlist2 modificationDate:modificationDate];
    NSString *versionPrefix = [self.mockParser.parserClass versionPrefix];
    [NxAParsedCrateEntryValueTests saveCrateEntryValuesForCrates:@[ playlist1 ]  inCrate:crate usingVersionPrefix:versionPrefix];
    [NxAParsedTrackEntryValueTests saveTrackEntryValuesForTracks:@[ ] inCrate:playlist2 usingVersionPrefix:versionPrefix];

    NSDate *moreRecentModificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-12-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];

    // -- When.
    NxAParsingResultFlags result = [self.operation p_parseSourceFolder:folderSource
                                                              forCrate:crate
          andReplaceContentIfDifferentThanPreviouslyParsedAndOlderThan:moreRecentModificationDate];

    // -- Then.
    XCTAssertEqual(result, NxAParsedObjectCausedUpdating);
    NSArray *crates = crate.crates;
    XCTAssertEqual(crates.count, 2);
    NxACrate *newPlaylist1 = crates[0];
    XCTAssertEqual(crates[1], playlist2);
    XCTAssertEqual(self.operation.p_objectSourceByObjectID.count, 3);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[crate.objectID], folderSource);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[newPlaylist1.objectID], playlist1Source);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[playlist2.objectID], playlist2Source);
    XCTAssertEqual(self.operation.p_objectsUpdatedStoredInParsingOrder.count, 2);
    XCTAssertTrue([self.operation.p_objectsUpdatedStoredInParsingOrder containsObject:crate]);
    XCTAssertTrue([self.operation.p_objectsUpdatedStoredInParsingOrder containsObject:newPlaylist1]);
    XCTAssertEqual(self.operation.p_objectsToUpdateStoredInParsingOrder.count, 0);

    assertThatNothingWasLogged();
}

- (void)testp_ParseSourceFolder_ParseIntoACrateWithOlderContentAndWithNewParsedData_ReturnTheCrateWithNewContent
{
    // -- Given.
    NSString *folderSource = @"TestFolder";
    NSString *playlist1Source = @"Playlist1";
    NSString *playlist2Source = @"Playlist2";

    [given([self.mockParser nameOfFolderEntrySource:folderSource]) willReturn:@"TestFolder"];
    [given([self.mockParser nameOfFolderEntrySource:playlist1Source]) willReturn:@"Playlist1"];
    [given([self.mockParser nameOfFolderEntrySource:playlist2Source]) willReturn:@"Playlist2"];
    [given([self.mockParser nameOfPlaylistSource:playlist1Source]) willReturn:@"Playlist1"];
    [given([self.mockParser nameOfPlaylistSource:playlist2Source]) willReturn:@"Playlist2"];
    [given([self.mockParser childrenOfFolderSource:folderSource]) willReturn:@[ playlist1Source, playlist2Source ]];
    [given([self.mockParser folderEntrySourceIsAPlaylist:playlist1Source]) willReturn:@(YES)];
    [given([self.mockParser folderEntrySourceIsAPlaylist:playlist2Source]) willReturn:@(YES)];

    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-11-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    NxACrate *crate = [self folderWithName:@"TestFolder"
            withEmptyContentLastModifiedOn:modificationDate];
    NxACrate *playlist1 = [self playlistWithName:@"Playlist1"
                  withEmptyContentLastModifiedOn:modificationDate];
    NxACrate *playlist2 = [self playlistWithName:@"Playlist2"
                  withEmptyContentLastModifiedOn:modificationDate];
    [crate addCrate:playlist2 modificationDate:modificationDate];
    NxATrack *track = [NxATrackTests testTrack1InContext:self.context keyName:@"DO NOT MATTER" modificationDate:modificationDate];
    NSString *versionPrefix = [self.mockParser.parserClass versionPrefix];
    [NxAParsedCrateEntryValueTests saveCrateEntryValuesForCrates:@[ playlist1 ] inCrate:crate usingVersionPrefix:versionPrefix];
    [NxAParsedTrackEntryValueTests saveTrackEntryValuesForTracks:@[ track ] inCrate:playlist1 usingVersionPrefix:versionPrefix];
    [NxAParsedTrackEntryValueTests saveTrackEntryValuesForTracks:@[ ] inCrate:playlist2 usingVersionPrefix:versionPrefix];

    NSDate *moreRecentModificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-12-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];

    // -- When.
    NxAParsingResultFlags result = [self.operation p_parseSourceFolder:folderSource
                                                              forCrate:crate
          andReplaceContentIfDifferentThanPreviouslyParsedAndOlderThan:moreRecentModificationDate];

    // -- Then.
    XCTAssertEqual(result, NxAParsedObjectCausedUpdating);
    NSArray *crates = crate.crates;
    XCTAssertEqual(crates.count, 2);
    NxACrate *newPlaylist1 = crates[0];
    XCTAssertEqual(crates[1], playlist2);
    XCTAssertEqual(self.operation.p_objectSourceByObjectID.count, 3);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[crate.objectID], folderSource);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[newPlaylist1.objectID], playlist1Source);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[playlist2.objectID], playlist2Source);
    XCTAssertEqual(self.operation.p_objectsUpdatedStoredInParsingOrder.count, 2);
    XCTAssertTrue([self.operation.p_objectsUpdatedStoredInParsingOrder containsObject:crate]);
    XCTAssertTrue([self.operation.p_objectsUpdatedStoredInParsingOrder containsObject:newPlaylist1]);
    XCTAssertEqual(self.operation.p_objectsToUpdateStoredInParsingOrder.count, 0);

    assertThatNothingWasLogged();
}

- (void)testp_UpdateCollectionSourceFor_UpdateExistingCollectionWithNewTrack_RemovesTheExistingTracksAndAddsTheNewOne
{
    // -- Given.
    NSString *existingTrack1Source = @"Track1";
    NSString *existingTrack2Source = @"Track2";
    NSString *newTrack3Source = @"Track3";
    NSString *collectionSource = @"Collection";

    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2012-06-02 04:54:23" format:@"yyyy-MM-dd HH:mm:ss"];
    NxATrack *existingTrack1 = [NxATrackTests testTrack1InContext:self.context
                                                          keyName:@"DOESNOTMATTER"
                                                 modificationDate:modificationDate];
    NxATrack *existingTrack2 = [NxATrackTests testTrack2InContext:self.context
                                                          keyName:@"DOESNOTMATTER"
                                                 modificationDate:modificationDate];
    NxATrack *newTrack3 = [NxATrackTests testTrack3InContext:self.context
                                                     keyName:@"DOESNOTMATTER"
                                            modificationDate:modificationDate];

    [given([self.mockParser removeAndReturnValidChildrenFromCollectionSource:collectionSource]) willReturn:@[ existingTrack1Source, existingTrack2Source ]];
    [given([self.mockParser numberOfChildrenLeftInCollectionSource:collectionSource]) willReturn:@(4)];
    [given([self.mockParser emptyTrackSourceFor:newTrack3 ]) willReturn:newTrack3Source];
    [given([self.mockParser parseOnlyToForceUpdateSavedValues]) willReturn:@(YES)];

    NxACrate *crate = [self playlistWithName:@"ROOT"
            withEmptyContentLastModifiedOn:modificationDate];
    [crate addTrack:newTrack3 modificationDate:modificationDate];

    self.operation.p_objectSourceByObjectID[crate.objectID] = collectionSource;
    self.operation.p_objectSourceByObjectID[existingTrack1.objectID] = existingTrack1Source;
    self.operation.p_objectSourceByObjectID[existingTrack2.objectID] = existingTrack2Source;

    // -- When.
    [self.operation p_updateCollectionSourceFor:crate];

    // -- Then.
    [verifyCount(self.mockParser, times(1)) updateTrackSource:anything() forTrack:anything()];
    [verifyCount(self.mockParser, times(1)) updateTrackSource:newTrack3Source forTrack:newTrack3];
    [verifyCount(self.mockParser, times(1)) addTrackSource:anything() asChildOf:anything()];
    [verifyCount(self.mockParser, times(1)) addTrackSource:newTrack3Source asChildOf:collectionSource];
    [verifyCount(self.mockParser, times(1)) setEntriesCountForCollectionSource:anything() to:anything()];
    [verifyCount(self.mockParser, times(1)) setEntriesCountForCollectionSource:collectionSource to:@(5)];
    [verifyCount(self.mockParser, times(1)) deleteCollectionEntrySources:anything()];
    MKTArgumentCaptor *argument = [[MKTArgumentCaptor alloc] init];
    [verify(self.mockParser) deleteCollectionEntrySources:[argument capture]];
    NSSet *removedEntries = (NSSet *)argument.value;
    XCTAssertEqual(removedEntries.count, 2);
    XCTAssertTrue([removedEntries containsObject:existingTrack1Source]);
    XCTAssertTrue([removedEntries containsObject:existingTrack2Source]);

    assertThatNothingWasLogged();
}

- (void)testp_UpdateCollectionSourceFor_UpdateExistingCollectionWithExistingAndNewTrack_ReturnCollectionWithBothNewAndExisting
{
    // -- Given.
    NSString *existingTrack1Source = @"Track1";
    NSString *existingTrack2Source = @"Track2";
    NSString *newTrack3Source = @"Track3";
    NSString *collectionSource = @"Collection";

    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-11-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    NxATrack *existingTrack1 = [NxATrackTests testTrack1InContext:self.context
                                                          keyName:@"DOESNOTMATTER"
                                                 modificationDate:modificationDate];
    NxATrack *existingTrack2 = [NxATrackTests testTrack2InContext:self.context
                                                          keyName:@"DOESNOTMATTER"
                                                 modificationDate:modificationDate];
    NxATrack *newTrack3 = [NxATrackTests testTrack3InContext:self.context
                                                     keyName:@"DOESNOTMATTER"
                                            modificationDate:modificationDate];

    NxACrate *allTracksCrate = [self playlistWithName:@"ROOT"
                       withEmptyContentLastModifiedOn:modificationDate];
    [allTracksCrate addTrack:existingTrack1 modificationDate:modificationDate];
    [allTracksCrate addTrack:newTrack3 modificationDate:modificationDate];
    [allTracksCrate addTrack:existingTrack2 modificationDate:modificationDate];

    [given([self.mockParser emptyTrackSourceFor:newTrack3 ]) willReturn:newTrack3Source];
    [given([self.mockParser numberOfChildrenLeftInCollectionSource:collectionSource]) willReturn:@(1)];
    [given([self.mockParser parseOnlyToForceUpdateSavedValues]) willReturn:@(YES)];

    self.operation.p_objectSourceByObjectID[allTracksCrate.objectID] = collectionSource;
    self.operation.p_objectSourceByObjectID[existingTrack1.objectID] = existingTrack1Source;
    self.operation.p_objectSourceByObjectID[existingTrack2.objectID] = existingTrack2Source;

    // -- When.
    [self.operation p_updateCollectionSourceFor:allTracksCrate];

    // -- Then.
    [verifyCount(self.mockParser, times(1)) updateTrackSource:anything() forTrack:anything()];
    [verifyCount(self.mockParser, times(1)) updateTrackSource:newTrack3Source forTrack:newTrack3];
    [verifyCount(self.mockParser, times(3)) addTrackSource:anything() asChildOf:anything()];
    [verifyCount(self.mockParser, times(1)) addTrackSource:existingTrack1Source asChildOf:collectionSource];
    [verifyCount(self.mockParser, times(1)) addTrackSource:existingTrack2Source asChildOf:collectionSource];
    [verifyCount(self.mockParser, times(1)) addTrackSource:newTrack3Source asChildOf:collectionSource];
    [verifyCount(self.mockParser, times(1)) setEntriesCountForCollectionSource:anything() to:anything()];
    [verifyCount(self.mockParser, times(1)) setEntriesCountForCollectionSource:collectionSource to:@(4)];
    [verifyCount(self.mockParser, times(1)) deleteCollectionEntrySources:anything()];
    MKTArgumentCaptor *argument = [[MKTArgumentCaptor alloc] init];
    [verify(self.mockParser) deleteCollectionEntrySources:[argument capture]];
    NSSet *removedEntries = (NSSet *)argument.value;
    XCTAssertEqual(removedEntries.count, 0);

    assertThatNothingWasLogged();
}

- (void)testp_UpdatePlaylistSourceFor_UpdatingAPlaylistWithOneNewTrack_UpdatesThePlaylistAndAddsTheNewTrack
{
    // -- Given.
    NSString *existingTrack1Source = @"Track1";
    NSString *existingTrack2Source = @"Track2";
    NSString *newTrack3Source = @"Track3";
    NSString *playlistSource = @"Playlist";

    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2012-06-02 04:54:23" format:@"yyyy-MM-dd HH:mm:ss"];
    NxATrack *track1 = [NxATrackTests testTrack1InContext:self.context
                                                  keyName:@"DOESNOTMATTER"
                                         modificationDate:modificationDate];
    NxATrack *track2 = [NxATrackTests testTrack2InContext:self.context
                                                  keyName:@"DOESNOTMATTER"
                                         modificationDate:modificationDate];
    NxATrack *newTrack3 = [NxATrackTests testTrack3InContext:self.context
                                                     keyName:@"DOESNOTMATTER"
                                            modificationDate:modificationDate];

    NxACrate *crate = [self playlistWithName:@"Test Playlist"
              withEmptyContentLastModifiedOn:modificationDate];
    [crate addTrack:newTrack3 modificationDate:modificationDate];

    NxACrate *playlist = [self playlistWithName:@"Playlist"
                 withEmptyContentLastModifiedOn:modificationDate];
    [playlist addTrack:newTrack3 modificationDate:modificationDate];

    [given([self.mockParser removeAndReturnValidChildrenFromPlaylistSource:playlistSource]) willReturn:@[ existingTrack1Source, existingTrack2Source ]];
    [given([self.mockParser nameOfPlaylistSource:playlistSource]) willReturn:@"Playlist"];
    [given([self.mockParser playlistEntrySourceFor:newTrack3]) willReturn:newTrack3Source];
    [given([self.mockParser folderEntrySourceIsAPlaylist:playlistSource]) willReturn:@(YES)];
    [given([self.mockParser trackForPlaylistEntrySource:existingTrack1Source]) willReturn:track1];
    [given([self.mockParser trackForPlaylistEntrySource:existingTrack2Source]) willReturn:track2];
    [given([self.mockParser parseOnlyToForceUpdateSavedValues]) willReturn:@(YES)];

    self.operation.p_objectSourceByObjectID[playlist.objectID] = playlistSource;

    // -- When.
    [self.operation p_updatePlaylistSourceFor:playlist];

    // -- Then.
    [verifyCount(self.mockParser, times(1)) addPlaylistEntrySource:anything() asChildOf:anything()];
    [verifyCount(self.mockParser, times(1)) addPlaylistEntrySource:newTrack3Source asChildOf:playlistSource];
    [verifyCount(self.mockParser, times(1)) setEntriesCountForPlaylistSource:anything() to:anything()];
    [verifyCount(self.mockParser, times(1)) setEntriesCountForPlaylistSource:playlistSource to:@(1)];
    [verifyCount(self.mockParser, times(1)) deletePlaylistEntrySources:anything()];
    MKTArgumentCaptor *argument = [[MKTArgumentCaptor alloc] init];
    [verify(self.mockParser) deletePlaylistEntrySources:[argument capture]];
    NSSet *removedEntries = (NSSet *)argument.value;
    XCTAssertEqual(removedEntries.count, 2);
    XCTAssertTrue([removedEntries containsObject:existingTrack1Source]);
    XCTAssertTrue([removedEntries containsObject:existingTrack2Source]);

    assertThatNothingWasLogged();
}

- (void)testp_UpdatePlaylistSourceFor_UpdatingAPlaylistWithOneNewTrackAndOneKnownOne_UpdatesThePlaylistKeepsTheKnownTrackAndAddsTheNewTrack
{
    // -- Given.
    NSString *existingTrack1Source = @"Track1";
    NSString *existingTrack2Source = @"Track2";
    NSString *newTrack3Source = @"Track3";
    NSString *playlistSource = @"Playlist";

    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2012-06-02 04:54:23" format:@"yyyy-MM-dd HH:mm:ss"];
    NxATrack *existingTrack1 = [NxATrackTests testTrack1InContext:self.context
                                                          keyName:@"DOESNOTMATTER"
                                                 modificationDate:modificationDate];
    NxATrack *track2 = [NxATrackTests testTrack2InContext:self.context
                                                  keyName:@"DOESNOTMATTER"
                                         modificationDate:modificationDate];
    NxATrack *newTrack3 = [NxATrackTests testTrack3InContext:self.context
                                                     keyName:@"DOESNOTMATTER"
                                            modificationDate:modificationDate];

    NxACrate *playlist = [self playlistWithName:@"Other Playlist"
                 withEmptyContentLastModifiedOn:modificationDate];
    [playlist addTrack:newTrack3 modificationDate:modificationDate];
    [playlist addTrack:existingTrack1 modificationDate:modificationDate];

    [given([self.mockParser removeAndReturnValidChildrenFromPlaylistSource:playlistSource]) willReturn:@[ existingTrack1Source, existingTrack2Source ]];
    [given([self.mockParser nameOfPlaylistSource:playlistSource]) willReturn:@"Other Playlist"];
    [given([self.mockParser playlistEntrySourceFor:existingTrack1]) willReturn:nil];
    [given([self.mockParser playlistEntrySourceFor:newTrack3]) willReturn:newTrack3Source];
    [given([self.mockParser folderEntrySourceIsAPlaylist:playlistSource]) willReturn:@(YES)];
    [given([self.mockParser trackForPlaylistEntrySource:existingTrack1Source]) willReturn:existingTrack1];
    [given([self.mockParser trackForPlaylistEntrySource:existingTrack2Source]) willReturn:track2];
    [given([self.mockParser parseOnlyToForceUpdateSavedValues]) willReturn:@(YES)];

    self.operation.p_objectSourceByObjectID[playlist.objectID] = playlistSource;

    // -- When.
    [self.operation p_updatePlaylistSourceFor:playlist];

    // -- Then.
    [verifyCount(self.mockParser, times(2)) addPlaylistEntrySource:anything() asChildOf:anything()];
    [verifyCount(self.mockParser, times(1)) addPlaylistEntrySource:existingTrack1Source asChildOf:playlistSource];
    [verifyCount(self.mockParser, times(1)) addPlaylistEntrySource:newTrack3Source asChildOf:playlistSource];
    [verifyCount(self.mockParser, times(1)) setEntriesCountForPlaylistSource:anything() to:anything()];
    [verifyCount(self.mockParser, times(1)) setEntriesCountForPlaylistSource:playlistSource to:@(2)];
    [verifyCount(self.mockParser, times(1)) deletePlaylistEntrySources:anything()];
    MKTArgumentCaptor *argument = [[MKTArgumentCaptor alloc] init];
    [verify(self.mockParser) deletePlaylistEntrySources:[argument capture]];
    NSSet *removedEntries = (NSSet *)argument.value;
    XCTAssertEqual(removedEntries.count, 1);
    XCTAssertTrue([removedEntries containsObject:existingTrack2Source]);

    assertThatNothingWasLogged();
}

- (void)testp_updateFolderSourceFor_UpdatingAFolderWithAnUnknownPlaylist_UpdatesTheFolderAndAddsThePlaylist
{
    // -- Given.
    NSString *playlist1Source = @"Playlist1";
    NSString *folderSource = @"Folder";

    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2012-06-02 04:54:23" format:@"yyyy-MM-dd HH:mm:ss"];
    NxACrate *playlist1 = [self playlistWithName:@"Playlist"
                  withEmptyContentLastModifiedOn:modificationDate];
    NxACrate *folder = [self folderWithName:@"Test Folder"
             withEmptyContentLastModifiedOn:modificationDate];
    [folder addCrate:playlist1 modificationDate:modificationDate];

    [given([self.mockParser removeAndReturnValidChildrenFromFolderSource:folderSource]) willReturn:@[ playlist1Source ]];
    [given([self.mockParser nameOfFolderEntrySource:folderSource]) willReturn:@"Test Folder"];
    [given([self.mockParser folderEntrySourceIsAFolder:folderSource]) willReturn:@(YES)];
    [given([self.mockParser folderEntrySourceIsAPlaylist:playlist1Source]) willReturn:@(YES)];
    [given([self.mockParser parseOnlyToForceUpdateSavedValues]) willReturn:@(YES)];

    self.operation.p_objectSourceByObjectID[folder.objectID] = folderSource;
    self.operation.p_objectSourceByObjectID[playlist1.objectID] = playlist1Source;

    // -- When.
    [self.operation p_updateFolderSourceFor:folder];

    // -- Then.
    XCTAssertEqual(self.operation.p_objectSourceByObjectID.count, 2);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[folder.objectID], folderSource);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[playlist1.objectID], playlist1Source);
    [verifyCount(self.mockParser, times(1)) addFolderEntrySource:anything() asChildOf:anything()];
    [verifyCount(self.mockParser, times(1)) addFolderEntrySource:playlist1Source asChildOf:folderSource];
    [verifyCount(self.mockParser, times(1)) setEntriesCountForFolderSource:anything() to:anything()];
    [verifyCount(self.mockParser, times(1)) setEntriesCountForFolderSource:folderSource to:@(1)];
    [verifyCount(self.mockParser, times(1)) deleteFolderEntrySources:anything()];
    MKTArgumentCaptor *argument = [[MKTArgumentCaptor alloc] init];
    [verify(self.mockParser) deleteFolderEntrySources:[argument capture]];
    NSSet *removedEntries = (NSSet *)argument.value;
    XCTAssertEqual(removedEntries.count, 0);

    assertThatNothingWasLogged();
}

- (void)testp_updateFolderSourceFor_UpdatingAFolderWithAnUnknownPlaylist_UpdatesTheFolderAddsThePlaylistAndRemovesPreviousPlaylist
{
    // -- Given.
    NSString *playlist1Source = @"Playlist1";
    NSString *playlist2Source = @"Playlist2";
    NSString *folderSource = @"Folder";

    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2012-06-02 04:54:23" format:@"yyyy-MM-dd HH:mm:ss"];
    NxACrate *playlist1 = [self playlistWithName:@"Playlist"
                  withEmptyContentLastModifiedOn:modificationDate];
    NxACrate *folder = [self folderWithName:@"Test Folder"
             withEmptyContentLastModifiedOn:modificationDate];
    [folder addCrate:playlist1 modificationDate:modificationDate];

    [given([self.mockParser removeAndReturnValidChildrenFromFolderSource:folderSource]) willReturn:@[ playlist2Source ]];
    [given([self.mockParser nameOfFolderEntrySource:folderSource]) willReturn:@"Test Folder"];
    [given([self.mockParser folderEntrySourceIsAFolder:folderSource]) willReturn:@(YES)];
    [given([self.mockParser folderEntrySourceIsAPlaylist:playlist1Source]) willReturn:@(YES)];
    [given([self.mockParser parseOnlyToForceUpdateSavedValues]) willReturn:@(YES)];

    self.operation.p_objectSourceByObjectID[folder.objectID] = folderSource;
    self.operation.p_objectSourceByObjectID[playlist1.objectID] = playlist1Source;

    // -- When.
    [self.operation p_updateFolderSourceFor:folder];

    // -- Then.
    XCTAssertEqual(self.operation.p_objectSourceByObjectID.count, 2);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[folder.objectID], folderSource);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[playlist1.objectID], playlist1Source);
    [verifyCount(self.mockParser, times(1)) addFolderEntrySource:anything() asChildOf:anything()];
    [verifyCount(self.mockParser, times(1)) addFolderEntrySource:playlist1Source asChildOf:folderSource];
    [verifyCount(self.mockParser, times(1)) setEntriesCountForFolderSource:anything() to:anything()];
    [verifyCount(self.mockParser, times(1)) setEntriesCountForFolderSource:folderSource to:@(1)];
    [verifyCount(self.mockParser, times(1)) deleteFolderEntrySources:anything()];
    MKTArgumentCaptor *argument = [[MKTArgumentCaptor alloc] init];
    [verify(self.mockParser) deleteFolderEntrySources:[argument capture]];
    NSSet *removedEntries = (NSSet *)argument.value;
    XCTAssertEqual(removedEntries.count, 1);
    XCTAssertTrue([removedEntries containsObject:playlist2Source]);

    assertThatNothingWasLogged();
}

- (void)testp_UpdateFolderSourceFor_UpdatingAFolderWithKnownPlaylistAndANewOne_UpdatesTheFolderWithTheCorrectContent
{
    // -- Given.
    NSString *playlist1Source = @"Playlist1";
    NSString *playlist2Source = @"Playlist2";
    NSString *playlist3Source = @"Playlist3";
    NSString *folderSource = @"Folder";

    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2012-06-02 04:54:23" format:@"yyyy-MM-dd HH:mm:ss"];
    NxACrate *playlist1 = [self playlistWithName:@"Playlist One"
                  withEmptyContentLastModifiedOn:modificationDate];
    NxACrate *playlist3 = [self playlistWithName:@"Playlist Three"
                  withEmptyContentLastModifiedOn:modificationDate];
    NxACrate *folder = [self folderWithName:@"Test Folder"
             withEmptyContentLastModifiedOn:modificationDate];
    [folder addCrate:playlist1 modificationDate:modificationDate];
    [folder addCrate:playlist3 modificationDate:modificationDate];

    [given([self.mockParser removeAndReturnValidChildrenFromFolderSource:folderSource]) willReturn:@[ playlist1Source, playlist2Source ]];
    [given([self.mockParser nameOfFolderEntrySource:folderSource]) willReturn:@"Test Folder"];
    [given([self.mockParser folderEntrySourceIsAFolder:folderSource]) willReturn:@(YES)];
    [given([self.mockParser emptyPlaylistSourceFor:playlist3]) willReturn:playlist3Source];
    [given([self.mockParser nameOfPlaylistSource:playlist3Source]) willReturn:@"Playlist Three"];
    [given([self.mockParser folderEntrySourceIsAPlaylist:playlist3Source]) willReturn:@(YES)];
    [given([self.mockParser folderEntrySourceIsAPlaylist:playlist1Source]) willReturn:@(YES)];
    [given([self.mockParser parseOnlyToForceUpdateSavedValues]) willReturn:@(YES)];

    self.operation.p_objectSourceByObjectID[folder.objectID] = folderSource;
    self.operation.p_objectSourceByObjectID[playlist1.objectID] = playlist1Source;

    // -- When.
    [self.operation p_updateFolderSourceFor:folder];

    // -- Then.
    // TODO: We should find a way to check that playlist3 was completely updated.
    XCTAssertEqual(self.operation.p_objectSourceByObjectID.count, 3);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[folder.objectID], folderSource);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[playlist1.objectID], playlist1Source);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[playlist3.objectID], playlist3Source);

    [verifyCount(self.mockParser, times(2)) addFolderEntrySource:anything() asChildOf:anything()];
    [verifyCount(self.mockParser, times(1)) addFolderEntrySource:playlist1Source asChildOf:folderSource];
    [verifyCount(self.mockParser, times(1)) addFolderEntrySource:playlist3Source asChildOf:folderSource];
    [verifyCount(self.mockParser, times(1)) setEntriesCountForFolderSource:anything() to:anything()];
    [verifyCount(self.mockParser, times(1)) setEntriesCountForFolderSource:folderSource to:@(2)];
    [verifyCount(self.mockParser, times(1)) deleteFolderEntrySources:anything()];
    MKTArgumentCaptor *argument = [[MKTArgumentCaptor alloc] init];
    [verify(self.mockParser) deleteFolderEntrySources:[argument capture]];
    NSSet *removedEntries = (NSSet *)argument.value;
    XCTAssertEqual(removedEntries.count, 1);
    XCTAssertTrue([removedEntries containsObject:playlist2Source]);

    assertThatNothingWasLogged();
}

- (void)testp_UpdateFolderSourceFor_UpdatingAFolderWithKnownPlaylistAndANewFolder_UpdatesTheFolderWithTheCorrectContent
{
    // -- Given.
    NSString *playlist1Source = @"Playlist1";
    NSString *playlist2Source = @"Playlist2";
    NSString *folder3Source = @"Folder3";
    NSString *folderSource = @"Folder";

    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2012-06-02 04:54:23" format:@"yyyy-MM-dd HH:mm:ss"];
    NxACrate *playlist1 = [self playlistWithName:@"Playlist One"
                  withEmptyContentLastModifiedOn:modificationDate];
    NxACrate *folder3 = [self folderWithName:@"Folder Three"
                  withEmptyContentLastModifiedOn:modificationDate];
    NxACrate *folder = [self folderWithName:@"Test Folder"
             withEmptyContentLastModifiedOn:modificationDate];
    [folder addCrate:playlist1 modificationDate:modificationDate];
    [folder addCrate:folder3 modificationDate:modificationDate];

    [given([self.mockParser removeAndReturnValidChildrenFromFolderSource:folderSource]) willReturn:@[ playlist1Source, playlist2Source ]];
    [given([self.mockParser nameOfFolderEntrySource:folderSource]) willReturn:@"Test Folder"];
    [given([self.mockParser folderEntrySourceIsAFolder:folderSource]) willReturn:@(YES)];
    [given([self.mockParser emptyFolderSourceFor:folder3]) willReturn:folder3Source];
    [given([self.mockParser nameOfFolderEntrySource:folder3Source]) willReturn:@"Folder Three"];
    [given([self.mockParser folderEntrySourceIsAFolder:folder3Source]) willReturn:@(YES)];
    [given([self.mockParser folderEntrySourceIsAPlaylist:playlist1Source]) willReturn:@(YES)];
    [given([self.mockParser parseOnlyToForceUpdateSavedValues]) willReturn:@(YES)];

    self.operation.p_objectSourceByObjectID[folder.objectID] = folderSource;
    self.operation.p_objectSourceByObjectID[playlist1.objectID] = playlist1Source;

    // -- When.
    [self.operation p_updateFolderSourceFor:folder];

    // -- Then.
    // TODO: We should find a way to check that folder3 was completely updated.
    XCTAssertEqual(self.operation.p_objectSourceByObjectID.count, 3);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[folder.objectID], folderSource);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[playlist1.objectID], playlist1Source);
    XCTAssertEqualObjects(self.operation.p_objectSourceByObjectID[folder3.objectID], folder3Source);

    [verifyCount(self.mockParser, times(2)) addFolderEntrySource:anything() asChildOf:anything()];
    [verifyCount(self.mockParser, times(1)) addFolderEntrySource:playlist1Source asChildOf:folderSource];
    [verifyCount(self.mockParser, times(1)) addFolderEntrySource:folder3Source asChildOf:folderSource];
    [verifyCount(self.mockParser, times(2)) setEntriesCountForFolderSource:anything() to:anything()];
    [verifyCount(self.mockParser, times(1)) setEntriesCountForFolderSource:folderSource to:@(2)];
    [verifyCount(self.mockParser, times(1)) setEntriesCountForFolderSource:folder3Source to:@(0)];
    MKTArgumentCaptor *argument = [[MKTArgumentCaptor alloc] init];
    [verifyCount(self.mockParser, times(2)) deleteFolderEntrySources:[argument capture]];
    NSArray *capturedValues = argument.allValues;
    XCTAssertEqual(capturedValues.count, 2);
    // -- Not quite sure why the first capture is a NSNull object here.
    XCTAssertEqual(capturedValues[0], [NSNull null]);
    XCTAssertTrue([capturedValues[1] isKindOfClass:[NSArray class]]);
    NSArray *secondCapturedValue = (NSArray *)capturedValues[1];
    XCTAssertEqual(secondCapturedValue.count, 1);
    XCTAssertEqual(secondCapturedValue.firstObject, playlist2Source);

    assertThatNothingWasLogged();
}

@end
