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

#include "Tags/PathTag.hpp"
#include "Base/Test.hpp"

using namespace testing;
using namespace NxA;
using namespace NxA::Serato;

NXA_CONTAINS_TEST_SUITE_NAMED(SeratoDB_PathTag_Tests);

static String::PointerToConst path = String::stringWith("Some/Cool/path.mp3");

TEST(SeratoDB_PathTag, TagWithIdentifierAndValue_TagCreated_TagHasCorrectValue)
{
    // -- Given.
    // -- When.
    auto tag = PathTag::tagWithIdentifierAndValue('ptst', path);

    // -- Then.
    ASSERT_EQ('ptst', tag->identifier());
    ASSERT_STREQ(*path, tag->value());
}

TEST(SeratoDB_PathTag, TagWithIdentifierAndValue_IncorrectIdentifier_ThrowsException)
{
    // -- Given.
    // -- When.
    // -- Then.
    ASSERT_THROW(PathTag::tagWithIdentifierAndValue('stst', path), NxA::Exception);
}

TEST(SeratoDB_PathTag, TagWithMemoryAt_TagCreated_TagHasCorrectValue)
{
    // -- Given.
    constexpr byte data[] = {
        0x70, 0x74, 0x73, 0x74, 0x00, 0x00, 0x00, 0x24, 0x00, 0x53, 0x00, 0x6F, 0x00, 0x6D, 0x00, 0x65, 0x00, 0x2F,
        0x00, 0x43, 0x00, 0x6F, 0x00, 0x6F, 0x00, 0x6C, 0x00, 0x2F, 0x00, 0x70, 0x00, 0x61, 0x00, 0x74, 0x00, 0x68,
        0x00, 0x2E, 0x00, 0x6D, 0x00, 0x70, 0x00, 0x33
    };

    // -- When.
    auto tag = PathTag::tagWithMemoryAt(data);

    // -- Then.
    ASSERT_EQ('ptst', tag->identifier());
    ASSERT_STREQ(*path, tag->value());
}

TEST(SeratoDB_PathTag, TagWithMemoryAt_IncorrectIdentifier_ThrowsException)
{
    // -- Given.
    constexpr byte data[] = {
        0x74, 0x74, 0x73, 0x74, 0x00, 0x00, 0x00, 0x24, 0x00, 0x53, 0x00, 0x6F, 0x00, 0x6D, 0x00, 0x65, 0x00, 0x2F,
        0x00, 0x43, 0x00, 0x6F, 0x00, 0x6F, 0x00, 0x6C, 0x00, 0x2F, 0x00, 0x70, 0x00, 0x61, 0x00, 0x74, 0x00, 0x68,
        0x00, 0x2E, 0x00, 0x6D, 0x00, 0x70, 0x00, 0x33
    };

    // -- When.
    ASSERT_THROW(PathTag::tagWithMemoryAt(data), NxA::Exception);
}

TEST(SeratoDB_PathTag, SetValue_TagWithAValue_SetsTheValueCorrectly)
{
    // -- Given.
    auto other = String::stringWith("Different/path.aiff");
    auto tag = PathTag::tagWithIdentifierAndValue('ptst', path);

    // -- When.
    tag->setValue(other);

    // -- Then.
    ASSERT_EQ('ptst', tag->identifier());
    ASSERT_STREQ(*other, tag->value());
}

TEST(SeratoDB_PathTag, AddTo_TagWrittenToBlob_WriteCorrectData)
{
    // -- Given.
    auto tag = PathTag::tagWithIdentifierAndValue('ptst', path);
    auto destination = Blob::blob();

    // -- When.
    tag->addTo(destination);

    // -- Then.
    auto data = destination->data();
    constexpr byte expectedData[] = {
        0x70, 0x74, 0x73, 0x74, 0x00, 0x00, 0x00, 0x24, 0x00, 0x53, 0x00, 0x6F, 0x00, 0x6D, 0x00, 0x65, 0x00, 0x2F,
        0x00, 0x43, 0x00, 0x6F, 0x00, 0x6F, 0x00, 0x6C, 0x00, 0x2F, 0x00, 0x70, 0x00, 0x61, 0x00, 0x74, 0x00, 0x68,
        0x00, 0x2E, 0x00, 0x6D, 0x00, 0x70, 0x00, 0x33
    };
    ASSERT_EQ(sizeof(expectedData), destination->size());
    ASSERT_EQ(0, ::memcmp(expectedData, data, sizeof(expectedData)));
}
