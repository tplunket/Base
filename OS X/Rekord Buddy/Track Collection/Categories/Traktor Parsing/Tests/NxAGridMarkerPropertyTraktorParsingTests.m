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

#import "NxAGridMarkerPropertyTests.h"
#import "Track Collection/Parsers/NxATraktorParser.h"
#import "NxAParsedMarkerValue+TraktorParsing.h"
#import "NxAMarkerProperty+TraktorParsing.h"
#import "NxAGridMarkerProperty+TraktorParsing.h"
#import "Track Collection/Tags/Marker Properties/Internal/NxAGridMarkerProperty.h"
#import "Utility Classes/Internal/NxAWarning.h"

#pragma mark Private Interface

@interface NxAGridMarkerPropertyTraktorParsingTests : NxAGridMarkerPropertyTests

@property (strong, nonatomic) id<NxAParserImplementation> mockParser;

@end

#pragma mark - Implementation

@implementation NxAGridMarkerPropertyTraktorParsingTests

#pragma mark Accessors

- (id<NxAParserImplementation>)mockParser
{
    if (!_mockParser) {
        _mockParser = [self mockParserWithClass:[NxATraktorParser class]];
        NxAAssertNotNil(_mockParser);
    }

    return _mockParser;
}

#pragma mark Test methods

- (void)testp_MarkerWithTraktorCueV2Element_ParseGridMarker_ParsesCorrectly
{
    // -- Given a Traktor CUE V2 marker element.
    NSXMLElement *element = [NxATestCase xmlElementFromString:@"<CUE_V2 NAME=\"TestGrid\" DISPL_ORDER=\"0\" TYPE=\"4\" START=\"23230\" \
                             LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2>"];
    NSDecimalNumber *tempo = [NSDecimalNumber decimalNumberWithString:@"126.34"];

    // -- When parsing this element into a marker object.
    NSArray *result = [NxAParsedMarkerValue markerValuesWithTraktorCueV2Element:element
                                                                          tempo:tempo
                                                                            log:self.mockLog];

    // -- Then make sure that the marker matches the source data.
    XCTAssertEqual(result.count, 1);
    NxAParsedMarkerValue *marker = result.firstObject;
    XCTAssertEqual(marker.type, NxAParsedMarkerValueGridType);
    XCTAssertEqualObjects(marker.positionInSeconds, [NSDecimalNumber decimalNumberWithString:@"23.23"]);
    XCTAssertNil(marker.name);
    XCTAssertNil(marker.hotCueNumber);
    XCTAssertEqualObjects(marker.beatsPerMinute, [NSDecimalNumber decimalNumberWithString:@"126.34"]);
    XCTAssertEqual(marker.customFlags.count, 0);

    assertThatNothingWasLogged();
}

- (void)testp_MarkerValueWithTraktorCueV2Element_ParseGridMarkerWithNoTempo_ParsesNothing
{
    // -- Given a Traktor CUE V2 marker element.
    NSXMLElement *element = [NxATestCase xmlElementFromString:@"<CUE_V2 NAME=\"TestCue\" DISPL_ORDER=\"0\" TYPE=\"4\" START=\"1100\" \
                             LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2>"];

    // -- When parsing this element into a marker object.
    NSArray *result = [NxAParsedMarkerValue markerValuesWithTraktorCueV2Element:element
                                                                          tempo:nil
                                                                            log:self.mockLog];

    // -- Then make sure that the markers match the source data.
    XCTAssertEqual(result.count, 0);

    [verifyCount(self.mockLog, never()) addWarning:anything()];
    [verifyCount(self.mockLog, times(1)) addWarning:anything() inContextWithPerObjectWarningInfo:anything()];
    MKTArgumentCaptor *argument1 = [[MKTArgumentCaptor alloc] init];
    [verify(self.mockLog) addWarning:[argument1 capture] inContextWithPerObjectWarningInfo:@""];
    NSArray *values1 = argument1.allValues;
    XCTAssertEqual(values1.count, 1);
    XCTAssertTrue([[values1.firstObject p_description] hasPrefix:@"in your Traktor collection with grid markers but no tempo set for the track."]);
}

