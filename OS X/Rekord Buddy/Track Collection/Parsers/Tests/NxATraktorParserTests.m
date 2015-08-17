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
#import "NxATrackTests.h"
#import "NxATrackTraktorParsingTests.h"
#import "NxATrackDateModified.h"
#import "NSDate+NxAUtility.h"
#import "NSString+NxAUtility.h"
#import "NSXMLElement+NxAUtility.h"
#import "NxATrackTitle.h"
#import "NxATrackArtistCredit.h"
#import "NxATrackProducerCredit.h"
#import "NxATrackRemixerCredit.h"
#import "NxATrackGenre.h"
#import "Track Collection/Tags/Track Properties/NxATrackAlbum.h"
#import "NxATrackRecordLabel.h"
#import "NxATrackComments.h"
#import "NxATrackNumber.h"
#import "NxATrackMixName.h"
#import "NxATrackPlayCount.h"
#import "NxATrackRating.h"
#import "NxATrackDateAdded.h"
#import "Track Collection/Crates/NxACrate.h"
#import "NxATrackKey+TraktorParsing.h"
#import "Track Collection/AudioFiles/OSX/Internal/NxAOSXAudioFile.h"
#import "Track Collection/Parsers/Internal/NxATraktorParser.h"
#import "Utility Classes/Internal/NxAWarning.h"

#pragma mark Private Interface

@interface NxATraktorParserTests : NxATestCase

@property (strong, nonatomic) NxATraktorParser *parser;

@end

#pragma mark Implementation

@implementation NxATraktorParserTests

#pragma mark Accessors

- (NxATraktorParser *)parser
{
    if (!_parser) {
        _parser = [NxATraktorParser implementationWithContext:self.context
                                            loggingMessagesIn:self.mockLog];
        NxAAssertNotNil(_parser);
    }
    return _parser;
}

#pragma mark Test methods

- (void)testRootFolderSource_SampleCollection_ReturnsTheRootFolder
{
    // -- Given.
    NSXMLElement *rootElement = [NxATestCase xmlElementFromString:@"<NML VERSION=\"18\"> \
                                 <HEAD COMPANY=\"www.native-instruments.com\" PROGRAM=\"Traktor\"></HEAD> \
                                 <MUSICFOLDERS></MUSICFOLDERS></NML>"];
    NSXMLElement *collectionElement = [NxATestCase xmlElementFromString:@"<COLLECTION ENTRIES=\"443\"> \
                                       </COLLECTION>"];
    NSXMLElement *entryElement = [NxATestCase xmlElementFromString:@"<ENTRY></ENTRY>"];
    NSXMLElement *playlistsElement = [NxATestCase xmlElementFromString:@"<PLAYLISTS></PLAYLISTS>"];
    NSXMLElement *topPlaylistElement = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"FOLDER\" NAME=\"$ROOT\"></NODE>"];
    [rootElement addChild:collectionElement];
    [rootElement addChild:playlistsElement];
    [collectionElement addChild:entryElement];
    [playlistsElement addChild:topPlaylistElement];
    NSXMLDocument *stubDocument = mock([NSXMLDocument class]);
    stubProperty(stubDocument, rootElement, rootElement);
    self.parser.p_collectionFile = stubDocument;

    // -- When.
    NSXMLElement *result = [self.parser rootFolderSource];

    // -- Then.
    XCTAssertEqualObjects(result, topPlaylistElement);

    assertThatNothingWasLogged();
}

- (void)testCollectionSource_SampleCollection_ReturnsTheCollectionElement
{
    // -- Given.
    NSXMLElement *rootElement = [NxATestCase xmlElementFromString:@"<NML VERSION=\"18\"> \
                                 <HEAD COMPANY=\"www.native-instruments.com\" PROGRAM=\"Traktor\"></HEAD> \
                                 <MUSICFOLDERS></MUSICFOLDERS></NML>"];
    NSXMLElement *collectionElement = [NxATestCase xmlElementFromString:@"<COLLECTION ENTRIES=\"443\"> \
                                       </COLLECTION>"];
    NSXMLElement *entryElement = [NxATestCase xmlElementFromString:@"<ENTRY></ENTRY>"];
    NSXMLElement *playlistsElement = [NxATestCase xmlElementFromString:@"<PLAYLISTS></PLAYLISTS>"];
    NSXMLElement *topPlaylistElement = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"FOLDER\" NAME=\"$ROOT\"></NODE>"];
    [rootElement addChild:collectionElement];
    [rootElement addChild:playlistsElement];
    [collectionElement addChild:entryElement];
    [playlistsElement addChild:topPlaylistElement];
    NSXMLDocument *stubDocument = mock([NSXMLDocument class]);
    stubProperty(stubDocument, rootElement, rootElement);
    self.parser.p_collectionFile = stubDocument;

    // -- When.
    NSXMLElement *result = [self.parser collectionSource];

    // -- Then.
    XCTAssertEqualObjects(result, collectionElement);

    assertThatNothingWasLogged();
}

- (void)testNameOfPlaylistSource_PlaylistWithName_ReturnsName
{
    // -- Given.
    NSXMLElement *source = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"PLAYLIST\" NAME=\"Playlist One\"></NODE>"];

    // -- When.
    NSString *name = [self.parser nameOfPlaylistSource:source];

    // -- Then.
    XCTAssertEqualObjects(name, @"Playlist One");

    assertThatNothingWasLogged();
}

- (void)testNameOfPlaylistSource_PlaylistWithEmptyName_ReturnsEmptyString
{
    // -- Given.
    NSXMLElement *source = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"PLAYLIST\" NAME=\"\"></NODE>"];

    // -- When.
    NSString *name = [self.parser nameOfPlaylistSource:source];

    // -- Then.
    XCTAssertEqualObjects(name, @"");

    assertThatNothingWasLogged();
}

- (void)testNameOfFolderEntrySource_FolderTypeEntry_ReturnsTheName
{
    // -- Given.
    NSXMLElement *source = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"FOLDER\" NAME=\"Folder One\"></NODE>"];

    // -- When.
    NSString *name = [self.parser nameOfFolderEntrySource:source];

    // -- Then.
    XCTAssertEqualObjects(name, @"Folder One");

    assertThatNothingWasLogged();
}

- (void)testNameOfFolderEntrySource_FolderTypeEntryWithEmptyName_ReturnsEmptyString
{
    // -- Given.
    NSXMLElement *source = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"FOLDER\" NAME=\"\"></NODE>"];

    // -- When.
    NSString *name = [self.parser nameOfFolderEntrySource:source];

    // -- Then.
    XCTAssertEqualObjects(name, @"");

    assertThatNothingWasLogged();
}

- (void)testNameOfFolderEntrySource_PlaylistTypeEntry_ReturnsTheName
{
    // -- Given.
    NSXMLElement *source = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"PLAYLIST\" NAME=\"Playlist One\"></NODE>"];

    // -- When.
    NSString *name = [self.parser nameOfFolderEntrySource:source];

    // -- Then.
    XCTAssertEqualObjects(name, @"Playlist One");

    assertThatNothingWasLogged();
}

- (void)testNameOfFolderEntrySource_PlaylistTypeEntryWithEmptyName_ReturnsEmptyString
{
    // -- Given.
    NSXMLElement *source = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"PLAYLIST\" NAME=\"\"></NODE>"];

    // -- When.
    NSString *name = [self.parser nameOfFolderEntrySource:source];

    // -- Then.
    XCTAssertEqualObjects(name, @"");

    assertThatNothingWasLogged();
}

- (void)testFolderEntrySourceIsAPlaylist_FolderTypeEntry_ReturnsFalse
{
    // -- Given.
    NSXMLElement *source = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"FOLDER\" NAME=\"Folder One\"></NODE>"];

    // -- When.
    BOOL result = [self.parser folderEntrySourceIsAPlaylist:source];

    // -- Then.
    XCTAssertFalse(result);

    assertThatNothingWasLogged();
}

- (void)testFolderEntrySourceIsAPlaylist_PlaylistTypeEntry_ReturnsTrue
{
    // -- Given.
    NSXMLElement *source = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"PLAYLIST\" NAME=\"Playlist One\"></NODE>"];

    // -- When.
    BOOL result = [self.parser folderEntrySourceIsAPlaylist:source];

    // -- Then.
    XCTAssertTrue(result);

    assertThatNothingWasLogged();
}

- (void)testLocationPathForTrackSource_FileLocationOnLocalDriveWithBothVolumeAndVolumeID_PathIsCorrectlyReturned
{
    // -- Given.
    NSString *bootVolumeName = [NxAOSXAudioFile bootVolumeName];
    NSXMLElement *trackElement = [NxATestCase xmlElementFromString:[NSString stringWithFormat:@"<ENTRY> \
                                  <LOCATION DIR=\"/:Users/:didier/:Music/:Acapellas/:\" FILE=\"04 Ghetto Kraviz (VOX Stem 1).aiff\" VOLUME=\"%@\" VOLUMEID=\"%@\"> \
                                  </LOCATION> \
                                  </ENTRY>",
                                                                    bootVolumeName, bootVolumeName]];

    // -- When.
    NSString *path = [self.parser locationPathForTrackSource:trackElement];

    // -- Then.
    XCTAssertEqualObjects(path, @"/Users/didier/Music/Acapellas/04 Ghetto Kraviz (VOX Stem 1).aiff");
    XCTAssertEqual(self.parser.p_traktorPlaylistPathByTrackLocation.count, 1);
    NSString *expectedPlaylistPath = [NSString stringWithFormat:@"%@/:Users/:didier/:Music/:Acapellas/:04 Ghetto Kraviz (VOX Stem 1).aiff",
                                      bootVolumeName];
    XCTAssertEqualObjects(self.parser.p_traktorPlaylistPathByTrackLocation[path], expectedPlaylistPath);

    assertThatNothingWasLogged();
}

