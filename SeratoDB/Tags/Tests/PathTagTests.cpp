//
//  Copyright (c) 2015-2016 Next Audio Labs, LLC. All rights reserved.
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
    ASSERT_STREQ(path->toUTF8(), tag->value().toUTF8());
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
    ASSERT_STREQ(path->toUTF8(), tag->value().toUTF8());
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

TEST(SeratoDB_PathTag, OperatorEqual_TwoEqualTags_ReturnsTrue)
{
    // -- Given.
    auto tag = PathTag::tagWithIdentifierAndValue('ptst', path);
    auto otherTag = PathTag::tagWithIdentifierAndValue('ptst', String::stringWith("Some/Cool/path.mp3"));

    // -- When.
    // -- Then.
    ASSERT_TRUE(*tag == *otherTag);
}

TEST(SeratoDB_PathTag, OperatorEqual_TwoEqualTagsSameObject_ReturnsTrue)
{
    // -- Given.
    auto tag = PathTag::tagWithIdentifierAndValue('ptst', path);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*tag == *tag);
}

TEST(SeratoDB_ObjectTag, OperatorEqual_TwoUnEqualTags_Returnsfalse)
{
    // -- Given.
    auto tag = PathTag::tagWithIdentifierAndValue('ptst', path);
    auto otherTag = PathTag::tagWithIdentifierAndValue('ptst', String::stringWith("Some/Other/path.mp3"));

    // -- When.
    // -- Then.
    ASSERT_FALSE(*tag == *otherTag);
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
    ASSERT_STREQ(other->toUTF8(), tag->value().toUTF8());
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
