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

#import "NxATrackRekordboxParsingTests.h"
#import "NxATrack+RekordboxParsing.h"
#import "Track Collection/Parsers/Internal/NxARekordboxParser.h"
#import "NSDate+NxAUtility.h"
#import "NxAMarkerPropertyTests.h"
#import "Track Collection/Tags/Marker Properties/Internal/NxAGridMarkerProperty.h"
#import "Track Collection/Tags/Marker Properties/Internal/NxALoopMarkerProperty.h"
#import "NxAParsedMarkerValue.h"

#pragma mark Private Interface

@interface NxATrackRekordboxParsingTests ()

@property (strong, nonatomic) id<NxAParserImplementation> mockParser;

@end

#pragma mark - Implementation

@implementation NxATrackRekordboxParsingTests

#pragma mark Class Methods

+ (NSXMLElement *)rekordboxTestSourceTrack1
{
    // -- The metadata for this track should match the one returns by [NxATrackTests testTrack1InContext:modificationDate]
    NSXMLElement *sourceTrack1 = [NxATestCase xmlElementFromString:@"<TRACK TrackID=\"21\" Name=\"I&apos;m In Love (Deep Mix)\" Artist=\"Artful, Donae&apos;O\" \
                                  Composer=\"My Digital Enemy, Pleasurekraft\" Remixer=\"Third Party, Rick James\" \
                                  Album=\"Defected Presents Dimitri from Paris In the House of Disco\" \
                                  Grouping=\"\" Genre=\"House, Progressive House\" Kind=\"M4A File\" Size=\"15028224\" TotalTime=\"439\" \
                                  DiscNumber=\"0\" TrackNumber=\"25\" Year=\"2012\" AverageBpm=\"112.67\" \
                                  DateAdded=\"2014-06-17\" BitRate=\"256\" SampleRate=\"44100\" Comments=\"Some snazzy comments!\" \
                                  PlayCount=\"142\" Rating=\"153\" Location=\"file://localhost/Users/didier/Music/Gigs/25%20I%27m%20In%20Love%20(feat.%20Donae%27O)%20%5bDeep%20Mix%5d.m4a\" \
                                  Tonality=\"Fm\" Label=\"Size Records\" Mix=\"Original Mix\"> \
                                  <TEMPO Inizio=\"0.068\" Bpm=\"124.00\" Metro=\"4/4\" Battito=\"1\"/> \
                                  <POSITION_MARK Name=\"\" Type=\"0\" Start=\"0.068\" Num=\"0\"/> \
                                  <POSITION_MARK Name=\"\" Type=\"0\" Start=\"31.036\" Num=\"1\"/> \
                                  <TEMPO Inizio=\"50.068\" Bpm=\"112.00\" Metro=\"4/4\" Battito=\"1\"/> \
                                  <POSITION_MARK Name=\"\" Type=\"0\" Start=\"62.004\" Num=\"-1\"/> \
                                  <TEMPO Inizio=\"200.068\" Bpm=\"102.00\" Metro=\"4/4\" Battito=\"1\"/> \
                                  <POSITION_MARK Name=\"\" Type=\"0\" Start=\"371.681\" Num=\"-1\"/> \
                                  </TRACK>"];
    return sourceTrack1;
}

+ (NSXMLElement *)rekordboxTestSourceTrack2
{
    // -- The metadata for this track should match the one returns by [NxATrackTests testTrack2InContext:modificationDate]
    NSXMLElement *sourceTrack2 = [NxATestCase xmlElementFromString:@"<TRACK TrackID=\"23\" Name=\"Need U (100%) (Original Mix)\" Artist=\"Duke Dumont, A*M*E\" \
                                  Composer=\"Steve Angello, Roger Sanchez\" Remixer=\"Arty, Bingo Players\" \
                                  Album=\"Need U (100%) [feat. A*M*E]\" Grouping=\"\" Genre=\"Deep House, Tech House\" \
                                  Kind=\"AIFF File\" Size=\"41283584\" TotalTime=\"233\" DiscNumber=\"0\" \
                                  TrackNumber=\"2\" Year=\"2014\" AverageBpm=\"124.00\" DateAdded=\"2014-09-10\" \
                                  BitRate=\"1411\" SampleRate=\"44100\" Comments=\"Some even better comments.\" PlayCount=\"4243\" Rating=\"255\" \
                                  Location=\"file://localhost/Users/didier/Music/Gigs/Need%20U%20(100%25)%20feat.%20A*M*E%20(Original%20Mix)%20-%20A*M*E,%20Duke%20Dumont.aiff\" \
                                  Tonality=\"Cm\" Label=\"Win Records/Downtown Records\" Mix=\"Club Mix\"> \
                                  <TEMPO Inizio=\"0.009\" Bpm=\"120.00\" Metro=\"4/4\" Battito=\"1\"/> \
                                  <POSITION_MARK Name=\"\" Type=\"0\" Start=\"0.208\" Num=\"0\"/> \
                                  <POSITION_MARK Name=\"\" Type=\"0\" Start=\"31.428\" Num=\"1\"/> \
                                  <POSITION_MARK Name=\"\" Type=\"0\" Start=\"62.647\" Num=\"-1\"/> \
                                  <POSITION_MARK Name=\"\" Type=\"0\" Start=\"406.062\" Num=\"-1\"/> \
                                  </TRACK>"];
    return sourceTrack2;
}

