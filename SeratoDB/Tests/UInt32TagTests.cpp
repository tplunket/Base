//
//  Copyright (c) 2015 Next Audio Labs, LLC. All rights reserved.
//
//  This file contains confidential and proprietary information of Next
//  Audio Labs, LLC ("Next"). No use is permitted without express written
//  permission of Next. If you are not a party to a Confidentiality/
//  Non-Disclosure Agreement with Next, please immediately delete this
//  file as well as all copies in your possession. For further information,
//  please email info@nextaudiolabs.com.
//

#include <GoogleMock/GoogleMock.h>

#include "SeratoDB/MakeXcodeUseCppTests.hpp"
#include "SeratoDB/SeratoUint32Tag.h"

using namespace testing;
using namespace NxA;

XCODE_CPP_TEST_NAMED(SeratoUInt32Tests);

TEST(SeratoUInt32, TagCreatedFromNormalConstructor_HasCorrectValue)
{
    // -- When.
    SeratoUInt32Tag tag('vrsn', 0xDEADBEEF);

    // -- Then.
    ASSERT_EQ(tag.value(), 0xDEADBEEF);
}

TEST(SeratoUInt32, TagCreatedFromParsingConstructor_HasCorrectValue)
{
    // -- Given.
    unsigned char data[] = { 0x75, 0x61, 0x64, 0x64, 0x00, 0x00, 0x00, 0x04, 0x54, 0xCA, 0x84, 0xAB };

    // -- When.
    SeratoUInt32Tag tag(data);

    // -- Then.
    ASSERT_EQ(tag.value(), 0x54CA84AB);
}

TEST(SeratoUInt32, TagWrittenToData_HasCorrectOutput)
{
    // -- When.
    SeratoUInt32Tag tag('vrsn', 0xDEADBEEF);

    // -- When.
    CharVector destination;
    tag.addTo(destination);

    // -- Then.
    ASSERT_EQ(destination.size(), 12);
    unsigned char expectedData[] = { 0x76, 0x72, 0x73, 0x6E, 0x00, 0x00, 0x00, 0x04, 0xDE, 0xAD, 0xBE, 0xEF };
    for (uint32_t index = 0; index < 12; ++index) {
        ASSERT_EQ((unsigned char )destination[index], expectedData[index]);
    }
}
