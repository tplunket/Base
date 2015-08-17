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

#import "NxAMarkerPropertyTests.h"
#import "Track Collection/Categories/Rekordbox Parsing/Internal/NxAMarkerProperty+RekordboxParsing.h"
#import "NxAParsedMarkerValue+RekordboxParsing.h"
#import "Track Collection/Tags/Marker Properties/NxACueMarkerProperty.h"
#import "Track Collection/Tags/Marker Properties/NxAGridMarkerProperty.h"

#pragma mark Private Interface

@interface NxAMarkerPropertyRekordboxParsingTests : NxAMarkerPropertyTests

@end

#pragma mark - Implementation

@implementation NxAMarkerPropertyRekordboxParsingTests

#pragma mark Test methods

- (void)testp_RekordboxMarkerElementWithMarkerOffsetBy_RegularMarker_ReturnsCorrectElement
{
    // -- Given.
    NxACueMarkerProperty *marker = [NxACueMarkerProperty cueMarkerWithName:@"MarkerTest"
                                                                  position:[NSDecimalNumber decimalNumberWithString:@"450.022"]
                                                               customFlags:@[ ]
                                                                   context:self.context];
    NxAAssertNotNil(marker);
    NSDecimalNumber *offset = [NSDecimalNumber zero];

    // -- When.
    NSXMLElement *result = [marker p_rekordboxMarkerElementWithMarkerOffsetBy:offset logMessagesIn:self.mockLog];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<POSITION_MARK Name=\"MarkerTest\" Type=\"0\" Start=\"450.022\" Num=\"-1\"></POSITION_MARK>"];
    XCTAssertEqualObjects(result, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testp_RekordboxMarkerElementWithMarkerOffsetBy_MarkerWithOffset_ReturnsCorrectElement
{
    // -- Given.
    NxACueMarkerProperty *marker = [NxACueMarkerProperty cueMarkerWithName:@"MarkerTest2"
                                                                  position:[NSDecimalNumber decimalNumberWithString:@"450.022"]
                                                               customFlags:@[ ]
                                                                   context:self.context];
    NxAAssertNotNil(marker);
    NSDecimalNumber *offset = [NSDecimalNumber decimalNumberWithString:@"7.43"];

    // -- When.
    NSXMLElement *result = [marker p_rekordboxMarkerElementWithMarkerOffsetBy:offset logMessagesIn:self.mockLog];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<POSITION_MARK Name=\"MarkerTest2\" Type=\"0\" Start=\"457.452\" Num=\"-1\"></POSITION_MARK>"];
    XCTAssertEqualObjects(result, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testp_RekordboxMarkerElementWithMarkerOffsetBy_MarkerWithHotcue_ReturnsCorrectElement
{
    // -- Given.
    NxACueMarkerProperty *marker = [NxACueMarkerProperty cueMarkerWithName:@"MarkerTest3"
                                                                  position:[NSDecimalNumber decimalNumberWithString:@"34535.353"]
                                                                    hotCue:@(3)
                                                               customFlags:@[ ]
                                                                   context:self.context];
    NxAAssertNotNil(marker);
    NSDecimalNumber *offset = [NSDecimalNumber zero];

    // -- When.
    NSXMLElement *result = [marker p_rekordboxMarkerElementWithMarkerOffsetBy:offset logMessagesIn:self.mockLog];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<POSITION_MARK Name=\"MarkerTest3\" Type=\"0\" Start=\"34535.352\" Num=\"3\"></POSITION_MARK>"];
    XCTAssertEqualObjects(result, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testp_RekordboxMarkerElementWithMarkerOffsetBy_FadeInMarker_ReturnsCorrectElement
{
    // -- Given.
    NxACueMarkerProperty *marker = [NxACueMarkerProperty cueMarkerWithName:@"MarkerTest4"
                                                                  position:[NSDecimalNumber decimalNumberWithString:@"34535.353"]
                                                               customFlags:@[ NxAMarkerIsFadeInMarker ]
                                                                   context:self.context];
    NxAAssertNotNil(marker);
    NSDecimalNumber *offset = [NSDecimalNumber zero];

    // -- When.
    NSXMLElement *result = [marker p_rekordboxMarkerElementWithMarkerOffsetBy:offset logMessagesIn:self.mockLog];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<POSITION_MARK Name=\"MarkerTest4\" Type=\"1\" Start=\"34535.352\" Num=\"-1\"></POSITION_MARK>"];
    XCTAssertEqualObjects(result, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testp_RekordboxMarkerElementWithMarkerOffsetBy_FadeOutMarker_ReturnsCorrectElement
{
    // -- Given.
    NxACueMarkerProperty *marker = [NxACueMarkerProperty cueMarkerWithName:@"MarkerTest5"
                                                                  position:[NSDecimalNumber decimalNumberWithString:@"34535.353"]
                                                               customFlags:@[ NxAMarkerIsFadeOutMarker ]
                                                                   context:self.context];
    NxAAssertNotNil(marker);
    NSDecimalNumber *offset = [NSDecimalNumber zero];

    // -- When.
    NSXMLElement *result = [marker p_rekordboxMarkerElementWithMarkerOffsetBy:offset logMessagesIn:self.mockLog];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<POSITION_MARK Name=\"MarkerTest5\" Type=\"2\" Start=\"34535.352\" Num=\"-1\"></POSITION_MARK>"];
    XCTAssertEqualObjects(result, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testp_RekordboxMarkerElementWithMarkerOffsetBy_LoadMarker_ReturnsCorrectElement
{
    // -- Givent.
    NxACueMarkerProperty *marker = [NxACueMarkerProperty cueMarkerWithName:@"MarkerTest6"
                                                                  position:[NSDecimalNumber decimalNumberWithString:@"34535.353"]
                                                               customFlags:@[ NxAMarkerIsLoadMarker ]
                                                                   context:self.context];
    NxAAssertNotNil(marker);
    NSDecimalNumber *offset = [NSDecimalNumber zero];

    // -- When.
    NSXMLElement *result = [marker p_rekordboxMarkerElementWithMarkerOffsetBy:offset logMessagesIn:self.mockLog];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<POSITION_MARK Name=\"MarkerTest6\" Type=\"3\" Start=\"34535.352\" Num=\"-1\"></POSITION_MARK>"];
    XCTAssertEqualObjects(result, expectedResult);

    assertThatNothingWasLogged();
}

@end
