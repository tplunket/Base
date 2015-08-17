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
#import "NxAOSXAudioFileTests.h"
#import "Track Collection/AudioFiles/Internal/NxAAudioFile.h"
#import "NSDate+NxAUtility.h"
#import "NSString+NxAUtility.h"
#import "NSXMLElement+NxAUtility.h"
#import "Track Collection/Crates/NxACrate.h"
#import "NxATrackRekordboxParsingTests.h"
#import "NxATrackDateModified.h"
#import "NxATrackDateAdded.h"
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
#import "NxAProgressViewController.h"
#import "NxATrackKey+RekordboxParsing.h"
#import "Track Collection/Parsers/Internal/NxARekordboxParser.h"
#import "Utility Classes/Internal/NxAWarning.h"

#pragma mark Private Interface

@interface NxARekordboxParserTests : NxATestCase

@property (strong, nonatomic) NxARekordboxParser *parser;

@end

#pragma mark Implementation

@implementation NxARekordboxParserTests

#pragma mark Accessors

- (NxARekordboxParser *)parser
{
    if (!_parser) {
        _parser = [NxARekordboxParser implementationWithContext:self.context
                                              loggingMessagesIn:self.mockLog];
        NxAAssertNotNil(_parser);
    }
    return _parser;
}

#pragma mark Test methods

- (void)testp_TrackForRekordboxPlaylistIDEntryElement_ParseAKnownID_ReturnsTheCorrectTrack
{
    // -- Given.
    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-11-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    NxATrack *existingTrack1 = [NxATrackTests testTrack2InContext:self.context
                                                          keyName:NxAKeyPropertyRekordboxKeyName
                                                 modificationDate:modificationDate];

    self.parser.p_tracksByRekordboxTrackID[@"103"] = existingTrack1;

    NSXMLElement *entryElement = [NxATestCase xmlElementFromString:@"<TRACK Key=\"103\"></TRACK>"];

    // -- When.
    NxATrack *track = [self.parser p_trackForRekordboxPlaylistIDEntryElement:entryElement];

    // -- Then.
    XCTAssertEqualObjects(track, existingTrack1);

    assertThatNothingWasLogged();
}

- (void)testp_TrackForRekordboxPlaylistIDEntryElement_ParseAnUnknownID_ReturnsNil
{
    // -- Given.
    NSXMLElement *entryElement = [NxATestCase xmlElementFromString:@"<TRACK Key=\"3\"></TRACK>"];

    // -- When.
    NxATrack *track = [self.parser p_trackForRekordboxPlaylistIDEntryElement:entryElement];

    // -- Then.
    XCTAssertNil(track);

    assertThatNothingWasLogged();
}

- (void)testp_TrackForRekordboxPlaylistPathEntryElement_ParseAKnownTrack_ReturnsTheCorrectTrack
{
    // -- Given.
    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-11-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    NxATrack *existingTrack1 = [NxATrackTests testTrack1InContext:self.context
                                                          keyName:NxAKeyPropertyRekordboxKeyName
                                                 modificationDate:modificationDate];

    NSString *track1Path = @"/Users/didier/Music/Gigs/25 I'm In Love (feat. Donae'O) [Deep Mix].m4a";
    NxAOSXAudioFile *stubbedFile1 = [NxAOSXAudioFileTests stubbedOSXAudioFileWithPath:track1Path
                                                                              context:self.context];
    stubbedFile1.p_parentTrack = existingTrack1;

    self.parser.p_tracksByRekordboxTrackLocation[@"file://localhost/Users/didier/Music/Gigs/25%20I%27m%20In%20Love%20(feat.%20Donae%27O)%20%5bDeep%20Mix%5d.m4a"] = existingTrack1;

    NSXMLElement *entryElement = [NxATestCase xmlElementFromString:@"<TRACK Key=\"file://localhost/Users/didier/Music/Gigs/25%20I%27m%20In%20Love%20(feat.%20Donae%27O)%20%5bDeep%20Mix%5d.m4a\"></TRACK>"];

    // -- When.
    NxATrack *track = [self.parser p_trackForRekordboxPlaylistPathEntryElement:entryElement];

    // -- Then.
    XCTAssertEqualObjects(track, existingTrack1);

    assertThatNothingWasLogged();
}

- (void)testp_TrackForRekordboxPlaylistPathEntryElement_ParseAnUnknownTrack_ReturnsNil
{
    // -- Given.
    NSXMLElement *entryElement = [NxATestCase xmlElementFromString:@"<TRACK Key=\"file://localhost/Users/didier/Music/Gigs/Terra%20(Original%20Mix)%20-%20Manuel%20De%20La%20Mare%20and%20Luigi%20Rocca.aiff\"></TRACK>"];

    // -- When.
    NxATrack *track = [self.parser p_trackForRekordboxPlaylistPathEntryElement:entryElement];

    // -- Then.
    XCTAssertNil(track);

    assertThatNothingWasLogged();
}

- (void)testRootFolderSource_SampleCollection_ReturnsTheRootFolder
{
    // -- Given.
    NSXMLElement *rootElement = [NxATestCase xmlElementFromString:@"<DJ_PLAYLISTS Version=\"1.0.0\"> \
                                 <PRODUCT Name=\"rekordbox\" Version=\"3.1.0\" Company=\"Pioneer\"/> \
                                 </DJ_PLAYLISTS>"];
    NSXMLElement *collectionElement = [NxATestCase xmlElementFromString:@"<COLLECTION Entries=\"437\"> \
                                       </COLLECTION>"];
    NSXMLElement *trackElement = [NxATestCase xmlElementFromString:@"<TRACK></TRACK>"];
    NSXMLElement *playlistsElement = [NxATestCase xmlElementFromString:@"<PLAYLISTS></PLAYLISTS>"];
    NSXMLElement *topPlaylistElement = [NxATestCase xmlElementFromString:@"<NODE></NODE>"];
    [rootElement addChild:collectionElement];
    [rootElement addChild:playlistsElement];
    [collectionElement addChild:trackElement];
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
    NSXMLElement *rootElement = [NxATestCase xmlElementFromString:@"<DJ_PLAYLISTS Version=\"1.0.0\"> \
                                 <PRODUCT Name=\"rekordbox\" Version=\"3.1.0\" Company=\"Pioneer\"/> \
                                 </DJ_PLAYLISTS>"];
    NSXMLElement *collectionElement = [NxATestCase xmlElementFromString:@"<COLLECTION Entries=\"437\"> \
                                       </COLLECTION>"];
    NSXMLElement *trackElement = [NxATestCase xmlElementFromString:@"<TRACK></TRACK>"];
    NSXMLElement *playlistsElement = [NxATestCase xmlElementFromString:@"<PLAYLISTS></PLAYLISTS>"];
    NSXMLElement *topPlaylistElement = [NxATestCase xmlElementFromString:@"<NODE></NODE>"];
    [rootElement addChild:collectionElement];
    [rootElement addChild:playlistsElement];
    [collectionElement addChild:trackElement];
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
    NSXMLElement *source = [NxATestCase xmlElementFromString:@"<NODE Name=\"Playlist One\" Type=\"1\" KeyType=\"1\" Entries=\"0\"></NODE>"];

    // -- When.
    NSString *name = [self.parser nameOfPlaylistSource:source];

    // -- Then.
    XCTAssertEqualObjects(name, @"Playlist One");

    assertThatNothingWasLogged();
}

