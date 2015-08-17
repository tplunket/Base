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
#import "Track Collection/Tags/Marker Properties/NxACueMarkerProperty.h"
#import "Track Collection/Tags/Marker Properties/NxAGridMarkerProperty.h"
#import "Track Collection/Tags/Marker Properties/NxALoopMarkerProperty.h"

#pragma mark Private Interface

@interface NxAMarkerPropertyTraktorParsingTests : NxAMarkerPropertyTests

@property (strong, nonatomic) id<NxAParserImplementation> mockParser;

@end

#pragma mark - Implementation

@implementation NxAMarkerPropertyTraktorParsingTests

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

- (void)testUpdateTraktorMarkers_AGridMarkerAndACueMarkerWithAHotCue_AddsBothMarkers
{
    // -- Given.
    NxAGridMarkerProperty *gridMarker = [NxAGridMarkerProperty gridMarkerAtPosition:[NSDecimalNumber decimalNumberWithString:@"450.022"]
                                                                                bpm:[NSDecimalNumber decimalNumberWithString:@"124.353"]
                                                                        customFlags:@[ ]
                                                                            context:self.context];
    NxACueMarkerProperty *cueMarker = [NxACueMarkerProperty cueMarkerWithName:@"MarkerTest"
                                                                     position:[NSDecimalNumber decimalNumberWithString:@"450.022"]
                                                                       hotCue:@(3)
                                                                  customFlags:@[ ]
                                                                      context:self.context];
    NSArray *markers = @[ gridMarker, cueMarker ];
    NSXMLElement *entryElement = [NxATestCase xmlElementFromString:@"<ENTRY></ENTRY>"];

    // -- When.
    [NxAMarkerProperty updateTraktorMarkers:markers
                               entryElement:entryElement
                                        log:self.mockLog];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<ENTRY> \
                                    <TEMPO BPM=\"124.353\" BPM_QUALITY=\"100\"></TEMPO> \
                                    <CUE_V2 NAME=\"\" DISPL_ORDER=\"0\" TYPE=\"4\" START=\"450022.0000\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                    <CUE_V2 NAME=\"MarkerTest\" DISPL_ORDER=\"0\" TYPE=\"0\" START=\"450022.0000\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"3\"></CUE_V2> \
                                    </ENTRY>"];
    XCTAssertEqualObjects(entryElement, expectedResult);
    
    assertThatNothingWasLogged();
}

