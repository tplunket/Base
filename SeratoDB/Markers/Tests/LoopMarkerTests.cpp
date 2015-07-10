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
        0x00, 0x02, 0x31, 0x66, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x27, 0xAA, 0xE1, 0x00, 0x00, 0x54, 0x45, 0x53, 0x54, 0x00
    };

    // -- When.
    auto test = LoopMarker::markerWithMemoryAt(data);

    // -- Then.
    ASSERT_EQ(139847, test->startPositionInMilliseconds());
    ASSERT_EQ(143718, test->endPositionInMilliseconds());
    ASSERT_EQ(0, test->index());
    ASSERT_STREQ("TEST", test->label().toUTF8());
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
    ASSERT_THROW(LoopMarker::markerWithMemoryAt(data), NxA::Exception);
}

TEST(SeratoDB_LoopMarker, markerWithLabelStartEndPositionsAndIndex_AMarkerWithInitialValues_ReturnsACorrectMarker)
{
    // -- Given.
    // -- When.
    auto test = LoopMarker::markerWithLabelStartEndPositionsAndIndex(String::stringWith("TEST"), 139847, 143718, 0);

    // -- Then.
    ASSERT_EQ(139847, test->startPositionInMilliseconds());
    ASSERT_EQ(143718, test->endPositionInMilliseconds());
    ASSERT_EQ(0, test->index());
    ASSERT_STREQ("TEST", test->label().toUTF8());
}

TEST(SeratoDB_LoopMarker, markerWith_AMarkerAsSource_ReturnsACorrectMarker)
{
    // -- Given.
    auto initial = LoopMarker::markerWithLabelStartEndPositionsAndIndex(String::stringWith("TEST"), 139847, 143718, 0);

    // -- When.
    auto test = LoopMarker::markerWith(initial);

    // -- Then.
    ASSERT_EQ(139847, test->startPositionInMilliseconds());
    ASSERT_EQ(143718, test->endPositionInMilliseconds());
    ASSERT_EQ(0, test->index());
    ASSERT_STREQ("TEST", test->label().toUTF8());
}

TEST(SeratoDB_LoopMarker, OperatorEqual_TwoEqualMarkers_ReturnsTrue)
{
    // -- Given.
    auto test = LoopMarker::markerWithLabelStartEndPositionsAndIndex(String::stringWith("TEST"), 139847, 143718, 0);
    auto other = LoopMarker::markerWithLabelStartEndPositionsAndIndex(String::stringWith("TEST"), 139847, 143718, 0);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*test == *other);
}

TEST(SeratoDB_LoopMarker, OperatorEqual_TwoUnequalMarkersDifferentIndex_ReturnsFalse)
{
    // -- Given.
    auto test = LoopMarker::markerWithLabelStartEndPositionsAndIndex(String::stringWith("TEST"), 139847, 143718, 0);
    auto other = LoopMarker::markerWithLabelStartEndPositionsAndIndex(String::stringWith("TEST"), 139847, 143718, 2);

    // -- When.
    // -- Then.
    ASSERT_FALSE(*test == *other);
}

TEST(SeratoDB_LoopMarker, OperatorEqual_TwoUnequalMarkersDifferentLabel_ReturnsFalse)
{
    // -- Given.
    auto test = LoopMarker::markerWithLabelStartEndPositionsAndIndex(String::stringWith("TEST"), 139847, 143718, 0);
    auto other = LoopMarker::markerWithLabelStartEndPositionsAndIndex(String::stringWith("TEST2"), 139847, 143718, 0);

    // -- When.
    // -- Then.
    ASSERT_FALSE(*test == *other);
}

TEST(SeratoDB_LoopMarker, OperatorEqual_TwoUnequalMarkersDifferentStart_ReturnsFalse)
{
    // -- Given.
    auto test = LoopMarker::markerWithLabelStartEndPositionsAndIndex(String::stringWith("TEST"), 139847, 143718, 0);
    auto other = LoopMarker::markerWithLabelStartEndPositionsAndIndex(String::stringWith("TEST"), 139837, 143718, 0);

    // -- When.
    // -- Then.
    ASSERT_FALSE(*test == *other);
}

