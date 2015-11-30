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

#include "Markers/LoopMarker.hpp"
#include "Base/Test.hpp"

using namespace testing;
using namespace NxA;
using namespace NxA::Serato;

NXA_CONTAINS_TEST_SUITE_NAMED(SeratoDB_LoopMarker_Tests);

TEST(SeratoDB_LoopMarker, markerWithMemoryAt_ASeratoMarkerTag_ReturnsACorrectMarker)
{
    // -- Given.
    constexpr byte data[] = {
        0x4C, 0x4F, 0x4F, 0x50, 0x00, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x02, 0x22, 0x47,
        0x00, 0x02, 0x31, 0x66, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x22, 0xff, 0xcc, 0x00, 0x00, 0x54, 0x45, 0x53, 0x54, 0x00
    };

    // -- When.
    auto test = LoopMarker::markerWithMemoryAt(data);

    // -- Then.
    ASSERT_EQ(139847, test->startPositionInMilliseconds());
    ASSERT_EQ(143718, test->endPositionInMilliseconds());
    ASSERT_EQ(0, test->index());
    ASSERT_STREQ("TEST", test->label().toUTF8());
    ASSERT_EQ(0x22, test->colorRedComponent());
    ASSERT_EQ(0xff, test->colorGreenComponent());
    ASSERT_EQ(0xcc, test->colorBlueComponent());
}

TEST(SeratoDB_LoopMarker, markerWithMemoryAt_ASeratoMarkerTagWithAnInvalidTagName_ThrowsAnException)
{
    // -- Given.
    constexpr byte data[] = {
        0x43, 0x55, 0x45, 0x00, 0x00, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x02, 0x22, 0x47,
        0x00, 0x02, 0x31, 0x66, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x27, 0xAA, 0xE1, 0x00, 0x00, 0x54, 0x45, 0x53, 0x54, 0x00
    };

    // -- When.
    // -- Then.
    ASSERT_THROW(LoopMarker::markerWithMemoryAt(data), NxA::AssertionFailed);
}

TEST(SeratoDB_LoopMarker, markerWithLabelStartEndPositionsIndexAndColor_AMarkerWithInitialValues_ReturnsACorrectMarker)
{
    // -- Given.
    // -- When.
    auto test = LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith("TEST"), 139847, 143718, 0, 0x22, 0xff, 0xcc);

    // -- Then.
    ASSERT_EQ(139847, test->startPositionInMilliseconds());
    ASSERT_EQ(143718, test->endPositionInMilliseconds());
    ASSERT_EQ(0, test->index());
    ASSERT_EQ(0x22, test->colorRedComponent());
    ASSERT_EQ(0xff, test->colorGreenComponent());
    ASSERT_EQ(0xcc, test->colorBlueComponent());
    ASSERT_STREQ("TEST", test->label().toUTF8());
}

TEST(SeratoDB_LoopMarker, markerWithLabelStartEndPositionsIndexAndColor_AMarkerWithInvalidStartEndValues_ThrowAnException)
{
    // -- Given.
    // -- When.
    // -- Then.
    ASSERT_THROW(LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith("TEST"), 159847, 143718, 0, 0x22, 0xff, 0xcc), NxA::Serato::LoopMarkerError);
}

TEST(SeratoDB_LoopMarker, markerWith_AMarkerAsSource_ReturnsACorrectMarker)
{
    // -- Given.
    auto initial = LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith("TEST"), 139847, 143718, 0, 0x22, 0xff, 0xcc);

    // -- When.
    auto test = LoopMarker::markerWith(initial);

    // -- Then.
    ASSERT_EQ(139847, test->startPositionInMilliseconds());
    ASSERT_EQ(143718, test->endPositionInMilliseconds());
    ASSERT_EQ(0, test->index());
    ASSERT_EQ(0x22, test->colorRedComponent());
    ASSERT_EQ(0xff, test->colorGreenComponent());
    ASSERT_EQ(0xcc, test->colorBlueComponent());
    ASSERT_STREQ("TEST", test->label().toUTF8());
}

