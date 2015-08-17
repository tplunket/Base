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
#import "Track Collection/Parsers/NxATraktorParser.h"
#import "NxAMarkerProperty+TraktorParsing.h"

#pragma mark Private Interface

@interface NxACueMarkerPropertyTraktorParsingTests : NxAMarkerPropertyTests

@property (strong, nonatomic) id<NxAParserImplementation> mockParser;

@end

#pragma mark - Implementation

@implementation NxACueMarkerPropertyTraktorParsingTests

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

- (void)testAddToTraktorEntryElement_AddMarker_AddsCorrectly
{
    // -- Given a marker and a track element.
    NxAMarkerProperty *marker = [NxACueMarkerProperty cueMarkerWithName:@"MarkerTest"
                                                               position:[NSDecimalNumber decimalNumberWithString:@"450.022"]
                                                            customFlags:@[ ]
                                                                context:self.context];
    NxAAssertNotNil(marker);
    NSXMLElement *entryElement = [NxATestCase xmlElementFromString:@"<ENTRY></ENTRY>"];

    // -- When adding the marker to the element.
    [marker addToTraktorEntryElement:entryElement log:self.mockLog];

    // -- Then make sure the element content matches the marker.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<ENTRY> \
                                    <CUE_V2 NAME=\"MarkerTest\" DISPL_ORDER=\"0\" TYPE=\"0\" START=\"450022.0000\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                    </ENTRY>"];
    XCTAssertEqualObjects(entryElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testAddToTraktorEntryElement_AddMarkerWithInvalidHotcue_AddsCorrectly
{
    // -- Given a marker and a track element.
    NxAMarkerProperty *marker = [NxACueMarkerProperty cueMarkerWithName:@"MarkerTest3"
                                                               position:[NSDecimalNumber decimalNumberWithString:@"34535.353"]
                                                                 hotCue:@(8)
                                                            customFlags:@[ ]
                                                                context:self.context];
    NxAAssertNotNil(marker);
    NSXMLElement *entryElement = [NxATestCase xmlElementFromString:@"<ENTRY></ENTRY>"];

    // -- When adding the marker to the element.
    [marker addToTraktorEntryElement:entryElement log:self.mockLog];

    // -- Then make sure the element content matches the marker.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<ENTRY> \
                                    <CUE_V2 NAME=\"MarkerTest3\" DISPL_ORDER=\"0\" TYPE=\"0\" START=\"34535352.0000\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                    </ENTRY>"];
    XCTAssertEqualObjects(entryElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testAddToTraktorEntryElement_AddMarkerWithHotcue_AddsCorrectly
{
    // -- Given a marker and a track element.
    NxAMarkerProperty *marker = [NxACueMarkerProperty cueMarkerWithName:@"MarkerTest3"
                                                               position:[NSDecimalNumber decimalNumberWithString:@"34535.353"]
                                                                 hotCue:@(3)
                                                            customFlags:@[ ]
                                                                context:self.context];
    NxAAssertNotNil(marker);
    NSXMLElement *entryElement = [NxATestCase xmlElementFromString:@"<ENTRY></ENTRY>"];

    // -- When adding the marker to the element.
    [marker addToTraktorEntryElement:entryElement log:self.mockLog];

    // -- Then make sure the element content matches the marker.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<ENTRY> \
                                    <CUE_V2 NAME=\"MarkerTest3\" DISPL_ORDER=\"0\" TYPE=\"0\" START=\"34535352.0000\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"3\"></CUE_V2> \
                                    </ENTRY>"];
    XCTAssertEqualObjects(entryElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testAddToTraktorEntryElement_AddMarkerWithNoName_AddsCorrectly
{
    // -- Given a marker and a track element.
    NxAMarkerProperty *marker = [NxACueMarkerProperty cueMarkerWithName:@""
                                                               position:[NSDecimalNumber decimalNumberWithString:@"450.022"]
                                                            customFlags:@[ ]
                                                                context:self.context];
    NxAAssertNotNil(marker);
    NSXMLElement *entryElement = [NxATestCase xmlElementFromString:@"<ENTRY></ENTRY>"];

    // -- When adding the marker to the element.
    [marker addToTraktorEntryElement:entryElement log:self.mockLog];

    // -- Then make sure the element content matches the marker.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<ENTRY> \
                                    <CUE_V2 NAME=\"\" DISPL_ORDER=\"0\" TYPE=\"0\" START=\"450022.0000\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                    </ENTRY>"];
    XCTAssertEqualObjects(entryElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testAddToTraktorEntryElement_AddFadeInMarker_AddsCorrectly
{
    // -- Given a marker and a track element.
    NxAMarkerProperty *marker = [NxACueMarkerProperty cueMarkerWithName:@"MarkerTest4"
                                                               position:[NSDecimalNumber decimalNumberWithString:@"34535.353"]
                                                            customFlags:@[ NxAMarkerIsFadeInMarker ]
                                                                context:self.context];
    NxAAssertNotNil(marker);
    NSXMLElement *entryElement = [NxATestCase xmlElementFromString:@"<ENTRY></ENTRY>"];

    // -- When adding the marker to the element.
    [marker addToTraktorEntryElement:entryElement log:self.mockLog];

    // -- Then make sure the element content matches the marker.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<ENTRY> \
                                    <CUE_V2 NAME=\"MarkerTest4\" DISPL_ORDER=\"0\" TYPE=\"1\" START=\"34535352.0000\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                    </ENTRY>"];
    XCTAssertEqualObjects(entryElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testAddToTraktorEntryElement_AddFadeOutMarker_AddsCorrectly
{
    // -- Given a marker and a track element.
    NxAMarkerProperty *marker = [NxACueMarkerProperty cueMarkerWithName:@"MarkerTest5"
                                                               position:[NSDecimalNumber decimalNumberWithString:@"34535.353"]
                                                            customFlags:@[ NxAMarkerIsFadeOutMarker ]
                                                                context:self.context];
    NxAAssertNotNil(marker);
    NSXMLElement *entryElement = [NxATestCase xmlElementFromString:@"<ENTRY></ENTRY>"];

    // -- When adding the marker to the element.
    [marker addToTraktorEntryElement:entryElement log:self.mockLog];

    // -- Then make sure the element content matches the marker.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<ENTRY> \
                                    <CUE_V2 NAME=\"MarkerTest5\" DISPL_ORDER=\"0\" TYPE=\"2\" START=\"34535352.0000\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                    </ENTRY>"];
    XCTAssertEqualObjects(entryElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testAddToTraktorEntryElement_AddLoadMarker_AddsCorrectly
{
    // -- Given a marker and a track element.
    NxAMarkerProperty *marker = [NxACueMarkerProperty cueMarkerWithName:@"MarkerTest6"
                                                               position:[NSDecimalNumber decimalNumberWithString:@"34535.353"]
                                                            customFlags:@[ NxAMarkerIsLoadMarker ]
                                                                context:self.context];
    NxAAssertNotNil(marker);
    NSXMLElement *entryElement = [NxATestCase xmlElementFromString:@"<ENTRY></ENTRY>"];

    // -- When adding the marker to the element.
    [marker addToTraktorEntryElement:entryElement log:self.mockLog];

    // -- Then make sure the element content matches the marker.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<ENTRY> \
                                    <CUE_V2 NAME=\"MarkerTest6\" DISPL_ORDER=\"0\" TYPE=\"3\" START=\"34535352.0000\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                    </ENTRY>"];
    XCTAssertEqualObjects(entryElement, expectedResult);

    assertThatNothingWasLogged();
}

@end
