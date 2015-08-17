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
#import "NxAParsedMarkerValue+RekordboxParsing.h"
#import "Track Collection/Categories/Rekordbox Parsing/Internal/NxAMarkerProperty+RekordboxParsing.h"

#pragma mark Private Interface

@interface NxALoopMarkerPropertyRekordboxParsingTests : NxALoopMarkerPropertyTests

@end

#pragma mark - Implementation

@implementation NxALoopMarkerPropertyRekordboxParsingTests

#pragma mark Test methods

- (void)testMarkerWithRekordboxPositionElement_ParseLoopMarker_ParsesCorrectly
{
    // -- Given a Rekordbox XML track element with a marker.
    NSXMLElement *element = [NxATestCase xmlElementFromString:@"<POSITION_MARK Name=\"TestLoop\" Type=\"4\" Start=\"5.5\" End=\"55.534\" Num=\"-1\"/>"];
    NSDecimalNumber *offset = [NSDecimalNumber zero];

    // -- When parsing this element into a marker object.
    NxAParsedMarkerValue *marker = [NxAParsedMarkerValue markerValueWithRekordboxPositionElement:element
                                                                                          offset:offset
                                                                                             log:self.mockLog];

    // -- Then make sure that the markers match the source data.
    XCTAssertEqual(marker.type, NxAParsedMarkerValueLoopType);
    XCTAssertEqualObjects(marker.positionInSeconds, [NSDecimalNumber decimalNumberWithString:@"5.5"]);
    XCTAssertEqualObjects(marker.lengthInSeconds, [NSDecimalNumber decimalNumberWithString:@"50.034"]);
    XCTAssertEqualObjects(marker.name, @"TestLoop");
    XCTAssertNil(marker.hotCueNumber);

    assertThatNothingWasLogged();
}

- (void)testMarkerWithRekordboxPositionElement_ParseLoopMarkerWithOffset_ParsesCorrectly
{
    // -- Given a Rekordbox XML track element with a marker.
    NSXMLElement *element = [NxATestCase xmlElementFromString:@"<POSITION_MARK Name=\"TestLoop\" Type=\"4\" Start=\"5.5\" End=\"55.534\" Num=\"-1\"/>"];
    NSDecimalNumber *offset = [NSDecimalNumber decimalNumberWithString:@"45.65"];

    // -- When parsing this element into a marker object.
    NxAParsedMarkerValue *marker = [NxAParsedMarkerValue markerValueWithRekordboxPositionElement:element
                                                                                          offset:offset
                                                                                             log:self.mockLog];

    // -- Then make sure that the markers match the source data.
    XCTAssertEqual(marker.type, NxAParsedMarkerValueLoopType);
    XCTAssertEqualObjects(marker.positionInSeconds, [NSDecimalNumber decimalNumberWithString:@"51.15"]);
    XCTAssertEqualObjects(marker.lengthInSeconds, [NSDecimalNumber decimalNumberWithString:@"50.034"]);
    XCTAssertEqualObjects(marker.name, @"TestLoop");
    XCTAssertNil(marker.hotCueNumber);

    assertThatNothingWasLogged();
}

- (void)testMarkerWithRekordboxPositionElement_ParseLoopMarkerWithNoName_ParsesCorrectly
{
    // -- Given a Rekordbox XML track element with a marker.
    NSXMLElement *element = [NxATestCase xmlElementFromString:@"<POSITION_MARK Name=\"\" Type=\"4\" Start=\"7.544\" End=\"5325354.872\" Num=\"-1\"/>"];
    NSDecimalNumber *offset = [NSDecimalNumber zero];

    // -- When parsing this element into a marker object.
    NxAParsedMarkerValue *marker = [NxAParsedMarkerValue markerValueWithRekordboxPositionElement:element
                                                                                          offset:offset
                                                                                             log:self.mockLog];

    // -- Then make sure that the markers match the source data.
    XCTAssertEqual(marker.type, NxAParsedMarkerValueLoopType);
    XCTAssertEqualObjects(marker.positionInSeconds, [NSDecimalNumber decimalNumberWithString:@"7.544"]);
    XCTAssertEqualObjects(marker.lengthInSeconds, [NSDecimalNumber decimalNumberWithString:@"5325347.328"]);
    XCTAssertNil(marker.name);
    XCTAssertNil(marker.hotCueNumber);

    assertThatNothingWasLogged();
}

- (void)testMarkerWithRekordboxPositionElement_ParseLoopMarkerWithHotCue_ParsesCorrectly
{
    // -- Given a Rekordbox XML track element with a marker.
    NSXMLElement *element = [NxATestCase xmlElementFromString:@"<POSITION_MARK Name=\"TestLoop\" Type=\"4\" Start=\"5.5\" End=\"55.534\" Num=\"7\"/>"];
    NSDecimalNumber *offset = [NSDecimalNumber zero];

    // -- When parsing this element into a marker object.
    NxAParsedMarkerValue *marker = [NxAParsedMarkerValue markerValueWithRekordboxPositionElement:element
                                                                                          offset:offset
                                                                                             log:self.mockLog];

    // -- Then make sure that the markers match the source data.
    XCTAssertEqual(marker.type, NxAParsedMarkerValueLoopType);
    XCTAssertEqualObjects(marker.positionInSeconds, [NSDecimalNumber decimalNumberWithString:@"5.5"]);
    XCTAssertEqualObjects(marker.lengthInSeconds, [NSDecimalNumber decimalNumberWithString:@"50.034"]);
    XCTAssertEqualObjects(marker.name, @"TestLoop");
    XCTAssertEqualObjects(marker.hotCueNumber, @(7));

    assertThatNothingWasLogged();
}