- (void)testLocationPathForTrackSource_FileLocationOnLocalDriveWithNoVolumeOrVolumeID_PathIsCorrectlyReturned
{
    // -- Given.
    NSXMLElement *trackElement = [NxATestCase xmlElementFromString:@"<ENTRY> \
                                                                    <LOCATION DIR=\"/:Users/:Tsarlsy/:Music/:iTunes/:iTunes Media/:Music/:Katy Perry/:Prism (Deluxe Version)/:\" FILE=\"03 Birthday.mp3\" VOLUME=\"\" VOLUMEID=\"\"></LOCATION> \
                                                                    </ENTRY>"];

    // -- When.
    NSString *path = [self.parser locationPathForTrackSource:trackElement];

    // -- Then.
    XCTAssertEqualObjects(path, @"/Users/Tsarlsy/Music/iTunes/iTunes Media/Music/Katy Perry/Prism (Deluxe Version)/03 Birthday.mp3");
    XCTAssertEqual(self.parser.p_traktorPlaylistPathByTrackLocation.count, 1);
    NSString *expectedPlaylistPath = @"/:Users/:Tsarlsy/:Music/:iTunes/:iTunes Media/:Music/:Katy Perry/:Prism (Deluxe Version)/:03 Birthday.mp3";
    XCTAssertEqualObjects(self.parser.p_traktorPlaylistPathByTrackLocation[path], expectedPlaylistPath);
    
    assertThatNothingWasLogged();
}

- (void)testLocationPathForTrackSource_FileLocationOnLocalDriveWithOnlyVolumeAndNumericVolumeID_PathIsCorrectlyReturned
{
    // -- Given.
    NSString *bootVolumeName = [NxAOSXAudioFile bootVolumeName];
    NSXMLElement *trackElement = [NxATestCase xmlElementFromString:[NSString stringWithFormat:@"<ENTRY> \
                                  <LOCATION DIR=\"/:Users/:Tsarlsy/:Music/:iTunes/:iTunes Media/:Music/:Bob Sinclar Presents Fireball/:Unknown Album/:\" FILE=\"What I Want (Wideboys Club Mix).mp3\" VOLUME=\"%@\" VOLUMEID=\"26ee233d\"></LOCATION> \
                                  </ENTRY>", bootVolumeName]];

    // -- When.
    NSString *path = [self.parser locationPathForTrackSource:trackElement];

    // -- Then.
    XCTAssertEqualObjects(path, @"/Users/Tsarlsy/Music/iTunes/iTunes Media/Music/Bob Sinclar Presents Fireball/Unknown Album/What I Want (Wideboys Club Mix).mp3");
    XCTAssertEqual(self.parser.p_traktorPlaylistPathByTrackLocation.count, 1);
    NSString *expectedPlaylistPath = [NSString stringWithFormat:@"%@/:Users/:Tsarlsy/:Music/:iTunes/:iTunes Media/:Music/:Bob Sinclar Presents Fireball/:Unknown Album/:What I Want (Wideboys Club Mix).mp3", bootVolumeName];
    XCTAssertEqualObjects(self.parser.p_traktorPlaylistPathByTrackLocation[path], expectedPlaylistPath);

    assertThatNothingWasLogged();
}

- (void)testLocationPathForTrackSource_FileLocationOnLocalDriveWithVolumeAndNoVolumeID_PathIsCorrectlyReturned
{
    // -- Given.
    NSString *bootVolumeName = [NxAOSXAudioFile bootVolumeName];
    NSXMLElement *trackElement = [NxATestCase xmlElementFromString:[NSString stringWithFormat:@"<ENTRY> \
                                                                    <LOCATION DIR=\"/:Users/:Marco Petkovski/:Music/:iTunes/:iTunes Media/:Music/:Acre/:Forgotten E.P/:\" FILE=\"01 Hyperreality.m4a\" VOLUME=\"%@\" VOLUMEID=\"\"></LOCATION> \
                                                                    </ENTRY>", bootVolumeName]];

    // -- When.
    NSString *path = [self.parser locationPathForTrackSource:trackElement];

    // -- Then.
    XCTAssertEqualObjects(path, @"/Users/Marco Petkovski/Music/iTunes/iTunes Media/Music/Acre/Forgotten E.P/01 Hyperreality.m4a");
    XCTAssertEqual(self.parser.p_traktorPlaylistPathByTrackLocation.count, 1);
    NSString *expectedPlaylistPath = [NSString stringWithFormat:@"%@/:Users/:Marco Petkovski/:Music/:iTunes/:iTunes Media/:Music/:Acre/:Forgotten E.P/:01 Hyperreality.m4a", bootVolumeName];
    XCTAssertEqualObjects(self.parser.p_traktorPlaylistPathByTrackLocation[path], expectedPlaylistPath);
    
    assertThatNothingWasLogged();
}

- (void)testLocationPathForTrackSource_FileLocationOnExternalDriveWithVolumeAndNoVolumeID_PathIsCorrectlyReturned
{
    // -- Given.
    NSString *externalVolumeName = [[NxAOSXAudioFile otherVolumeNames] anyObject];
    NSXMLElement *trackElement = [NxATestCase xmlElementFromString:[NSString stringWithFormat:@"<ENTRY> \
                                                                    <LOCATION DIR=\"/:DJ Tunes/:July 2014 House/:\" FILE=\"Deekline - Bolivia (Original Mix) [Gutter Gutter].mp3\" VOLUME=\"%@\" VOLUMEID=\"\"></LOCATION> \
                                                                    </ENTRY>", externalVolumeName]];

    // -- When.
    NSString *path = [self.parser locationPathForTrackSource:trackElement];

    // -- Then.
    NSString *expectedResult = [NSString stringWithFormat:@"/Volumes/%@/DJ Tunes/July 2014 House/Deekline - Bolivia (Original Mix) [Gutter Gutter].mp3", externalVolumeName];
    XCTAssertEqualObjects(path, expectedResult);
    XCTAssertEqual(self.parser.p_traktorPlaylistPathByTrackLocation.count, 1);
    NSString *expectedPlaylistPath = [NSString stringWithFormat:@"%@/:DJ Tunes/:July 2014 House/:Deekline - Bolivia (Original Mix) [Gutter Gutter].mp3", externalVolumeName];
    XCTAssertEqualObjects(self.parser.p_traktorPlaylistPathByTrackLocation[path], expectedPlaylistPath);

    assertThatNothingWasLogged();
}

- (void)testLocationPathForTrackSource_FileLocationOnLocalDriveWithIncorrectVolumeAndNoVolumeID_PathIsCorrectlyReturned
{
    // -- Given.
    NSXMLElement *trackElement = [NxATestCase xmlElementFromString:@"<ENTRY> \
                                  <LOCATION DIR=\"/:miledmoussa/:Music/:iTunes/:iTunes Media/:Music/:Tiesto feat Kay/:Avicii - Levels 4/:\" FILE=\"Work Hard, Play Hard (Paris FZ &amp; Simo T Remix).mp3\" VOLUME=\"Users\" VOLUMEID=\"\"></LOCATION> \
                                                                    </ENTRY>"];
    NSFileManager *stubManager = mock([NSFileManager class]);
    [given([stubManager fileExistsAtPath:@"/Users/miledmoussa/Music/iTunes/iTunes Media/Music/Tiesto feat Kay/Avicii - Levels 4/Work Hard, Play Hard (Paris FZ & Simo T Remix).mp3"]) willReturn:@(YES)];
    [NxAOSXAudioFile p_setTestingFileManager:stubManager];

    // -- When.
    NSString *path = [self.parser locationPathForTrackSource:trackElement];

    // -- Then.
    NSString *expectedResult = @"/Users/miledmoussa/Music/iTunes/iTunes Media/Music/Tiesto feat Kay/Avicii - Levels 4/Work Hard, Play Hard (Paris FZ & Simo T Remix).mp3";
    XCTAssertEqualObjects(path, expectedResult);
    XCTAssertEqual(self.parser.p_traktorPlaylistPathByTrackLocation.count, 1);
    NSString *expectedPlaylistPath = @"Users/:miledmoussa/:Music/:iTunes/:iTunes Media/:Music/:Tiesto feat Kay/:Avicii - Levels 4/:Work Hard, Play Hard (Paris FZ & Simo T Remix).mp3";
    XCTAssertEqualObjects(self.parser.p_traktorPlaylistPathByTrackLocation[path], expectedPlaylistPath);

    assertThatNothingWasLogged();
}

