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
#import "NxAParsedMarkerValue+RekordboxParsing.h"
#import "Track Collection/Categories/Rekordbox Parsing/Internal/NxAMarkerProperty+RekordboxParsing.h"

#pragma mark Private Interface

@interface NxAGridMarkerPropertyRekordboxParsingTests : NxAGridMarkerPropertyTests

@end

#pragma mark - Implementation

@implementation NxAGridMarkerPropertyRekordboxParsingTests

#pragma mark Test methods

- (void)testGridMarkerWithRekordboxTempoElement_ParseGridMarker_ParsesCorrectly
{
    // -- Given a grid marker element.
    NSXMLElement *element = [NxATestCase xmlElementFromString:@"<TEMPO Inizio=\"23.23\" Bpm=\"126.34\" Metro=\"4/4\" Battito=\"1\"/>"];
    NSDecimalNumber *offset = [NSDecimalNumber zero];

    // -- When parsing this element into a marker object.
    NxAParsedMarkerValue *marker = [NxAParsedMarkerValue gridMarkerValueWithRekordboxTempoElement:element
                                                                                           offset:offset
                                                                                              log:self.mockLog];

    // -- Then make sure that the marker match the source data.
    XCTAssertEqual(marker.type, NxAParsedMarkerValueGridType);
    XCTAssertEqualObjects(marker.positionInSeconds, [NSDecimalNumber decimalNumberWithString:@"23.23"]);
    XCTAssertNil(marker.name);
    XCTAssertNil(marker.hotCueNumber);
    XCTAssertEqualObjects(marker.beatsPerMinute, [NSDecimalNumber decimalNumberWithString:@"126.34"]);

    assertThatNothingWasLogged();
}

- (void)testGridMarkerWithRekordboxTempoElement_ParseGridMarkerWithOffset_ParsesCorrectly
{
    // -- Given a grid marker element.
    NSXMLElement *element = [NxATestCase xmlElementFromString:@"<TEMPO Inizio=\"23.23\" Bpm=\"126.34\" Metro=\"4/4\" Battito=\"1\"/>"];
    NSDecimalNumber *offset = [NSDecimalNumber decimalNumberWithString:@"300.00"];

    // -- When parsing this element into a marker object.
    NxAParsedMarkerValue *marker = [NxAParsedMarkerValue gridMarkerValueWithRekordboxTempoElement:element
                                                                                           offset:offset
                                                                                              log:self.mockLog];

    // -- Then make sure that the marker match the source data.
    XCTAssertEqual(marker.type, NxAParsedMarkerValueGridType);
    XCTAssertEqualObjects(marker.positionInSeconds, [NSDecimalNumber decimalNumberWithString:@"323.23"]);
    XCTAssertNil(marker.name);
    XCTAssertNil(marker.hotCueNumber);
    XCTAssertEqualObjects(marker.beatsPerMinute, [NSDecimalNumber decimalNumberWithString:@"126.34"]);

    assertThatNothingWasLogged();
}

- (void)testp_RekordboxMarkerElementWithMarkerOffsetBy_GridMarker_ReturnsCorrectElement
{
    // -- Given.
    NxAGridMarkerProperty *marker = [NxAGridMarkerProperty gridMarkerAtPosition:[NSDecimalNumber decimalNumberWithString:@"450.022"]
                                                                            bpm:[NSDecimalNumber decimalNumberWithString:@"124.33"]
                                                                    customFlags:@[ ]
                                                                        context:self.context];
    NxAAssertNotNil(marker);
    NSDecimalNumber *offset = [NSDecimalNumber zero];

    // -- When.
    NSXMLElement *result = [marker p_rekordboxMarkerElementWithMarkerOffsetBy:offset logMessagesIn:self.mockLog];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<TEMPO Inizio=\"450.022\" Bpm=\"124.33\" Metro=\"4/4\" Battito=\"1\"> \
                                                                     </TEMPO>"];
    XCTAssertEqualObjects(result, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testp_RekordboxMarkerElementWithMarkerOffsetBy_GridMarkerWithOffset_ReturnsCorrectElement
{
    // -- Given.
    NxAGridMarkerProperty *marker = [NxAGridMarkerProperty gridMarkerAtPosition:[NSDecimalNumber decimalNumberWithString:@"450.022"]
                                                                            bpm:[NSDecimalNumber decimalNumberWithString:@"124.33"]
                                                                    customFlags:@[ ]
                                                                        context:self.context];
    NxAAssertNotNil(marker);
    NSDecimalNumber *offset = [NSDecimalNumber decimalNumberWithString:@"34.2"];

    // -- When.
    NSXMLElement *result = [marker p_rekordboxMarkerElementWithMarkerOffsetBy:offset logMessagesIn:self.mockLog];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<TEMPO Inizio=\"484.222\" Bpm=\"124.33\" Metro=\"4/4\" Battito=\"1\"> \
                                    </TEMPO>"];
    XCTAssertEqualObjects(result, expectedResult);

    assertThatNothingWasLogged();
}

@end
