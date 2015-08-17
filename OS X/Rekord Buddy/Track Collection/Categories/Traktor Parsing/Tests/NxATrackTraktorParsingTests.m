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

#import "NxATrackTraktorParsingTests.h"
#import "NxATrack+TraktorParsing.h"
#import "Track Collection/Parsers/Internal/NxATraktorParser.h"
#import "NSDate+NxAUtility.h"
#import "NxAMarkerPropertyTests.h"
#import "Track Collection/Tags/Marker Properties/Internal/NxACueMarkerProperty.h"
#import "Track Collection/Tags/Marker Properties/Internal/NxAGridMarkerProperty.h"
#import "Track Collection/Tags/Marker Properties/Internal/NxALoopMarkerProperty.h"
#import "Utility Classes/Internal/NxAWarning.h"
#import "NxAParsedMarkerValue.h"

#pragma mark Private Interface

@interface NxATrackTraktorParsingTests ()

@property (strong, nonatomic) id<NxAParserImplementation> mockParser;

@end

#pragma mark - Implementation

@implementation NxATrackTraktorParsingTests

#pragma mark Class Methods

+ (NSXMLElement *)traktorTestSourceTrack1
{
    // -- The metadata for this track should match the one returns by [NxATrackTests testTrack1InContext:modificationDate]
    NSXMLElement *sourceTrack1 = [NxATestCase xmlElementFromString:@"<ENTRY MODIFIED_DATE=\"2014/9/15\" \
                                  MODIFIED_TIME=\"35631\" LOCK=\"1\" LOCK_MODIFICATION_TIME=\"2014-07-06T22:08:05\" \
                                  AUDIO_ID=\"AbdXd3d3d3d2M0mYiIiIiIiIiHaJiZmImJiZiam6q6uqqqmndmnLu6urqqqqqavKu7urqqqqqbpTI0REVDM0MzSJdomIh4eJeHmaqImZh4iJiru7y7vLqqupnN3M3MzLu7y8vc3MzNzLu7u8vMy7y8u6qqqbqrupmqqpqampqquqmaq6mKqaqrqrupu6maqrl6y8u8vLu7u6vM28zMu6urq7zMurq6uqqrq7uqq7u6u7mqq7qqmZiJiHd3eHmKmZmYmHd3eIy8zMvMy7q7q7y9zMzLy7u6u7y6mampmZmampqqqampmZmampuZiJiXiIiJiXiZiJiXiIiJiZqUEAAA==\" \
                                  TITLE=\"I'm In Love (Deep Mix)\" ARTIST=\"Artful, Donae'O\"> \
                                  <LOCATION DIR=\"/:Users/:didier/:Music/:Gigs/:\" FILE=\"25 I'm In Love (feat. Donae'O) [Deep Mix].m4a\" VOLUME=\"Macintosh HD\" VOLUMEID=\"Macintosh HD\"></LOCATION> \
                                  <ALBUM OF_TRACKS=\"39\" TRACK=\"25\" TITLE=\"Defected Presents Dimitri from Paris In the House of Disco\"></ALBUM> \
                                  <MODIFICATION_INFO AUTHOR_TYPE=\"user\"></MODIFICATION_INFO> \
                                  <INFO BITRATE=\"-1\" LABEL=\"Size Records\" MIX=\"Original Mix\" COMMENT=\"Some snazzy comments!\" GENRE=\"House, Progressive House\" REMIXER=\"Third Party, Rick James\" PRODUCER=\"My Digital Enemy, Pleasurekraft\" RATING=\"Awesome, Play:Now\" COVERARTID=\"023/XIFLNRAUKICXDAF1H0BCBQMYGYPD\" KEY=\"Fmin\" PLAYCOUNT=\"142\" RANKING=\"153\" PLAYTIME=\"440\" PLAYTIME_FLOAT=\"439.414\" IMPORT_DATE=\"2014/6/17\" LAST_PLAYED=\"2014/7/6\" RELEASE_DATE=\"2014/6/15\" FLAGS=\"28\" FILESIZE=\"14676\"></INFO> \
                                  <TEMPO BPM=\"124.67\" BPM_QUALITY=\"100\"></TEMPO> \
                                  <LOUDNESS PEAK_DB=\"-0\" PERCEIVED_DB=\"1.11107\" ANALYZED_DB=\"1.11107\"></LOUDNESS> \
                                  <MUSICAL_KEY VALUE=\"17\"></MUSICAL_KEY> \
                                  <CUE_V2 NAME=\"Test1\" DISPL_ORDER=\"0\" TYPE=\"4\" START=\"68.0\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                  <CUE_V2 NAME=\"Test2\" DISPL_ORDER=\"0\" TYPE=\"0\" START=\"68.0\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"0\"></CUE_V2> \
                                  <CUE_V2 NAME=\"Test3\" DISPL_ORDER=\"0\" TYPE=\"0\" START=\"31036.0\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"1\"></CUE_V2> \
                                  <CUE_V2 NAME=\"Test4\" DISPL_ORDER=\"0\" TYPE=\"4\" START=\"50068.0\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                  <CUE_V2 NAME=\"Test5\" DISPL_ORDER=\"0\" TYPE=\"0\" START=\"62004\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                  <CUE_V2 NAME=\"Test6\" DISPL_ORDER=\"0\" TYPE=\"4\" START=\"200068.0\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                  <CUE_V2 NAME=\"Test7\" DISPL_ORDER=\"0\" TYPE=\"0\" START=\"371681\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                  </ENTRY>"];
    return sourceTrack1;
}

