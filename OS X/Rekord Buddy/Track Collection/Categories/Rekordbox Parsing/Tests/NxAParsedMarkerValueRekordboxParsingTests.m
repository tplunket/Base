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
#import "NxAParsedMarkerValue+RekordboxParsing.h"

#pragma mark Private Interface

@interface NxAParsedMarkerValueRekordboxParsingTests : NxATestCase

@end

#pragma mark - Implementation

@implementation NxAParsedMarkerValueRekordboxParsingTests

#pragma mark Test methods

- (void)testParseAndSortRekordboxMarkerValuesInTrackElement_ParseOutOfOrderMarkers_ReturnsOrderedMarkers
{
    // -- Given a Rekordbox XML track element with markers out of order.
    NSXMLElement *element = [NxATestCase xmlElementFromString:@"<TRACK> \
                             <POSITION_MARK Name=\"Five\" Type=\"3\" Start=\"289.523\" Num=\"1\"/> \
                             <POSITION_MARK Name=\"Two\" Type=\"4\" Start=\"29.523\" End=\"59.523\" Num=\"-1\"/> \
                             <POSITION_MARK Name=\"Three\" Type=\"1\" Start=\"34.789\" Num=\"-1\"/> \
                             <TEMPO Inizio=\"0.123\" Bpm=\"126.00\" Metro=\"4/4\" Battito=\"1\"/> \
                             <POSITION_MARK Name=\"Four\" Type=\"2\" Start=\"60.952\" Num=\"2\"/> \
                             <POSITION_MARK Name=\"One\" Type=\"0\" Start=\"0.456\" Num=\"0\"/> \
                             </TRACK>"];

    // -- When parsing this element into marker objects.
    NSArray *values = [NxAParsedMarkerValue parseAndSortRekordboxMarkerValuesInTrackElement:element
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

- (void)testMarkerValueWithRekordboxPositionElement_ParseCueMarker_ParsesCorrectly
{
    // -- Given a Rekordbox XML marker element.
    NSXMLElement *element = [NxATestCase xmlElementFromString:@"<POSITION_MARK Name=\"TestCue\" Type=\"0\" Start=\"1.1\" Num=\"-1\"/>"];
    NSDecimalNumber *offset = [NSDecimalNumber zero];

    // -- When parsing this element into a marker object.
    NxAParsedMarkerValue *marker = [NxAParsedMarkerValue markerValueWithRekordboxPositionElement:element
                                                                                          offset:offset
                                                                                             log:self.mockLog];

    // -- Then make sure that the marker matches the source data.
    XCTAssertEqual(marker.type, NxAParsedMarkerValueCueType);
    XCTAssertEqualObjects(marker.positionInSeconds, [NSDecimalNumber decimalNumberWithString:@"1.1"]);
    XCTAssertEqualObjects(marker.name, @"TestCue");
    XCTAssertNil(marker.hotCueNumber);
    XCTAssertEqual(marker.customFlags.count, 0);

    assertThatNothingWasLogged();
}

- (void)testMarkerValueWithRekordboxPositionElement_ParseCueMarkerWithOffset_ParsesCorrectly
{
    // -- Given a Rekordbox XML marker element.
    NSXMLElement *element = [NxATestCase xmlElementFromString:@"<POSITION_MARK Name=\"TestCue\" Type=\"0\" Start=\"1.1\" Num=\"-1\"/>"];
    NSDecimalNumber *offset = [NSDecimalNumber decimalNumberWithString:@"1234.34"];

    // -- When parsing this element into a marker object.
    NxAParsedMarkerValue *marker = [NxAParsedMarkerValue markerValueWithRekordboxPositionElement:element
                                                                                          offset:offset
                                                                                             log:self.mockLog];

    // -- Then make sure that the marker matches the source data.
    XCTAssertEqual(marker.type, NxAParsedMarkerValueCueType);
    XCTAssertEqualObjects(marker.positionInSeconds, [NSDecimalNumber decimalNumberWithString:@"1235.44"]);
    XCTAssertEqualObjects(marker.name, @"TestCue");
    XCTAssertNil(marker.hotCueNumber);
    XCTAssertEqual(marker.customFlags.count, 0);

    assertThatNothingWasLogged();
}

- (void)testMarkerValueWithRekordboxPositionElement_ParseCueMarkerWithNoName_ParsesCorrectly
{
    // -- Given a Rekordbox XML marker element.
    NSXMLElement *element = [NxATestCase xmlElementFromString:@"<POSITION_MARK Name=\"\" Type=\"0\" Start=\"13.14\" Num=\"-1\"/>"];
    NSDecimalNumber *offset = [NSDecimalNumber zero];

    // -- When parsing this element into a marker object.
    NxAParsedMarkerValue *marker = [NxAParsedMarkerValue markerValueWithRekordboxPositionElement:element
                                                                                          offset:offset
                                                                                             log:self.mockLog];

    // -- Then make sure that the marker matches the source data.
    XCTAssertEqual(marker.type, NxAParsedMarkerValueCueType);
    XCTAssertEqualObjects(marker.positionInSeconds, [NSDecimalNumber decimalNumberWithString:@"13.14"]);
    XCTAssertNil(marker.name);
    XCTAssertNil(marker.hotCueNumber);
    XCTAssertEqual(marker.customFlags.count, 0);

    assertThatNothingWasLogged();
}

- (void)testMarkerValueWithRekordboxPositionElement_ParseMarkerWithHotCue_ParsesCorrectly
{
    // -- Given a Rekordbox XML marker element.
    NSXMLElement *element = [NxATestCase xmlElementFromString:@"<POSITION_MARK Name=\"TestHotCue\" Type=\"0\" Start=\"1.133\" Num=\"5\"/>"];
    NSDecimalNumber *offset = [NSDecimalNumber zero];

    // -- When parsing this element into a marker object.
    NxAParsedMarkerValue *marker = [NxAParsedMarkerValue markerValueWithRekordboxPositionElement:element
                                                                                          offset:offset
                                                                                             log:self.mockLog];

    // -- Then make sure that the marker matches the source data.
    XCTAssertEqual(marker.type, NxAParsedMarkerValueCueType);
    XCTAssertEqualObjects(marker.positionInSeconds, [NSDecimalNumber decimalNumberWithString:@"1.133"]);
    XCTAssertEqualObjects(marker.name, @"TestHotCue");
    XCTAssertEqualObjects(marker.hotCueNumber.stringValue, @"5");
    XCTAssertEqual(marker.customFlags.count, 0);

    assertThatNothingWasLogged();
}

- (void)testMarkerValueWithRekordboxPositionElement_ParseFadeInMarker_ParsesCorrectly
{
    // -- Given a Rekordbox XML marker element.
    NSXMLElement *element = [NxATestCase xmlElementFromString:@"<POSITION_MARK Name=\"TestFadeIn\" Type=\"1\" Start=\"2.2\" Num=\"-1\"/>"];
    NSDecimalNumber *offset = [NSDecimalNumber zero];

    // -- When parsing this element into a marker object.
    NxAParsedMarkerValue *marker = [NxAParsedMarkerValue markerValueWithRekordboxPositionElement:element
                                                                                          offset:offset
                                                                                             log:self.mockLog];

    // -- Then make sure that the marker matches the source data.
    XCTAssertEqual(marker.type, NxAParsedMarkerValueCueType);
    XCTAssertEqualObjects(marker.positionInSeconds, [NSDecimalNumber decimalNumberWithString:@"2.2"]);
    XCTAssertEqualObjects(marker.name, @"TestFadeIn");
    XCTAssertNil(marker.hotCueNumber);
    XCTAssertEqual(marker.customFlags.count, 1);
    XCTAssertTrue([marker hasCustomFlag:NxAMarkerIsFadeInMarker]);

    assertThatNothingWasLogged();
}

- (void)testMarkerValueWithRekordboxPositionElement_ParseFadeOutMarker_ParsesCorrectly
{
    // -- Given a Rekordbox XML marker element.
    NSXMLElement *element = [NxATestCase xmlElementFromString:@"<POSITION_MARK Name=\"TestFadeOut\" Type=\"2\" Start=\"3.3\" Num=\"-1\"/>"];
    NSDecimalNumber *offset = [NSDecimalNumber zero];

    // -- When parsing this element into a marker object.
    NxAParsedMarkerValue *marker = [NxAParsedMarkerValue markerValueWithRekordboxPositionElement:element
                                                                                          offset:offset
                                                                                             log:self.mockLog];

    // -- Then make sure that the marker matches the source data.
    XCTAssertEqual(marker.type, NxAParsedMarkerValueCueType);
    XCTAssertEqualObjects(marker.positionInSeconds, [NSDecimalNumber decimalNumberWithString:@"3.3"]);
    XCTAssertEqualObjects(marker.name, @"TestFadeOut");
    XCTAssertNil(marker.hotCueNumber);
    XCTAssertEqual(marker.customFlags.count, 1);
    XCTAssertTrue([marker hasCustomFlag:NxAMarkerIsFadeOutMarker]);

    assertThatNothingWasLogged();
}

- (void)testMarkerValueWithRekordboxPositionElement_ParseLoadMarker_ParsesCorrectly
{
    // -- Given a Rekordbox XML marker element.
    NSXMLElement *element = [NxATestCase xmlElementFromString:@"<POSITION_MARK Name=\"TestLoad\" Type=\"3\" Start=\"4.4\" Num=\"-1\"/>"];
    NSDecimalNumber *offset = [NSDecimalNumber zero];

    // -- When parsing this element into a marker object.
    NxAParsedMarkerValue *marker = [NxAParsedMarkerValue markerValueWithRekordboxPositionElement:element
                                                                                          offset:offset
                                                                                             log:self.mockLog];

    // -- Then make sure that the marker matches the source data.
    XCTAssertEqual(marker.type, NxAParsedMarkerValueCueType);
    XCTAssertEqualObjects(marker.positionInSeconds, [NSDecimalNumber decimalNumberWithString:@"4.4"]);
    XCTAssertEqualObjects(marker.name, @"TestLoad");
    XCTAssertNil(marker.hotCueNumber);
    XCTAssertEqual(marker.customFlags.count, 1);
    XCTAssertTrue([marker hasCustomFlag:NxAMarkerIsLoadMarker]);

    assertThatNothingWasLogged();
}

@end