- (void)testNameOfPlaylistSource_PlaylistWithEmptyName_ReturnsEmptyString
{
    // -- Given.
    NSXMLElement *source = [NxATestCase xmlElementFromString:@"<NODE Name=\"\" Type=\"1\" KeyType=\"1\" Entries=\"0\"></NODE>"];

    // -- When.
    NSString *name = [self.parser nameOfPlaylistSource:source];

    // -- Then.
    XCTAssertEqualObjects(name, @"");

    assertThatNothingWasLogged();
}

- (void)testNameOfFolderEntrySource_FolderTypeEntry_ReturnsTheName
{
    // -- Given.
    NSXMLElement *source = [NxATestCase xmlElementFromString:@"<NODE Name=\"Folder\" Type=\"0\" Count=\"1\"></NODE>"];

    // -- When.
    NSString *name = [self.parser nameOfFolderEntrySource:source];

    // -- Then.
    XCTAssertEqualObjects(name, @"Folder");

    assertThatNothingWasLogged();
}

- (void)testNameOfFolderEntrySource_FolderTypeEntryWithEmptyName_ReturnsEmptyString
{
    // -- Given.
    NSXMLElement *source = [NxATestCase xmlElementFromString:@"<NODE Name=\"\" Type=\"0\" Count=\"1\"></NODE>"];

    // -- When.
    NSString *name = [self.parser nameOfFolderEntrySource:source];

    // -- Then.
    XCTAssertEqualObjects(name, @"");

    assertThatNothingWasLogged();
}

- (void)testNameOfFolderEntrySource_PlaylistTypeEntry_ReturnsTheName
{
    // -- Given.
    NSXMLElement *source = [NxATestCase xmlElementFromString:@"<NODE Name=\"Playlist\" Type=\"1\" KeyType=\"0\" Entries=\"2\"></NODE>"];

    // -- When.
    NSString *name = [self.parser nameOfFolderEntrySource:source];

    // -- Then.
    XCTAssertEqualObjects(name, @"Playlist");

    assertThatNothingWasLogged();
}

- (void)testNameOfFolderEntrySource_PlaylistTypeEntryWithEmptyName_ReturnsEmptyString
{
    // -- Given.
    NSXMLElement *source = [NxATestCase xmlElementFromString:@"<NODE Name=\"\" Type=\"1\" KeyType=\"0\" Entries=\"2\"></NODE>"];

    // -- When.
    NSString *name = [self.parser nameOfFolderEntrySource:source];

    // -- Then.
    XCTAssertEqualObjects(name, @"");

    assertThatNothingWasLogged();
}

- (void)testFolderEntrySourceIsAPlaylist_FolderTypeEntry_ReturnsFalse
{
    // -- Given.
    NSXMLElement *source = [NxATestCase xmlElementFromString:@"<NODE Name=\"Folder\" Type=\"0\" Count=\"1\"></NODE>"];

    // -- When.
    BOOL result = [self.parser folderEntrySourceIsAPlaylist:source];

    // -- Then.
    XCTAssertFalse(result);

    assertThatNothingWasLogged();
}

- (void)testFolderEntrySourceIsAPlaylist_PlaylistTypeEntry_ReturnsTrue
{
    // -- Given.
    NSXMLElement *source = [NxATestCase xmlElementFromString:@"<NODE Name=\"Playlist\" Type=\"1\" KeyType=\"0\" Entries=\"2\"></NODE>"];

    // -- When.
    BOOL result = [self.parser folderEntrySourceIsAPlaylist:source];

    // -- Then.
    XCTAssertTrue(result);

    assertThatNothingWasLogged();
}

- (void)testLocationPathForTrackSource_FileLocationOnLocalDrive_PathIsCorrectlyReturned
{
    // -- Given.
    NSXMLElement *trackElement = [NxATestCase xmlElementFromString:@"<TRACK Location=\"file://localhost/Users/didier/Music/Acapellas/04%20Ghetto%20Kraviz%20(VOX%20Stem%201).aiff\"></TRACK>"];

    // -- When.
    NSString *path = [self.parser locationPathForTrackSource:trackElement];

    // -- Then.
    XCTAssertEqualObjects(path, @"/Users/didier/Music/Acapellas/04 Ghetto Kraviz (VOX Stem 1).aiff");

    assertThatNothingWasLogged();
}

- (void)testLocationPathForTrackSource_FileLocationNotOnLocalDrive_PathIsCorrectlyReturned
{
    // -- Given.
    NSXMLElement *trackElement = [NxATestCase xmlElementFromString:@"<TRACK Location=\"file://Volumes/Test/Music/Acapellas/04%20Ghetto%20Kraviz%20(VOX%20Stem%201).aiff\"></TRACK>"];;

    // -- When.
    NSString *path = [self.parser locationPathForTrackSource:trackElement];

    // -- Then.
    XCTAssertEqualObjects(path, @"/Volumes/Test/Music/Acapellas/04 Ghetto Kraviz (VOX Stem 1).aiff");

    assertThatNothingWasLogged();
}

- (void)testLocationPathForTrackSource_FileLocationWithEscapedCharacters_EscapedCharactersAreCorrectlyConverted
{
    // -- Given.
    NSXMLElement *trackElement = [NxATestCase xmlElementFromString:@"<TRACK Location=\"file://Volumes/Test/Path%20!%22%23$%25&amp;'()*+,-./0123456789:;%3C=%3E?@ABCDEFGHIJKLMNOPQRSTUVWXYZ%5B%5C%5D%5E_%60abcdefghijklmnopqrstuvwxyz%7B%7C%7D~.MP3\"></TRACK>"];;

    // -- When.
    NSString *path = [self.parser locationPathForTrackSource:trackElement];

    // -- Then.
    XCTAssertEqualObjects(path, @"/Volumes/Test/Path !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~.MP3");

    assertThatNothingWasLogged();
}