+ (NSXMLElement *)traktorTestSourceTrack2
{
    // -- The metadata for this track should match the one returns by [NxATrackTests testTrack2InContext:modificationDate]
    NSXMLElement *sourceTrack2 = [NxATestCase xmlElementFromString:@"<ENTRY MODIFIED_DATE=\"2014/9/15\" \
                                  MODIFIED_TIME=\"36305\" AUDIO_ID=\"AOoTFEIjUiJiNIIxUzFEIjUiV1d4MxVXaFMnZneDMmRVU0WK//7//////////+//xmdTVI/////+//////////+qeqmLv//////////+/////aqKqZuv////////////////////6v/////////////////9q8mp////////////////2pe5ibz///////////////+rmZqKlXdjVTJndFRBV3ZFUyeIV4p2eFZ4hoh1eIZ4hXibqpqonv/////////////////////8////////////////+pmFVmb////////v/+//7////////v/+///////////v/+//4yOBAA==\" \
                                  TITLE=\"Need U (100%) (Original Mix)\" ARTIST=\"Duke Dumont, A*M*E\"> \
                                  <LOCATION DIR=\"/:Users/:didier/:Music/:Gigs/:\" FILE=\"Need U (100%) feat. A*M*E (Original Mix) - A*M*E, Duke Dumont.aiff\" VOLUME=\"Macintosh HD\" VOLUMEID=\"Macintosh HD\"></LOCATION> \
                                  <ALBUM TRACK=\"2\" TITLE=\"Need U (100%) [feat. A*M*E]\"></ALBUM> \
                                  <MODIFICATION_INFO AUTHOR_TYPE=\"user\"></MODIFICATION_INFO> \
                                  <INFO BITRATE=\"1411200\" MIX=\"Club Mix\" COMMENT=\"Some even better comments.\" GENRE=\"Deep House, Tech House\" LABEL=\"Win Records/Downtown Records\" REMIXER=\"Arty, Bingo Players\" PRODUCER=\"Steve Angello, Roger Sanchez\" RATING=\"Even Better, Play:Later\" COVERARTID=\"093/34FW1KAZYNESIDVXSSC5CEVNN2AD\" KEY=\"Cmin\" PLAYCOUNT=\"4243\" RANKING=\"255\" PLAYTIME=\"234\" PLAYTIME_FLOAT=\"233.707\" IMPORT_DATE=\"2014/9/10\" RELEASE_DATE=\"2014/9/7\" FLAGS=\"12\" FILESIZE=\"40316\"></INFO> \
                                  <TEMPO BPM=\"124\" BPM_QUALITY=\"100\"></TEMPO> \
                                  <LOUDNESS PEAK_DB=\"0.0841694\" PERCEIVED_DB=\"-2.31561\" ANALYZED_DB=\"-2.31561\"></LOUDNESS> \
                                  <MUSICAL_KEY VALUE=\"12\"></MUSICAL_KEY> \
                                  <CUE_V2 NAME=\"OtherTest1\" DISPL_ORDER=\"0\" TYPE=\"4\" START=\"9.04375\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                  <CUE_V2 NAME=\"OtherTest2\" DISPL_ORDER=\"0\" TYPE=\"0\" START=\"208.0\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"0\"></CUE_V2> \
                                  <CUE_V2 NAME=\"OtherTest3\" DISPL_ORDER=\"0\" TYPE=\"0\" START=\"31428.0\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"1\"></CUE_V2> \
                                  <CUE_V2 NAME=\"OtherTest4\" DISPL_ORDER=\"0\" TYPE=\"0\" START=\"62647\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                  <CUE_V2 NAME=\"OtherTest5\" DISPL_ORDER=\"0\" TYPE=\"0\" START=\"406062.0\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                  </ENTRY>"];
    return sourceTrack2;
}