- (void)testLocationPathForTrackSource_FileLocationOnLocalDriveWithIncorrectVolumeAndVolumeID_PathIsCorrectlyReturned
{
    // -- Given.
    NSXMLElement *trackElement = [NxATestCase xmlElementFromString:@"<ENTRY> \
                                  <LOCATION DIR=\"/:Music/:Traktor/:Recordings/:\" FILE=\"2011-12-05_1h23m20.wav\" VOLUME=\"Shai\" VOLUMEID=\"Shai\"></LOCATION> \
                                  </ENTRY>"];
    NSFileManager *stubManager = mock([NSFileManager class]);
    [given([stubManager fileExistsAtPath:@"/Users/Shai/Music/Traktor/Recordings/2011-12-05_1h23m20.wav"]) willReturn:@(YES)];
    [NxAOSXAudioFile p_setTestingFileManager:stubManager];

    // -- When.
    NSString *path = [self.parser locationPathForTrackSource:trackElement];

    // -- Then.
    NSString *expectedResult = @"/Users/Shai/Music/Traktor/Recordings/2011-12-05_1h23m20.wav";
    XCTAssertEqualObjects(path, expectedResult);
    XCTAssertEqual(self.parser.p_traktorPlaylistPathByTrackLocation.count, 1);
    NSString *expectedPlaylistPath = @"Shai/:Music/:Traktor/:Recordings/:2011-12-05_1h23m20.wav";
    XCTAssertEqualObjects(self.parser.p_traktorPlaylistPathByTrackLocation[path], expectedPlaylistPath);

    assertThatNothingWasLogged();
}

- (void)testLocationPathForTrackSource_IncorrectFileLocationFileLocationWithIncorrectVolumeAndNoVolumeID_PathIsCorrectlyReturned
{
    // -- Given.
    NSXMLElement *trackElement = [NxATestCase xmlElementFromString:@"<ENTRY> \
                                  <LOCATION DIR=\"/:miledmoussa/:Music/:iTunes/:iTunes Media/:Music/:Tiesto feat Kay/:Avicii - Levels 4/:\" FILE=\"Work Hard, Play Hard (Paris FZ &amp; Simo T Remix).mp3\" VOLUME=\"Users\" VOLUMEID=\"\"></LOCATION> \
                                  </ENTRY>"];

    // -- When.
    NSString *path = [self.parser locationPathForTrackSource:trackElement];

    // -- Then.
    XCTAssertNil(path);

    [verifyCount(self.mockLog, times(1)) addWarning:anything()];
    MKTArgumentCaptor *argument1 = [[MKTArgumentCaptor alloc] init];
    [verify(self.mockLog) addWarning:[argument1 capture]];
    NSArray *values1 = argument1.allValues;
    XCTAssertEqual(values1.count, 1);
    XCTAssertTrue([[values1.firstObject p_description] hasPrefix:@"Found a track entry in your Traktor collection with a missing file"]);
}

- (void)testLocationPathForTrackSource_IncorrectFileLocationWithIncorrectVolumeAndVolumeID_ReturnsNil
{
    // -- Given.
    NSXMLElement *trackElement = [NxATestCase xmlElementFromString:@"<ENTRY> \
                                  <LOCATION DIR=\"/:Music/:Traktor/:Recordings/:\" FILE=\"2011-12-05_1h23m20.wav\" VOLUME=\"Shai\" VOLUMEID=\"Shai\"></LOCATION> \
                                  </ENTRY>"];

    // -- When.
    NSString *path = [self.parser locationPathForTrackSource:trackElement];

    // -- Then.
    XCTAssertNil(path);

    [verifyCount(self.mockLog, times(1)) addWarning:anything()];
    MKTArgumentCaptor *argument1 = [[MKTArgumentCaptor alloc] init];
    [verify(self.mockLog) addWarning:[argument1 capture]];
    NSArray *values1 = argument1.allValues;
    XCTAssertEqual(values1.count, 1);
    XCTAssertTrue([[values1.firstObject p_description] hasPrefix:@"Found a track entry in your Traktor collection with a missing file"]);
}

- (void)testLocationPathForTrackSource_FileLocationOnExternalDriveWithBothVolumeAndVolumeID_PathIsCorrectlyReturned
{
    // -- Given.
    NSString *externalVolumeName = [[NxAOSXAudioFile otherVolumeNames] anyObject];
    NSXMLElement *trackElement = [NxATestCase xmlElementFromString:[NSString stringWithFormat:@"<ENTRY> \
                                  <LOCATION DIR=\"/:DJ Tracks/:2010 Progressive House/:Nov 2010/:\" FILE=\"Morgan Page - Fight For You (DJ Dan Mix)www.livingelectro.com.mp3\" VOLUME=\"%@\" VOLUMEID=\"%@\"></LOCATION> \
                                                                    </ENTRY>", externalVolumeName, externalVolumeName]];

    // -- When.
    NSString *path = [self.parser locationPathForTrackSource:trackElement];

    // -- Then.
    NSString *expectedResult = [NSString stringWithFormat:@"/Volumes/%@/DJ Tracks/2010 Progressive House/Nov 2010/Morgan Page - Fight For You (DJ Dan Mix)www.livingelectro.com.mp3", externalVolumeName];
    XCTAssertEqualObjects(path, expectedResult);
    XCTAssertEqual(self.parser.p_traktorPlaylistPathByTrackLocation.count, 1);
    NSString *expectedPlaylistPath = [NSString stringWithFormat:@"%@/:DJ Tracks/:2010 Progressive House/:Nov 2010/:Morgan Page - Fight For You (DJ Dan Mix)www.livingelectro.com.mp3", externalVolumeName];
    XCTAssertEqualObjects(self.parser.p_traktorPlaylistPathByTrackLocation[path], expectedPlaylistPath);

    assertThatNothingWasLogged();
}

- (void)testModificationDateForTrackSource_ParseATrackElement_ReturnsTheCorrectDate
{
    // -- Given.
    NSXMLElement *element = [NxATrackTraktorParsingTests traktorTestSourceTrack1];
    [[element attributeForName:@"MODIFIED_DATE"] setStringValue:@"2014/7/23"];
    [[element attributeForName:@"MODIFIED_TIME"] setStringValue:@"32400"];

    // -- When.
    NSDate *result = [self.parser modificationDateForTrackSource:element];

    // -- Then.
    XCTAssertEqualObjects([result stringValueInLocalTimeZoneWithFormat:@"yyyy-MM-dd HH:mm:ss"], @"2014-07-23 03:00:00");

    assertThatNothingWasLogged();
}

- (void)testParseTrackSource_ParseTrackWithMissingProperties_ReturnsTrackWithContent
{
    // -- Given.
    NSXMLElement *element = [NxATrackTraktorParsingTests traktorTestSourceTrack1];
    NxATrack *track = [NxATrackTests stubbedTrackWithPath:@"/Test/Path/File.MP3"
                                                  context:self.context];
    NSDate *sourceModificationDate = [NSDate dateWithStringInLocalTimeZone:@"2014-07-23 07:00:00" format:@"yyyy-MM-dd HH:mm:ss"];


    // -- When.
    NxAParsingResultFlags result = [self.parser parseTrackSource:element
                                              whichWasModifiedOn:sourceModificationDate
                                                        forTrack:track];

    // -- Then.
    XCTAssertEqual(result, NxAParsedObjectCausedUpdating);

    assertThatNothingWasLogged();
}

- (void)testParseTrackSource_ParseMoreRecentTrack_ReturnsMoreRecentContent
{
    // -- Given.
    NSXMLElement *element = [NxATrackTraktorParsingTests traktorTestSourceTrack1];
    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2014-07-23 07:00:00" format:@"yyyy-MM-dd HH:mm:ss"];
    NxATrack *track = [NxATrackTests testTrack1InContext:self.context
                                                 keyName:NxAKeyPropertyTraktorKeyName
                                        modificationDate:modificationDate];
    NSDate *sourceModificationDate = [NSDate dateWithStringInLocalTimeZone:@"2014-08-23 07:00:00" format:@"yyyy-MM-dd HH:mm:ss"];;

    // -- When.
    NxAParsingResultFlags result = [self.parser parseTrackSource:element
                                              whichWasModifiedOn:sourceModificationDate
                                                        forTrack:track];

    // -- Then.
    XCTAssertEqual(result, NxAParsedObjectCausedUpdating);

    assertThatNothingWasLogged();
}

- (void)testParseTrackSource_ParseUnchangedTrack_ReturnsUnchangedContent
{
    // -- Given.
    NSXMLElement *element = [NxATrackTraktorParsingTests traktorTestSourceTrack1];
    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2014-07-23 03:00:00" format:@"yyyy-MM-dd HH:mm:ss"];
    NxATrack *track = [NxATrackTests testTrack1InContext:self.context
                                                 keyName:NxAKeyPropertyTraktorKeyName
                                        modificationDate:modificationDate];

    // -- When.
    NxAParsingResultFlags result = [self.parser parseTrackSource:element
                                              whichWasModifiedOn:modificationDate
                                                        forTrack:track];

    // -- Then.
    XCTAssertEqual(result, NxAParsedObjectWasUpToDate);

    assertThatNothingWasLogged();
}

- (void)testParseTrackSource_ParseOlderTrack_ReturnsUnchangedContentAndTrackNeedsUpdating
{
    // -- Given.
    NSXMLElement *element = [NxATrackTraktorParsingTests traktorTestSourceTrack1];
    NSDate *laterDate = [NSDate dateWithStringInLocalTimeZone:@"2014-08-23 07:00:00" format:@"yyyy-MM-dd HH:mm:ss"];
    NxATrack *track = [NxATrackTests testTrack1InContext:self.context
                                                 keyName:NxAKeyPropertyTraktorKeyName
                                        modificationDate:laterDate];
    NSDate *sourceModificationDate = [NSDate dateWithStringInLocalTimeZone:@"2014-07-23 07:00:00" format:@"yyyy-MM-dd HH:mm:ss"];

    // -- When.
    NxAParsingResultFlags result = [self.parser parseTrackSource:element
                                              whichWasModifiedOn:sourceModificationDate
                                                        forTrack:track];

    // -- Then.
    XCTAssertEqual(result, NxAParsedObjectNeedsUpdating);

    assertThatNothingWasLogged();
}

