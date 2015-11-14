//
//  Copyright (c) 2015 Next Audio Labs, LLC. All rights reserved.
//
//  This file contains confidential and proprietary information of Serato
//  Inc. LLP ("Serato"). No use is permitted without express written
//  permission of Serato. If you are not a party to a Confidentiality/
//  Non-Disclosure Agreement with Serato, please immediately delete this
//  file as well as all copies in your possession. For further information,
//  please refer to the modified MIT license provided with this library,
//  or email licensing@serato.com.
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
//  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
//  PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
//  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
        0x02, 0xDD, 0x66, 0x00, 0x45, 0xdc, 0x02, 0x00, 0x00, 0x54, 0x45, 0x53, 0x54, 0x00
    };

    // -- When.
    auto test = CueMarker::markerWithMemoryAt(data);

    // -- Then.
    ASSERT_EQ(187750, test->positionInMilliseconds());
    ASSERT_EQ(1, test->index());
    ASSERT_STREQ("TEST", test->label().toUTF8());
    ASSERT_EQ(0x45, test->colorRedComponent());
    ASSERT_EQ(0xdc, test->colorGreenComponent());
    ASSERT_EQ(0x02, test->colorBlueComponent());
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

TEST(SeratoDB_CueMarker, markerWithLabelPositionIndexAndColor_AMarkerWithInitialValues_ReturnsACorrectMarker)
{
    // -- Given.
    // -- When.
    auto test = CueMarker::markerWithLabelPositionIndexAndColor(String::stringWith("TEST"), 34985, 43, 0x45, 0xdc, 0x02);

    // -- Then.
    ASSERT_EQ(34985, test->positionInMilliseconds());
    ASSERT_EQ(43, test->index());
    ASSERT_STREQ("TEST", test->label().toUTF8());
    ASSERT_EQ(0x45, test->colorRedComponent());
    ASSERT_EQ(0xdc, test->colorGreenComponent());
    ASSERT_EQ(0x02, test->colorBlueComponent());
}

TEST(SeratoDB_CueMarker, markerWith_AMarkerAsSource_ReturnsACorrectMarker)
{
    // -- Given.
    auto initial = CueMarker::markerWithLabelPositionIndexAndColor(String::stringWith("TEST"), 34985, 43, 0x45, 0xdc, 0x02);

    // -- When.
    auto test = CueMarker::markerWith(initial);

    // -- Then.
    ASSERT_EQ(34985, test->positionInMilliseconds());
    ASSERT_EQ(43, test->index());
    ASSERT_STREQ("TEST", test->label().toUTF8());
    ASSERT_EQ(0x45, test->colorRedComponent());
    ASSERT_EQ(0xdc, test->colorGreenComponent());
    ASSERT_EQ(0x02, test->colorBlueComponent());
}

TEST(SeratoDB_CueMarker, OperatorEqual_TwoEqualMarkers_ReturnsTrue)
{
    // -- Given.
    auto test = CueMarker::markerWithLabelPositionIndexAndColor(String::stringWith("TEST"), 34985, 43, 0x45, 0xdc, 0x02);
    auto other = CueMarker::markerWithLabelPositionIndexAndColor(String::stringWith("TEST"), 34985, 43, 0x45, 0xdc, 0x02);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*test == *other);
}

TEST(SeratoDB_CueMarker, OperatorEqual_TwoUnequalMarkersDifferentIndex_ReturnsFalse)
{
    // -- Given.
    auto test = CueMarker::markerWithLabelPositionIndexAndColor(String::stringWith("TEST"), 34985, 43, 0x45, 0xdc, 0x02);
    auto other = CueMarker::markerWithLabelPositionIndexAndColor(String::stringWith("TEST"), 34985, 3, 0x45, 0xdc, 0x02);

    // -- When.
    // -- Then.
    ASSERT_FALSE(*test == *other);
}

TEST(SeratoDB_CueMarker, OperatorEqual_TwoUnequalMarkersDifferentLabel_ReturnsFalse)
{
    // -- Given.
    auto test = CueMarker::markerWithLabelPositionIndexAndColor(String::stringWith("TEST"), 34985, 43, 0x45, 0xdc, 0x02);
    auto other = CueMarker::markerWithLabelPositionIndexAndColor(String::stringWith("TEST2"), 34985, 43, 0x45, 0xdc, 0x02);

    // -- When.
    // -- Then.
    ASSERT_FALSE(*test == *other);
}

TEST(SeratoDB_CueMarker, OperatorEqual_TwoUnequalMarkersDifferentPos_ReturnsFalse)
{
    // -- Given.
    auto test = CueMarker::markerWithLabelPositionIndexAndColor(String::stringWith("TEST"), 34985, 43, 0x45, 0xdc, 0x02);
    auto other = CueMarker::markerWithLabelPositionIndexAndColor(String::stringWith("TEST"), 34984, 43, 0x45, 0xdc, 0x02);

    // -- When.
    // -- Then.
    ASSERT_FALSE(*test == *other);
}

TEST(SeratoDB_CueMarker, OperatorEqual_TwoUnequalMarkersDifferentRed_ReturnsFalse)
{
    // -- Given.
    auto test = CueMarker::markerWithLabelPositionIndexAndColor(String::stringWith("TEST"), 34985, 43, 0x45, 0xdc, 0x02);
    auto other = CueMarker::markerWithLabelPositionIndexAndColor(String::stringWith("TEST"), 34985, 43, 0x44, 0xdc, 0x02);

    // -- When.
    // -- Then.
    ASSERT_FALSE(*test == *other);
}