+ (NSXMLElement *)traktorTestSourceTrack3
{
    // -- The metadata for this track should match the one returns by [NxATrackTests testTrack3InContext:modificationDate]
    NSXMLElement *sourceTrack3 = [NxATestCase xmlElementFromString:@"<ENTRY MODIFIED_DATE=\"2014/9/15\" \
                                  MODIFIED_TIME=\"35635\" LOCK=\"1\" LOCK_MODIFICATION_TIME=\"2014-04-12T22:44:44\" \
                                  AUDIO_ID=\"AXct/v7+/v7+/////+/v7+/v/////v/kQzMzM0MzMzMzRDMzMzM0MzMzMzV4VURERFVERUVEVlVVVURVRERERFnv///////////////////////8eIn////////////////////////////6uaupibqYeZiGmYhnh3ZpmGV3iHzLuauYeKqXfMq67rp4qairupi7qqv////////////nr/////////////+6l4mZiaqnjLupz///////////////////////////+oiIiIiJiXiIiJ3///////////////////////////////////////7/////////7+/v7/wAAA==\" \
                                  TITLE=\"Terra (Original Mix)\" ARTIST=\"Manuel De La Mare, Luigi Rocca\"> \
                                  <LOCATION DIR=\"/:Users/:didier/:Music/:Gigs/:\" FILE=\"Terra (Original Mix) - Manuel De La Mare and Luigi Rocca.aiff\" VOLUME=\"Macintosh HD\" VOLUMEID=\"Macintosh HD\"></LOCATION> \
                                  <ALBUM TRACK=\"1\" TITLE=\"Terra\"></ALBUM> \
                                  <MODIFICATION_INFO AUTHOR_TYPE=\"user\"></MODIFICATION_INFO> \
                                  <INFO BITRATE=\"1411000\" GENRE=\"Tech House\" LABEL=\"303Lovers\" COVERARTID=\"107/LXAA1IDV42PKFDEHQJTHBSDFOSCD\" KEY=\"12m\" PLAYCOUNT=\"0\" PLAYTIME=\"375\" IMPORT_DATE=\"2013/11/28\" LAST_PLAYED=\"2014/4/12\" RELEASE_DATE=\"2013/10/9\" FLAGS=\"28\" FILESIZE=\"64936\"></INFO> \
                                  <TEMPO BPM=\"124\" BPM_QUALITY=\"100\"></TEMPO> \
                                  <LOUDNESS PEAK_DB=\"0.099439\" PERCEIVED_DB=\"0.00582123\" ANALYZED_DB=\"0.00582123\"></LOUDNESS> \
                                  <MUSICAL_KEY VALUE=\"14\"></MUSICAL_KEY> \
                                  <CUE_V2 NAME=\"ThirdTest1\" DISPL_ORDER=\"0\" TYPE=\"4\" START=\"484.589\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                  <CUE_V2 NAME=\"ThirdTest2\" DISPL_ORDER=\"0\" TYPE=\"3\" START=\"484.589\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"0\"></CUE_V2> \
                                  <CUE_V2 NAME=\"ThirdTest3\" DISPL_ORDER=\"0\" TYPE=\"0\" START=\"31452.3\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"1\"></CUE_V2> \
                                  <CUE_V2 NAME=\"ThirdTest4\" DISPL_ORDER=\"0\" TYPE=\"0\" START=\"62420.1\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"2\"></CUE_V2> \
                                  <CUE_V2 NAME=\"ThirdTest5\" DISPL_ORDER=\"0\" TYPE=\"1\" START=\"93387.8\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                  <CUE_V2 NAME=\"ThirdTest6\" DISPL_ORDER=\"0\" TYPE=\"2\" START=\"341130\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                  </ENTRY>"];
    return sourceTrack3;
}

#pragma mark Accessors

- (NxATraktorParser *)mockParser
{
    if (!_mockParser) {
        _mockParser = [self mockParserWithClass:[NxATraktorParser class]];
        NxAAssertNotNil(_mockParser);
    }

    return _mockParser;
}

#pragma mark Test methods

