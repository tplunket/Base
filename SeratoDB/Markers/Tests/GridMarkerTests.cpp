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

#include "Markers/GridMarker.hpp"
#include "Base/Test.hpp"

using namespace testing;
using namespace NxA;
using namespace NxA::Serato;

NXA_CONTAINS_TEST_SUITE_NAMED(SeratoDB_GridMarker_Tests);

TEST(SeratoDB_GridMarker, markerWithMemoryAt_ASeratoMarkerTag_ReturnsACorrectMarker)
{
    // -- Given.
    constexpr byte data[] = {
        0x00, 0x00, 0x00, 0x1, 0x3C, 0x08, 0xE7, 0xA4, 0x42, 0xF8, 0x00, 0x00
    };

    // -- When.
    auto test = GridMarker::markersWithMemoryAt(data);

    // -- Then.
    ASSERT_EQ(1, test->length());
    auto& marker = test->firstObject();
    ASSERT_EQ(0.0083560086786746978, marker.positionInSeconds());
    ASSERT_EQ(124, marker.beatsPerMinute());
}

TEST(SeratoDB_GridMarker, markerWithPositionAndBeatsPerMinute_AMarkerWithInitialValues_ReturnsACorrectMarker)
{
    // -- Given.
    // -- When.
    auto test = GridMarker::markerWithPositionAndBeatsPerMinute(0.0083560086786746978, 124);

    // -- Then.
    ASSERT_EQ(0.0083560086786746978, test->positionInSeconds());
    ASSERT_EQ(124, test->beatsPerMinute());
}

TEST(SeratoDB_GridMarker, markerWith_AMarkerAsSource_ReturnsACorrectMarker)
{
    // -- Given.
    auto initial = GridMarker::markerWithPositionAndBeatsPerMinute(0.0083560086786746978, 124);

    // -- When.
    auto test = GridMarker::markerWith(initial);

    // -- Then.
    ASSERT_EQ(0.0083560086786746978, test->positionInSeconds());
    ASSERT_EQ(124, test->beatsPerMinute());
}

TEST(SeratoDB_GridMarker, OperatorEqual_TwoEqualMarkers_ReturnsTrue)
{
    // -- Given.
    auto test = GridMarker::markerWithPositionAndBeatsPerMinute(0.0083560086786746978, 124);
    auto other = GridMarker::markerWithPositionAndBeatsPerMinute(0.0083560086786746978, 124);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*test == *other);
}

TEST(SeratoDB_GridMarker, OperatorEqual_TwoUnequalMarkersDifferentPosition_ReturnsFalse)
{
    // -- Given.
    auto test = GridMarker::markerWithPositionAndBeatsPerMinute(0.0083560086786746978, 124);
    auto other = GridMarker::markerWithPositionAndBeatsPerMinute(0.0083560086786746278, 124);

    // -- When.
    // -- Then.
    ASSERT_FALSE(*test == *other);
}

TEST(SeratoDB_GridMarker, OperatorEqual_TwoUnequalMarkersDifferentBpm_ReturnsFalse)
{
    // -- Given.
    auto test = GridMarker::markerWithPositionAndBeatsPerMinute(0.0083560086786746978, 124);
    auto other = GridMarker::markerWithPositionAndBeatsPerMinute(0.0083560086786746978, 123);

    // -- When.
    // -- Then.
    ASSERT_FALSE(*test == *other);
}

TEST(SeratoDB_GridMarker, OperatorUnequal_TwoEqualMarkers_ReturnsFalse)
{
    // -- Given.
    auto test = GridMarker::markerWithPositionAndBeatsPerMinute(0.0083560086786746978, 124);
    auto other = GridMarker::markerWithPositionAndBeatsPerMinute(0.0083560086786746978, 124);

    // -- When.
    // -- Then.
    ASSERT_FALSE(*test != *other);
}

TEST(SeratoDB_GridMarker, OperatorUnequal_TwoUnequalMarkersDifferentPosition_ReturnsTrue)
{
    // -- Given.
    auto test = GridMarker::markerWithPositionAndBeatsPerMinute(0.0083560086786746978, 124);
    auto other = GridMarker::markerWithPositionAndBeatsPerMinute(0.0083560086786746278, 124);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*test != *other);
}

TEST(SeratoDB_GridMarker, OperatorUnequal_TwoUnequalMarkersDifferentBpm_ReturnsTrue)
{
    // -- Given.
    auto test = GridMarker::markerWithPositionAndBeatsPerMinute(0.0083560086786746978, 124);
    auto other = GridMarker::markerWithPositionAndBeatsPerMinute(0.0083560086786746978, 123);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*test != *other);
}

TEST(SeratoDB_GridMarker, addDataTo_AMarkerWithData_WritesTheCorrectData)
{
    // -- Given.
    auto destination = Blob::blob();
    auto marker = GridMarker::markerWithPositionAndBeatsPerMinute(0.0083560086786746978, 124);
    auto test = GridMarker::Array::array();
    test->append(marker);

    // -- When.
    GridMarker::addMarkersTo(test, destination);

    // -- Then.
    auto data = destination->data();
    constexpr byte expectedData[] = {
        0x00, 0x00, 0x00, 0x01, 0x3C, 0x08, 0xE7, 0xA4, 0x42, 0xF8, 0x00, 0x00, 0x00
    };
    ASSERT_EQ(sizeof(expectedData), destination->size());
    ASSERT_EQ(0, ::memcmp(expectedData, data, sizeof(expectedData)));
}