- (void)testp_MarkerWithTraktorCueV2Element_ParseGridMarkerWithHotCue_ParsesCorrectly
{
    // -- Given a Traktor CUE V2 marker element.
    NSXMLElement *element = [NxATestCase xmlElementFromString:@"<CUE_V2 NAME=\"TestMark\" DISPL_ORDER=\"0\" TYPE=\"4\" START=\"465832\" \
                             LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"7\"></CUE_V2>"];
    NSDecimalNumber *tempo = [NSDecimalNumber decimalNumberWithString:@"99.00"];

    // -- When parsing this element into a marker object.
    NSArray *result = [NxAParsedMarkerValue markerValuesWithTraktorCueV2Element:element
                                                                          tempo:tempo
                                                                            log:self.mockLog];

    // -- Then make sure that the marker matches the source data.
    XCTAssertEqual(result.count, 2);
    NxAParsedMarkerValue *marker = result[0];
    XCTAssertEqual(marker.type, NxAParsedMarkerValueGridType);
    XCTAssertEqualObjects(marker.positionInSeconds, [NSDecimalNumber decimalNumberWithString:@"465.832"]);
    XCTAssertNil(marker.name);
    XCTAssertNil(marker.hotCueNumber);
    XCTAssertEqualObjects(marker.beatsPerMinute, [NSDecimalNumber decimalNumberWithString:@"99.00"]);
    XCTAssertEqual(marker.customFlags.count, 1);
    XCTAssertTrue([marker hasCustomFlag:NxAMarkerWasStoredAsAHotCue]);

    marker = result[1];
    XCTAssertEqual(marker.type, NxAParsedMarkerValueCueType);
    XCTAssertEqualObjects(marker.positionInSeconds, [NSDecimalNumber decimalNumberWithString:@"465.832"]);
    XCTAssertEqualObjects(marker.name, @"TestMark");
    XCTAssertEqualObjects(marker.hotCueNumber, @(7));
    XCTAssertNil(marker.beatsPerMinute);
    XCTAssertEqual(marker.customFlags.count, 0);

    assertThatNothingWasLogged();
}

- (void)testp_MarkerWithTraktorCueV2Element_ParseGridMarkerWithNoName_ParsesCorrectly
{
    // -- Given a Traktor CUE V2 marker element.
    NSXMLElement *element = [NxATestCase xmlElementFromString:@"<CUE_V2 NAME=\"\" DISPL_ORDER=\"0\" TYPE=\"4\" START=\"465832\" \
                             LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2>"];
    NSDecimalNumber *tempo = [NSDecimalNumber decimalNumberWithString:@"99.00"];
    
    // -- When parsing this element into a marker object.
    NSArray *result = [NxAParsedMarkerValue markerValuesWithTraktorCueV2Element:element
                                                                          tempo:tempo
                                                                            log:self.mockLog];

    // -- Then make sure that the marker matches the source data.
    XCTAssertEqual(result.count, 1);
    NxAParsedMarkerValue *marker = result.firstObject;
    XCTAssertEqual(marker.type, NxAParsedMarkerValueGridType);
    XCTAssertEqualObjects(marker.positionInSeconds, [NSDecimalNumber decimalNumberWithString:@"465.832"]);
    XCTAssertNil(marker.name);
    XCTAssertNil(marker.hotCueNumber);
    XCTAssertEqualObjects(marker.beatsPerMinute, [NSDecimalNumber decimalNumberWithString:@"99.00"]);
    XCTAssertEqual(marker.customFlags.count, 0);

    assertThatNothingWasLogged();
}