- (void)testUpdateTraktorMarkers_AGridMarkerThatCameFromAHotCueAndACueMarkerWithAHotCue_MergesBothMarkers
{
    // -- Given.
    NxAGridMarkerProperty *gridMarker = [NxAGridMarkerProperty gridMarkerAtPosition:[NSDecimalNumber decimalNumberWithString:@"450.022"]
                                                                                bpm:[NSDecimalNumber decimalNumberWithString:@"124.353"]
                                                                        customFlags:@[ NxAMarkerWasStoredAsAHotCue ]
                                                                            context:self.context];
    NxACueMarkerProperty *cueMarker = [NxACueMarkerProperty cueMarkerWithName:@"MarkerTest"
                                                                     position:[NSDecimalNumber decimalNumberWithString:@"450.022"]
                                                                       hotCue:@(3)
                                                                  customFlags:@[ ]
                                                                      context:self.context];
    NxACueMarkerProperty *otherMarker = [NxACueMarkerProperty cueMarkerWithName:@"MarkerTest2"
                                                                       position:[NSDecimalNumber decimalNumberWithString:@"460.022"]
                                                                         hotCue:@(1)
                                                                    customFlags:@[ ]
                                                                        context:self.context];
    NSArray *markers = @[ gridMarker, cueMarker, otherMarker ];
    NSXMLElement *entryElement = [NxATestCase xmlElementFromString:@"<ENTRY></ENTRY>"];

    // -- When.
    [NxAMarkerProperty updateTraktorMarkers:markers
                               entryElement:entryElement
                                        log:self.mockLog];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<ENTRY> \
                                    <TEMPO BPM=\"124.353\" BPM_QUALITY=\"100\"></TEMPO> \
                                    <CUE_V2 NAME=\"MarkerTest\" DISPL_ORDER=\"0\" TYPE=\"4\" START=\"450022.0000\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"3\"></CUE_V2> \
                                    <CUE_V2 NAME=\"MarkerTest2\" DISPL_ORDER=\"0\" TYPE=\"0\" START=\"460022.0000\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"1\"></CUE_V2> \
                                    </ENTRY>"];
    XCTAssertEqualObjects(entryElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testUpdateTraktorMarkers_AGridMarkerThatCameFromAHotCueAndACueMarkerWithAHotCueButNotAtTheRightPosition_AddBothMarkers
{
    // -- Given.
    NxAGridMarkerProperty *gridMarker = [NxAGridMarkerProperty gridMarkerAtPosition:[NSDecimalNumber decimalNumberWithString:@"450.022"]
                                                                                bpm:[NSDecimalNumber decimalNumberWithString:@"124.353"]
                                                                        customFlags:@[ NxAMarkerWasStoredAsAHotCue ]
                                                                            context:self.context];
    NxACueMarkerProperty *cueMarker = [NxACueMarkerProperty cueMarkerWithName:@"MarkerTest"
                                                                     position:[NSDecimalNumber decimalNumberWithString:@"460.022"]
                                                                       hotCue:@(3)
                                                                  customFlags:@[ ]
                                                                      context:self.context];
    NSArray *markers = @[ gridMarker, cueMarker ];
    NSXMLElement *entryElement = [NxATestCase xmlElementFromString:@"<ENTRY></ENTRY>"];

    // -- When.
    [NxAMarkerProperty updateTraktorMarkers:markers
                               entryElement:entryElement
                                        log:self.mockLog];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<ENTRY> \
                                    <TEMPO BPM=\"124.353\" BPM_QUALITY=\"100\"></TEMPO> \
                                    <CUE_V2 NAME=\"\" DISPL_ORDER=\"0\" TYPE=\"4\" START=\"450022.0000\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                    <CUE_V2 NAME=\"MarkerTest\" DISPL_ORDER=\"0\" TYPE=\"0\" START=\"460022.0000\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"3\"></CUE_V2> \
                                    </ENTRY>"];
    XCTAssertEqualObjects(entryElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testUpdateTraktorMarkers_AGridMarkerThatCameFromAHotCueAndALoopMarker_AddBothMarkers
{
    // -- Given.
    NxAGridMarkerProperty *gridMarker = [NxAGridMarkerProperty gridMarkerAtPosition:[NSDecimalNumber decimalNumberWithString:@"450.022"]
                                                                                bpm:[NSDecimalNumber decimalNumberWithString:@"124.353"]
                                                                        customFlags:@[ NxAMarkerWasStoredAsAHotCue ]
                                                                            context:self.context];
    NxALoopMarkerProperty *loopMarker = [NxALoopMarkerProperty loopMarkerWithName:@"LoopMarkerTest"
                                                                         position:[NSDecimalNumber decimalNumberWithString:@"460.022"]
                                                                           length:[NSDecimalNumber decimalNumberWithString:@"10.0"]
                                                                           hotCue:@(3)
                                                                          context:self.context];
    NSArray *markers = @[ gridMarker, loopMarker ];
    NSXMLElement *entryElement = [NxATestCase xmlElementFromString:@"<ENTRY></ENTRY>"];

    // -- When.
    [NxAMarkerProperty updateTraktorMarkers:markers
                               entryElement:entryElement
                                        log:self.mockLog];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<ENTRY> \
                                    <TEMPO BPM=\"124.353\" BPM_QUALITY=\"100\"></TEMPO> \
                                    <CUE_V2 NAME=\"\" DISPL_ORDER=\"0\" TYPE=\"4\" START=\"450022.0000\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                    <CUE_V2 NAME=\"LoopMarkerTest\" DISPL_ORDER=\"0\" TYPE=\"5\" START=\"460022.0000\" LEN=\"10000.0000\" REPEATS=\"-1\" HOTCUE=\"3\"></CUE_V2> \
                                    </ENTRY>"];
    XCTAssertEqualObjects(entryElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testUpdateTraktorMarkers_AGridMarkerThatCameFromAHotCueAndAGridMarker_AddBothMarkers
{
    // -- Given.
    NxAGridMarkerProperty *gridMarker = [NxAGridMarkerProperty gridMarkerAtPosition:[NSDecimalNumber decimalNumberWithString:@"450.022"]
                                                                                bpm:[NSDecimalNumber decimalNumberWithString:@"124.353"]
                                                                        customFlags:@[ NxAMarkerWasStoredAsAHotCue ]
                                                                            context:self.context];
    NxAGridMarkerProperty *gridMarker2 = [NxAGridMarkerProperty gridMarkerAtPosition:[NSDecimalNumber decimalNumberWithString:@"550.022"]
                                                                                 bpm:[NSDecimalNumber decimalNumberWithString:@"124.353"]
                                                                         customFlags:@[ ]
                                                                             context:self.context];
    NSArray *markers = @[ gridMarker, gridMarker2 ];
    NSXMLElement *entryElement = [NxATestCase xmlElementFromString:@"<ENTRY></ENTRY>"];

    // -- When.
    [NxAMarkerProperty updateTraktorMarkers:markers
                               entryElement:entryElement
                                        log:self.mockLog];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<ENTRY> \
                                    <TEMPO BPM=\"124.353\" BPM_QUALITY=\"100\"></TEMPO> \
                                    <CUE_V2 NAME=\"\" DISPL_ORDER=\"0\" TYPE=\"4\" START=\"450022.0000\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                    <CUE_V2 NAME=\"\" DISPL_ORDER=\"0\" TYPE=\"4\" START=\"550022.0000\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                    </ENTRY>"];
    XCTAssertEqualObjects(entryElement, expectedResult);

    assertThatNothingWasLogged();
}

- (void)testUpdateTraktorMarkers_AGridMarkerThatCameFromAHotCueAndACueMarkerWithNoHotcue_AddsBothMarkers
{
    // -- Given.
    NxAGridMarkerProperty *gridMarker = [NxAGridMarkerProperty gridMarkerAtPosition:[NSDecimalNumber decimalNumberWithString:@"450.022"]
                                                                                bpm:[NSDecimalNumber decimalNumberWithString:@"124.353"]
                                                                        customFlags:@[ NxAMarkerWasStoredAsAHotCue ]
                                                                            context:self.context];
    NxACueMarkerProperty *cueMarker = [NxACueMarkerProperty cueMarkerWithName:@"MarkerTest"
                                                                     position:[NSDecimalNumber decimalNumberWithString:@"450.022"]
                                                                       hotCue:nil
                                                                  customFlags:@[ ]
                                                                      context:self.context];
    NSArray *markers = @[ gridMarker, cueMarker ];
    NSXMLElement *entryElement = [NxATestCase xmlElementFromString:@"<ENTRY></ENTRY>"];

    // -- When.
    [NxAMarkerProperty updateTraktorMarkers:markers
                               entryElement:entryElement
                                        log:self.mockLog];

    // -- Then.
    NSXMLElement *expectedResult = [NxATestCase xmlElementFromString:@"<ENTRY> \
                                    <TEMPO BPM=\"124.353\" BPM_QUALITY=\"100\"></TEMPO> \
                                    <CUE_V2 NAME=\"\" DISPL_ORDER=\"0\" TYPE=\"4\" START=\"450022.0000\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                    <CUE_V2 NAME=\"MarkerTest\" DISPL_ORDER=\"0\" TYPE=\"0\" START=\"450022.0000\" LEN=\"0\" REPEATS=\"-1\" HOTCUE=\"-1\"></CUE_V2> \
                                    </ENTRY>"];
    XCTAssertEqualObjects(entryElement, expectedResult);

    assertThatNothingWasLogged();
}

@end