- (void)testModificationDateForTrackSource_ParseATrackElement_ReturnsTheCorrectDate
{
    // -- Given.
    NSXMLElement *element = [NxATrackRekordboxParsingTests rekordboxTestSourceTrack1];
    self.parser.collectionModificationDate = [NSDate dateWithStringInLocalTimeZone:@"2014-07-23 07:00:00" format:@"yyyy-MM-dd HH:mm:ss"];

    // -- When.
    NSDate *result = [self.parser modificationDateForTrackSource:element];

    // -- Then.
    XCTAssertEqualObjects([result stringValueInLocalTimeZoneWithFormat:@"yyyy-MM-dd HH:mm:ss"], @"2014-07-23 07:00:00");

    assertThatNothingWasLogged();
}

- (void)testParseTrackSource_ParseTrackWithMissingProperties_ReturnsTrackWithContent
{
    // -- Given.
    NSXMLElement *element = [NxATrackRekordboxParsingTests rekordboxTestSourceTrack1];
    NxATrack *track = [NxATrackTests stubbedTrackWithPath:@"/Test/Path/File.MP3"
                                                  context:self.context];
    NSDate *trackModifiedDate = [NSDate dateWithStringInLocalTimeZone:@"2014-07-23 07:00:00" format:@"yyyy-MM-dd HH:mm:ss"];

    // -- When.
    NxAParsingResultFlags result = [self.parser parseTrackSource:element
                                              whichWasModifiedOn:trackModifiedDate
                                                        forTrack:track];

    // -- Then.
    XCTAssertEqual(result, NxAParsedObjectCausedUpdating);

    assertThatNothingWasLogged();
}

- (void)testParseTrackSource_ParseMoreRecentTrack_ReturnsMoreRecentContent
{
    // -- Given.
    NSXMLElement *element = [NxATrackRekordboxParsingTests rekordboxTestSourceTrack1];
    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2014-07-23 07:00:00" format:@"yyyy-MM-dd HH:mm:ss"];
    NxATrack *track = [NxATrackTests testTrack1InContext:self.context
                                                 keyName:NxAKeyPropertyRekordboxKeyName
                                        modificationDate:modificationDate];
    NSDate *trackModifiedDate = [NSDate dateWithStringInLocalTimeZone:@"2014-08-23 07:00:00" format:@"yyyy-MM-dd HH:mm:ss"];

    // -- When.
    NxAParsingResultFlags result = [self.parser parseTrackSource:element
                                              whichWasModifiedOn:trackModifiedDate
                                                        forTrack:track];

    // -- Then.
    XCTAssertEqual(result, NxAParsedObjectCausedUpdating);
    assertThatNothingWasLogged();
}

- (void)testParseTrackSource_ParseUnchangedTrack_ReturnsUnchangedContent
{
    // -- Given.
    NSXMLElement *element = [NxATrackRekordboxParsingTests rekordboxTestSourceTrack1];
    NSDate *trackModifiedDate = [NSDate dateWithStringInLocalTimeZone:@"2014-07-23 07:00:00" format:@"yyyy-MM-dd HH:mm:ss"];
    NxATrack *track = [NxATrackTests testTrack1InContext:self.context
                                                 keyName:NxAKeyPropertyRekordboxKeyName
                                        modificationDate:trackModifiedDate];

    // -- When.
    NxAParsingResultFlags result = [self.parser parseTrackSource:element
                                              whichWasModifiedOn:trackModifiedDate
                                                        forTrack:track];

    // -- Then.
    XCTAssertEqual(result, NxAParsedObjectWasUpToDate);

    assertThatNothingWasLogged();
}

- (void)testParseTrackSource_ParseOlderTrack_ReturnsUnchangedContentAndTrackNeedsUpdating
{
    // -- Given.
    NSXMLElement *element = [NxATrackRekordboxParsingTests rekordboxTestSourceTrack1];
    NSDate *trackModifiedDate = [NSDate dateWithStringInLocalTimeZone:@"2014-07-23 07:00:00" format:@"yyyy-MM-dd HH:mm:ss"];
    NSDate *laterDate = [NSDate dateWithStringInLocalTimeZone:@"2014-08-23 07:00:00" format:@"yyyy-MM-dd HH:mm:ss"];
    NxATrack *track = [NxATrackTests testTrack1InContext:self.context
                                                 keyName:NxAKeyPropertyRekordboxKeyName
                                        modificationDate:laterDate];

    // -- When.
    NxAParsingResultFlags result = [self.parser parseTrackSource:element
                                              whichWasModifiedOn:trackModifiedDate
                                                        forTrack:track];

    // -- Then.
    XCTAssertEqual(result, NxAParsedObjectNeedsUpdating);

    assertThatNothingWasLogged();
}

