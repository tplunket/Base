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

#include "Tags/Tag.hpp"
#include "Base/Test.hpp"

using namespace testing;
using namespace NxA;
using namespace NxA::Serato;

NXA_CONTAINS_TEST_SUITE_NAMED(SeratoDB_Tag_Tests);

TEST(SeratoDB_Tag, IdentifierForTagAt_TagDataWithAGivenIdentifier_ReturnsTheTagIdentifier)
{
    // -- Given.
    constexpr byte data[] = { 0x76, 0x72, 0x73, 0x6E, 0x00, 0x00, 0x00, 0x04, 0x54, 0xCA, 0x84, 0xAB };

    // -- When.
    auto identifier = Tag::identifierForTagAt(data);

    // -- Then.
    ASSERT_EQ('vrsn', identifier);
}

TEST(SeratoDB_Tag, NextTagAfterTagAt_TagDataWithTwoTags_ReturnsAPointerToTheSecondTag)
{
    // -- Given.
    constexpr byte data[] = {
        0x76, 0x72, 0x73, 0x6E, 0x00, 0x00, 0x00, 0x04, 0x54, 0xCA, 0x84, 0xAB,
        0x76, 0x72, 0x73, 0x6E, 0x00, 0x00, 0x00, 0x04, 0x54, 0xCA, 0x84, 0xAB
    };

    // -- When.
    auto nextTag = Tag::nextTagAfterTagAt(data);

    // -- Then.
    ASSERT_EQ(data + 12, nextTag);
}
