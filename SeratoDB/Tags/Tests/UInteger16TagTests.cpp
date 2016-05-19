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

#include "Base/Test.hpp"

#include "SeratoDB/Tags/UInteger16Tag.hpp"

using namespace testing;
using namespace NxA;
using namespace NxA::Serato;

NXA_CONTAINS_TEST_SUITE_NAMED(SeratoDB_UInteger16Tag_Tests);

TEST(SeratoDB_UInteger16Tag, TagWithIdentifierAndValue_TagCreated_TagHasCorrectValue)
{
    // -- Given.
    // -- When.
    auto tag = UInteger16Tag::tagWithIdentifierAndValue('stst', 0xBEEF);

    // -- Then.
    ASSERT_EQ('stst', tag->identifier());
    ASSERT_EQ(0xBEEF, tag->value());
}

TEST(SeratoDB_UInteger16Tag, TagWithIdentifierAndValue_IncorrectIdentifier_ThrowsException)
{
    // -- Given.
    // -- When.
    // -- Then.
    ASSERT_THROW(UInteger16Tag::tagWithIdentifierAndValue('vtst', 0xBEEF), NxA::AssertionFailed);
}

TEST(SeratoDB_UInteger16Tag, TagWithMemoryAt_TagCreated_TagHasCorrectValue)
{
    // -- Given.
    constexpr byte data[] = { 0x73, 0x74, 0x73, 0x74, 0x00, 0x00, 0x00, 0x02, 0x54, 0xCA };

    // -- When.
    auto tag = UInteger16Tag::tagWithMemoryAt(data);

    // -- Then.
    ASSERT_EQ('stst', tag->identifier());
    ASSERT_EQ(0x54CA, tag->value());
}

TEST(SeratoDB_UInteger16Tag, TagWithMemoryAt_IncorrectIdentifier_ThrowsException)
{
    // -- Given.
    constexpr byte data[] = { 0x12, 0x74, 0x73, 0x74, 0x00, 0x00, 0x00, 0x02, 0x54, 0xCA };

    // -- When.
    ASSERT_THROW(UInteger16Tag::tagWithMemoryAt(data), NxA::AssertionFailed);
}

TEST(SeratoDB_UInteger16Tag, TagWithMemoryAt_TagCreatedFromDataWithIncorrectSize_ThrowsException)
{
    // -- Given.
    constexpr byte data[] = { 0x73, 0x74, 0x73, 0x74, 0x00, 0x00, 0x02, 0x02, 0x54, 0xCA };

    // -- When.
    // -- Then.
    ASSERT_THROW(UInteger16Tag::tagWithMemoryAt(data), NxA::AssertionFailed);
}

TEST(SeratoDB_UInteger16Tag, OperatorEqual_TwoEqualTags_ReturnsTrue)
{
    // -- Given.
    auto tag = UInteger16Tag::tagWithIdentifierAndValue('stst', 0xBEEF);
    auto otherTag = UInteger16Tag::tagWithIdentifierAndValue('stst', 0xBEEF);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*tag == *otherTag);
}

TEST(SeratoDB_UInteger16Tag, OperatorEqual_TwoEqualTagsSameObject_ReturnsTrue)
{
    // -- Given.
    auto tag = UInteger16Tag::tagWithIdentifierAndValue('stst', 0xBEEF);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*tag == *tag);
}

TEST(SeratoDB_UInteger16Tag, OperatorEqual_TwoUnEqualTags_Returnsfalse)
{
    // -- Given.
    auto tag = UInteger16Tag::tagWithIdentifierAndValue('stst', 0xBEEF);
    auto otherTag = UInteger16Tag::tagWithIdentifierAndValue('stst', 0xDEAD);

    // -- When.
    // -- Then.
    ASSERT_FALSE(*tag == *otherTag);
}

TEST(SeratoDB_UInteger16Tag, SetValue_TagWithAValue_SetsTheValueCorrectly)
{
    // -- Given.
    auto tag = UInteger16Tag::tagWithIdentifierAndValue('stst', 0xBEEF);

    // -- When.
    tag->setValue(0x2323);

    // -- Then.
    ASSERT_EQ('stst', tag->identifier());
    ASSERT_EQ(0x2323, tag->value());
}

TEST(SeratoDB_UInteger16Tag, AddTo_TagWrittenToBlob_WriteCorrectData)
{
    // -- Given.
    auto tag = UInteger16Tag::tagWithIdentifierAndValue('stst', 0xBEEF);
    auto destination = Blob::blob();

    // -- When.
    tag->addTo(destination);

    // -- Then.
    auto data = destination->data();
    constexpr byte expectedData[] = { 0x73, 0x74, 0x73, 0x74, 0x00, 0x00, 0x00, 0x02, 0xBE, 0xEF };
    ASSERT_EQ(sizeof(expectedData), destination->size());
    ASSERT_EQ(0, ::memcmp(expectedData, data, sizeof(expectedData)));
}