+ (NSXMLElement *)rekordboxTestSourceTrack3
{
    // -- The metadata for this track should match the one returns by [NxATrackTests testTrack3InContext:modificationDate]
    NSXMLElement *sourceTrack3 = [NxATestCase xmlElementFromString:@"<TRACK TrackID=\"629\" Name=\"Terra (Original Mix)\" Artist=\"Manuel De La Mare, Luigi Rocca\" \
                                  Composer=\"\" Album=\"Terra\" Grouping=\"\" Genre=\"Tech House\" Kind=\"AIFF File\" \
                                  Size=\"66494464\" TotalTime=\"375\" DiscNumber=\"0\" TrackNumber=\"1\" \
                                  Year=\"2013\" AverageBpm=\"124.00\" DateAdded=\"2013-11-28\" BitRate=\"1411\" \
                                  SampleRate=\"44100\" Comments=\"\" PlayCount=\"0\" Rating=\"0\" Location=\"file://localhost/Users/didier/Music/Gigs/Terra%20(Original%20Mix)%20-%20Manuel%20De%20La%20Mare%20and%20Luigi%20Rocca.aiff\" \
                                  Remixer=\"\" Tonality=\"12m\" Label=\"303Lovers\" Mix=\"\"> \
                                  <TEMPO Inizio=\"0.485\" Bpm=\"124.00\" Metro=\"4/4\" Battito=\"1\"/> \
                                  <POSITION_MARK Name=\"\" Type=\"0\" Start=\"0.485\" Num=\"0\"/> \
                                  <POSITION_MARK Name=\"\" Type=\"0\" Start=\"31.452\" Num=\"1\"/> \
                                  <POSITION_MARK Name=\"\" Type=\"0\" Start=\"62.420\" Num=\"2\"/> \
                                  <POSITION_MARK Name=\"\" Type=\"0\" Start=\"93.388\" Num=\"-1\"/> \
                                  <POSITION_MARK Name=\"\" Type=\"0\" Start=\"341.130\" Num=\"-1\"/> \
                                  </TRACK>"];
    return sourceTrack3;
}

#pragma mark Accessors

- (id<NxAParserImplementation>)mockParser
{
    if (!_mockParser) {
        _mockParser = [self mockParserWithClass:[NxARekordboxParser class]];
        NxAAssertNotNil(_mockParser);
    }

    return _mockParser;
}

#pragma mark Test methods