- (void)testParseTrackSource_ParsesATrack_ParsesAllFieldsCorrectly
{
    // -- Given.
    NSXMLElement *element = [NxATrackRekordboxParsingTests rekordboxTestSourceTrack2];
    NxATrack *track = [NxATrackTests stubbedTrackWithPath:@"/Test/Path/File.MP3"
                                                  context:self.context];
    NSDate *trackModifiedDate = [NSDate dateWithStringInLocalTimeZone:@"2014-07-23 07:00:00" format:@"yyyy-MM-dd HH:mm:ss"];

    // -- When.
    NxAParsingResultFlags result = [self.parser parseTrackSource:element
                                              whichWasModifiedOn:trackModifiedDate
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
    NSArray *keys = [NxATrackKey tagsIn:track withName:NxAKeyPropertyRekordboxKeyName];
    XCTAssertEqual(keys.count, 1);
    XCTAssertEqualObjects([keys[0] stringValueForNotation:NxAMusicalNotationStandard], @"Cm");
    
    assertThatNothingWasLogged();
}

- (void)testParsePlaylistEntrySource_ParseKnownIDEntry_ReturnsCorrectTrack
{
    // -- Given.
    NSXMLElement *playlistElement = [NxATestCase xmlElementFromString:@"<NODE Name=\"Playlist\" Type=\"1\" KeyType=\"0\" Entries=\"2\"> \
                                     </NODE>"];
    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-11-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    NxATrack *existingTrack1 = [NxATrackTests testTrack1InContext:self.context
                                                          keyName:NxAKeyPropertyRekordboxKeyName
                                                 modificationDate:modificationDate];
    NSXMLElement *playlistEntry = [NxATestCase xmlElementFromString:@"<TRACK Key=\"203\"/>"];
    [playlistElement addChild:playlistEntry];

    [self.parser p_trackForRekordboxTrackID:@"203" is:existingTrack1];

    // -- When.
    NxATrack *track = [self.parser trackForPlaylistEntrySource:playlistEntry];

    // -- Then.
    XCTAssertEqualObjects(track, existingTrack1);

    assertThatNothingWasLogged();
}

- (void)testParsePlaylistEntrySource_UnknownIDEntry_ReturnsNil
{
    // -- Given.
    NSXMLElement *playlistElement = [NxATestCase xmlElementFromString:@"<NODE Name=\"Playlist\" Type=\"1\" KeyType=\"0\" Entries=\"2\"> \
                                     </NODE>"];
    NSXMLElement *playlistEntry = [NxATestCase xmlElementFromString:@"<TRACK Key=\"203\"/>"];
    [playlistElement addChild:playlistEntry];

    // -- When.
    NxATrack *track = [self.parser trackForPlaylistEntrySource:playlistEntry];

    // -- Then.
    XCTAssertNil(track);

    assertThatNothingWasLogged();
}

- (void)testParsePlaylistEntrySource_ParseKnownPathEntry_ReturnsCorrectTrack
{
    // -- Given.
    NSXMLElement *playlistElement = [NxATestCase xmlElementFromString:@"<NODE Name=\"Playlist\" Type=\"1\" KeyType=\"1\" Entries=\"2\"> \
                                     </NODE>"];
    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-11-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    NxATrack *existingTrack1 = [NxATrackTests testTrack1InContext:self.context
                                                          keyName:NxAKeyPropertyRekordboxKeyName
                                                 modificationDate:modificationDate];
    NSXMLElement *playlistEntry = [NxATestCase xmlElementFromString:@"<TRACK Key=\"file://localhost/Users/didier/Music/Gigs/25%20I%27m%20In%20Love%20(feat.%20Donae%27O)%20%5bDeep%20Mix%5d.m4a\"></TRACK>"];
    [self.parser p_trackForRekordboxFileLocation:@"file://localhost/Users/didier/Music/Gigs/25%20I%27m%20In%20Love%20(feat.%20Donae%27O)%20%5bDeep%20Mix%5d.m4a" is:existingTrack1];
    [playlistElement addChild:playlistEntry];

    // -- When.
    NxATrack *track = [self.parser trackForPlaylistEntrySource:playlistEntry];

    // -- Then.
    XCTAssertEqualObjects(track, existingTrack1);

    assertThatNothingWasLogged();
}

- (void)testParsePlaylistEntrySource_ParseUnknownPathEntry_ReturnsNil
{
    // -- Given.
    NSXMLElement *playlistElement = [NxATestCase xmlElementFromString:@"<NODE Name=\"Playlist\" Type=\"1\" KeyType=\"0\" Entries=\"2\"> \
                                     </NODE>"];
    NSXMLElement *playlistEntry = [NxATestCase xmlElementFromString:@"<TRACK Key=\"file://localhost/Users/didier/Music/Gigs/25%20I%27m%20In%20Love%20(feat.%20Donae%27O)%20%5bDeep%20Mix%5d.m4a\"></TRACK>"];
    [playlistElement addChild:playlistEntry];

    // -- When.
    NxATrack *track = [self.parser trackForPlaylistEntrySource:playlistEntry];

    // -- Then.
    XCTAssertNil(track);

    assertThatNothingWasLogged();
}

- (void)testChildrenOfCollectionSource_CollectionWithTracks_ReturnsTheTracks
{
    // -- Given.
    NSXMLElement *collectionElement = [NxATestCase xmlElementFromString:@"<COLLECTION Entries=\"437\"> \
                                       </COLLECTION>"];
    NSXMLElement *existingRekordboxTrack1 = [NxATrackRekordboxParsingTests rekordboxTestSourceTrack1];
    NSXMLElement *existingRekordboxTrack2 = [NxATrackRekordboxParsingTests rekordboxTestSourceTrack2];
    [collectionElement addChild:existingRekordboxTrack1];
    [collectionElement addChild:existingRekordboxTrack2];

    // -- When.
    NSArray *children = [self.parser childrenOfCollectionSource:collectionElement];

    // -- Then.
    XCTAssertEqual(children.count, 2);
    XCTAssertEqualObjects(children[0], existingRekordboxTrack1);
    XCTAssertEqualObjects(children[1], existingRekordboxTrack2);

    assertThatNothingWasLogged();
}

- (void)testChildrenOfPlaylistSource_PlaylistWithChildren_ReturnsTheChildren
{
    // -- Given.
    NSXMLElement *playlistChild1 = [NxATestCase xmlElementFromString:@"<TRACK Key=\"103\"/>"];
    NSXMLElement *playlistChild2 = [NxATestCase xmlElementFromString:@"<TRACK Key=\"203\"/>"];
    NSXMLElement *playlistElement = [NxATestCase xmlElementFromString:@"<NODE Name=\"Playlist\" Type=\"1\" KeyType=\"0\" Entries=\"2\"> \
                                      </NODE>"];
    [playlistElement addChild:playlistChild1];
    [playlistElement addChild:playlistChild2];

    // -- When.
    NSArray *children = [self.parser childrenOfPlaylistSource:playlistElement];

    // -- Then.
    XCTAssertEqual(children.count, 2);
    XCTAssertEqualObjects(children[0], playlistChild1);
    XCTAssertEqualObjects(children[1], playlistChild2);

    assertThatNothingWasLogged();
}

- (void)testChildrenOfFolderSource_FolderWithChildren_ReturnsTheChildren
{
    // -- Given.
    NSXMLElement *playlist1Element = [NxATestCase xmlElementFromString:@"<NODE Name=\"Playlist\" Type=\"1\" KeyType=\"0\" Entries=\"2\"> \
                                      <TRACK Key=\"103\"/> \
                                      </NODE>"];
    NSXMLElement *playlist2Element = [NxATestCase xmlElementFromString:@"<NODE Name=\"Playlist\" Type=\"1\" KeyType=\"0\" Entries=\"2\"> \
                                      <TRACK Key=\"203\"/> \
                                      </NODE>"];
    NSXMLElement *folderElement = [NxATestCase xmlElementFromString:@"<NODE Name=\"Folder\" Type=\"0\" Count=\"1\"></NODE>"];
    [folderElement addChild:playlist1Element];
    [folderElement addChild:playlist2Element];

    // -- When.
    NSArray *children = [self.parser childrenOfFolderSource:folderElement];

    // -- Then.
    XCTAssertEqual(children.count, 2);
    XCTAssertEqualObjects(children[0], playlist1Element);
    XCTAssertEqualObjects(children[1], playlist2Element);

    assertThatNothingWasLogged();
}

- (void)testRemoveAndReturnValidChildrenFromCollectionSource_CollectionElementWithChildren_ReturnsTheChildrenAndDetachesThemFromTheParent
{
    // -- Given.
    NSXMLElement *collectionElement = [NxATestCase xmlElementFromString:@"<COLLECTION Entries=\"437\"> \
                                       </COLLECTION>"];
    NSXMLElement *existingRekordboxTrack1 = [NxATrackRekordboxParsingTests rekordboxTestSourceTrack1];
    NSXMLElement *existingRekordboxTrack2 = [NxATrackRekordboxParsingTests rekordboxTestSourceTrack2];
    [collectionElement addChild:existingRekordboxTrack1];
    [collectionElement addChild:existingRekordboxTrack2];

    // -- When.
    NSSet *children = [self.parser removeAndReturnValidChildrenFromCollectionSource:collectionElement];

    // -- Then.
    XCTAssertEqual(children.count, 2);
    XCTAssertTrue([children containsObject:existingRekordboxTrack1]);
    XCTAssertTrue([children containsObject:existingRekordboxTrack2]);
    XCTAssertNil(existingRekordboxTrack1.parent);
    XCTAssertNil(existingRekordboxTrack2.parent);
    XCTAssertEqual(collectionElement.children.count, 0);

    assertThatNothingWasLogged();
}

- (void)testRemoveAndReturnValidChildrenFromPlaylistSource_PlaylistElementWithChildren_ReturnsTheChildrenAndDetachesThemFromTheParent
{
    // -- Given.
    NSXMLElement *playlistElement = [NxATestCase xmlElementFromString:@"<NODE Name=\"Test Playlist\" Type=\"1\" KeyType=\"0\" Entries=\"2\"> \
                                                                        </NODE>"];
    NSXMLElement *entryElement1 = [NxATestCase xmlElementFromString:@"<TRACK Key=\"3253\"/>"];
    NSXMLElement *entryElement2 = [NxATestCase xmlElementFromString:@"<TRACK Key=\"2342\"/>"];
    [playlistElement addChild:entryElement1];
    [playlistElement addChild:entryElement2];

    // -- When.
    NSSet *children = [self.parser removeAndReturnValidChildrenFromPlaylistSource:playlistElement];

    // -- Then.
    XCTAssertEqual(children.count, 2);
    XCTAssertTrue([children containsObject:entryElement1]);
    XCTAssertTrue([children containsObject:entryElement2]);
    XCTAssertNil(entryElement1.parent);
    XCTAssertNil(entryElement2.parent);
    XCTAssertEqual(playlistElement.children.count, 0);

    assertThatNothingWasLogged();
}

- (void)testRemoveAndReturnValidChildrenFromFolderSource_FolderElementWithChildren_ReturnsTheChildrenAndDetachesThemFromTheParent
{
    // -- Given.
    NSXMLElement *folderElement = [NxATestCase xmlElementFromString:@"<NODE Name=\"Test Playlist\" Type=\"0\" Count=\"2\"> \
                                   </NODE>"];
    NSXMLElement *entryElement1 = [NxATestCase xmlElementFromString:@"<NODE Name=\"Test Playlist 1\" Type=\"1\" KeyType=\"0\" Entries=\"0\"> \
                                   </NODE>"];
    NSXMLElement *entryElement2 = [NxATestCase xmlElementFromString:@"<NODE Name=\"Test Playlist 2\" Type=\"1\" KeyType=\"0\" Entries=\"0\"> \
                                   </NODE>"];
    [folderElement addChild:entryElement1];
    [folderElement addChild:entryElement2];

    // -- When.
    NSSet *children = [self.parser removeAndReturnValidChildrenFromFolderSource:folderElement];

    // -- Then.
    XCTAssertEqual(children.count, 2);
    XCTAssertTrue([children containsObject:entryElement1]);
    XCTAssertTrue([children containsObject:entryElement2]);
    XCTAssertNil(entryElement1.parent);
    XCTAssertNil(entryElement2.parent);
    XCTAssertEqual(folderElement.children.count, 0);

    assertThatNothingWasLogged();
}

- (void)testnumberOfChildrenLeftInCollectionSource_CollectionElementWithIncorrectEntryCount_ReturnsTheCorrectChildrenCount
{
    // -- Given.
    NSXMLElement *collectionElement = [NxATestCase xmlElementFromString:@"<COLLECTION Entries=\"437\"> \
                                       </COLLECTION>"];
    NSXMLElement *existingRekordboxTrack1 = [NxATrackRekordboxParsingTests rekordboxTestSourceTrack1];
    NSXMLElement *existingRekordboxTrack2 = [NxATrackRekordboxParsingTests rekordboxTestSourceTrack2];
    [collectionElement addChild:existingRekordboxTrack1];
    [collectionElement addChild:existingRekordboxTrack2];

    // -- When.
    NSUInteger count = [self.parser numberOfChildrenLeftInCollectionSource:collectionElement];

    // -- Then.
    XCTAssertEqual(count, 2);

    assertThatNothingWasLogged();
}

- (void)testnumberOfChildrenLeftInFolderSource_FolderElementWithIncorrectEntryCount_ReturnsTheCorrectChildrenCount
{
    // -- Given.
    NSXMLElement *folderElement = [NxATestCase xmlElementFromString:@"<NODE Name=\"Test Playlist\" Type=\"0\" Count=\"223\"> \
                                   </NODE>"];
    NSXMLElement *entryElement1 = [NxATestCase xmlElementFromString:@"<NODE Name=\"Test Playlist 1\" Type=\"1\" KeyType=\"0\" Entries=\"0\"> \
                                   </NODE>"];
    NSXMLElement *entryElement2 = [NxATestCase xmlElementFromString:@"<NODE Name=\"Test Playlist 2\" Type=\"1\" KeyType=\"0\" Entries=\"0\"> \
                                   </NODE>"];
    [folderElement addChild:entryElement1];
    [folderElement addChild:entryElement2];

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
    NxATrack *track = [NxATrackTests testTrack1InContext:self.context
                                                 keyName:NxAKeyPropertyRekordboxKeyName
                                        modificationDate:modificationDate];
    NSXMLElement *trackElement = [NxATestCase xmlElementFromString:@"<TRACK></TRACK>"];

    // -- When.
    [self.parser updateTrackSource:trackElement forTrack:track];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<TRACK Name=\"I'm In Love (Deep Mix)\" Artist=\"Artful, Donae'O\" \
                                    Composer=\"My Digital Enemy, Pleasurekraft\" Remixer=\"Third Party, Rick James\" Genre=\"House, Progressive House\" \
                                    Comments=\"Some snazzy comments!\" Label=\"Size Records\" Mix=\"Original Mix\" PlayCount=\"142\" TrackNumber=\"25\" \
                                    Album=\"Defected Presents Dimitri from Paris In the House of Disco\" Rating=\"153\" DateAdded=\"2014-06-17\" Year=\"2012\" \
                                    Tonality=\"Fm\" AverageBpm=\"112.67\"> \
                                    <TEMPO Inizio=\"0.068\" Bpm=\"124.00\" Metro=\"4/4\" Battito=\"1\"></TEMPO> \
                                    <POSITION_MARK Name=\"Test2\" Start=\"0.068\" Type=\"0\" Num=\"0\"></POSITION_MARK> \
                                    <POSITION_MARK Name=\"Test3\" Start=\"31.036\" Type=\"0\" Num=\"1\"></POSITION_MARK> \
                                    <TEMPO Inizio=\"50.068\" Bpm=\"112.00\" Metro=\"4/4\" Battito=\"1\"></TEMPO> \
                                    <POSITION_MARK Name=\"Test5\" Start=\"62.004\" Type=\"0\" Num=\"-1\"></POSITION_MARK> \
                                    <TEMPO Inizio=\"200.068\" Bpm=\"102.00\" Metro=\"4/4\" Battito=\"1\"></TEMPO> \
                                    <POSITION_MARK Name=\"Test7\" Start=\"371.681\" Type=\"0\" Num=\"-1\"></POSITION_MARK> \
                                    </TRACK>"];
    XCTAssertEqualObjects(trackElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testUpdateTrackSource_UpdatesAnEmptyTrackFromAnEmptyTrack_UpdatesAllFieldsCorrectly
{
    // -- Given.
    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2014-07-23 07:00:00" format:@"yyyy-MM-dd HH:mm:ss"];
    NxATrack *track = [NxATrackTests emptyTestTrackInContext:self.context
                                                usingKeyName:NxAKeyPropertyRekordboxKeyName
                                              lastModifiedOn:modificationDate];
    NSXMLElement *trackElement = [NxATestCase xmlElementFromString:@"<TRACK></TRACK>"];

    // -- When.
    [self.parser updateTrackSource:trackElement forTrack:track];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<TRACK Name=\"\" Artist=\"\" Composer=\"\" Remixer=\"\" \
                                    Genre=\"\" Comments=\"\" Label=\"\" Mix=\"\" PlayCount=\"\" TrackNumber=\"\" Album=\"\" \
                                    Rating=\"\" DateAdded=\"\" Year=\"\"></TRACK>"];
    XCTAssertEqualObjects(trackElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testUpdateTrackSource_UpdatesAnExistingTrack_UpdatesAllFieldsCorrectly
{
    // -- Given.
    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2014-07-23 07:00:00" format:@"yyyy-MM-dd HH:mm:ss"];
    NxATrack *track = [NxATrackTests testTrack3InContext:self.context
                                                 keyName:NxAKeyPropertyRekordboxKeyName
                                        modificationDate:modificationDate];
    NSXMLElement *trackElement = [NxATrackRekordboxParsingTests rekordboxTestSourceTrack1];
    [trackElement addAttributeWithName:@"MYTestAttribute" stringValue:@"Test Value"];
    NSXMLElement *testChildElement = [NxATestCase xmlElementFromString:@"<MY_OTHER_TEST VALUE=\"TEST\"></MY_OTHER_TEST>"];
    [trackElement addChild:testChildElement];

    // -- When.
    [self.parser updateTrackSource:trackElement forTrack:track];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<TRACK TrackID=\"21\" Name=\"Terra (Original Mix)\" Artist=\"Manuel De La Mare, Luigi Rocca\" \
                                    Composer=\"\" Remixer=\"\" Album=\"Terra\" Grouping=\"\" Genre=\"Tech House\" Kind=\"M4A File\" Size=\"15028224\" TotalTime=\"439\" \
                                    DiscNumber=\"0\" TrackNumber=\"1\" Year=\"2013\" AverageBpm=\"124.00\" DateAdded=\"2013-11-28\" BitRate=\"256\" SampleRate=\"44100\" \
                                    Comments=\"\" PlayCount=\"0\" Rating=\"0\" \
                                    Location=\"file://localhost/Users/didier/Music/Gigs/25%20I%27m%20In%20Love%20(feat.%20Donae%27O)%20%5bDeep%20Mix%5d.m4a\" \
                                    Tonality=\"Dm\" Label=\"303Lovers\" Mix=\"\" \
                                    MYTestAttribute=\"Test Value\"> \
                                    <MY_OTHER_TEST VALUE=\"TEST\"></MY_OTHER_TEST> \
                                    <TEMPO Inizio=\"0.480\" Bpm=\"124.00\" Metro=\"4/4\" Battito=\"1\"></TEMPO> \
                                    <POSITION_MARK Name=\"ThirdTest2\" Start=\"0.480\" Type=\"3\" Num=\"0\"></POSITION_MARK> \
                                    <POSITION_MARK Name=\"ThirdTest3\" Start=\"31.448\" Type=\"0\" Num=\"1\"></POSITION_MARK> \
                                    <POSITION_MARK Name=\"ThirdTest4\" Start=\"62.415\" Type=\"0\" Num=\"2\"></POSITION_MARK> \
                                    <POSITION_MARK Name=\"ThirdTest5\" Start=\"93.383\" Type=\"1\" Num=\"-1\"></POSITION_MARK> \
                                    <POSITION_MARK Name=\"ThirdTest6\" Start=\"341.125\" Type=\"2\" Num=\"-1\"></POSITION_MARK> \
                                    </TRACK>"];
    XCTAssertEqualObjects(trackElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testEmptyTrackSourceFor_FileOnBootVolumeUnknownID_FileLocationIsCorrectAndIDIdIsNew
{
    // -- Given.
    NxATrack *stubbedTrack = [NxATrackTests stubbedTrackWithPath:@"/Test/Path/File.MP3" context:self.context];
    self.parser.self.p_lastRekordboxTrackID = 23;

    // -- When.
    NSXMLElement *trackElement = [self.parser emptyTrackSourceFor:stubbedTrack];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<TRACK TrackID=\"23\" Location=\"file://localhost/Test/Path/File.MP3\"></TRACK>"];
    XCTAssertEqualObjects(trackElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testEmptyTrackSourceFor_FileOnBootVolumeKnownID_FileLocationIsCorrectAndCorrectID
{
    // -- Given.
    NxATrack *stubbedTrack1 = [NxATrackTests stubbedTrackWithPath:@"/Test/Path/File.MP3" context:self.context];
    NxATrack *stubbedTrack2 = [NxATrackTests stubbedTrackWithPath:@"/Test/Path/File2.MP3" context:self.context];
    self.parser.p_tracksByRekordboxTrackID[@"204"] = stubbedTrack1;
    self.parser.p_tracksByRekordboxTrackID[@"4"] = stubbedTrack2;
    self.parser.self.p_lastRekordboxTrackID = 23;

    // -- When.
    NSXMLElement *trackElement = [self.parser emptyTrackSourceFor:stubbedTrack1];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<TRACK TrackID=\"204\" Location=\"file://localhost/Test/Path/File.MP3\"></TRACK>"];
    XCTAssertEqualObjects(trackElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testEmptyTrackSourceFor_FileNotOnBootVolumeUnknownID_FileLocationIsCorrectAndIDIdIsNew
{
    // -- Given.
    NSString *externalVolumeName = [[NxAOSXAudioFile otherVolumeNames] anyObject];
    NSString *path = [NSString stringWithFormat:@"/Volumes/%@/Test/Path/File.MP3", externalVolumeName];
    NxATrack *stubbedTrack = [NxATrackTests stubbedTrackWithPath:path context:self.context];
    self.parser.self.p_lastRekordboxTrackID = 23;

    // -- When.
    NSXMLElement *trackElement = [self.parser emptyTrackSourceFor:stubbedTrack];

    // -- Then.
    NSString *expectedLocation = [[NSString stringWithFormat:@"file://localhost/Volumes/%@/Test/Path/File.MP3", externalVolumeName] stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:[NSString stringWithFormat:@"<TRACK TrackID=\"23\" Location=\"%@\"></TRACK>", expectedLocation]];
    XCTAssertEqualObjects(trackElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testEmptyTrackSourceFor_FilePathWithCharactersToEscapedUnknownID_FileLocationIsCorrectAndIDIdIsNew
{
    // -- Given.
    NSString *path = @"/Test/Path !\"#$\%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~.MP3";
    NxATrack *stubbedTrack = [NxATrackTests stubbedTrackWithPath:path context:self.context];
    self.parser.self.p_lastRekordboxTrackID = 23;

    // -- When.
    NSXMLElement *trackElement = [self.parser emptyTrackSourceFor:stubbedTrack];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<TRACK TrackID=\"23\" Location=\"file://localhost/Test/Path%20!%22%23$%25&amp;'()*+,-./0123456789:;%3C=%3E?@ABCDEFGHIJKLMNOPQRSTUVWXYZ%5B%5C%5D%5E_%60abcdefghijklmnopqrstuvwxyz%7B%7C%7D~.MP3\"></TRACK>"];
    XCTAssertEqualObjects(trackElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testEmptyPlaylistSourceFor_FolderWithAName_ReturnsCorrectFolderElement
{
    // -- Given.
    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2012-06-02 04:54:23" format:@"yyyy-MM-dd HH:mm:ss"];
    NxATrack *track1 = [NxATrackTests testTrack1InContext:self.context
                                                  keyName:NxAKeyPropertyRekordboxKeyName
                                         modificationDate:modificationDate];
    NxATrack *track2 = [NxATrackTests testTrack2InContext:self.context
                                                  keyName:NxAKeyPropertyRekordboxKeyName
                                         modificationDate:modificationDate];
    NxACrate *crate = [self playlistWithName:@"Test Playlist"
              withEmptyContentLastModifiedOn:modificationDate];
    [crate addTrack:track1 modificationDate:modificationDate];
    [crate addTrack:track2 modificationDate:modificationDate];

    // -- When.
    NSXMLElement *playlistElement = [self.parser emptyPlaylistSourceFor:crate];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<NODE Name=\"Test Playlist\" Type=\"1\" KeyType=\"0\" Entries=\"0\"> \
                                    </NODE>"];
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
    NxACrate *folder = [self folderWithName:@"Test Folder 2"
             withEmptyContentLastModifiedOn:modificationDate];
    [crate addCrate:playlist modificationDate:modificationDate];
    [crate addCrate:folder modificationDate:modificationDate];

    // -- When.
    NSXMLElement *folderElement = [self.parser emptyFolderSourceFor:crate];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<NODE Name=\"Test Folder\" Type=\"0\" Count=\"0\"> \
                                    </NODE>"];
    XCTAssertEqualObjects(folderElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testPlaylistEntrySourceFor_KnownTrack_ReturnsCorrectElement
{
    // -- Given.
    NxATrack *stubbedTrack = [NxATrackTests stubbedTrackWithPath:@"/Test/Path/File.MP3" context:self.context];
    self.parser.p_tracksByRekordboxTrackID[@"300"] = stubbedTrack;

    // -- When.
    NSXMLElement *playlistEntry = [self.parser playlistEntrySourceFor:stubbedTrack];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<TRACK Key=\"300\"></TRACK>"];
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
    XCTAssertTrue([[values1.firstObject p_description] hasPrefix:@"in a Rekordbox playlist with an invalid track"]);
    NSArray *values2 = argument2.allValues;
    XCTAssertEqual(values2.count, 1);
    XCTAssertEqualObjects(values2.firstObject, @"<Track using file '/Test/Path/File.MP3'>");
}

- (void)testAddTrackSource_TrackAndCollection_AddTheTrackCorrectly
{
    // -- Given.
    NSXMLElement *collectionElement = [NxATestCase xmlElementFromString:@"<COLLECTION Entries=\"437\"> \
                                       </COLLECTION>"];
    NSXMLElement *trackElement = [NxATestCase xmlElementFromString:@"<TRACK Mix=\"Test Mix\"></TRACK>"];

    // -- When.
    [self.parser addTrackSource:trackElement asChildOf:collectionElement];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<COLLECTION Entries=\"437\"> \
                                    <TRACK Mix=\"Test Mix\"></TRACK> \
                                    </COLLECTION>"];
    XCTAssertEqualObjects(collectionElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testAddPlaylistEntrySource_TrackEntryAndPlaylist_AddTheTrackEntryCorrectly
{
    // -- Given.
    NSXMLElement *playlistElement = [NxATestCase xmlElementFromString:@"<NODE Name=\"\" Type=\"1\" KeyType=\"0\" Entries=\"2\"></NODE>"];
    NSXMLElement *trackEntryElement = [NxATestCase xmlElementFromString:@"<TRACK Key=\"300\"></TRACK>"];

    // -- When.
    [self.parser addPlaylistEntrySource:trackEntryElement asChildOf:playlistElement];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<NODE Name=\"\" Type=\"1\" KeyType=\"0\" Entries=\"2\"> \
                                    <TRACK Key=\"300\"></TRACK> \
                                    </NODE>"];
    XCTAssertEqualObjects(playlistElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testAddFolderEntrySource_PlaylistAndFolder_AddThePlaylistCorrectly
{
    // -- Given.
    NSXMLElement *folderElement = [NxATestCase xmlElementFromString:@"<NODE Name=\"Folder\" Type=\"0\" Count=\"2\"></NODE>"];
    NSXMLElement *playlistElement = [NxATestCase xmlElementFromString:@"<NODE Name=\"\" Type=\"1\" KeyType=\"0\" Entries=\"2\"></NODE>"];

    // -- When.
    [self.parser addFolderEntrySource:playlistElement asChildOf:folderElement];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<NODE Name=\"Folder\" Type=\"0\" Count=\"2\"> \
                                    <NODE Name=\"\" Type=\"1\" KeyType=\"0\" Entries=\"2\"></NODE> \
                                    </NODE>"];
    XCTAssertEqualObjects(folderElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testSetEntriesCountForCollectionSource_CollectionWith437Entries_ChangesTheEntryCountCorrectly
{
    // -- Given.
    NSXMLElement *collectionElement = [NxATestCase xmlElementFromString:@"<COLLECTION Entries=\"437\"> \
                                       <TRACK Mix=\"Test Mix\"></TRACK> \
                                       </COLLECTION>"];

    // -- When.
    [self.parser setEntriesCountForCollectionSource:collectionElement to:@(23)];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<COLLECTION Entries=\"23\"> \
                                    <TRACK Mix=\"Test Mix\"></TRACK> \
                                    </COLLECTION>"];
    XCTAssertEqualObjects(collectionElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testSetEntriesCountForPlaylistSource_TrackEntryAndPlaylist_ChangesTheEntryCountCorrectly
{
    // -- Given.
    NSXMLElement *playlistElement = [NxATestCase xmlElementFromString:@"<NODE Name=\"\" Type=\"1\" KeyType=\"0\" Entries=\"2\"> \
                                     <TRACK Key=\"300\"></TRACK> \
                                     </NODE>"];

    // -- When.
    [self.parser setEntriesCountForPlaylistSource:playlistElement to:@(23)];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<NODE Name=\"\" Type=\"1\" KeyType=\"0\" Entries=\"23\"> \
                                    <TRACK Key=\"300\"></TRACK> \
                                    </NODE>"];
    XCTAssertEqualObjects(playlistElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testSetEntriesCountForFolderSource_PlaylistAndFolder_ChangesTheEntryCountCorrectly
{
    // -- Given.
    NSXMLElement *folderElement = [NxATestCase xmlElementFromString:@"<NODE Name=\"Folder\" Type=\"0\" Count=\"2\"> \
                                   <NODE Name=\"\" Type=\"1\" KeyType=\"0\" Entries=\"2\"></NODE> \
                                   </NODE>"];

    // -- When.
    [self.parser setEntriesCountForFolderSource:folderElement to:@(23)];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<NODE Name=\"Folder\" Type=\"0\" Count=\"23\"> \
                                    <NODE Name=\"\" Type=\"1\" KeyType=\"0\" Entries=\"2\"></NODE> \
                                    </NODE>"];
    XCTAssertEqualObjects(folderElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testDeleteCollectionEntrySources_CollectionWithChildren_RemovesTheChildren
{
    // -- Given.
    NSXMLElement *collectionElement = [NxATestCase xmlElementFromString:@"<COLLECTION Entries=\"437\"> \
                                       </COLLECTION>"];
    NSXMLElement *trackElement = [NxATestCase xmlElementFromString:@"<TRACK Mix=\"Test Mix\"></TRACK>"];
    [collectionElement addChild:trackElement];

    // -- When.
    [self.parser deleteCollectionEntrySources:[NSSet setWithObject:trackElement]];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<COLLECTION Entries=\"437\"> \
                                    </COLLECTION>"];
    XCTAssertEqualObjects(collectionElement, expectedResult);
    XCTAssertNil(trackElement.parent);

    assertThatNothingWasLogged();
}

- (void)testDeletePlaylistEntrySources_PlaylistWithChildren_RemovesTheChildren
{
    // -- Given.
    NSXMLElement *playlistElement = [NxATestCase xmlElementFromString:@"<NODE Name=\"\" Type=\"1\" KeyType=\"0\" Entries=\"2\"> \
                                     </NODE>"];
    NSXMLElement *playlistEntryElement = [NxATestCase xmlElementFromString:@"<TRACK Key=\"300\"></TRACK>"];
    [playlistElement addChild:playlistEntryElement];

    // -- When.
    [self.parser deletePlaylistEntrySources:[NSSet setWithObject:playlistEntryElement]];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<NODE Name=\"\" Type=\"1\" KeyType=\"0\" Entries=\"2\"> \
                                    </NODE>"];
    XCTAssertEqualObjects(playlistElement, expectedResult);
    XCTAssertNil(playlistEntryElement.parent);

    assertThatNothingWasLogged();
}

- (void)testDeleteFolderEntrySources_FolderWithChildren_RemovesTheChildren
{
    // -- Given.
    NSXMLElement *folderElement = [NxATestCase xmlElementFromString:@"<NODE Name=\"Folder\" Type=\"0\" Count=\"2\"> \
                                   </NODE>"];
    NSXMLElement *playlistElement = [NxATestCase xmlElementFromString:@"<NODE Name=\"\" Type=\"1\" KeyType=\"0\" Entries=\"2\"> \
                                     </NODE>"];
    [folderElement addChild:playlistElement];

    // -- When.
    [self.parser deleteFolderEntrySources:[NSSet setWithObject:playlistElement]];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<NODE Name=\"Folder\" Type=\"0\" Count=\"2\"> \
                                    </NODE>"];
    XCTAssertEqualObjects(folderElement, expectedResult);
    XCTAssertNil(playlistElement.parent);

    assertThatNothingWasLogged();
}

@end