- (void)testParseTrackSource_ParsesATrack_ParsesAllFieldsCorrectly
{
    // -- Given.
    NSXMLElement *element = [NxATrackTraktorParsingTests traktorTestSourceTrack2];
    NxATrack *track = [NxATrackTests stubbedTrackWithPath:@"/Test/Path/File.MP3"
                                                  context:self.context];
    NSDate *sourceModificationDate = [NSDate dateWithStringInLocalTimeZone:@"2014-07-23 07:00:00" format:@"yyyy-MM-dd HH:mm:ss"];

    // -- When.
    NxAParsingResultFlags result = [self.parser parseTrackSource:element
                                              whichWasModifiedOn:sourceModificationDate
                                                        forTrack:track];

    // -- Then.
    XCTAssertEqual(result, NxAParsedObjectCausedUpdating);
    XCTAssertEqualObjects([[[NxATrackTitle tagsIn:track] firstObject] stringValue], @"Need U (100%) (Original Mix)");
    XCTAssertEqualObjects([NSString stringSeparatedBy:@", " withStringValuesFromArray:[NxATrackArtistCredit tagsIn:track]], @"Duke Dumont, A*M*E");
    XCTAssertEqualObjects([NSString stringSeparatedBy:@", " withStringValuesFromArray:[NxATrackProducerCredit tagsIn:track]], @"Steve Angello, Roger Sanchez");
    XCTAssertEqualObjects([NSString stringSeparatedBy:@", " withStringValuesFromArray:[NxATrackRemixerCredit tagsIn:track]], @"Arty, Bingo Players");
    XCTAssertEqualObjects([NSString stringSeparatedBy:@", " withStringValuesFromArray:[NxATrackGenre tagsIn:track]], @"Deep House, Tech House");
    XCTAssertEqualObjects([[[NxATrackAlbum tagsIn:track] firstObject] stringValue], @"Need U (100%) [feat. A*M*E]");
    XCTAssertEqualObjects([[[NxATrackRecordLabel tagsIn:track] firstObject] stringValue], @"Win Records/Downtown Records");
    XCTAssertEqualObjects([[[NxATrackComments tagsIn:track] firstObject] stringValue], @"Some even better comments.");
    XCTAssertEqualObjects([[[NxATrackNumber tagsIn:track] firstObject] stringValue], @"2");
    XCTAssertEqualObjects([[[NxATrackMixName tagsIn:track] firstObject] stringValue], @"Club Mix");
    XCTAssertEqualObjects([[[NxATrackPlayCount tagsIn:track] firstObject] stringValue], @"4243");
    XCTAssertEqualObjects([[[NxATrackDateAdded tagsIn:track] firstObject] stringValueInLocalTimeZoneWithFormat:@"yyyy-MM-dd HH:mm:ss"], @"2014-09-10 00:00:00");
    XCTAssertEqualObjects([[[NxATrackRating tagsIn:track] firstObject] stringValue], @"5");
    NSArray *keys = [NxATrackKey tagsIn:track withName:NxAKeyPropertyTraktorKeyName];
    XCTAssertEqual(keys.count, 1);
    XCTAssertEqualObjects([keys[0] stringValueForNotation:NxAMusicalNotationStandard], @"Cm");

    assertThatNothingWasLogged();
}

- (void)testTrackForPlaylistEntrySource_ParseKnownPathEntry_ReturnsCorrectTrack
{
    // -- Given.
    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-11-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    NxATrack *existingTrack1 = [NxATrackTests testTrack1InContext:self.context
                                                          keyName:NxAKeyPropertyTraktorKeyName
                                                 modificationDate:modificationDate];
    NSXMLElement *playlistEntry = [NxATestCase xmlElementFromString:@"<ENTRY><PRIMARYKEY TYPE=\"TRACK\" KEY=\"Macintosh HD/:Users/:didier/:Music/:Acapellas/:Con Alegria (Acapella).aif\"></PRIMARYKEY></ENTRY>"];
    self.parser.p_trackByTraktorPlaylistPath[@"Macintosh HD/:Users/:didier/:Music/:Acapellas/:Con Alegria (Acapella).aif"] = existingTrack1;

    // -- When.
    NxATrack *track = [self.parser trackForPlaylistEntrySource:playlistEntry];

     // -- Then.
     XCTAssertEqualObjects(track, existingTrack1);

     assertThatNothingWasLogged();
}

- (void)testTrackForPlaylistEntrySource_ParseUnknownPathEntry_ReturnsNil
{
    // -- Given.
    NSXMLElement *playlistEntry = [NxATestCase xmlElementFromString:@"<ENTRY><PRIMARYKEY TYPE=\"TRACK\" KEY=\"Macintosh HD/:Users/:didier/:Music/:Acapellas/:Con Alegria (Acapella).aif\"></PRIMARYKEY></ENTRY>"];

    // -- When.
    NxATrack *track = [self.parser trackForPlaylistEntrySource:playlistEntry];

    // -- Then.
    XCTAssertNil(track);

    assertThatNothingWasLogged();
}

- (void)testChildrenOfCollectionSource_CollectionWithTracksAndSamples_ReturnsTheTracks
{
    // -- Given.
    NSXMLElement *collectionElement = [NxATestCase xmlElementFromString:@"<COLLECTION ENTRIES=\"437\"> \
                                       </COLLECTION>"];
    NSXMLElement *existingTrack1 = [NxATrackTraktorParsingTests traktorTestSourceTrack1];
    NSXMLElement *existingTrack2 = [NxATrackTraktorParsingTests traktorTestSourceTrack2];
    NSXMLElement *loopInfoElement = [NxATestCase xmlElementFromString:@"<LOOPINFO SAMPLE_TYPE_INFO=\"0\"></LOOPINFO>"];
    [existingTrack2 addChild:loopInfoElement];
    NSXMLElement *existingTrack3 = [NxATrackTraktorParsingTests traktorTestSourceTrack3];
    [collectionElement addChild:existingTrack1];
    [collectionElement addChild:existingTrack2];
    [collectionElement addChild:existingTrack3];

    // -- When.
    NSArray *children = [self.parser childrenOfCollectionSource:collectionElement];

    // -- Then.
    XCTAssertEqual(children.count, 2);
    XCTAssertEqualObjects(children[0], existingTrack1);
    XCTAssertEqualObjects(children[1], existingTrack3);

    assertThatNothingWasLogged();
}

- (void)testChildrenOfPlaylistSource_PlaylistWithChildren_ReturnsTheChildren
{
    // -- Given.
    NSXMLElement *playlistChild1 = [NxATestCase xmlElementFromString:@"<ENTRY><PRIMARYKEY TYPE=\"TRACK\" KEY=\"Macintosh HD/:Users/:didier/:Music/:Gigs/:Angel Johnson, Tammy Cartwright - Lovers Heaven (Jacob Bech Remix).aif\"></PRIMARYKEY></ENTRY>"];
    NSXMLElement *playlistChild2 = [NxATestCase xmlElementFromString:@"<ENTRY><PRIMARYKEY TYPE=\"TRACK\" KEY=\"Macintosh HD/:Users/:didier/:Music/:Gigs/:Callin' feat. S-Man (Original Mix) - Huxley, S-Man.aiff\"></PRIMARYKEY></ENTRY>"];
    NSXMLElement *nodeElement = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"PLAYLIST\" NAME=\"DeepDeepDown 7/24/14\"></NODE>"];
    NSXMLElement *playlistElement = [NxATestCase xmlElementFromString:@"<PLAYLIST ENTRIES=\"34\" TYPE=\"LIST\" UUID=\"9bf4382a1acf49f7b095ecefa52f7da0\"></PLAYLIST>"];
    [nodeElement addChild:playlistElement];
    [playlistElement addChild:playlistChild1];
    [playlistElement addChild:playlistChild2];

    // -- When.
    NSArray *children = [self.parser childrenOfPlaylistSource:nodeElement];

    // -- Then.
    XCTAssertEqual(children.count, 2);
    XCTAssertEqualObjects(children[0], playlistChild1);
    XCTAssertEqualObjects(children[1], playlistChild2);

    assertThatNothingWasLogged();
}

- (void)testChildrenOfFolderSource_FolderWithChildren_ReturnsTheChildren
{
    // -- Given.
    NSXMLElement *playlist1Element = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"PLAYLIST\" NAME=\"DeepDeepDown 7/24/14\"></NODE>"];
    NSXMLElement *playlist2Element = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"PLAYLIST\" NAME=\"_Internal\"></NODE>"];
    NSXMLElement *playlist3Element = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"PLAYLIST\" NAME=\"Other\"></NODE>"];
    NSXMLElement *folderElement = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"FOLDER\" NAME=\"Gigs\"></NODE>"];
    NSXMLElement *subNodesElement = [NxATestCase xmlElementFromString:@"<SUBNODES COUNT=\"2\"></SUBNODES>"];
    [folderElement addChild:subNodesElement];
    [subNodesElement addChild:playlist1Element];
    [subNodesElement addChild:playlist2Element];
    [subNodesElement addChild:playlist3Element];

    // -- When.
    NSArray *children = [self.parser childrenOfFolderSource:folderElement];

    // -- Then.
    XCTAssertEqual(children.count, 2);
    XCTAssertEqualObjects(children[0], playlist1Element);
    XCTAssertEqualObjects(children[1], playlist3Element);

    assertThatNothingWasLogged();
}

