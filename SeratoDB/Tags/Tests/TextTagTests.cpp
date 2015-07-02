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

#include "Tags/TextTag.hpp"
#include "Base/Test.hpp"

using namespace testing;
using namespace NxA;
using namespace NxA::Serato;

NXA_CONTAINS_TEST_SUITE_NAMED(SeratoDB_TextTag_Tests);

static String::PointerToConst text = String::stringWith("Some Text");

TEST(SeratoDB_TextTag, TagWithIdentifierAndValue_TagCreated_TagHasCorrectValue)
{
    // -- Given.
    // -- When.
    auto tag = TextTag::tagWithIdentifierAndValue('ttst', text);

    // -- Then.
    ASSERT_EQ('ttst', tag->identifier());
    ASSERT_STREQ(*text, tag->value());
}

TEST(SeratoDB_TextTag, TagWithIdentifierAndValue_IncorrectIdentifier_ThrowsException)
{
    // -- Given.
    // -- When.
    // -- Then.
    ASSERT_THROW(TextTag::tagWithIdentifierAndValue('stst', text), NxA::Exception);
}

TEST(SeratoDB_TextTag, TagWithMemoryAt_TagCreated_TagHasCorrectValue)
{
    // -- Given.
    constexpr byte data[] = {
        0x74, 0x74, 0x73, 0x74, 0x00, 0x00, 0x00, 0x12, 0x00, 0x53, 0x00, 0x6F, 0x00, 0x6D, 0x00, 0x65,
        0x00, 0x20, 0x00, 0x54, 0x00, 0x65, 0x00, 0x78, 0x00, 0x74
    };

    // -- When.
    auto tag = TextTag::tagWithMemoryAt(data);

    // -- Then.
    ASSERT_EQ('ttst', tag->identifier());
    ASSERT_STREQ(*text, tag->value());
}

TEST(SeratoDB_TextTag, TagWithMemoryAt_IncorrectIdentifier_ThrowsException)
{
    // -- Given.
    constexpr byte data[] = {
        0x14, 0x74, 0x73, 0x74, 0x00, 0x00, 0x00, 0x12, 0x00, 0x53, 0x00, 0x6F, 0x00, 0x6D, 0x00, 0x65,
        0x00, 0x20, 0x00, 0x54, 0x00, 0x65, 0x00, 0x78, 0x00, 0x74
    };

    // -- When.
    ASSERT_THROW(TextTag::tagWithMemoryAt(data), NxA::Exception);
}

TEST(SeratoDB_TextTag, SetValue_TagWithAValue_SetsTheValueCorrectly)
{
    // -- Given.
    auto other = String::stringWith("Different Text");
    auto tag = TextTag::tagWithIdentifierAndValue('ttst', text);

    // -- When.
    tag->setValue(other);

    // -- Then.
    ASSERT_EQ('ttst', tag->identifier());
    ASSERT_STREQ(*other, tag->value());
}

TEST(SeratoDB_TextTag, AddTo_TagWrittenToBlob_WriteCorrectData)
{
    // -- Given.
    auto tag = TextTag::tagWithIdentifierAndValue('ttst', text);
    auto destination = Blob::blob();

    // -- When.
    tag->addTo(destination);

    // -- Then.
    auto data = destination->data();
    constexpr byte expectedData[] = {
        0x74, 0x74, 0x73, 0x74, 0x00, 0x00, 0x00, 0x12, 0x00, 0x53, 0x00, 0x6F, 0x00, 0x6D, 0x00, 0x65,
        0x00, 0x20, 0x00, 0x54, 0x00, 0x65, 0x00, 0x78, 0x00, 0x74
    };
    ASSERT_EQ(sizeof(expectedData), destination->size());
    ASSERT_EQ(0, ::memcmp(expectedData, data, sizeof(expectedData)));
}
