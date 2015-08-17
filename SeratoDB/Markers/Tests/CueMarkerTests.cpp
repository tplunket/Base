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

#include "Markers/CueMarker.hpp"
#include "Base/Test.hpp"

using namespace testing;
using namespace NxA;
using namespace NxA::Serato;

NXA_CONTAINS_TEST_SUITE_NAMED(SeratoDB_CueMarker_Tests);

TEST(SeratoDB_CueMarker, markerWithMemoryAt_ASeratoMarkerTag_ReturnsACorrectMarker)
{
    // -- Given.
    constexpr byte data[] = {
        0x43, 0x55, 0x45, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x01, 0x00,
        0x02, 0xDD, 0x66, 0x00, 0xCC, 0x88, 0x00, 0x00, 0x00, 0x54, 0x45, 0x53, 0x54, 0x00
    };

    // -- When.
    auto test = CueMarker::markerWithMemoryAt(data);

    // -- Then.
    ASSERT_EQ(187750, test->positionInMilliseconds());
    ASSERT_EQ(1, test->index());
    ASSERT_STREQ("TEST", test->label().toUTF8());
}

TEST(SeratoDB_CueMarker, markerWithMemoryAt_ASeratoMarkerTagWithAnInvalidTagName_ThrowsAnException)
{
    // -- Given.
    constexpr byte data[] = {
        0x4C, 0x4F, 0x4F, 0x50, 0x00, 0x00, 0x00, 0x11, 0x00, 0x01, 0x00,
        0x02, 0xDD, 0x66, 0x00, 0xCC, 0x88, 0x00, 0x00, 0x00, 0x54, 0x45, 0x53, 0x54, 0x00
    };

    // -- When.
    // -- Then.
    ASSERT_THROW(CueMarker::markerWithMemoryAt(data), NxA::Exception);
}

TEST(SeratoDB_CueMarker, markerWithLabelPositionAndIndex_AMarkerWithInitialValues_ReturnsACorrectMarker)
{
    // -- Given.
    // -- When.
    auto test = CueMarker::markerWithLabelPositionAndIndex(String::stringWith("TEST"), 34985, 43);

    // -- Then.
    ASSERT_EQ(34985, test->positionInMilliseconds());
    ASSERT_EQ(43, test->index());
    ASSERT_STREQ("TEST", test->label().toUTF8());
}

TEST(SeratoDB_CueMarker, markerWith_AMarkerAsSource_ReturnsACorrectMarker)
{
    // -- Given.
    auto initial = CueMarker::markerWithLabelPositionAndIndex(String::stringWith("TEST"), 34985, 43);

    // -- When.
    auto test = CueMarker::markerWith(initial);

    // -- Then.
    ASSERT_EQ(34985, test->positionInMilliseconds());
    ASSERT_EQ(43, test->index());
    ASSERT_STREQ("TEST", test->label().toUTF8());
}

TEST(SeratoDB_CueMarker, OperatorEqual_TwoEqualMarkers_ReturnsTrue)
{
    // -- Given.
    auto test = CueMarker::markerWithLabelPositionAndIndex(String::stringWith("TEST"), 34985, 43);
    auto other = CueMarker::markerWithLabelPositionAndIndex(String::stringWith("TEST"), 34985, 43);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*test == *other);
}

TEST(SeratoDB_CueMarker, OperatorEqual_TwoUnequalMarkersDifferentIndex_ReturnsFalse)
{
    // -- Given.
    auto test = CueMarker::markerWithLabelPositionAndIndex(String::stringWith("TEST"), 34985, 43);
    auto other = CueMarker::markerWithLabelPositionAndIndex(String::stringWith("TEST"), 34985, 3);

    // -- When.
    // -- Then.
    ASSERT_FALSE(*test == *other);
}

TEST(SeratoDB_CueMarker, OperatorEqual_TwoUnequalMarkersDifferentLabel_ReturnsFalse)
{
    // -- Given.
    auto test = CueMarker::markerWithLabelPositionAndIndex(String::stringWith("TEST"), 34985, 43);
    auto other = CueMarker::markerWithLabelPositionAndIndex(String::stringWith("TEST2"), 34985, 43);

    // -- When.
    // -- Then.
    ASSERT_FALSE(*test == *other);
}

TEST(SeratoDB_CueMarker, OperatorEqual_TwoUnequalMarkersDifferentPos_ReturnsFalse)
{
    // -- Given.
    auto test = CueMarker::markerWithLabelPositionAndIndex(String::stringWith("TEST"), 34985, 43);
    auto other = CueMarker::markerWithLabelPositionAndIndex(String::stringWith("TEST"), 34984, 43);

    // -- When.
    // -- Then.
    ASSERT_FALSE(*test == *other);
}

TEST(SeratoDB_CueMarker, OperatorUnequal_TwoEqualMarkers_ReturnsFalse)
{
    // -- Given.
    auto test = CueMarker::markerWithLabelPositionAndIndex(String::stringWith("TEST"), 34985, 43);
    auto other = CueMarker::markerWithLabelPositionAndIndex(String::stringWith("TEST"), 34985, 43);

    // -- When.
    // -- Then.
    ASSERT_FALSE(*test != *other);
}

TEST(SeratoDB_CueMarker, OperatorUnequal_TwoUnequalMarkersDifferentIndex_ReturnsTrue)
{
    // -- Given.
    auto test = CueMarker::markerWithLabelPositionAndIndex(String::stringWith("TEST"), 34985, 43);
    auto other = CueMarker::markerWithLabelPositionAndIndex(String::stringWith("TEST"), 34985, 3);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*test != *other);
}

TEST(SeratoDB_CueMarker, OperatorUnequal_TwoUnequalMarkersDifferentLabel_ReturnsTrue)
{
    // -- Given.
    auto test = CueMarker::markerWithLabelPositionAndIndex(String::stringWith("TEST"), 34985, 43);
    auto other = CueMarker::markerWithLabelPositionAndIndex(String::stringWith("TEST2"), 34985, 43);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*test != *other);
}

TEST(SeratoDB_CueMarker, OperatorUnequal_TwoUnequalMarkersDifferentPosition_ReturnsTrue)
{
    // -- Given.
    auto test = CueMarker::markerWithLabelPositionAndIndex(String::stringWith("TEST"), 34985, 43);
    auto other = CueMarker::markerWithLabelPositionAndIndex(String::stringWith("TEST"), 34986, 43);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*test != *other);
}

TEST(SeratoDB_CueMarker, addId3TagTo_AMarkerWithData_WritesTheCorrectData)
{
    // -- Given.
    auto destination = Blob::blob();
    auto test = CueMarker::markerWithLabelPositionAndIndex(String::stringWith("TEST"), 187750, 1);

    // -- When.
    test->addId3TagTo(destination);

    // -- Then.
    auto data = destination->data();
    constexpr byte expectedData[] = {
        0x43, 0x55, 0x45, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x01, 0x00,
        0x02, 0xDD, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x54, 0x45, 0x53, 0x54, 0x00
    };
    ASSERT_EQ(sizeof(expectedData), destination->size());
    ASSERT_EQ(0, ::memcmp(expectedData, data, sizeof(expectedData)));
}
