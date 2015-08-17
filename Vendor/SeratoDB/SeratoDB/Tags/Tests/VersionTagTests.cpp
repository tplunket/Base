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

#include "Tags/VersionTag.hpp"
#include "Base/Test.hpp"

using namespace testing;
using namespace NxA;
using namespace NxA::Serato;

NXA_CONTAINS_TEST_SUITE_NAMED(SeratoDB_VersionTag_Tests);

static String::PointerToConst version = String::stringWith("Cool Version");

TEST(SeratoDB_VersionTag, TagWithIdentifierAndValue_TagCreated_TagHasCorrectValue)
{
    // -- Given.
    // -- When.
    auto tag = VersionTag::tagWithIdentifierAndValue('vtst', version);

    // -- Then.
    ASSERT_EQ('vtst', tag->identifier());
    ASSERT_STREQ(version->toUTF8(), tag->value().toUTF8());
}

TEST(SeratoDB_VersionTag, TagWithIdentifierAndValue_IncorrectIdentifier_ThrowsException)
{
    // -- Given.
    // -- When.
    // -- Then.
    ASSERT_THROW(VersionTag::tagWithIdentifierAndValue('stst', version), NxA::Exception);
}

TEST(SeratoDB_VersionTag, TagWithMemoryAt_TagCreated_TagHasCorrectValue)
{
    // -- Given.
    constexpr byte data[] = {
        0x76, 0x74, 0x73, 0x74, 0x00, 0x00, 0x00, 0x18, 0x00, 0x43, 0x00, 0x6F, 0x00, 0x6F, 0x00, 0x6C,
        0x00, 0x20, 0x00, 0x56, 0x00, 0x65, 0x00, 0x72, 0x00, 0x73, 0x00, 0x69, 0x00, 0x6F, 0x00, 0x6E
    };

    // -- When.
    auto tag = VersionTag::tagWithMemoryAt(data);

    // -- Then.
    ASSERT_EQ('vtst', tag->identifier());
    ASSERT_STREQ(version->toUTF8(), tag->value().toUTF8());
}

TEST(SeratoDB_VersionTag, TagWithMemoryAt_IncorrectIdentifier_ThrowsException)
{
    // -- Given.
    constexpr byte data[] = {
        0x66, 0x74, 0x73, 0x74, 0x00, 0x00, 0x00, 0x18, 0x00, 0x43, 0x00, 0x6F, 0x00, 0x6F, 0x00, 0x6C,
        0x00, 0x20, 0x00, 0x56, 0x00, 0x65, 0x00, 0x72, 0x00, 0x73, 0x00, 0x69, 0x00, 0x6F, 0x00, 0x6E
    };

    // -- When.
    ASSERT_THROW(VersionTag::tagWithMemoryAt(data), NxA::Exception);
}

TEST(SeratoDB_VersionTag, OperatorEqual_TwoEqualTags_ReturnsTrue)
{
    // -- Given.
    auto tag = VersionTag::tagWithIdentifierAndValue('vtst', version);
    auto otherTag = VersionTag::tagWithIdentifierAndValue('vtst', String::stringWith("Cool Version"));

    // -- When.
    // -- Then.
    ASSERT_TRUE(*tag == *otherTag);
}

TEST(SeratoDB_VersionTag, OperatorEqual_TwoEqualTagsSameObject_ReturnsTrue)
{
    // -- Given.
    auto tag = VersionTag::tagWithIdentifierAndValue('vtst', version);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*tag == *tag);
}

TEST(SeratoDB_VersionTag, OperatorEqual_TwoUnEqualTags_Returnsfalse)
{
    // -- Given.
    auto tag = VersionTag::tagWithIdentifierAndValue('vtst', version);
    auto otherTag = VersionTag::tagWithIdentifierAndValue('vtst', String::stringWith("Some Other Version"));

    // -- When.
    // -- Then.
    ASSERT_FALSE(*tag == *otherTag);
}

TEST(SeratoDB_VersionTag, SetValue_TagWithAValue_SetsTheValueCorrectly)
{
    // -- Given.
    auto other = String::stringWith("Other Version");
    auto tag = VersionTag::tagWithIdentifierAndValue('vtst', version);

    // -- When.
    tag->setValue(other);

    // -- Then.
    ASSERT_EQ('vtst', tag->identifier());
    ASSERT_STREQ(other->toUTF8(), tag->value().toUTF8());
}

TEST(SeratoDB_VersionTag, AddTo_TagWrittenToBlob_WriteCorrectData)
{
    // -- Given.
    auto tag = VersionTag::tagWithIdentifierAndValue('vtst', version);
    auto destination = Blob::blob();

    // -- When.
    tag->addTo(destination);

    // -- Then.
    auto data = destination->data();
    constexpr byte expectedData[] = {
        0x76, 0x74, 0x73, 0x74, 0x00, 0x00, 0x00, 0x18, 0x00, 0x43, 0x00, 0x6F, 0x00, 0x6F, 0x00, 0x6C,
        0x00, 0x20, 0x00, 0x56, 0x00, 0x65, 0x00, 0x72, 0x00, 0x73, 0x00, 0x69, 0x00, 0x6F, 0x00, 0x6E
    };
    ASSERT_EQ(sizeof(expectedData), destination->size());
    ASSERT_EQ(0, ::memcmp(expectedData, data, sizeof(expectedData)));
}