TEST(SeratoDB_CueMarker, OperatorEqual_TwoUnequalMarkersDifferentGreen_ReturnsFalse)
{
    // -- Given.
    auto test = CueMarker::markerWithLabelPositionIndexAndColor(String::stringWith("TEST"), 34985, 43, 0x45, 0xdc, 0x02);
    auto other = CueMarker::markerWithLabelPositionIndexAndColor(String::stringWith("TEST"), 34985, 43, 0x45, 0x00, 0x02);

    // -- When.
    // -- Then.
    ASSERT_FALSE(*test == *other);
}

TEST(SeratoDB_CueMarker, OperatorEqual_TwoUnequalMarkersDifferentBlue_ReturnsFalse)
{
    // -- Given.
    auto test = CueMarker::markerWithLabelPositionIndexAndColor(String::stringWith("TEST"), 34985, 43, 0x45, 0xdc, 0x02);
    auto other = CueMarker::markerWithLabelPositionIndexAndColor(String::stringWith("TEST"), 34985, 43, 0x45, 0xdc, 0x22);

    // -- When.
    // -- Then.
    ASSERT_FALSE(*test == *other);
}

TEST(SeratoDB_CueMarker, OperatorUnequal_TwoEqualMarkers_ReturnsFalse)
{
    // -- Given.
    auto test = CueMarker::markerWithLabelPositionIndexAndColor(String::stringWith("TEST"), 34985, 43, 0x45, 0xdc, 0x02);
    auto other = CueMarker::markerWithLabelPositionIndexAndColor(String::stringWith("TEST"), 34985, 43, 0x45, 0xdc, 0x02);

    // -- When.
    // -- Then.
    ASSERT_FALSE(*test != *other);
}

TEST(SeratoDB_CueMarker, OperatorUnequal_TwoUnequalMarkersDifferentIndex_ReturnsTrue)
{
    // -- Given.
    auto test = CueMarker::markerWithLabelPositionIndexAndColor(String::stringWith("TEST"), 34985, 43, 0x45, 0xdc, 0x02);
    auto other = CueMarker::markerWithLabelPositionIndexAndColor(String::stringWith("TEST"), 34985, 3, 0x45, 0xdc, 0x02);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*test != *other);
}

TEST(SeratoDB_CueMarker, OperatorUnequal_TwoUnequalMarkersDifferentLabel_ReturnsTrue)
{
    // -- Given.
    auto test = CueMarker::markerWithLabelPositionIndexAndColor(String::stringWith("TEST"), 34985, 43, 0x45, 0xdc, 0x02);
    auto other = CueMarker::markerWithLabelPositionIndexAndColor(String::stringWith("TEST2"), 34985, 43, 0x45, 0xdc, 0x02);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*test != *other);
}

TEST(SeratoDB_CueMarker, OperatorUnequal_TwoUnequalMarkersDifferentPosition_ReturnsTrue)
{
    // -- Given.
    auto test = CueMarker::markerWithLabelPositionIndexAndColor(String::stringWith("TEST"), 34985, 43, 0x45, 0xdc, 0x02);
    auto other = CueMarker::markerWithLabelPositionIndexAndColor(String::stringWith("TEST"), 34986, 43, 0x45, 0xdc, 0x02);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*test != *other);
}

TEST(SeratoDB_CueMarker, OperatorUnequal_TwoUnequalMarkersDifferentRed_ReturnsTrue)
{
    // -- Given.
    auto test = CueMarker::markerWithLabelPositionIndexAndColor(String::stringWith("TEST"), 34985, 43, 0x45, 0xdc, 0x02);
    auto other = CueMarker::markerWithLabelPositionIndexAndColor(String::stringWith("TEST"), 34985, 43, 0x44, 0xdc, 0x02);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*test != *other);
}

TEST(SeratoDB_CueMarker, OperatorUnequal_TwoUnequalMarkersDifferentGreen_ReturnsTrue)
{
    // -- Given.
    auto test = CueMarker::markerWithLabelPositionIndexAndColor(String::stringWith("TEST"), 34985, 43, 0x45, 0xdc, 0x02);
    auto other = CueMarker::markerWithLabelPositionIndexAndColor(String::stringWith("TEST"), 34985, 43, 0x45, 0x0c, 0x02);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*test != *other);
}

TEST(SeratoDB_CueMarker, OperatorUnequal_TwoUnequalMarkersDifferentBlue_ReturnsTrue)
{
    // -- Given.
    auto test = CueMarker::markerWithLabelPositionIndexAndColor(String::stringWith("TEST"), 34985, 43, 0x45, 0xdc, 0x02);
    auto other = CueMarker::markerWithLabelPositionIndexAndColor(String::stringWith("TEST"), 34985, 43, 0x45, 0xdc, 0x00);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*test != *other);
}

TEST(SeratoDB_CueMarker, addId3TagTo_AMarkerWithData_WritesTheCorrectData)
{
    // -- Given.
    auto destination = Blob::blob();
    auto test = CueMarker::markerWithLabelPositionIndexAndColor(String::stringWith("TEST"), 187750, 1, 0x45, 0xdc, 0x02);

    // -- When.
    test->addId3TagTo(destination);

    // -- Then.
    auto data = destination->data();
    constexpr byte expectedData[] = {
        0x43, 0x55, 0x45, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x01, 0x00,
        0x02, 0xDD, 0x66, 0x00, 0x45, 0xdc, 0x02, 0x00, 0x00, 0x54, 0x45, 0x53, 0x54, 0x00
    };
    ASSERT_EQ(sizeof(expectedData), destination->size());
    ASSERT_EQ(0, ::memcmp(expectedData, data, sizeof(expectedData)));
}