TEST(SeratoDB_LoopMarker, OperatorEqual_TwoEqualMarkers_ReturnsTrue)
{
    // -- Given.
    auto test = LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith("TEST"), 139847, 143718, 0, 0x22, 0xff, 0xcc);
    auto other = LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith("TEST"), 139847, 143718, 0, 0x22, 0xff, 0xcc);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*test == *other);
}

TEST(SeratoDB_LoopMarker, OperatorEqual_TwoUnequalMarkersDifferentIndex_ReturnsFalse)
{
    // -- Given.
    auto test = LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith("TEST"), 139847, 143718, 0, 0x22, 0xff, 0xcc);
    auto other = LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith("TEST"), 139847, 143718, 2, 0x22, 0xff, 0xcc);

    // -- When.
    // -- Then.
    ASSERT_FALSE(*test == *other);
}

TEST(SeratoDB_LoopMarker, OperatorEqual_TwoUnequalMarkersDifferentLabel_ReturnsFalse)
{
    // -- Given.
    auto test = LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith("TEST"), 139847, 143718, 0, 0x22, 0xff, 0xcc);
    auto other = LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith("TEST2"), 139847, 143718, 0, 0x22, 0xff, 0xcc);

    // -- When.
    // -- Then.
    ASSERT_FALSE(*test == *other);
}

TEST(SeratoDB_LoopMarker, OperatorEqual_TwoUnequalMarkersDifferentStart_ReturnsFalse)
{
    // -- Given.
    auto test = LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith("TEST"), 139847, 143718, 0, 0x22, 0xff, 0xcc);
    auto other = LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith("TEST"), 139837, 143718, 0, 0x22, 0xff, 0xcc);

    // -- When.
    // -- Then.
    ASSERT_FALSE(*test == *other);
}

TEST(SeratoDB_LoopMarker, OperatorEqual_TwoUnequalMarkersDifferentEnd_ReturnsFalse)
{
    // -- Given.
    auto test = LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith("TEST"), 139847, 143718, 0, 0x22, 0xff, 0xcc);
    auto other = LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith("TEST"), 139847, 143720, 0, 0x22, 0xff, 0xcc);

    // -- When.
    // -- Then.
    ASSERT_FALSE(*test == *other);
}

TEST(SeratoDB_LoopMarker, OperatorEqual_TwoUnequalMarkersDifferentRed_ReturnsFalse)
{
    // -- Given.
    auto test = LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith("TEST"), 139847, 143718, 0, 0x22, 0xff, 0xcc);
    auto other = LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith("TEST"), 139847, 143718, 0, 0x21, 0xff, 0xcc);

    // -- When.
    // -- Then.
    ASSERT_FALSE(*test == *other);
}

TEST(SeratoDB_LoopMarker, OperatorEqual_TwoUnequalMarkersDifferentGreen_ReturnsFalse)
{
    // -- Given.
    auto test = LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith("TEST"), 139847, 143718, 0, 0x22, 0xff, 0xcc);
    auto other = LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith("TEST"), 139847, 143718, 0, 0x22, 0xef, 0xcc);

    // -- When.
    // -- Then.
    ASSERT_FALSE(*test == *other);
}

TEST(SeratoDB_LoopMarker, OperatorEqual_TwoUnequalMarkersDifferentBlue_ReturnsFalse)
{
    // -- Given.
    auto test = LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith("TEST"), 139847, 143718, 0, 0x22, 0xff, 0x00);
    auto other = LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith("TEST"), 139847, 143718, 0, 0x22, 0xff, 0xcc);

    // -- When.
    // -- Then.
    ASSERT_FALSE(*test == *other);
}

TEST(SeratoDB_LoopMarker, OperatorUnequal_TwoEqualMarkers_ReturnsFalse)
{
    // -- Given.
    auto test = LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith("TEST"), 139847, 143718, 0, 0x22, 0xff, 0xcc);
    auto other = LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith("TEST"), 139847, 143718, 0, 0x22, 0xff, 0xcc);

    // -- When.
    // -- Then.
    ASSERT_FALSE(*test != *other);
}