- (void)testParseTraktorModifiedDateFromEntryElement_ParseModificationDate_DateParsesCorrectly
{
    // -- Given an entry element and it modified date/time.
    NSXMLElement *element = [NxATestCase xmlElementFromString:@"<ENTRY MODIFIED_DATE=\"2014/7/23\" \
                             MODIFIED_TIME=\"53064\"></ENTRY>"];

    // -- When parsing the element.
    NSDate *result = [NxATrack parseTraktorModifiedDateFromEntryElement:element loggingMessagesIn:self.mockLog];

    // -- Then make sure the modified date/time was parsed correctly.
    XCTAssertEqualObjects([result stringValueInLocalTimeZoneWithFormat:@"yyyy-MM-dd HH:mm:ss"], @"2014-07-23 08:44:24");

    assertThatNothingWasLogged();
}

- (void)testParseTraktorModifiedDateFromEntryElement_ParseInvalidModificationDate_ReturnNil
{
    // -- Given an invalid entry element.
    NSXMLElement *element = [NxATestCase xmlElementFromString:@"<ENTRY></ENTRY>"];

    // -- When parsing the element.
    NSDate *result = [NxATrack parseTraktorModifiedDateFromEntryElement:element loggingMessagesIn:self.mockLog];

    // -- Then make sure the result is nil and a warning was logged.
    XCTAssertNil(result);

    [verifyCount(self.mockLog, times(1)) addWarning:anything() inContextWithPerObjectWarningInfo:anything()];
    MKTArgumentCaptor *argument1 = [[MKTArgumentCaptor alloc] init];
    MKTArgumentCaptor *argument2 = [[MKTArgumentCaptor alloc] init];
    [verify(self.mockLog) addWarning:[argument1 capture] inContextWithPerObjectWarningInfo:[argument2 capture]];
    NSArray *values1 = argument1.allValues;
    XCTAssertEqual(values1.count, 1);
    XCTAssertTrue([[values1.firstObject p_description] hasPrefix:@"in your Traktor collection with an invalid modification date."]);
    NSArray *values2 = argument2.allValues;
    XCTAssertEqual(values2.count, 1);
    XCTAssertEqualObjects(values2.firstObject, @"");
}

