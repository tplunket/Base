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

#include "SeratoDB/Tags/UInteger32Tag.hpp"

using namespace testing;
using namespace NxA;
using namespace NxA::Serato;

NXA_CONTAINS_TEST_SUITE_NAMED(SeratoDB_UInteger32Tag_Tests);

TEST(SeratoDB_UInteger32Tag, TagWithIdentifierAndValue_TagCreated_TagHasCorrectValue)
{
    // -- Given.
    // -- When.
    auto tag = UInteger32Tag::tagWithIdentifierAndValue('utst', 0xDEADBEEF);

    // -- Then.
    ASSERT_EQ('utst', tag->identifier());
    ASSERT_EQ(0xDEADBEEF, tag->value());
}

TEST(SeratoDB_UInteger32Tag, TagWithIdentifierAndValue_IncorrectIdentifier_ThrowsException)
{
    // -- Given.
    // -- When.
    // -- Then.
    ASSERT_THROW(UInteger32Tag::tagWithIdentifierAndValue('stst', 0xDEADBEEF), NxA::Exception);
}

TEST(SeratoDB_UInteger32Tag, TagWithMemoryAt_TagCreated_TagHasCorrectValue)
{
    // -- Given.
    constexpr byte data[] = { 0x75, 0x74, 0x73, 0x74, 0x00, 0x00, 0x00, 0x04, 0x54, 0xCA, 0x84, 0xAB };

    // -- When.
    auto tag = UInteger32Tag::tagWithMemoryAt(data);

    // -- Then.
    ASSERT_EQ('utst', tag->identifier());
    ASSERT_EQ(0x54CA84AB, tag->value());
}

TEST(SeratoDB_UInteger32Tag, TagWithMemoryAt_IncorrectIdentifier_ThrowsException)
{
    // -- Given.
    constexpr byte data[] = { 0x12, 0x74, 0x73, 0x74, 0x00, 0x00, 0x00, 0x04, 0x54, 0xCA, 0x84, 0xAB };

    // -- When.
    ASSERT_THROW(UInteger32Tag::tagWithMemoryAt(data), NxA::Exception);
}

TEST(SeratoDB_UInteger32Tag, TagWithMemoryAt_TagCreatedFromDataWithIncorrectSize_ThrowsException)
{
    // -- Given.
    constexpr byte data[] = { 0x75, 0x74, 0x73, 0x74, 0x00, 0x00, 0x00, 0x02, 0x54, 0xCA };

    // -- When.
    // -- Then.
    ASSERT_THROW(UInteger32Tag::tagWithMemoryAt(data), NxA::Exception);
}

TEST(SeratoDB_UInteger32Tag, OperatorEqual_TwoEqualTags_ReturnsTrue)
{
    // -- Given.
    auto tag = UInteger32Tag::tagWithIdentifierAndValue('utst', 0xDEADBEEF);
    auto otherTag = UInteger32Tag::tagWithIdentifierAndValue('utst', 0xDEADBEEF);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*tag == *otherTag);
}

TEST(SeratoDB_UInteger32Tag, OperatorEqual_TwoEqualTagsSameObject_ReturnsTrue)
{
    // -- Given.
    auto tag = UInteger32Tag::tagWithIdentifierAndValue('utst', 0xDEADBEEF);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*tag == *tag);
}

TEST(SeratoDB_UInteger32Tag, OperatorEqual_TwoUnEqualTags_Returnsfalse)
{
    // -- Given.
    auto tag = UInteger32Tag::tagWithIdentifierAndValue('utst', 0xDEADBEEF);
    auto otherTag = UInteger32Tag::tagWithIdentifierAndValue('utst', 0xDEADBEFF);

    // -- When.
    // -- Then.
    ASSERT_FALSE(*tag == *otherTag);
}

TEST(SeratoDB_UInteger32Tag, SetValue_TagWithAValue_SetsTheValueCorrectly)
{
    // -- Given.
    auto tag = UInteger32Tag::tagWithIdentifierAndValue('utst', 0xDEADBEEF);

    // -- When.
    tag->setValue(0x23232323);

    // -- Then.
    ASSERT_EQ('utst', tag->identifier());
    ASSERT_EQ(0x23232323, tag->value());
}

TEST(SeratoDB_UInteger32Tag, AddTo_TagWrittenToBlob_WriteCorrectData)
{
    // -- Given.
    auto tag = UInteger32Tag::tagWithIdentifierAndValue('utst', 0xDEADBEEF);
    auto destination = Blob::blob();

    // -- When.
    tag->addTo(destination);

    // -- Then.
    auto data = destination->data();
    constexpr byte expectedData[] = { 0x75, 0x74, 0x73, 0x74, 0x00, 0x00, 0x00, 0x04, 0xDE, 0xAD, 0xBE, 0xEF };
    ASSERT_EQ(sizeof(expectedData), destination->size());
    ASSERT_EQ(0, ::memcmp(expectedData, data, sizeof(expectedData)));
}