- (void)testp_RekordboxMarkerElementWithMarkerOffsetBy_LoopMarker_ReturnsCorrectElement
{
    // -- Given.
    NxALoopMarkerProperty *marker = [NxALoopMarkerProperty loopMarkerWithName:@"TestLoopMarker"
                                                                     position:[NSDecimalNumber decimalNumberWithString:@"950.3424"]
                                                                       length:[NSDecimalNumber decimalNumberWithString:@"10.22"]
                                                                      context:self.context];
    NxAAssertNotNil(marker);
    NSDecimalNumber *offset = [NSDecimalNumber zero];

    // -- When.
    NSXMLElement *result = [marker p_rekordboxMarkerElementWithMarkerOffsetBy:offset logMessagesIn:self.mockLog];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<POSITION_MARK Name=\"TestLoopMarker\" Start=\"950.342\" Type=\"4\" End=\"960.562\" Num=\"-1\"></POSITION_MARK>"];
    XCTAssertEqualObjects(result, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testp_RekordboxMarkerElementWithMarkerOffsetBy_LoopMarkerWithOffset_ReturnsCorrectElement
{
    // -- Given.
    NxALoopMarkerProperty *marker = [NxALoopMarkerProperty loopMarkerWithName:@"TestLoopMarker"
                                                                     position:[NSDecimalNumber decimalNumberWithString:@"950.3424"]
                                                                       length:[NSDecimalNumber decimalNumberWithString:@"10.22"]
                                                                      context:self.context];
    NxAAssertNotNil(marker);
    NSDecimalNumber *offset = [NSDecimalNumber decimalNumberWithString:@"12.043"];

    // -- When.
    NSXMLElement *result = [marker p_rekordboxMarkerElementWithMarkerOffsetBy:offset logMessagesIn:self.mockLog];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<POSITION_MARK Name=\"TestLoopMarker\" Start=\"962.385\" Type=\"4\" End=\"972.605\" Num=\"-1\"></POSITION_MARK>"];
    XCTAssertEqualObjects(result, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testp_RekordboxMarkerElementWithMarkerOffsetBy_LoopMarkerWithNoName_ReturnsCorrectElement
{
    // -- Given.
    NxALoopMarkerProperty *marker = [NxALoopMarkerProperty loopMarkerWithName:@""
                                                                     position:[NSDecimalNumber decimalNumberWithString:@"950.3424"]
                                                                       length:[NSDecimalNumber decimalNumberWithString:@"10.22"]
                                                                      context:self.context];
    NxAAssertNotNil(marker);
    NSDecimalNumber *offset = [NSDecimalNumber zero];

    // -- When.
    NSXMLElement *result = [marker p_rekordboxMarkerElementWithMarkerOffsetBy:offset logMessagesIn:self.mockLog];
    
    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<POSITION_MARK Name=\"\" Start=\"950.342\" Type=\"4\" End=\"960.562\" Num=\"-1\"></POSITION_MARK>"];
    XCTAssertEqualObjects(result, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testp_RekordboxMarkerElementWithMarkerOffsetBy_LoopMarkerWithInvalidHotcue_ReturnsCorrectElement
{
    // -- Given.
    NxALoopMarkerProperty *marker = [NxALoopMarkerProperty loopMarkerWithName:@"LoopMarkerTest"
                                                                     position:[NSDecimalNumber decimalNumberWithString:@"950.3424"]
                                                                       length:[NSDecimalNumber decimalNumberWithString:@"10.22"]
                                                                       hotCue:@(5)
                                                                      context:self.context];
    NxAAssertNotNil(marker);
    NSDecimalNumber *offset = [NSDecimalNumber zero];

    // -- When.
    NSXMLElement *result = [marker p_rekordboxMarkerElementWithMarkerOffsetBy:offset logMessagesIn:self.mockLog];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<POSITION_MARK Name=\"LoopMarkerTest\" Start=\"950.342\" Type=\"4\" End=\"960.562\" Num=\"-1\"></POSITION_MARK>"];
    XCTAssertEqualObjects(result, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testp_RekordboxMarkerElementWithMarkerOffsetBy_LoopMarkerWithHotcue_ReturnsCorrectElement
{
    // -- Given.
    NxALoopMarkerProperty *marker = [NxALoopMarkerProperty loopMarkerWithName:@"LoopMarkerTest"
                                                                     position:[NSDecimalNumber decimalNumberWithString:@"950.3424"]
                                                                       length:[NSDecimalNumber decimalNumberWithString:@"10.22"]
                                                                       hotCue:@(3)
                                                                      context:self.context];
    NxAAssertNotNil(marker);
    NSDecimalNumber *offset = [NSDecimalNumber zero];

    // -- When.
    NSXMLElement *result = [marker p_rekordboxMarkerElementWithMarkerOffsetBy:offset logMessagesIn:self.mockLog];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<POSITION_MARK Name=\"LoopMarkerTest\" Start=\"950.342\" Type=\"4\" End=\"960.562\" Num=\"3\"></POSITION_MARK>"];
    XCTAssertEqualObjects(result, expectedResult);

    assertThatNothingWasLogged();
}

@end