- (void)testRemoveAndReturnValidChildrenFromCollectionSource_CollectionElementWithChildrenOneNotATrack_ReturnsTheTrackChildrenAndDetachesThemFromTheParent
{
    // -- Given.
    NSXMLElement *collectionElement = [NxATestCase xmlElementFromString:@"<COLLECTION ENTRIES=\"437\"> \
                                       </COLLECTION>"];
    NSXMLElement *existingTrack1 = [NxATrackTraktorParsingTests traktorTestSourceTrack1];
    NSXMLElement *existingTrack2 = [NxATrackTraktorParsingTests traktorTestSourceTrack2];
    NSXMLElement *loopInfoElement = [NxATestCase xmlElementFromString:@"<LOOPINFO SAMPLE_TYPE_INFO=\"0\"></LOOPINFO>"];
    [existingTrack2 addChild:loopInfoElement];
    NSXMLElement *existingTrack3 = [NxATrackTraktorParsingTests traktorTestSourceTrack3];
    [collectionElement addChild:existingTrack1];
    [collectionElement addChild:existingTrack2];
    [collectionElement addChild:existingTrack3];

    // -- When.
    NSSet *children = [self.parser removeAndReturnValidChildrenFromCollectionSource:collectionElement];

    // -- Then.
    XCTAssertEqual(children.count, 2);
    XCTAssertTrue([children containsObject:existingTrack1]);
    XCTAssertTrue([children containsObject:existingTrack3]);
    XCTAssertNil(existingTrack1.parent);
    XCTAssertNil(existingTrack3.parent);
    XCTAssertEqual(collectionElement.children.count, 1);
    XCTAssertEqual(collectionElement.children.firstObject, existingTrack2);

    assertThatNothingWasLogged();
}

- (void)testRemoveAndReturnValidChildrenFromPlaylistSource_PlaylistElementWithChildren_ReturnsTheChildrenAndDetachesThemFromTheParent
{
    // -- Given.
    NSXMLElement *playlistChild1 = [NxATestCase xmlElementFromString:@"<ENTRY><PRIMARYKEY TYPE=\"TRACK\" KEY=\"Macintosh HD/:Users/:didier/:Music/:Gigs/:Angel Johnson, Tammy Cartwright - Lovers Heaven (Jacob Bech Remix).aif\"></PRIMARYKEY></ENTRY>"];
    NSXMLElement *playlistChild2 = [NxATestCase xmlElementFromString:@"<ENTRY><PRIMARYKEY TYPE=\"TRACK\" KEY=\"Macintosh HD/:Users/:didier/:Music/:Gigs/:Callin' feat. S-Man (Original Mix) - Huxley, S-Man.aiff\"></PRIMARYKEY></ENTRY>"];
    NSXMLElement *nodeElement = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"PLAYLIST\" NAME=\"DeepDeepDown 7/24/14\"></NODE>"];
    NSXMLElement *playlistElement = [NxATestCase xmlElementFromString:@"<PLAYLIST ENTRIES=\"34\" TYPE=\"LIST\" UUID=\"9bf4382a1acf49f7b095ecefa52f7da0\"></PLAYLIST>"];
    [nodeElement addChild:playlistElement];
    [playlistElement addChild:playlistChild1];
    [playlistElement addChild:playlistChild2];

    // -- When.
    NSSet *children = [self.parser removeAndReturnValidChildrenFromPlaylistSource:nodeElement];

    // -- Then.
    XCTAssertEqual(children.count, 2);
    XCTAssertTrue([children containsObject:playlistChild1]);
    XCTAssertTrue([children containsObject:playlistChild2]);
    XCTAssertEqual(playlistElement.children.count, 0);

    assertThatNothingWasLogged();
}

- (void)testRemoveAndReturnValidChildrenFromFolderSource_FolderElementWithChildren_ReturnsTheChildrenAndDetachesThemFromTheParent
{
    // -- Given.
    NSXMLElement *playlist1Element = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"PLAYLIST\" NAME=\"DeepDeepDown 7/24/14\"></NODE>"];
    NSXMLElement *playlist2Element = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"PLAYLIST\" NAME=\"_Internal\"></NODE>"];
    NSXMLElement *playlist3Element = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"PLAYLIST\" NAME=\"Other\"></NODE>"];
    NSXMLElement *folderElement = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"FOLDER\" NAME=\"Gigs\"></NODE>"];
    NSXMLElement *subNodesElement = [NxATestCase xmlElementFromString:@"<SUBNODES COUNT=\"2\"></SUBNODES>"];
    [folderElement addChild:subNodesElement];
    [subNodesElement addChild:playlist1Element];
    [subNodesElement addChild:playlist2Element];
    [subNodesElement addChild:playlist3Element];

    // -- When.
    NSSet *children = [self.parser removeAndReturnValidChildrenFromFolderSource:folderElement];

    // -- Then.
    XCTAssertEqual(children.count, 2);
    XCTAssertTrue([children containsObject:playlist1Element]);
    XCTAssertTrue([children containsObject:playlist3Element]);
    XCTAssertEqual(subNodesElement.children.count, 1);
    XCTAssertEqual(subNodesElement.children.firstObject, playlist2Element);

    assertThatNothingWasLogged();
}

- (void)testNumberOfChildrenLeftInCollectionSource_CollectionElementWithIncorrectEntryCount_ReturnsTheCorrectChildrencCount
{
    // -- Given.
    NSXMLElement *collectionElement = [NxATestCase xmlElementFromString:@"<COLLECTION ENTRIES=\"437\"> \
                                       </COLLECTION>"];
    NSXMLElement *existingTrack1 = [NxATrackTraktorParsingTests traktorTestSourceTrack1];
    NSXMLElement *existingTrack2 = [NxATrackTraktorParsingTests traktorTestSourceTrack2];
    NSXMLElement *loopInfoElement = [NxATestCase xmlElementFromString:@"<LOOPINFO SAMPLE_TYPE_INFO=\"0\"></LOOPINFO>"];
    [existingTrack2 addChild:loopInfoElement];
    NSXMLElement *existingTrack3 = [NxATrackTraktorParsingTests traktorTestSourceTrack3];
    [collectionElement addChild:existingTrack1];
    [collectionElement addChild:existingTrack2];
    [collectionElement addChild:existingTrack3];

    // -- When.
    NSUInteger count = [self.parser numberOfChildrenLeftInCollectionSource:collectionElement];

    // -- Then.
    XCTAssertEqual(count, 3);

    assertThatNothingWasLogged();
}

- (void)testNumberOfChildrenLeftInFolderSource_FolderElementWithIncorrectEntryCount_ReturnsTheCorrectChildrenCount
{
    // -- Given.
    NSXMLElement *folderElement = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"FOLDER\" NAME=\"TEST\"></NODE>"];
    NSXMLElement *subNodesElement = [NxATestCase xmlElementFromString:@"<SUBNODES COUNT=\"19\"></SUBNODES>"];
    NSXMLElement *playlist1Element = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"PLAYLIST\" NAME=\"_RECORDINGS\"><PLAYLIST ENTRIES=\"0\" TYPE=\"LIST\" UUID=\"fe677a538feb43abbaa65cb2dc180604\"></PLAYLIST></NODE>"];
    NSXMLElement *playlist2Element = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"PLAYLIST\" NAME=\"RECORDINGS2\"><PLAYLIST ENTRIES=\"0\" TYPE=\"LIST\" UUID=\"fe677a538feb43abbaa65cb2dc180604\"></PLAYLIST></NODE>"];
    [subNodesElement addChild:playlist1Element];
    [subNodesElement addChild:playlist2Element];
    [folderElement addChild:subNodesElement];

    // -- When.
    NSUInteger count = [self.parser numberOfChildrenLeftInFolderSource:folderElement];

    // -- Then.
    XCTAssertEqual(count, 2);

    assertThatNothingWasLogged();
}