- (void)testAddToTraktorEntryElement_AddGridMarker_AddsCorrectly
{
    // -- Given a grid marker and an XML Element which already contains one.
    NxAGridMarkerProperty *marker = [NxAGridMarkerProperty gridMarkerAtPosition:[NSDecimalNumber decimalNumberWithString:@"450.022"]
                                                                            bpm:[NSDecimalNumber decimalNumberWithString:@"124.353"]
                                                                    customFlags:@[ ]
                                                                        context:self.context];
    NSXMLElement *entryElement = [NxATestCase xmlElementFromString:@"<ENTRY></ENTRY>"];

    // -- When adding the marker to the element.
    [marker addToTraktorEntryElement:entryElement log:self.mockLog];

    // -- Then make sure the marker is not added and a warning is not logged.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<ENTRY> \
                                    <TEMPO BPM=\"124.353\" BPM_QUALITY=\"100\"></TEMPO> \
                                    <CUE_V2 NAME=\"\" DISPL_ORDER=\"0\" TYPE=\"4\" START=\"450022.0000\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                    </ENTRY>"];
    XCTAssertEqualObjects(entryElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testAddToTraktorEntryElement_AddGridMarkerWithHotCueAndName_AddsCorrectly
{
    // -- Given a grid marker and an XML Element which already contains one.
    NxAGridMarkerProperty *marker = [NxAGridMarkerProperty gridMarkerAtPosition:[NSDecimalNumber decimalNumberWithString:@"450.022"]
                                                                            bpm:[NSDecimalNumber decimalNumberWithString:@"124.353"]
                                                                    customFlags:@[ ]
                                                                        context:self.context];
    NSXMLElement *entryElement = [NxATestCase xmlElementFromString:@"<ENTRY></ENTRY>"];

    // -- When adding the marker to the element.
    [marker addToTraktorEntryElement:entryElement withName:@"TestName" andHotCue:[NSNumber numberWithInteger:4] log:self.mockLog];

    // -- Then make sure the marker is not added and a warning is not logged.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<ENTRY> \
                                    <TEMPO BPM=\"124.353\" BPM_QUALITY=\"100\"></TEMPO> \
                                    <CUE_V2 NAME=\"TestName\" DISPL_ORDER=\"0\" TYPE=\"4\" START=\"450022.0000\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"4\"></CUE_V2> \
                                    </ENTRY>"];
    XCTAssertEqualObjects(entryElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testAddToTraktorEntryElement_AddGridMarkerWithNoNameAndNoHotCue_AddsCorrectly
{
    // -- Given a grid marker and an XML Element which already contains one.
    NxAGridMarkerProperty *marker = [NxAGridMarkerProperty gridMarkerAtPosition:[NSDecimalNumber decimalNumberWithString:@"450.022"]
                                                                            bpm:[NSDecimalNumber decimalNumberWithString:@"124.353"]
                                                                    customFlags:@[ ]
                                                                        context:self.context];
    NSXMLElement *entryElement = [NxATestCase xmlElementFromString:@"<ENTRY></ENTRY>"];

    // -- When adding the marker to the element.
    [marker addToTraktorEntryElement:entryElement withName:@"" andHotCue:nil log:self.mockLog];

    // -- Then make sure the marker is not added and a warning is not logged.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<ENTRY> \
                                    <TEMPO BPM=\"124.353\" BPM_QUALITY=\"100\"></TEMPO> \
                                    <CUE_V2 NAME=\"\" DISPL_ORDER=\"0\" TYPE=\"4\" START=\"450022.0000\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                    </ENTRY>"];
    XCTAssertEqualObjects(entryElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testAddToTraktorEntryElement_AddGridMarkerWithAMarkerAlreadyThereButDifferentBpm_LogsAWarning
{
    // -- Given a grid marker and an XML Element which already contains one.
    NxAGridMarkerProperty *marker = [NxAGridMarkerProperty gridMarkerAtPosition:[NSDecimalNumber decimalNumberWithString:@"450.022"]
                                                                            bpm:[NSDecimalNumber decimalNumberWithString:@"124.33"]
                                                                    customFlags:@[ ]
                                                                        context:self.context];
    NSXMLElement *entryElement = [NxATestCase xmlElementFromString:@"<ENTRY> \
                                  <TEMPO BPM=\"122.875\" BPM_QUALITY=\"100\"></TEMPO> \
                                  <CUE_V2 NAME=\"AutoGrid\" DISPL_ORDER=\"0\" TYPE=\"4\" START=\"23.5576\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                  </ENTRY>"];

    // -- When adding the marker to the element.
    [marker addToTraktorEntryElement:entryElement withName:@"GridMarkerTest" andHotCue:nil log:self.mockLog];

    // -- Then make sure the marker is added and a warning is logged.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<ENTRY> \
                                    <TEMPO BPM=\"122.875\" BPM_QUALITY=\"100\"></TEMPO> \
                                    <CUE_V2 NAME=\"AutoGrid\" DISPL_ORDER=\"0\" TYPE=\"4\" START=\"23.5576\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                    <CUE_V2 NAME=\"GridMarkerTest\" DISPL_ORDER=\"0\" TYPE=\"4\" START=\"450022.0000\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                    </ENTRY>"];
    XCTAssertEqualObjects(entryElement, expectedResult);

    [verifyCount(self.mockLog, never()) addWarning:anything()];
    [verifyCount(self.mockLog, times(1)) addWarning:anything() inContextWithPerObjectWarningInfo:anything()];
    MKTArgumentCaptor *argument1 = [[MKTArgumentCaptor alloc] init];
    [verify(self.mockLog) addWarning:[argument1 capture] inContextWithPerObjectWarningInfo:@""];
    NSArray *values1 = argument1.allValues;
    XCTAssertEqual(values1.count, 1);
    XCTAssertTrue([[values1.firstObject p_description] hasPrefix:@"in your Rekord Buddy collection with multiple BPM value"]);
}

- (void)testAddToTraktorEntryElement_AddGridMarkerWithAMarkerAlreadyThereButSameBpm_DoesNotLogsAWarning
{
    // -- Given a grid marker and an XML Element which already contains one.
    NxAGridMarkerProperty *marker = [NxAGridMarkerProperty gridMarkerAtPosition:[NSDecimalNumber decimalNumberWithString:@"450.022"]
                                                                            bpm:[NSDecimalNumber decimalNumberWithString:@"124.353"]
                                                                    customFlags:@[ ]
                                                                        context:self.context];
    NSXMLElement *entryElement = [NxATestCase xmlElementFromString:@"<ENTRY> \
                                  <TEMPO BPM=\"124.353\" BPM_QUALITY=\"100\"></TEMPO> \
                                  <CUE_V2 NAME=\"AutoGrid\" DISPL_ORDER=\"0\" TYPE=\"4\" START=\"23.5576\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                  </ENTRY>"];

    // -- When adding the marker to the element.
    [marker addToTraktorEntryElement:entryElement withName:@"GridMarkerTest" andHotCue:nil log:self.mockLog];

    // -- Then make sure the marker is not added and a warning is not logged.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<ENTRY> \
                                    <TEMPO BPM=\"124.353\" BPM_QUALITY=\"100\"></TEMPO> \
                                    <CUE_V2 NAME=\"AutoGrid\" DISPL_ORDER=\"0\" TYPE=\"4\" START=\"23.5576\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                    <CUE_V2 NAME=\"GridMarkerTest\" DISPL_ORDER=\"0\" TYPE=\"4\" START=\"450022.0000\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                    </ENTRY>"];
    XCTAssertEqualObjects(entryElement, expectedResult);

    assertThatNothingWasLogged();
}

@end