- (void)test_parseRekordboxMarkerElementsIn_parseElementsWithNoSavedValues_ReturnAllNewMarkers
{
    // -- Given.
    NSXMLElement *trackElement = [NxATrackRekordboxParsingTests rekordboxTestSourceTrack3];
    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-12-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    NSDate *laterModificationDate = [NSDate dateWithStringInLocalTimeZone:@"2001-12-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    NxARekordboxParser *mockParser = self.mockParser;
    NxATrack *track = [NxATrackTests emptyTestTrackInContext:self.context
                                                usingKeyName:@"DOES NOT MATTER"
                                              lastModifiedOn:modificationDate];

    // -- When.
    NxAParsingResultFlags result = [track parseRekordboxMarkerElementsIn:trackElement
                                                     elementModifiedDate:laterModificationDate
                                                                  parser:mockParser];

    // -- Then.
    XCTAssertEqual(result, NxAParsedObjectCausedUpdating);
    NSArray *markers = [NxAMarkerProperty tagsIn:track];
    XCTAssertEqual(markers.count, 6);
    NxAGridMarkerProperty *gridMarker = markers[0];
    XCTAssertTrue([gridMarker isKindOfClass:[NxAGridMarkerProperty class]]);
    XCTAssertEqualObjects(gridMarker.p_pos, [NSDecimalNumber decimalNumberWithString:@"0.485"]);
    XCTAssertEqualObjects(gridMarker.p_bpm, [NSDecimalNumber decimalNumberWithString:@"124.00"]);
    NxACueMarkerProperty *marker = markers[1];
    XCTAssertTrue([marker isKindOfClass:[NxAMarkerProperty class]]);
    XCTAssertEqualObjects(marker.p_pos, [NSDecimalNumber decimalNumberWithString:@"0.485"]);
    XCTAssertEqual(marker.p_hotCue.integerValue, 0);
    XCTAssertEqualObjects(marker.p_name, @"");
    marker = markers[2];
    XCTAssertTrue([marker isKindOfClass:[NxAMarkerProperty class]]);
    XCTAssertEqualObjects(marker.p_pos, [NSDecimalNumber decimalNumberWithString:@"31.452"]);
    XCTAssertEqual(marker.p_hotCue.integerValue, 1);
    XCTAssertEqualObjects(marker.p_name, @"");
    marker = markers[3];
    XCTAssertTrue([marker isKindOfClass:[NxAMarkerProperty class]]);
    XCTAssertEqualObjects(marker.p_pos, [NSDecimalNumber decimalNumberWithString:@"62.420"]);
    XCTAssertEqual(marker.p_hotCue.integerValue, 2);
    XCTAssertEqualObjects(marker.p_name, @"");
    marker = markers[4];
    XCTAssertTrue([marker isKindOfClass:[NxAMarkerProperty class]]);
    XCTAssertEqualObjects(marker.p_pos, [NSDecimalNumber decimalNumberWithString:@"93.388"]);
    XCTAssertNil(marker.p_hotCue);
    XCTAssertEqualObjects(marker.p_name, @"");
    marker = markers[5];
    XCTAssertTrue([marker isKindOfClass:[NxAMarkerProperty class]]);
    XCTAssertEqualObjects(marker.p_pos, [NSDecimalNumber decimalNumberWithString:@"341.130"]);
    XCTAssertNil(marker.p_hotCue);
    XCTAssertEqualObjects(marker.p_name, @"");

    assertThatNothingWasLogged();
}

- (void)test_parseRekordboxMarkerElementsIn_parseElementsWithSameSavedValues_ReturnOldMarkers
{
    // -- Given.
    NSXMLElement *trackElement = [NxATrackRekordboxParsingTests rekordboxTestSourceTrack3];
    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-12-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    NSDate *laterModificationDate = [NSDate dateWithStringInLocalTimeZone:@"2001-12-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    NxARekordboxParser *mockParser = self.mockParser;
    NxATrack *track = [NxATrackTests emptyTestTrackInContext:self.context
                                                usingKeyName:@"DOES NOT MATTER"
                                              lastModifiedOn:modificationDate];

    NxAParsedMarkerValue *gridValue = [NxAParsedMarkerValue parsedGridMarkerValueAtPositionInSeconds:[NSDecimalNumber decimalNumberWithString:@"0.485"]
                                                                                            beatsPerMinute:[NSDecimalNumber decimalNumberWithString:@"124.00"]];
    NxAParsedMarkerValue *cueValue1 = [NxAParsedMarkerValue parsedMarkerValueWithName:@""
                                                                    positionInSeconds:[NSDecimalNumber decimalNumberWithString:@"0.485"]
                                                                         hotCueNumber:@(0)];
    NxAParsedMarkerValue *cueValue2 = [NxAParsedMarkerValue parsedMarkerValueWithName:@""
                                                                    positionInSeconds:[NSDecimalNumber decimalNumberWithString:@"31.452"]
                                                                         hotCueNumber:@(1)];
    NxAParsedMarkerValue *cueValue3 = [NxAParsedMarkerValue parsedMarkerValueWithName:@""
                                                                    positionInSeconds:[NSDecimalNumber decimalNumberWithString:@"62.420"]
                                                                         hotCueNumber:@(2)];
    NxAParsedMarkerValue *cueValue4 = [NxAParsedMarkerValue parsedMarkerValueWithName:@""
                                                                    positionInSeconds:[NSDecimalNumber decimalNumberWithString:@"93.388"]
                                                                         hotCueNumber:nil];
    NxAParsedMarkerValue *cueValue5 = [NxAParsedMarkerValue parsedMarkerValueWithName:@""
                                                                    positionInSeconds:[NSDecimalNumber decimalNumberWithString:@"341.130"]
                                                                         hotCueNumber:nil];
    NSString *versionPrefix = [self.mockParser.parserClass versionPrefix];
    [track saveValues:@[ gridValue, cueValue1, cueValue2, cueValue3, cueValue4, cueValue5 ]
          forTagClass:[NxAMarkerProperty class]
           andVersion:versionPrefix];

    // -- When.
    NxAParsingResultFlags result = [track parseRekordboxMarkerElementsIn:trackElement
                                                     elementModifiedDate:laterModificationDate
                                                                  parser:mockParser];

    // -- Then.
    XCTAssertEqual(result, NxAParsedObjectWasUpToDate);
    NSArray *markers = [NxAMarkerProperty tagsIn:track];
    XCTAssertEqual(markers.count, 0);

    assertThatNothingWasLogged();
}

@end
