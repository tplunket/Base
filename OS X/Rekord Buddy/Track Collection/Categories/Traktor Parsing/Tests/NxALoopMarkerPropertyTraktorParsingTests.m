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

#import "NxALoopMarkerPropertyTests.h"
#import "Track Collection/Parsers/NxATraktorParser.h"
#import "Track Collection/Tags/Marker Properties/Internal/NxALoopMarkerProperty.h"
#import "NxAParsedMarkerValue+TraktorParsing.h"
#import "NxAMarkerProperty+TraktorParsing.h"

#pragma mark Private Interface

@interface NxALoopMarkerPropertyTraktorParsingTests : NxALoopMarkerPropertyTests

@property (strong, nonatomic) id<NxAParserImplementation> mockParser;

@end

#pragma mark - Implementation

@implementation NxALoopMarkerPropertyTraktorParsingTests

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

- (void)testMarkerValuesWithTraktorPositionElement_ParseLoopMarker_ParsesCorrectly
{
    // -- Given a Traktor CUE V2 marker element.
    NSXMLElement *element = [NxATestCase xmlElementFromString:@"<CUE_V2 NAME=\"TestLoop\" DISPL_ORDER=\"0\" TYPE=\"5\" START=\"5500\" \
                             LEN=\"50034\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2>"];

    // -- When parsing this element into a marker object.
    NSArray *result = [NxAParsedMarkerValue markerValuesWithTraktorCueV2Element:element
                                                                          tempo:nil
                                                                            log:self.mockLog];

    // -- Then make sure that the markers match the source data.
    XCTAssertEqual(result.count, 1);
    NxAParsedMarkerValue *marker = result.firstObject;
    XCTAssertEqual(marker.type, NxAParsedMarkerValueLoopType);
    XCTAssertEqualObjects(marker.positionInSeconds, [NSDecimalNumber decimalNumberWithString:@"5.5"]);
    XCTAssertEqualObjects(marker.lengthInSeconds, [NSDecimalNumber decimalNumberWithString:@"50.034"]);
    XCTAssertEqualObjects(marker.name, @"TestLoop");
    XCTAssertNil(marker.hotCueNumber);

    assertThatNothingWasLogged();
}

- (void)testMarkerValuesWithTraktorPositionElement_ParseLoopMarkerNoName_ParsesCorrectly
{
    // -- Given a Traktor CUE V2 marker element.
    NSXMLElement *element = [NxATestCase xmlElementFromString:@"<CUE_V2 NAME=\"\" DISPL_ORDER=\"0\" TYPE=\"5\" START=\"7544\" \
                             LEN=\"5325339784\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2>"];

    // -- When parsing this element into a marker object.
    NSArray *result = [NxAParsedMarkerValue markerValuesWithTraktorCueV2Element:element
                                                                          tempo:nil
                                                                            log:self.mockLog];

    // -- Then make sure that the markers match the source data.
    XCTAssertEqual(result.count, 1);
    NxAParsedMarkerValue *marker = result.firstObject;
    XCTAssertEqual(marker.type, NxAParsedMarkerValueLoopType);
    XCTAssertEqualObjects(marker.positionInSeconds, [NSDecimalNumber decimalNumberWithString:@"7.544"]);
    XCTAssertEqualObjects(marker.lengthInSeconds, [NSDecimalNumber decimalNumberWithString:@"5325339.784"]);
    XCTAssertNil(marker.name);
    XCTAssertNil(marker.hotCueNumber);

    assertThatNothingWasLogged();
}

- (void)testMarkerValuesWithTraktorPositionElement_ParseLoopMarkerWithHotCue_ParsesCorrectly
{
    // -- Given a Traktor CUE V2 marker element.
    NSXMLElement *element = [NxATestCase xmlElementFromString:@"<CUE_V2 NAME=\"TestLoop\" DISPL_ORDER=\"0\" TYPE=\"5\" START=\"5500\" \
                             LEN=\"50034\" REPEATS=\"-1\" HOTCUE=\"7\"></CUE_V2>"];

    // -- When parsing this element into a marker object.
    NSArray *result = [NxAParsedMarkerValue markerValuesWithTraktorCueV2Element:element
                                                                          tempo:nil
                                                                            log:self.mockLog];

    // -- Then make sure that the markers match the source data.
    XCTAssertEqual(result.count, 1);
    NxAParsedMarkerValue *marker = result.firstObject;
    XCTAssertEqual(marker.type, NxAParsedMarkerValueLoopType);
    XCTAssertEqualObjects(marker.positionInSeconds, [NSDecimalNumber decimalNumberWithString:@"5.5"]);
    XCTAssertEqualObjects(marker.lengthInSeconds, [NSDecimalNumber decimalNumberWithString:@"50.034"]);
    XCTAssertEqualObjects(marker.name, @"TestLoop");
    XCTAssertEqualObjects(marker.hotCueNumber, @(7));

    assertThatNothingWasLogged();
}

