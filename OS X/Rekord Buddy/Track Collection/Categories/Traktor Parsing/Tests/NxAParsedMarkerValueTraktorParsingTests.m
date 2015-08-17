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

#import "NxAParsedMarkerValueTests.h"
#import "Track Collection/Parsers/NxATraktorParser.h"
#import "NxAParsedMarkerValue+TraktorParsing.h"

#pragma mark Private Interface

@interface NxAParsedMarkerValueTraktorParsingTests : NxAParsedMarkerValueTests

@end

#pragma mark Implementation

@implementation NxAParsedMarkerValueTraktorParsingTests

#pragma mark Test methods

- (void)testParseAndSortTraktorMarkerValuesInEntryElement_ParseOutOfOrderMarkers_ReturnsOrderedMarkers
{
    // -- Given a Traktor entry element with markers out of order.
    NSXMLElement *element = [NxATestCase xmlElementFromString:@"<ENTRY> \
                             <TEMPO BPM=\"123\" BPM_QUALITY=\"100\"></TEMPO> \
                             <CUE_V2 NAME=\"Five\" DISPL_ORDER=\"0\" TYPE=\"0\" START=\"289523\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"0\"></CUE_V2> \
                             <CUE_V2 NAME=\"Two\" DISPL_ORDER=\"0\" TYPE=\"0\" START=\"29523\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"0\"></CUE_V2> \
                             <CUE_V2 NAME=\"Three\" DISPL_ORDER=\"0\" TYPE=\"0\" START=\"34789\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"0\"></CUE_V2> \
                             <CUE_V2 NAME=\"\" DISPL_ORDER=\"0\" TYPE=\"4\" START=\"123\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                             <CUE_V2 NAME=\"Four\" DISPL_ORDER=\"0\" TYPE=\"0\" START=\"60952\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"0\"></CUE_V2> \
                             <CUE_V2 NAME=\"One\" DISPL_ORDER=\"0\" TYPE=\"0\" START=\"456\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"0\"></CUE_V2> \
                             </ENTRY>"];

    // -- When parsing this element into marker objects.
    NSArray *values = [NxAParsedMarkerValue parseAndSortTraktorMarkerValuesInEntryElement:element
                                                                                      log:self.mockLog];

    // -- Then make sure that the markers are all read and in order.
    XCTAssertEqual(values.count, 6, @"Not enough markers parsed.");

    NxAParsedMarkerValue *firstValue = values.firstObject;
    XCTAssertEqual(firstValue.type, NxAParsedMarkerValueGridType);
    XCTAssertEqualObjects([values[1] name], @"One");
    XCTAssertEqualObjects([values[2] name], @"Two");
    XCTAssertEqualObjects([values[3] name], @"Three");
    XCTAssertEqualObjects([values[4] name], @"Four");
    XCTAssertEqualObjects([values[5] name], @"Five");

    assertThatNothingWasLogged();
}

- (void)testp_MarkerValueWithTraktorCueV2Element_ParseCueMarker_ParsesCorrectly
{
    // -- Given a Traktor CUE V2 marker element.
    NSXMLElement *element = [NxATestCase xmlElementFromString:@"<CUE_V2 NAME=\"TestCue\" DISPL_ORDER=\"0\" TYPE=\"0\" START=\"1100\" \
                             LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2>"];

    // -- When parsing this element into a marker object.
    NSArray *result = [NxAParsedMarkerValue markerValuesWithTraktorCueV2Element:element
                                                                          tempo:nil
                                                                            log:self.mockLog];

    // -- Then make sure that the markers match the source data.
    XCTAssertEqual(result.count, 1);
    NxAParsedMarkerValue *marker = result.firstObject;
    XCTAssertEqual(marker.type, NxAParsedMarkerValueCueType);
    XCTAssertEqualObjects(marker.positionInSeconds, [NSDecimalNumber decimalNumberWithString:@"1.1"]);
    XCTAssertEqualObjects(marker.name, @"TestCue");
    XCTAssertNil(marker.hotCueNumber);
    XCTAssertEqual(marker.customFlags.count, 0);

    assertThatNothingWasLogged();
}

- (void)testp_MarkerValueWithTraktorCueV2Element_ParseCueMarkerWithNoName_ParsesCorrectly
{
    // -- Given a Traktor CUE V2 marker element.
    NSXMLElement *element = [NxATestCase xmlElementFromString:@"<CUE_V2 NAME=\"\" DISPL_ORDER=\"0\" TYPE=\"0\" START=\"13140\" \
                             LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2>"];

    // -- When parsing this element into a marker object.
    NSArray *result = [NxAParsedMarkerValue markerValuesWithTraktorCueV2Element:element
                                                                          tempo:nil
                                                                            log:self.mockLog];

    // -- Then make sure that the marker matches the source data.
    XCTAssertEqual(result.count, 1);
    NxAParsedMarkerValue *marker = result.firstObject;
    XCTAssertEqual(marker.type, NxAParsedMarkerValueCueType);
    XCTAssertEqualObjects(marker.positionInSeconds, [NSDecimalNumber decimalNumberWithString:@"13.14"]);
    XCTAssertNil(marker.name);
    XCTAssertNil(marker.hotCueNumber);
    XCTAssertEqual(marker.customFlags.count, 0);

    assertThatNothingWasLogged();
}

