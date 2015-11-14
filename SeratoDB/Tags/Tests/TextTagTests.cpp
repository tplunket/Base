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
    ASSERT_STREQ(text->toUTF8(), tag->value().toUTF8());
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
    ASSERT_STREQ(text->toUTF8(), tag->value().toUTF8());
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

TEST(SeratoDB_TextTag, OperatorEqual_TwoEqualTags_ReturnsTrue)
{
    // -- Given.
    auto tag = TextTag::tagWithIdentifierAndValue('ttst', text);
    auto otherTag = TextTag::tagWithIdentifierAndValue('ttst', String::stringWith("Some Text"));

    // -- When.
    // -- Then.
    ASSERT_TRUE(*tag == *otherTag);
}

TEST(SeratoDB_TextTag, OperatorEqual_TwoEqualTagsSameObject_ReturnsTrue)
{
    // -- Given.
    auto tag = TextTag::tagWithIdentifierAndValue('ttst', text);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*tag == *tag);
}

TEST(SeratoDB_TextTag, OperatorEqual_TwoUnEqualTags_Returnsfalse)
{
    // -- Given.
    auto tag = TextTag::tagWithIdentifierAndValue('ttst', text);
    auto otherTag = TextTag::tagWithIdentifierAndValue('ttst', String::stringWith("Some Other Text"));

    // -- When.
    // -- Then.
    ASSERT_FALSE(*tag == *otherTag);
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
    ASSERT_STREQ(other->toUTF8(), tag->value().toUTF8());
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
