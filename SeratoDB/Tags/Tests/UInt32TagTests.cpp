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

#include "Tags/UInteger32Tag.hpp"
#include "Base/Base.hpp"
#include "Base/Test.hpp"

using namespace testing;
using namespace NxA;
using namespace NxA::Serato;

NXA_CONTAINS_TEST_SUITE_NAMED(SeratoDB_UInteger32Tag_Tests);

TEST(SeratoDB_UInteger32Tag, TagCreatedFromNormalConstructor_HasCorrectValue)
{
    // -- When.
    auto tag = UInteger32Tag::tagWithIdentifierAndValue('vrsn', 0xDEADBEEF);

    // -- Then.
    ASSERT_EQ(tag->value(), 0xDEADBEEF);
}

TEST(SeratoDB_UInteger32Tag, TagCreatedFromParsingConstructor_HasCorrectValue)
{
    // -- Given.
    constexpr unsigned char data[] = { 0x75, 0x61, 0x64, 0x64, 0x00, 0x00, 0x00, 0x04, 0x54, 0xCA, 0x84, 0xAB };

    // -- When.
    auto tag = UInteger32Tag::tagWithMemoryAt(data);

    // -- Then.
    ASSERT_EQ(tag->value(), 0x54CA84AB);
}

TEST(SeratoDB_UInteger32Tag, TagWrittenToData_HasCorrectOutput)
{
    // -- When.
    auto tag = UInteger32Tag::tagWithIdentifierAndValue('vrsn', 0xDEADBEEF);

    // -- When.
    auto destination = Blob::blob();
    tag->addTo(destination);

    // -- Then.
    ASSERT_EQ(destination->size(), 12);
    constexpr uinteger8 expectedData[] = { 0x76, 0x72, 0x73, 0x6E, 0x00, 0x00, 0x00, 0x04, 0xDE, 0xAD, 0xBE, 0xEF };
    for (uint32_t index = 0; index < sizeof(expectedData); ++index) {
        ASSERT_EQ((*destination)[index], expectedData[index]);
    }
}