TEST(SeratoDB_LoopMarker, OperatorUnequal_TwoUnequalMarkersDifferentIndex_ReturnsTrue)
{
    // -- Given.
    auto test = LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith("TEST"), 139847, 143718, 0, 0x22, 0xff, 0xcc);
    auto other = LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith("TEST"), 139847, 143718, 2, 0x22, 0xff, 0xcc);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*test != *other);
}

TEST(SeratoDB_LoopMarker, OperatorUnequal_TwoUnequalMarkersDifferentLabel_ReturnsTrue)
{
    // -- Given.
    auto test = LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith("TEST"), 139847, 143718, 0, 0x22, 0xff, 0xcc);
    auto other = LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith("TEST2"), 139847, 143718, 0, 0x22, 0xff, 0xcc);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*test != *other);
}

TEST(SeratoDB_LoopMarker, OperatorUnequal_TwoUnequalMarkersDifferentStart_ReturnsTrue)
{
    // -- Given.
    auto test = LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith("TEST"), 139847, 143718, 0, 0x22, 0xff, 0x00);
    auto other = LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith("TEST"), 139843, 143718, 0, 0x22, 0xff, 0x00);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*test != *other);
}

TEST(SeratoDB_LoopMarker, OperatorUnequal_TwoUnequalMarkersDifferentEnd_ReturnsTrue)
{
    // -- Given.
    auto test = LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith("TEST"), 139847, 143718, 0, 0x22, 0xff, 0x00);
    auto other = LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith("TEST"), 139847, 143711, 0, 0x22, 0xff, 0x00);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*test != *other);
}

TEST(SeratoDB_LoopMarker, OperatorUnequal_TwoUnequalMarkersDifferentRed_ReturnsFalse)
{
    // -- Given.
    auto test = LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith("TEST"), 139847, 143718, 0, 0x22, 0xff, 0xcc);
    auto other = LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith("TEST"), 139847, 143718, 0, 0x21, 0xff, 0xcc);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*test != *other);
}

TEST(SeratoDB_LoopMarker, OperatorUnequal_TwoUnequalMarkersDifferentGreen_ReturnsFalse)
{
    // -- Given.
    auto test = LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith("TEST"), 139847, 143718, 0, 0x22, 0xff, 0xcc);
    auto other = LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith("TEST"), 139847, 143718, 0, 0x22, 0xef, 0xcc);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*test != *other);
}

TEST(SeratoDB_LoopMarker, OperatorUnequal_TwoUnequalMarkersDifferentBlue_ReturnsFalse)
{
    // -- Given.
    auto test = LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith("TEST"), 139847, 143718, 0, 0x22, 0xff, 0x00);
    auto other = LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith("TEST"), 139847, 143718, 0, 0x22, 0xff, 0xcc);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*test != *other);
}

TEST(SeratoDB_LoopMarker, addMarkerV2TagTo_AMarkerWithData_WritesTheCorrectData)
{
    // -- Given.
    auto destination = Blob::blob();
    auto test = LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith("TEST"), 139847, 143718, 0, 0x22, 0xff, 0xcc);
    
    // -- When.
    test->addMarkerV2TagTo(destination);
    
    // -- Then.
    auto data = destination->data();
    constexpr byte expectedData[] = {
        0x4C, 0x4F, 0x4F, 0x50, 0x00, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x02, 0x22, 0x47,
        0x00, 0x02, 0x31, 0x66, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x22, 0xff, 0xcc, 0x00, 0x00, 0x54, 0x45, 0x53, 0x54, 0x00
    };
    ASSERT_EQ(sizeof(expectedData), destination->size());
    ASSERT_EQ(0, ::memcmp(expectedData, data, sizeof(expectedData)));
}