- (void)testUpdateTrackSource_UpdatesAnEmptyTrack_UpdatesAllFieldsCorrectly
{
    // -- Given.
    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2014-07-23 07:00:00" format:@"yyyy-MM-dd HH:mm:ss"];
    NxATrack *track = [NxATrackTests testTrack3InContext:self.context
                                                 keyName:NxAKeyPropertyTraktorKeyName
                                        modificationDate:modificationDate];
    NSXMLElement *nmlElement = [NxATestCase xmlElementFromString:@"<ENTRY></ENTRY>"];

    // -- When.
    [self.parser updateTrackSource:nmlElement forTrack:track];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<ENTRY MODIFIED_DATE=\"2014/7/23\" MODIFIED_TIME=\"46800\" \
                                    TITLE=\"Terra (Original Mix)\" ARTIST=\"Manuel De La Mare, Luigi Rocca\"> \
                                    <ALBUM TRACK=\"1\" TITLE=\"Terra\"></ALBUM> \
                                    <MODIFICATION_INFO AUTHOR_TYPE=\"user\"></MODIFICATION_INFO> \
                                    <INFO GENRE=\"Tech House\" LABEL=\"303Lovers\" MIX=\"\" COMMENT=\"\" RATING=\"\" \
                                    REMIXER=\"\" PRODUCER=\"\" PLAYCOUNT=\"0\" RANKING=\"0\" \
                                    IMPORT_DATE=\"2013/11/28\" RELEASE_DATE=\"2013/10/9\" KEY=\"Dm\"></INFO> \
                                    <TEMPO BPM=\"124.000\" BPM_QUALITY=\"100\"></TEMPO> \
                                    <CUE_V2 NAME=\"\" DISPL_ORDER=\"0\" TYPE=\"4\" START=\"484.5890\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                    <CUE_V2 NAME=\"ThirdTest2\" DISPL_ORDER=\"0\" TYPE=\"3\" START=\"484.5890\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"0\"></CUE_V2> \
                                    <CUE_V2 NAME=\"ThirdTest3\" DISPL_ORDER=\"0\" TYPE=\"0\" START=\"31452.3008\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"1\"></CUE_V2> \
                                    <CUE_V2 NAME=\"ThirdTest4\" DISPL_ORDER=\"0\" TYPE=\"0\" START=\"62420.1016\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"2\"></CUE_V2> \
                                    <CUE_V2 NAME=\"ThirdTest5\" DISPL_ORDER=\"0\" TYPE=\"1\" START=\"93387.7969\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                    <CUE_V2 NAME=\"ThirdTest6\" DISPL_ORDER=\"0\" TYPE=\"2\" START=\"341130.0000\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                    <MUSICAL_KEY VALUE=\"14\"></MUSICAL_KEY> \
                                    </ENTRY>"];
    XCTAssertEqualObjects(nmlElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testUpdateTrackSource_UpdatesAnEmptyTrackFromAnEmptyTrack_UpdatesAllFieldsCorrectly
{
    // -- Given.
    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2014-07-23 07:00:00" format:@"yyyy-MM-dd HH:mm:ss"];
    NxATrack *track = [NxATrackTests emptyTestTrackInContext:self.context
                                                usingKeyName:NxAKeyPropertyTraktorKeyName
                                              lastModifiedOn:modificationDate];
    NSXMLElement *nmlElement = [NxATestCase xmlElementFromString:@"<ENTRY></ENTRY>"];

    // -- When.
    [self.parser updateTrackSource:nmlElement forTrack:track];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<ENTRY MODIFIED_DATE=\"2014/7/23\" MODIFIED_TIME=\"46800\" TITLE=\"\" ARTIST=\"\"> \
                                    <MODIFICATION_INFO AUTHOR_TYPE=\"user\"> \
                                    </MODIFICATION_INFO> \
                                    <INFO GENRE=\"\" LABEL=\"\" MIX=\"\" COMMENT=\"\" RATING=\"\" REMIXER=\"\" PRODUCER=\"\" \
                                    PLAYCOUNT=\"\" RANKING=\"\" IMPORT_DATE=\"\" RELEASE_DATE=\"\"></INFO> \
                                    </ENTRY>"];
    XCTAssertEqualObjects(nmlElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testUpdateTrackSource_UpdatesAnEmptyTrackWithATrackWithMultipleBPM_UpdatesAllFieldsCorrectlyAndWarnsAboutTheBPM
{
    // -- Given.
    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2014-07-23 07:00:15" format:@"yyyy-MM-dd HH:mm:ss"];
    NxATrack *track = [NxATrackTests testTrack1InContext:self.context
                                                 keyName:NxAKeyPropertyTraktorKeyName
                                        modificationDate:modificationDate];
    NSXMLElement *nmlElement = [NxATestCase xmlElementFromString:@"<ENTRY></ENTRY>"];

    // -- When.
    [self.parser updateTrackSource:nmlElement forTrack:track];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<ENTRY MODIFIED_DATE=\"2014/7/23\" MODIFIED_TIME=\"46815\" \
                                    TITLE=\"I'm In Love (Deep Mix)\" ARTIST=\"Artful, Donae'O\"> \
                                    <ALBUM TRACK=\"25\" TITLE=\"Defected Presents Dimitri from Paris In the House of Disco\"></ALBUM> \
                                    <MODIFICATION_INFO AUTHOR_TYPE=\"user\"></MODIFICATION_INFO> \
                                    <INFO GENRE=\"House, Progressive House\" LABEL=\"Size Records\" MIX=\"Original Mix\" COMMENT=\"Some snazzy comments!\" RATING=\"Awesome, Play:Now\" \
                                    REMIXER=\"Third Party, Rick James\" PRODUCER=\"My Digital Enemy, Pleasurekraft\" PLAYCOUNT=\"142\" RANKING=\"153\" \
                                    IMPORT_DATE=\"2014/6/17\" RELEASE_DATE=\"2012/3/23\" KEY=\"Fm\"></INFO> \
                                    <TEMPO BPM=\"124.000\" BPM_QUALITY=\"100\"></TEMPO> \
                                    <CUE_V2 NAME=\"\" DISPL_ORDER=\"0\" TYPE=\"4\" START=\"68.0000\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                    <CUE_V2 NAME=\"Test2\" DISPL_ORDER=\"0\" TYPE=\"0\" START=\"68.0000\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"0\"></CUE_V2> \
                                    <CUE_V2 NAME=\"Test3\" DISPL_ORDER=\"0\" TYPE=\"0\" START=\"31036.0000\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"1\"></CUE_V2> \
                                    <CUE_V2 NAME=\"\" DISPL_ORDER=\"0\" TYPE=\"4\" START=\"50068.0000\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                    <CUE_V2 NAME=\"Test5\" DISPL_ORDER=\"0\" TYPE=\"0\" START=\"62004.0000\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                    <CUE_V2 NAME=\"\" DISPL_ORDER=\"0\" TYPE=\"4\" START=\"200068.0000\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                    <CUE_V2 NAME=\"Test7\" DISPL_ORDER=\"0\" TYPE=\"0\" START=\"371681.0000\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                    <MUSICAL_KEY VALUE=\"17\"></MUSICAL_KEY> \
                                    </ENTRY>"];
    XCTAssertEqualObjects(nmlElement, expectedResult);

    [verifyCount(self.mockLog, times(2)) addWarning:anything() inContextWithPerObjectWarningInfo:anything()];
    MKTArgumentCaptor *argument1 = [[MKTArgumentCaptor alloc] init];
    MKTArgumentCaptor *argument2 = [[MKTArgumentCaptor alloc] init];
    [verifyCount(self.mockLog, times(2)) addWarning:[argument1 capture] inContextWithPerObjectWarningInfo:[argument2 capture]];
    NSArray *values1 = argument1.allValues;
    XCTAssertEqual(values1.count, 2);
    XCTAssertTrue([[values1[0] p_description] hasPrefix:@"in your Rekord Buddy collection with multiple BPM value"]);
    XCTAssertTrue([[values1[1] p_description] hasPrefix:@"in your Rekord Buddy collection with multiple BPM value"]);
    NSArray *values2 = argument2.allValues;
    XCTAssertEqual(values2.count, 2);
    XCTAssertEqualObjects(values2[0], @"");
    XCTAssertEqualObjects(values2[1], @"");
}

- (void)testUpdateTrackSource_UpdatesAnExistingTrack_UpdatesAllFieldsCorrectly
{
    // -- Given.
    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2014-07-23 07:10:00" format:@"yyyy-MM-dd HH:mm:ss"];
    NxATrack *track = [NxATrackTests testTrack3InContext:self.context
                                                 keyName:NxAKeyPropertyTraktorKeyName
                                        modificationDate:modificationDate];
    NSXMLElement *nmlElement = [NxATrackTraktorParsingTests traktorTestSourceTrack1];
    NSXMLElement *infoElement = [nmlElement elementForName:@"INFO"];
    [infoElement addAttributeWithName:@"MYTestAttribute" stringValue:@"Test Value"];
    NSXMLElement *testChildElement = [NxATestCase xmlElementFromString:@"<MY_OTHER_TEST VALUE=\"TEST\"></MY_OTHER_TEST>"];
    [nmlElement addChild:testChildElement];

    // -- When.
    [self.parser updateTrackSource:nmlElement forTrack:track];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<ENTRY MODIFIED_DATE=\"2014/7/23\" MODIFIED_TIME=\"47400\" LOCK=\"1\" \
                                    LOCK_MODIFICATION_TIME=\"2014-07-06T22:08:05\" \
                                    AUDIO_ID=\"AbdXd3d3d3d2M0mYiIiIiIiIiHaJiZmImJiZiam6q6uqqqmndmnLu6urqqqqqavKu7urqqqqqbpTI0REVDM0MzSJdomIh4eJeHmaqImZh4iJiru7y7vLqqupnN3M3MzLu7y8vc3MzNzLu7u8vMy7y8u6qqqbqrupmqqpqampqquqmaq6mKqaqrqrupu6maqrl6y8u8vLu7u6vM28zMu6urq7zMurq6uqqrq7uqq7u6u7mqq7qqmZiJiHd3eHmKmZmYmHd3eIy8zMvMy7q7q7y9zMzLy7u6u7y6mampmZmampqqqampmZmampuZiJiXiIiJiXiZiJiXiIiJiZqUEAAA==\" \
                                    TITLE=\"Terra (Original Mix)\" ARTIST=\"Manuel De La Mare, Luigi Rocca\"> \
                                    <LOCATION DIR=\"/:Users/:didier/:Music/:Gigs/:\" FILE=\"25 I'm In Love (feat. Donae'O) [Deep Mix].m4a\" VOLUME=\"Macintosh HD\" VOLUMEID=\"Macintosh HD\"></LOCATION> \
                                    <ALBUM OF_TRACKS=\"39\" TRACK=\"1\" TITLE=\"Terra\"></ALBUM> \
                                    <MODIFICATION_INFO AUTHOR_TYPE=\"user\"></MODIFICATION_INFO> \
                                    <INFO BITRATE=\"-1\" LABEL=\"303Lovers\"  MIX=\"\" COMMENT=\"\" GENRE=\"Tech House\" REMIXER=\"\" PRODUCER=\"\" RATING=\"\"  \
                                    COVERARTID=\"023/XIFLNRAUKICXDAF1H0BCBQMYGYPD\" KEY=\"Dm\" PLAYCOUNT=\"0\" RANKING=\"0\" PLAYTIME=\"440\" PLAYTIME_FLOAT=\"439.414\" \
                                    IMPORT_DATE=\"2013/11/28\" LAST_PLAYED=\"2014/7/6\" RELEASE_DATE=\"2013/10/9\" FLAGS=\"28\" FILESIZE=\"14676\" \
                                    MYTestAttribute=\"Test Value\"></INFO> \
                                    <LOUDNESS PEAK_DB=\"-0\" PERCEIVED_DB=\"1.11107\" ANALYZED_DB=\"1.11107\"></LOUDNESS> \
                                    <MUSICAL_KEY VALUE=\"14\"></MUSICAL_KEY> \
                                    <MY_OTHER_TEST VALUE=\"TEST\"></MY_OTHER_TEST> \
                                    <TEMPO BPM=\"124.000\" BPM_QUALITY=\"100\"></TEMPO> \
                                    <CUE_V2 NAME=\"\" DISPL_ORDER=\"0\" TYPE=\"4\" START=\"484.5890\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                    <CUE_V2 NAME=\"ThirdTest2\" DISPL_ORDER=\"0\" TYPE=\"3\" START=\"484.5890\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"0\"></CUE_V2> \
                                    <CUE_V2 NAME=\"ThirdTest3\" DISPL_ORDER=\"0\" TYPE=\"0\" START=\"31452.3008\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"1\"></CUE_V2> \
                                    <CUE_V2 NAME=\"ThirdTest4\" DISPL_ORDER=\"0\" TYPE=\"0\" START=\"62420.1016\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"2\"></CUE_V2> \
                                    <CUE_V2 NAME=\"ThirdTest5\" DISPL_ORDER=\"0\" TYPE=\"1\" START=\"93387.7969\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                    <CUE_V2 NAME=\"ThirdTest6\" DISPL_ORDER=\"0\" TYPE=\"2\" START=\"341130.0000\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                    </ENTRY>"];
    XCTAssertEqualObjects(nmlElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testEmptyTrackSourceFor_FileOnBootVolume_FileLocationIsCorrect
{
    // -- Given.
    NxATrack *stubbedTrack = [NxATrackTests stubbedTrackWithPath:@"/Test/Path:Test/File.MP3" context:self.context];

    // -- When.
    NSXMLElement *trackElement = [self.parser emptyTrackSourceFor:stubbedTrack];

    // -- Then.
    NSString *bootVolumeName = [NxAOSXAudioFile bootVolumeName];
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:[NSString stringWithFormat:@"<ENTRY><LOCATION DIR=\"/:Test/:Path//Test/:\" \
                                                                      FILE=\"File.MP3\" VOLUME=\"%@\" VOLUMEID=\"%@\"> \
                                                                      </LOCATION></ENTRY>", bootVolumeName, bootVolumeName]];
    XCTAssertEqualObjects(trackElement, expectedResult);
}

- (void)testEmptyTrackSourceFor_FileNotOnBootVolume_FileLocationIsCorrectAndIDIdIsNew
{
    // -- Given.
    NSString *otherVolumeName = [[NxAOSXAudioFile otherVolumeNames] anyObject];
    NxATrack *stubbedTrack = [NxATrackTests stubbedTrackWithPath:[NSString stringWithFormat:@"/Volumes/%@/Test/Path/File.MP3", otherVolumeName]
                                                         context:self.context];

    // -- When.
    NSXMLElement *trackElement = [self.parser emptyTrackSourceFor:stubbedTrack];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:[NSString stringWithFormat:@"<ENTRY><LOCATION DIR=\"/:Test/:Path/:\" \
                                                                      FILE=\"File.MP3\" VOLUME=\"%@\" VOLUMEID=\"%@\"> \
                                                                      </LOCATION></ENTRY>", otherVolumeName, otherVolumeName]];
    XCTAssertEqualObjects(trackElement, expectedResult);
}