- (void)test_parseTraktorMarkerElementsIn_parseElementsWithNoSavedValues_ReturnAllNewMarkers
{
    // -- Given.
    NSXMLElement *trackElement = [NxATrackTraktorParsingTests traktorTestSourceTrack3];
    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-12-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    NSDate *laterModificationDate = [NSDate dateWithStringInLocalTimeZone:@"2001-12-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    NxATraktorParser *mockParser = self.mockParser;
    NxATrack *track = [NxATrackTests emptyTestTrackInContext:self.context
                                                usingKeyName:@"DOES NOT MATTER"
                                              lastModifiedOn:modificationDate];

    // -- When.
    NxAParsingResultFlags result = [track parseTraktorMarkerElementsIn:trackElement
                                                   elementModifiedDate:laterModificationDate
                                                                parser:mockParser];

    // -- Then.
    XCTAssertEqual(result, NxAParsedObjectCausedUpdating);
    NSArray *markers = [NxAMarkerProperty tagsIn:track];
    XCTAssertEqual(markers.count, 6);
    NxAGridMarkerProperty *gridMarker = markers[0];
    XCTAssertTrue([gridMarker isKindOfClass:[NxAGridMarkerProperty class]]);
    XCTAssertEqualObjects(gridMarker.p_pos, [NSDecimalNumber decimalNumberWithString:@"0.484589"]);
    XCTAssertEqualObjects(gridMarker.p_bpm, [NSDecimalNumber decimalNumberWithString:@"124.00"]);
    NxACueMarkerProperty *marker = markers[1];
    XCTAssertTrue([marker isKindOfClass:[NxACueMarkerProperty class]]);
    XCTAssertEqualObjects(marker.p_pos, [NSDecimalNumber decimalNumberWithString:@"0.484589"]);
    XCTAssertEqual(marker.p_hotCue.integerValue, 0);
    XCTAssertEqualObjects(marker.p_name, @"ThirdTest2");
    marker = markers[2];
    XCTAssertTrue([marker isKindOfClass:[NxACueMarkerProperty class]]);
    XCTAssertEqualObjects(marker.p_pos, [NSDecimalNumber decimalNumberWithString:@"31.4523"]);
    XCTAssertEqual(marker.p_hotCue.integerValue, 1);
    XCTAssertEqualObjects(marker.p_name, @"ThirdTest3");
    marker = markers[3];
    XCTAssertTrue([marker isKindOfClass:[NxACueMarkerProperty class]]);
    XCTAssertEqualObjects(marker.p_pos, [NSDecimalNumber decimalNumberWithString:@"62.4201"]);
    XCTAssertEqual(marker.p_hotCue.integerValue, 2);
    XCTAssertEqualObjects(marker.p_name, @"ThirdTest4");
    marker = markers[4];
    XCTAssertTrue([marker isKindOfClass:[NxACueMarkerProperty class]]);
    XCTAssertEqualObjects(marker.p_pos, [NSDecimalNumber decimalNumberWithString:@"93.3878"]);
    XCTAssertNil(marker.p_hotCue);
    XCTAssertEqualObjects(marker.p_name, @"ThirdTest5");
    marker = markers[5];
    XCTAssertTrue([marker isKindOfClass:[NxACueMarkerProperty class]]);
    XCTAssertEqualObjects(marker.p_pos, [NSDecimalNumber decimalNumberWithString:@"341.130"]);
    XCTAssertNil(marker.p_hotCue);
    XCTAssertEqualObjects(marker.p_name, @"ThirdTest6");

    assertThatNothingWasLogged();
}

- (void)test_parseTraktorMarkerElementsIn_parseElementsWithSameSavedValues_ReturnOldMarkers
{
    // -- Given.
    NSXMLElement *trackElement = [NxATrackTraktorParsingTests traktorTestSourceTrack3];
    NSDate *modificationDate = [NSDate dateWithStringInLocalTimeZone:@"2000-12-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    NSDate *laterModificationDate = [NSDate dateWithStringInLocalTimeZone:@"2001-12-12 23:22:45" format:@"yyyy-MM-dd HH:mm:ss"];
    NxATraktorParser *mockParser = self.mockParser;
    NxATrack *track = [NxATrackTests emptyTestTrackInContext:self.context
                                                usingKeyName:@"DOES NOT MATTER"
                                              lastModifiedOn:modificationDate];

    NxAParsedMarkerValue *gridValue = [NxAParsedMarkerValue parsedGridMarkerValueAtPositionInSeconds:[NSDecimalNumber decimalNumberWithString:@"0.484589"]
                                                                                      beatsPerMinute:[NSDecimalNumber decimalNumberWithString:@"124.00"]];
    NxAParsedMarkerValue *cueValue1 = [NxAParsedMarkerValue parsedMarkerValueWithName:@"ThirdTest2"
                                                                    positionInSeconds:[NSDecimalNumber decimalNumberWithString:@"0.484589"]
                                                                         hotCueNumber:@(0)];
    [cueValue1 addCustomFlag:NxAMarkerIsLoadMarker];
    NxAParsedMarkerValue *cueValue2 = [NxAParsedMarkerValue parsedMarkerValueWithName:@"ThirdTest3"
                                                                    positionInSeconds:[NSDecimalNumber decimalNumberWithString:@"31.4523"]
                                                                         hotCueNumber:@(1)];
    NxAParsedMarkerValue *cueValue3 = [NxAParsedMarkerValue parsedMarkerValueWithName:@"ThirdTest4"
                                                                    positionInSeconds:[NSDecimalNumber decimalNumberWithString:@"62.4201"]
                                                                         hotCueNumber:@(2)];
    NxAParsedMarkerValue *cueValue4 = [NxAParsedMarkerValue parsedMarkerValueWithName:@"ThirdTest5"
                                                                    positionInSeconds:[NSDecimalNumber decimalNumberWithString:@"93.3878"]
                                                                         hotCueNumber:nil];
    [cueValue4 addCustomFlag:NxAMarkerIsFadeInMarker];
    NxAParsedMarkerValue *cueValue5 = [NxAParsedMarkerValue parsedMarkerValueWithName:@"ThirdTest6"
                                                                    positionInSeconds:[NSDecimalNumber decimalNumberWithString:@"341.130"]
                                                                         hotCueNumber:nil];
    [cueValue5 addCustomFlag:NxAMarkerIsFadeOutMarker];
    NSString *versionPrefix = [self.mockParser.parserClass versionPrefix];
    [track saveValues:@[ gridValue, cueValue1, cueValue2, cueValue3, cueValue4, cueValue5 ]
          forTagClass:[NxAMarkerProperty class]
           andVersion:versionPrefix];

    // -- When.
    NxAParsingResultFlags result = [track parseTraktorMarkerElementsIn:trackElement
                                                   elementModifiedDate:laterModificationDate
                                                                parser:mockParser];

    // -- Then.
    XCTAssertEqual(result, NxAParsedObjectWasUpToDate);
    NSArray *markers = [NxAMarkerProperty tagsIn:track];
    XCTAssertEqual(markers.count, 0);

    assertThatNothingWasLogged();
}

@end