- (void)testAddToTraktorEntryElement_AddLoopMarker_AddsCorrectly
{
    // -- Given a marker and an XML Element.
    NxALoopMarkerProperty *marker = [NxALoopMarkerProperty loopMarkerWithName:@"TestLoopMarker"
                                                                     position:[NSDecimalNumber decimalNumberWithString:@"950.3424"]
                                                                       length:[NSDecimalNumber decimalNumberWithString:@"10.22"]
                                                                      context:self.context];
    NxAAssertNotNil(marker);
    NSXMLElement *entryElement = [NxATestCase xmlElementFromString:@"<ENTRY></ENTRY>"];

    // -- When adding the marker to the element.
    [marker addToTraktorEntryElement:entryElement log:self.mockLog];

    // -- Then make sure the element content matches the markers.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<ENTRY> \
                                    <CUE_V2 NAME=\"TestLoopMarker\" DISPL_ORDER=\"0\" TYPE=\"5\" START=\"950342.3750\" LEN=\"10220.0000\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                    </ENTRY>"];
    XCTAssertEqualObjects(entryElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testAddToTraktorEntryElement_AddLoopMarkerWithNoName_AddsCorrectly
{
    // -- Given a marker and an XML Element.
    NxALoopMarkerProperty *marker = [NxALoopMarkerProperty loopMarkerWithName:@""
                                                                     position:[NSDecimalNumber decimalNumberWithString:@"950.3424"]
                                                                       length:[NSDecimalNumber decimalNumberWithString:@"10.22"]
                                                                      context:self.context];
    NxAAssertNotNil(marker);
    NSXMLElement *entryElement = [NxATestCase xmlElementFromString:@"<ENTRY></ENTRY>"];

    // -- When adding the marker to the element.
    [marker addToTraktorEntryElement:entryElement log:self.mockLog];

    // -- Then make sure the element content matches the markers.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<ENTRY> \
                                    <CUE_V2 NAME=\"\" DISPL_ORDER=\"0\" TYPE=\"5\" START=\"950342.3750\" LEN=\"10220.0000\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                    </ENTRY>"];
    XCTAssertEqualObjects(entryElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testAddToTraktorEntryElement_AddLoopMarkerWithInvalidHotcue_AddsCorrectly
{
    // -- Given a marker and an track Element.
    NxALoopMarkerProperty *marker = [NxALoopMarkerProperty loopMarkerWithName:@"LoopMarkerTest"
                                                                     position:[NSDecimalNumber decimalNumberWithString:@"950.3424"]
                                                                       length:[NSDecimalNumber decimalNumberWithString:@"10.22"]
                                                                       hotCue:@(8)
                                                                      context:self.context];
    NxAAssertNotNil(marker);
    NSXMLElement *entryElement = [NxATestCase xmlElementFromString:@"<ENTRY></ENTRY>"];

    // -- When adding the marker to the element.
    [marker addToTraktorEntryElement:entryElement log:self.mockLog];

    // -- Then make sure the element content matches the markers.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<ENTRY> \
                                    <CUE_V2 NAME=\"LoopMarkerTest\" DISPL_ORDER=\"0\" TYPE=\"5\" START=\"950342.3750\" LEN=\"10220.0000\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                    </ENTRY>"];
    XCTAssertEqualObjects(entryElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testAddToTraktorEntryElement_AddLoopMarkerWithHotcue_AddsCorrectly
{
    // -- Given a marker and an track Element.
    NxALoopMarkerProperty *marker = [NxALoopMarkerProperty loopMarkerWithName:@"LoopMarkerTest"
                                                                     position:[NSDecimalNumber decimalNumberWithString:@"950.3424"]
                                                                       length:[NSDecimalNumber decimalNumberWithString:@"10.22"]
                                                                       hotCue:@(5)
                                                                      context:self.context];
    NxAAssertNotNil(marker);
    NSXMLElement *entryElement = [NxATestCase xmlElementFromString:@"<ENTRY></ENTRY>"];

    // -- When adding the marker to the element.
    [marker addToTraktorEntryElement:entryElement log:self.mockLog];

    // -- Then make sure the element content matches the markers.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<ENTRY> \
                                    <CUE_V2 NAME=\"LoopMarkerTest\" DISPL_ORDER=\"0\" TYPE=\"5\" START=\"950342.3750\" LEN=\"10220.0000\" REPEATS=\"-1\" HOTCUE=\"5\"></CUE_V2> \
                                    </ENTRY>"];
    XCTAssertEqualObjects(entryElement, expectedResult);

    assertThatNothingWasLogged();
}

@end