- (void)testEmptyPlaylistSourceFor_FolderWithAName_ReturnsCorrectFolderElement
{
    // -- Given.
    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2012-06-02 04:54:23" format:@"yyyy-MM-dd HH:mm:ss"];
    NxACrate *crate = [self playlistWithName:@"Test Playlist"
              withEmptyContentLastModifiedOn:modificationDate];

    // -- When.
    NSXMLElement *playlistElement = [self.parser emptyPlaylistSourceFor:crate];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"PLAYLIST\" NAME=\"Test Playlist\"><PLAYLIST ENTRIES=\"0\" TYPE=\"LIST\"></PLAYLIST></NODE>"];
    XCTAssertEqualObjects(playlistElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testEmptyFolderSourceFor_FolderWithAName_ReturnsCorrectFolderElement
{
    // -- Given.
    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2012-06-02 04:54:23" format:@"yyyy-MM-dd HH:mm:ss"];
    NxACrate *crate = [self folderWithName:@"Test Folder"
            withEmptyContentLastModifiedOn:modificationDate];
    NxACrate *playlist = [self playlistWithName:@"Test Playlist"
                 withEmptyContentLastModifiedOn:modificationDate];
    NxACrate *folder = [self playlistWithName:@"Test Folder 2"
               withEmptyContentLastModifiedOn:modificationDate];
    [crate addCrate:playlist modificationDate:modificationDate];
    [crate addCrate:folder modificationDate:modificationDate];

    // -- When.
    NSXMLElement *folderElement = [self.parser emptyFolderSourceFor:crate];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"FOLDER\" NAME=\"Test Folder\"><SUBNODES COUNT=\"0\"></SUBNODES></NODE>"];
    XCTAssertEqualObjects(folderElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testPlaylistEntrySourceFor_KnownTrack_ReturnsCorrectElement
{
    // -- Given.
    NxATrack *stubbedTrack = [NxATrackTests stubbedTrackWithPath:@"/Test/Path/File.MP3" context:self.context];
    self.parser.p_traktorPlaylistPathByTrackID[stubbedTrack.objectID] = [NSString stringWithFormat:@"%@/:Test/:Path/:File.MP3",
                                                                         [NxAOSXAudioFile bootVolumeName]];

    // -- When.
    NSXMLElement *playlistEntry = [self.parser playlistEntrySourceFor:stubbedTrack];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:[NSString stringWithFormat:@"<ENTRY><PRIMARYKEY TYPE=\"TRACK\" KEY=\"%@/:Test/:Path/:File.MP3\"></PRIMARYKEY></ENTRY>", [NxAOSXAudioFile bootVolumeName]]];
    XCTAssertEqualObjects(playlistEntry, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testPlaylistEntrySourceFor_UnknownTrack_ReturnsNilAndLogsAWarning
{
    // -- Given.
    NxATrack *stubbedTrack = [NxATrackTests stubbedTrackWithPath:@"/Test/Path/File.MP3" context:self.context];

    // -- When.
    NSXMLElement *playlistEntry = [self.parser playlistEntrySourceFor:stubbedTrack];

    // -- Then.
    XCTAssertNil(playlistEntry);
    
    [verifyCount(self.mockLog, times(1)) addWarning:anything() inContextWithPerObjectWarningInfo:anything()];
    MKTArgumentCaptor *argument1 = [[MKTArgumentCaptor alloc] init];
    MKTArgumentCaptor *argument2 = [[MKTArgumentCaptor alloc] init];
    [verify(self.mockLog) addWarning:[argument1 capture] inContextWithPerObjectWarningInfo:[argument2 capture]];
    NSArray *values1 = argument1.allValues;
    XCTAssertEqual(values1.count, 1);
    XCTAssertTrue([[values1.firstObject p_description] hasPrefix:@"in a Traktor playlist with an invalid track"]);
    NSArray *values2 = argument2.allValues;
    XCTAssertEqual(values2.count, 1);
    XCTAssertEqualObjects(values2.firstObject, @"");
}

- (void)testAddTrackSource_TrackAndCollection_AddTheTrackCorrectly
{
    // -- Given.
    NSXMLElement *collectionElement = [NxATestCase xmlElementFromString:@"<COLLECTION ENTRIES=\"437\"> \
                                       </COLLECTION>"];
    NSXMLElement *trackElement = [NxATestCase xmlElementFromString:@"<ENTRY MODIFIED_DATE=\"2013/12/19\" TITLE=\"Test\"></ENTRY>"];

    // -- When.
    [self.parser addTrackSource:trackElement asChildOf:collectionElement];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<COLLECTION ENTRIES=\"437\"> \
                                    <ENTRY MODIFIED_DATE=\"2013/12/19\" TITLE=\"Test\"></ENTRY> \
                                    </COLLECTION>"];
    XCTAssertEqualObjects(collectionElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testAddPlaylistEntrySource_TrackEntryAndPlaylist_AddTheTrackEntryCorrectly
{
    // -- Given.
    NSXMLElement *playlistElement = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"PLAYLIST\" NAME=\"RECORDINGS\"><PLAYLIST ENTRIES=\"0\" TYPE=\"LIST\" UUID=\"fe677a538feb43abbaa65cb2dc180604\"></PLAYLIST></NODE>"];
    NSXMLElement *trackEntryElement = [NxATestCase xmlElementFromString:@"<ENTRY><PRIMARYKEY TYPE=\"TRACK\" KEY=\"Macintosh HD/:Users/:didier/:Music/:Acapellas/:Walking Alone (Acapella).aif\"></PRIMARYKEY></ENTRY>"];

    // -- When.
    [self.parser addPlaylistEntrySource:trackEntryElement asChildOf:playlistElement];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"PLAYLIST\" NAME=\"RECORDINGS\"><PLAYLIST ENTRIES=\"0\" TYPE=\"LIST\" UUID=\"fe677a538feb43abbaa65cb2dc180604\"><ENTRY><PRIMARYKEY TYPE=\"TRACK\" KEY=\"Macintosh HD/:Users/:didier/:Music/:Acapellas/:Walking Alone (Acapella).aif\"></PRIMARYKEY></ENTRY></PLAYLIST></NODE>"];
    XCTAssertEqualObjects(playlistElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testAddFolderEntrySource_PlaylistAndFolder_AddThePlaylistCorrectly
{
    // -- Given.
    NSXMLElement *folderElement = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"FOLDER\" NAME=\"TEST\"><SUBNODES COUNT=\"19\"></SUBNODES></NODE>"];
    NSXMLElement *playlistElement = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"PLAYLIST\" NAME=\"RECORDINGS\"><PLAYLIST ENTRIES=\"0\" TYPE=\"LIST\" UUID=\"fe677a538feb43abbaa65cb2dc180604\"></PLAYLIST></NODE>"];

    // -- When.
    [self.parser addFolderEntrySource:playlistElement asChildOf:folderElement];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"FOLDER\" NAME=\"TEST\"><SUBNODES COUNT=\"19\"><NODE TYPE=\"PLAYLIST\" NAME=\"RECORDINGS\"><PLAYLIST ENTRIES=\"0\" TYPE=\"LIST\" UUID=\"fe677a538feb43abbaa65cb2dc180604\"></PLAYLIST></NODE></SUBNODES></NODE>"];
    XCTAssertEqualObjects(folderElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testSetEntriesCountForCollectionSource_CollectionWith437Entries_ChangesTheEntryCountCorrectly
{
    // -- Given.
    NSXMLElement *collectionElement = [NxATestCase xmlElementFromString:@"<COLLECTION ENTRIES=\"437\"> \
                                       <ENTRY MODIFIED_DATE=\"2013/12/19\" TITLE=\"Test\"></ENTRY> \
                                       </COLLECTION>"];

    // -- When.
    [self.parser setEntriesCountForCollectionSource:collectionElement to:@(23)];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<COLLECTION ENTRIES=\"23\"> \
                                    <ENTRY MODIFIED_DATE=\"2013/12/19\" TITLE=\"Test\"></ENTRY> \
                                    </COLLECTION>"];
    XCTAssertEqualObjects(collectionElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testSetEntriesCountForPlaylistSource_TrackEntryAndPlaylist_ChangesTheEntryCountCorrectly
{
    // -- Given.
    NSXMLElement *playlistElement = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"PLAYLIST\" NAME=\"RECORDINGS\"><PLAYLIST ENTRIES=\"0\" TYPE=\"LIST\" UUID=\"fe677a538feb43abbaa65cb2dc180604\"><ENTRY><PRIMARYKEY TYPE=\"TRACK\" KEY=\"Macintosh HD/:Users/:didier/:Music/:Acapellas/:Walking Alone (Acapella).aif\"></PRIMARYKEY></ENTRY></PLAYLIST></NODE>"];

    // -- When.
    [self.parser setEntriesCountForPlaylistSource:playlistElement to:@(23)];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"PLAYLIST\" NAME=\"RECORDINGS\"><PLAYLIST ENTRIES=\"23\" TYPE=\"LIST\" UUID=\"fe677a538feb43abbaa65cb2dc180604\"><ENTRY><PRIMARYKEY TYPE=\"TRACK\" KEY=\"Macintosh HD/:Users/:didier/:Music/:Acapellas/:Walking Alone (Acapella).aif\"></PRIMARYKEY></ENTRY></PLAYLIST></NODE>"];
    XCTAssertEqualObjects(playlistElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testSetEntriesCountForFolderSource_PlaylistAndFolder_ChangesTheEntryCountCorrectly
{
    // -- Given.
    NSXMLElement *folderElement = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"FOLDER\" NAME=\"TEST\"><SUBNODES COUNT=\"19\"><NODE TYPE=\"PLAYLIST\" NAME=\"RECORDINGS\"><PLAYLIST ENTRIES=\"0\" TYPE=\"LIST\" UUID=\"fe677a538feb43abbaa65cb2dc180604\"></PLAYLIST></NODE></SUBNODES></NODE>"];

    // -- When.
    [self.parser setEntriesCountForFolderSource:folderElement to:@(23)];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"FOLDER\" NAME=\"TEST\"><SUBNODES COUNT=\"23\"><NODE TYPE=\"PLAYLIST\" NAME=\"RECORDINGS\"><PLAYLIST ENTRIES=\"0\" TYPE=\"LIST\" UUID=\"fe677a538feb43abbaa65cb2dc180604\"></PLAYLIST></NODE></SUBNODES></NODE>"];
    XCTAssertEqualObjects(folderElement, expectedResult);

    assertThatNothingWasLogged()
}

- (void)testDeleteCollectionEntrySources_CollectionWithChildren_RemovesTheChildren
{
    // -- Given.
    NSXMLElement *collectionElement = [NxATestCase xmlElementFromString:@"<COLLECTION ENTRIES=\"437\"> \
                                       </COLLECTION>"];
    NSXMLElement *trackElement = [NxATestCase xmlElementFromString:@"<ENTRY MODIFIED_DATE=\"2013/12/19\" TITLE=\"Test\"></ENTRY>"];
    [collectionElement addChild:trackElement];

    // -- When.
    [self.parser deleteCollectionEntrySources:[NSSet setWithObject:trackElement]];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<COLLECTION ENTRIES=\"437\"> \
                                    </COLLECTION>"];
    XCTAssertEqualObjects(collectionElement, expectedResult);
    XCTAssertNil(trackElement.parent);

    assertThatNothingWasLogged();
}

- (void)testDeletePlaylistEntrySources_PlaylistWithChildren_RemovesTheChildren
{
    // -- Given.
    NSXMLElement *playlistElement = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"PLAYLIST\" NAME=\"RECORDINGS\"><PLAYLIST ENTRIES=\"0\" TYPE=\"LIST\" UUID=\"fe677a538feb43abbaa65cb2dc180604\"></PLAYLIST></NODE>"];
    NSXMLElement *playlistEntryElement = [NxATestCase xmlElementFromString:@"<ENTRY><PRIMARYKEY TYPE=\"TRACK\" KEY=\"Macintosh HD/:Users/:didier/:Music/:Acapellas/:Walking Alone (Acapella).aif\"></PRIMARYKEY></ENTRY>"];
    [playlistElement addChild:playlistEntryElement];

    // -- When.
    [self.parser deletePlaylistEntrySources:[NSSet setWithObject:playlistEntryElement]];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"PLAYLIST\" NAME=\"RECORDINGS\"><PLAYLIST ENTRIES=\"0\" TYPE=\"LIST\" UUID=\"fe677a538feb43abbaa65cb2dc180604\"></PLAYLIST></NODE>"];
    XCTAssertEqualObjects(playlistElement, expectedResult);
    XCTAssertNil(playlistEntryElement.parent);

    assertThatNothingWasLogged();
}

- (void)testDeleteFolderEntrySources_FolderWithChildren_RemovesTheChildren
{
    // -- Given.
    NSXMLElement *folderElement = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"FOLDER\" NAME=\"TEST\"><SUBNODES COUNT=\"19\"></SUBNODES></NODE>"];
    NSXMLElement *playlistElement = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"PLAYLIST\" NAME=\"RECORDINGS\"><PLAYLIST ENTRIES=\"0\" TYPE=\"LIST\" UUID=\"fe677a538feb43abbaa65cb2dc180604\"></PLAYLIST></NODE>"];
    [folderElement addChild:playlistElement];

    // -- When.
    [self.parser deleteFolderEntrySources:[NSSet setWithObject:playlistElement]];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<NODE TYPE=\"FOLDER\" NAME=\"TEST\"><SUBNODES COUNT=\"19\"></SUBNODES></NODE>"];
    XCTAssertEqualObjects(folderElement, expectedResult);
    XCTAssertNil(playlistElement.parent);

    assertThatNothingWasLogged();
}

@end