TEST(SeratoDB_LoopMarker, OperatorEqual_TwoUnequalMarkersDifferentEnd_ReturnsFalse)
{
    // -- Given.
    auto test = LoopMarker::markerWithLabelStartEndPositionsAndIndex(String::stringWith("TEST"), 139847, 143718, 0);
    auto other = LoopMarker::markerWithLabelStartEndPositionsAndIndex(String::stringWith("TEST"), 139847, 14318, 0);

    // -- When.
    // -- Then.
    ASSERT_FALSE(*test == *other);
}

TEST(SeratoDB_LoopMarker, OperatorUnequal_TwoEqualMarkers_ReturnsFalse)
{
    // -- Given.
    auto test = LoopMarker::markerWithLabelStartEndPositionsAndIndex(String::stringWith("TEST"), 139847, 143718, 0);
    auto other = LoopMarker::markerWithLabelStartEndPositionsAndIndex(String::stringWith("TEST"), 139847, 143718, 0);

    // -- When.
    // -- Then.
    ASSERT_FALSE(*test != *other);
}

TEST(SeratoDB_LoopMarker, OperatorUnequal_TwoUnequalMarkersDifferentIndex_ReturnsTrue)
{
    // -- Given.
    auto test = LoopMarker::markerWithLabelStartEndPositionsAndIndex(String::stringWith("TEST"), 139847, 143718, 0);
    auto other = LoopMarker::markerWithLabelStartEndPositionsAndIndex(String::stringWith("TEST"), 139847, 143718, 2);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*test != *other);
}

TEST(SeratoDB_LoopMarker, OperatorUnequal_TwoUnequalMarkersDifferentLabel_ReturnsTrue)
{
    // -- Given.
    auto test = LoopMarker::markerWithLabelStartEndPositionsAndIndex(String::stringWith("TEST"), 139847, 143718, 0);
    auto other = LoopMarker::markerWithLabelStartEndPositionsAndIndex(String::stringWith("TEST2"), 139847, 143718, 0);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*test != *other);
}

TEST(SeratoDB_LoopMarker, OperatorUnequal_TwoUnequalMarkersDifferentStart_ReturnsTrue)
{
    // -- Given.
    auto test = LoopMarker::markerWithLabelStartEndPositionsAndIndex(String::stringWith("TEST"), 139847, 143718, 0);
    auto other = LoopMarker::markerWithLabelStartEndPositionsAndIndex(String::stringWith("TEST"), 139843, 143718, 0);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*test != *other);
}

TEST(SeratoDB_LoopMarker, OperatorUnequal_TwoUnequalMarkersDifferentEnd_ReturnsTrue)
{
    // -- Given.
    auto test = LoopMarker::markerWithLabelStartEndPositionsAndIndex(String::stringWith("TEST"), 139847, 143718, 0);
    auto other = LoopMarker::markerWithLabelStartEndPositionsAndIndex(String::stringWith("TEST"), 139847, 143711, 0);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*test != *other);
}

TEST(SeratoDB_LoopMarker, addId3TagTo_AMarkerWithData_WritesTheCorrectData)
{
    // -- Given.
    auto destination = Blob::blob();
    auto test = LoopMarker::markerWithLabelStartEndPositionsAndIndex(String::stringWith("TEST"), 139847, 143718, 0);

    // -- When.
    test->addId3TagTo(destination);

    // -- Then.
    auto data = destination->data();
    constexpr byte expectedData[] = {
        0x4C, 0x4F, 0x4F, 0x50, 0x00, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x02, 0x22, 0x47,
        0x00, 0x02, 0x31, 0x66, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x54, 0x45, 0x53, 0x54, 0x00
    };
    ASSERT_EQ(sizeof(expectedData), destination->size());
    ASSERT_EQ(0, ::memcmp(expectedData, data, sizeof(expectedData)));
}