- (void)testp_MarkerValueWithTraktorCueV2Element_ParseCueMarkerWithHotCue_ParsesCorrectly
{
    // -- Given a Traktor CUE V2 marker element.
    NSXMLElement *element = [NxATestCase xmlElementFromString:@"<CUE_V2 NAME=\"TestHotCue\" DISPL_ORDER=\"0\" TYPE=\"0\" START=\"1133\" \
                             LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"5\"></CUE_V2>"];

    // -- When parsing this element into a marker object.
    NSArray *result = [NxAParsedMarkerValue markerValuesWithTraktorCueV2Element:element
                                                                          tempo:nil
                                                                            log:self.mockLog];

    // -- Then make sure that the marker matches the source data.
    XCTAssertEqual(result.count, 1);
    NxAParsedMarkerValue *marker = result.firstObject;
    XCTAssertEqual(marker.type, NxAParsedMarkerValueCueType);
    XCTAssertEqualObjects(marker.positionInSeconds, [NSDecimalNumber decimalNumberWithString:@"1.133"]);
    XCTAssertEqualObjects(marker.name, @"TestHotCue");
    XCTAssertEqualObjects(marker.hotCueNumber.stringValue, @"5");
    XCTAssertEqual(marker.customFlags.count, 0);

    assertThatNothingWasLogged();
}

- (void)testp_MarkerValueWithTraktorCueV2Element_ParseFadeInMarker_ParsesCorrectly
{
    // -- Given a Traktor CUE V2 marker element.
    NSXMLElement *element = [NxATestCase xmlElementFromString:@"<CUE_V2 NAME=\"TestFadeIn\" DISPL_ORDER=\"0\" TYPE=\"1\" START=\"2200\" \
                             LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2>"];

    // -- When parsing this element into a marker object.
    NSArray *result = [NxAParsedMarkerValue markerValuesWithTraktorCueV2Element:element
                                                                          tempo:nil
                                                                            log:self.mockLog];

    // -- Then make sure that the marker matches the source data.
    XCTAssertEqual(result.count, 1);
    NxAParsedMarkerValue *marker = result.firstObject;
    XCTAssertEqual(marker.type, NxAParsedMarkerValueCueType);
    XCTAssertEqualObjects(marker.positionInSeconds, [NSDecimalNumber decimalNumberWithString:@"2.2"]);
    XCTAssertEqualObjects(marker.name, @"TestFadeIn");
    XCTAssertNil(marker.hotCueNumber);
    XCTAssertEqual(marker.customFlags.count, 1);
    XCTAssertTrue([marker hasCustomFlag:NxAMarkerIsFadeInMarker]);

    assertThatNothingWasLogged();
}

- (void)testp_MarkerValueWithTraktorCueV2Element_ParseFadeOutMarker_ParsesCorrectly
{
    // -- Given a Traktor CUE V2 marker element.
    NSXMLElement *element = [NxATestCase xmlElementFromString:@"<CUE_V2 NAME=\"TestFadeOut\" DISPL_ORDER=\"0\" TYPE=\"2\" START=\"3300\" \
                             LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2>"];

    // -- When parsing this element into a marker object.
    NSArray *result = [NxAParsedMarkerValue markerValuesWithTraktorCueV2Element:element
                                                                          tempo:nil
                                                                            log:self.mockLog];

    // -- Then make sure that the marker matches the source data.
    XCTAssertEqual(result.count, 1);
    NxAParsedMarkerValue *marker = result.firstObject;
    XCTAssertEqual(marker.type, NxAParsedMarkerValueCueType);
    XCTAssertEqualObjects(marker.positionInSeconds, [NSDecimalNumber decimalNumberWithString:@"3.3"]);
    XCTAssertEqualObjects(marker.name, @"TestFadeOut");
    XCTAssertNil(marker.hotCueNumber);
    XCTAssertEqual(marker.customFlags.count, 1);
    XCTAssertTrue([marker hasCustomFlag:NxAMarkerIsFadeOutMarker]);

    assertThatNothingWasLogged();
}

- (void)testp_MarkerValueWithTraktorCueV2Element_ParseLoadMarker_ParsesCorrectly
{
    // -- Given a Traktor CUE V2 marker element.
    NSXMLElement *element = [NxATestCase xmlElementFromString:@"<CUE_V2 NAME=\"TestLoad\" DISPL_ORDER=\"0\" TYPE=\"3\" START=\"4400\" \
                             LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2>"];

    // -- When parsing this element into a marker object.
    NSArray *result = [NxAParsedMarkerValue markerValuesWithTraktorCueV2Element:element
                                                                          tempo:nil
                                                                            log:self.mockLog];

    // -- Then make sure that the marker matches the source data.
    XCTAssertEqual(result.count, 1);
    NxAParsedMarkerValue *marker = result.firstObject;
    XCTAssertEqual(marker.type, NxAParsedMarkerValueCueType);
    XCTAssertEqualObjects(marker.positionInSeconds, [NSDecimalNumber decimalNumberWithString:@"4.4"]);
    XCTAssertEqualObjects(marker.name, @"TestLoad");
    XCTAssertNil(marker.hotCueNumber);
    XCTAssertEqual(marker.customFlags.count, 1);
    XCTAssertTrue([marker hasCustomFlag:NxAMarkerIsLoadMarker]);

    assertThatNothingWasLogged();
}

@end
