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

#include "Tags/ObjectTag.hpp"
#include "Tags/BlobTag.hpp"
#include "Tags/BooleanTag.hpp"
#include "Tags/PathTag.hpp"
#include "Tags/TextTag.hpp"
#include "Tags/UInteger16Tag.hpp"
#include "Tags/UInteger32Tag.hpp"
#include "Tags/VersionTag.hpp"
#include "Base/Test.hpp"

using namespace testing;
using namespace NxA;
using namespace NxA::Serato;

NXA_CONTAINS_TEST_SUITE_NAMED(SeratoDB_ObjectTag_Tests);

static const byte testData[] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};
static Blob::PointerToConst testBlob = Blob::blobWithMemoryAndSize(testData, sizeof(testData));
static String::PointerToConst path = String::stringWith("Some/Cool/path.mp3");
static String::PointerToConst text = String::stringWith("Some Text");
static String::PointerToConst version = String::stringWith("Cool Version");

TEST(SeratoDB_ObjectTag, TagWithIdentifierAndValue_TagCreated_TagHasCorrectValue)
{
    // -- Given.
    auto tags = Tag::Array::array();
    tags->append(BlobTag::tagWithIdentifierAndValue('atst', testBlob));
    tags->append(BooleanTag::tagWithIdentifierAndValue('btst', true));
    tags->append(PathTag::tagWithIdentifierAndValue('ptst', path));
    tags->append(TextTag::tagWithIdentifierAndValue('ttst', text));
    tags->append(UInteger16Tag::tagWithIdentifierAndValue('stst', 0xBEEF));
    tags->append(UInteger32Tag::tagWithIdentifierAndValue('utst', 0xDEADBEEF));
    tags->append(VersionTag::tagWithIdentifierAndValue('vtst', version));

    // -- When.
    auto tag = ObjectTag::tagWithIdentifierAndValue('otst', tags);

    // -- Then.
    ASSERT_EQ('otst', tag->identifier());
    ASSERT_EQ(7, tag->numberOfSubTags());
    ASSERT_TRUE(tag->hasSubTagForIdentifier('atst'));
    ASSERT_TRUE(tag->hasSubTagForIdentifier('btst'));
    ASSERT_TRUE(tag->hasSubTagForIdentifier('ptst'));
    ASSERT_TRUE(tag->hasSubTagForIdentifier('ttst'));
    ASSERT_TRUE(tag->hasSubTagForIdentifier('stst'));
    ASSERT_TRUE(tag->hasSubTagForIdentifier('utst'));
    ASSERT_TRUE(tag->hasSubTagForIdentifier('vtst'));

    auto& blobTag = dynamic_cast<const BlobTag&>(tag->subTagForIdentifier('atst'));
    ASSERT_EQ('atst', blobTag.identifier());
    ASSERT_EQ(sizeof(testData), blobTag.value().size());
    ASSERT_EQ(0, memcmp(testData, blobTag.value().data(), sizeof(testData)));
    auto& booleanTag = dynamic_cast<const BooleanTag&>(tag->subTagForIdentifier('btst'));
    ASSERT_EQ('btst', booleanTag.identifier());
    ASSERT_EQ(true, booleanTag.value());
    auto& pathTag = dynamic_cast<const PathTag&>(tag->subTagForIdentifier('ptst'));
    ASSERT_EQ('ptst', pathTag.identifier());
    ASSERT_STREQ(path->toUTF8(), pathTag.value().toUTF8());
    auto& textTag = dynamic_cast<const TextTag&>(tag->subTagForIdentifier('ttst'));
    ASSERT_EQ('ttst', textTag.identifier());
    ASSERT_STREQ(text->toUTF8(), textTag.value().toUTF8());
    auto& uinteger16Tag = dynamic_cast<const UInteger16Tag&>(tag->subTagForIdentifier('stst'));
    ASSERT_EQ('stst', uinteger16Tag.identifier());
    ASSERT_EQ(0xBEEF, uinteger16Tag.value());
    auto& uinteger32Tag = dynamic_cast<const UInteger32Tag&>(tag->subTagForIdentifier('utst'));
    ASSERT_EQ('utst', uinteger32Tag.identifier());
    ASSERT_EQ(0xDEADBEEF, uinteger32Tag.value());
    auto& versionTag = dynamic_cast<const VersionTag&>(tag->subTagForIdentifier('vtst'));
    ASSERT_EQ('vtst', versionTag.identifier());
    ASSERT_STREQ(version->toUTF8(), versionTag.value().toUTF8());
}

TEST(SeratoDB_ObjectTag, TagWithIdentifierAndValue_IncorrectIdentifier_ThrowsException)
{
    // -- Given.
    auto tags = Tag::Array::array();
    tags->append(BlobTag::tagWithIdentifierAndValue('atst', testBlob));

    // -- When.
    // -- Then.
    ASSERT_THROW(ObjectTag::tagWithIdentifierAndValue('stst', tags), NxA::Exception);
}

TEST(SeratoDB_ObjectTag, TagFromAnEmptyArray_ThrowsException)
{
    // -- Given.
    auto tags = Tag::Array::array();

    // -- When.
    // -- Then.
    ASSERT_THROW(ObjectTag::tagWithIdentifierAndValue('otst', tags), NxA::Exception);
}

TEST(SeratoDB_ObjectTag, TagWithMemoryAt_TagCreated_TagHasCorrectValue)
{
    // -- Given.
    constexpr byte data[] = {
        0x6F, 0x74, 0x73, 0x74, 0x00, 0x00, 0x00, 0x9D, 0x61, 0x74, 0x73, 0x74, 0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x02, 0x03, 0x04,
        0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x62, 0x74, 0x73, 0x74, 0x00, 0x00, 0x00, 0x01, 0x01, 0x70,
        0x74, 0x73, 0x74, 0x00, 0x00, 0x00, 0x24, 0x00, 0x53, 0x00, 0x6F, 0x00, 0x6D, 0x00, 0x65, 0x00, 0x2F, 0x00, 0x43, 0x00, 0x6F,
        0x00, 0x6F, 0x00, 0x6C, 0x00, 0x2F, 0x00, 0x70, 0x00, 0x61, 0x00, 0x74, 0x00, 0x68, 0x00, 0x2E, 0x00, 0x6D, 0x00, 0x70, 0x00,
        0x33, 0x73, 0x74, 0x73, 0x74, 0x00, 0x00, 0x00, 0x02, 0xBE, 0xEF, 0x74, 0x74, 0x73, 0x74, 0x00, 0x00, 0x00, 0x12, 0x00, 0x53,
        0x00, 0x6F, 0x00, 0x6D, 0x00, 0x65, 0x00, 0x20, 0x00, 0x54, 0x00, 0x65, 0x00, 0x78, 0x00, 0x74, 0x75, 0x74, 0x73, 0x74, 0x00,
        0x00, 0x00, 0x04, 0xDE, 0xAD, 0xBE, 0xEF, 0x76, 0x74, 0x73, 0x74, 0x00, 0x00, 0x00, 0x18, 0x00, 0x43, 0x00, 0x6F, 0x00, 0x6F,
        0x00, 0x6C, 0x00, 0x20, 0x00, 0x56, 0x00, 0x65, 0x00, 0x72, 0x00, 0x73, 0x00, 0x69, 0x00, 0x6F, 0x00, 0x6E
    };

    // -- When.
    auto tag = ObjectTag::tagWithMemoryAt(data);

    // -- Then.
    ASSERT_EQ('otst', tag->identifier());
    ASSERT_EQ(7, tag->numberOfSubTags());
    ASSERT_TRUE(tag->hasSubTagForIdentifier('atst'));
    ASSERT_TRUE(tag->hasSubTagForIdentifier('btst'));
    ASSERT_TRUE(tag->hasSubTagForIdentifier('ptst'));
    ASSERT_TRUE(tag->hasSubTagForIdentifier('ttst'));
    ASSERT_TRUE(tag->hasSubTagForIdentifier('stst'));
    ASSERT_TRUE(tag->hasSubTagForIdentifier('utst'));
    ASSERT_TRUE(tag->hasSubTagForIdentifier('vtst'));

    auto& blobTag = dynamic_cast<const BlobTag&>(tag->subTagForIdentifier('atst'));
    ASSERT_EQ('atst', blobTag.identifier());
    ASSERT_EQ(sizeof(testData), blobTag.value().size());
    ASSERT_EQ(0, memcmp(testData, blobTag.value().data(), sizeof(testData)));
    auto& booleanTag = dynamic_cast<const BooleanTag&>(tag->subTagForIdentifier('btst'));
    ASSERT_EQ('btst', booleanTag.identifier());
    ASSERT_EQ(true, booleanTag.value());
    auto& pathTag = dynamic_cast<const PathTag&>(tag->subTagForIdentifier('ptst'));
    ASSERT_EQ('ptst', pathTag.identifier());
    ASSERT_STREQ(path->toUTF8(), pathTag.value().toUTF8());
    auto& textTag = dynamic_cast<const TextTag&>(tag->subTagForIdentifier('ttst'));
    ASSERT_EQ('ttst', textTag.identifier());
    ASSERT_STREQ(text->toUTF8(), textTag.value().toUTF8());
    auto& uinteger16Tag = dynamic_cast<const UInteger16Tag&>(tag->subTagForIdentifier('stst'));
    ASSERT_EQ('stst', uinteger16Tag.identifier());
    ASSERT_EQ(0xBEEF, uinteger16Tag.value());
    auto& uinteger32Tag = dynamic_cast<const UInteger32Tag&>(tag->subTagForIdentifier('utst'));
    ASSERT_EQ('utst', uinteger32Tag.identifier());
    ASSERT_EQ(0xDEADBEEF, uinteger32Tag.value());
    auto& versionTag = dynamic_cast<const VersionTag&>(tag->subTagForIdentifier('vtst'));
    ASSERT_EQ('vtst', versionTag.identifier());
    ASSERT_STREQ(version->toUTF8(), versionTag.value().toUTF8());
}

TEST(SeratoDB_ObjectTag, TagWithMemoryAt_IncorrectIdentifier_ThrowsException)
{
    // -- Given.
    constexpr byte data[] = {
        0x74, 0x74, 0x73, 0x74, 0x00, 0x00, 0x00, 0x24, 0x00, 0x53, 0x00, 0x6F, 0x00, 0x6D, 0x00, 0x65, 0x00, 0x2F,
        0x00, 0x43, 0x00, 0x6F, 0x00, 0x6F, 0x00, 0x6C, 0x00, 0x2F, 0x00, 0x70, 0x00, 0x61, 0x00, 0x74, 0x00, 0x68,
        0x00, 0x2E, 0x00, 0x6D, 0x00, 0x70, 0x00, 0x33
    };

    // -- When.
    ASSERT_THROW(ObjectTag::tagWithMemoryAt(data), NxA::Exception);
}

TEST(SeratoDB_ObjectTag, OperatorEqual_TwoEqualTags_ReturnsTrue)
{
    // -- Given.
    auto tags = Tag::Array::array();
    tags->append(BlobTag::tagWithIdentifierAndValue('atst', testBlob));
    tags->append(BooleanTag::tagWithIdentifierAndValue('btst', true));
    tags->append(PathTag::tagWithIdentifierAndValue('ptst', path));
    auto tag = ObjectTag::tagWithIdentifierAndValue('otst', tags);
    auto otherTags = Tag::Array::array();
    otherTags->append(BlobTag::tagWithIdentifierAndValue('atst', testBlob));
    otherTags->append(BooleanTag::tagWithIdentifierAndValue('btst', true));
    otherTags->append(PathTag::tagWithIdentifierAndValue('ptst', path));
    auto otherTag = ObjectTag::tagWithIdentifierAndValue('otst', otherTags);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*tag == *otherTag);
}

TEST(SeratoDB_ObjectTag, OperatorEqual_TwoEqualTagsSameObject_ReturnsTrue)
{
    // -- Given.
    auto tags = Tag::Array::array();
    tags->append(BlobTag::tagWithIdentifierAndValue('atst', testBlob));
    tags->append(BooleanTag::tagWithIdentifierAndValue('btst', true));
    tags->append(PathTag::tagWithIdentifierAndValue('ptst', path));
    auto tag = ObjectTag::tagWithIdentifierAndValue('otst', tags);

    // -- When.
    // -- Then.
    ASSERT_TRUE(*tag == *tag);
}

TEST(SeratoDB_ObjectTag, OperatorEqual_TwoUnEqualTagsDifferentNumberOfSubtags_Returnsfalse)
{
    // -- Given.
    auto tags = Tag::Array::array();
    tags->append(BlobTag::tagWithIdentifierAndValue('atst', testBlob));
    tags->append(BooleanTag::tagWithIdentifierAndValue('btst', true));
    tags->append(PathTag::tagWithIdentifierAndValue('ptst', path));
    auto tag = ObjectTag::tagWithIdentifierAndValue('otst', tags);
    auto otherTags = Tag::Array::array();
    otherTags->append(BlobTag::tagWithIdentifierAndValue('atst', testBlob));
    otherTags->append(BooleanTag::tagWithIdentifierAndValue('btst', true));
    auto otherTag = ObjectTag::tagWithIdentifierAndValue('otst', otherTags);

    // -- When.
    // -- Then.
    ASSERT_FALSE(*tag == *otherTag);
}

TEST(SeratoDB_ObjectTag, OperatorEqual_TwoUnEqualTagsDifferentSubtags_Returnsfalse)
{
    // -- Given.
    auto tags = Tag::Array::array();
    tags->append(BlobTag::tagWithIdentifierAndValue('atst', testBlob));
    tags->append(BooleanTag::tagWithIdentifierAndValue('btst', true));
    tags->append(PathTag::tagWithIdentifierAndValue('ptst', path));
    auto tag = ObjectTag::tagWithIdentifierAndValue('otst', tags);
    auto otherTags = Tag::Array::array();
    otherTags->append(BlobTag::tagWithIdentifierAndValue('atst', testBlob));
    otherTags->append(BooleanTag::tagWithIdentifierAndValue('btst', true));
    tags->append(UInteger16Tag::tagWithIdentifierAndValue('stst', 0xBEEF));
    auto otherTag = ObjectTag::tagWithIdentifierAndValue('otst', otherTags);

    // -- When.
    // -- Then.
    ASSERT_FALSE(*tag == *otherTag);
}

TEST(SeratoDB_ObjectTag, OperatorEqual_TwoUnEqualTagsDifferentSubtagsValues_Returnsfalse)
{
    // -- Given.
    auto tags = Tag::Array::array();
    tags->append(BlobTag::tagWithIdentifierAndValue('atst', testBlob));
    tags->append(BooleanTag::tagWithIdentifierAndValue('btst', true));
    tags->append(PathTag::tagWithIdentifierAndValue('ptst', path));
    auto tag = ObjectTag::tagWithIdentifierAndValue('otst', tags);
    auto otherTags = Tag::Array::array();
    otherTags->append(BlobTag::tagWithIdentifierAndValue('atst', testBlob));
    otherTags->append(BooleanTag::tagWithIdentifierAndValue('btst', false));
    otherTags->append(PathTag::tagWithIdentifierAndValue('ptst', path));
    auto otherTag = ObjectTag::tagWithIdentifierAndValue('otst', otherTags);

    // -- When.
    // -- Then.
    ASSERT_FALSE(*tag == *otherTag);
}

TEST(SeratoDB_ObjectTag, SetSubTag_TagWithExistingSubTagForThatIdentifier_SetsTheValueCorrectly)
{
    // -- Given.
    auto tags = Tag::Array::array();
    tags->append(BlobTag::tagWithIdentifierAndValue('atst', testBlob));
    tags->append(BooleanTag::tagWithIdentifierAndValue('btst', true));
    tags->append(PathTag::tagWithIdentifierAndValue('ptst', path));
    auto tag = ObjectTag::tagWithIdentifierAndValue('otst', tags);

    // -- When.
    tag->setSubTag(BooleanTag::tagWithIdentifierAndValue('btst', false));

    // -- Then.
    ASSERT_EQ('otst', tag->identifier());
    ASSERT_EQ(3, tag->numberOfSubTags());
    ASSERT_TRUE(tag->hasSubTagForIdentifier('atst'));
    ASSERT_TRUE(tag->hasSubTagForIdentifier('btst'));
    ASSERT_TRUE(tag->hasSubTagForIdentifier('ptst'));

    auto& blobTag = dynamic_cast<const BlobTag&>(tag->subTagForIdentifier('atst'));
    ASSERT_EQ('atst', blobTag.identifier());
    ASSERT_EQ(sizeof(testData), blobTag.value().size());
    ASSERT_EQ(0, memcmp(testData, blobTag.value().data(), sizeof(testData)));
    auto& booleanTag = dynamic_cast<const BooleanTag&>(tag->subTagForIdentifier('btst'));
    ASSERT_EQ('btst', booleanTag.identifier());
    ASSERT_EQ(false, booleanTag.value());
    auto& pathTag = dynamic_cast<const PathTag&>(tag->subTagForIdentifier('ptst'));
    ASSERT_EQ('ptst', pathTag.identifier());
    ASSERT_STREQ(path->toUTF8(), pathTag.value().toUTF8());
}

TEST(SeratoDB_ObjectTag, SetSubTag_TagWithSubTagForANewIdentifier_SetsTheValueCorrectly)
{
    // -- Given.
    auto tags = Tag::Array::array();
    tags->append(BlobTag::tagWithIdentifierAndValue('atst', testBlob));
    tags->append(PathTag::tagWithIdentifierAndValue('ptst', path));
    auto tag = ObjectTag::tagWithIdentifierAndValue('otst', tags);

    // -- When.
    tag->setSubTag(BooleanTag::tagWithIdentifierAndValue('btst', false));

    // -- Then.
    ASSERT_EQ('otst', tag->identifier());
    ASSERT_EQ(3, tag->numberOfSubTags());
    ASSERT_TRUE(tag->hasSubTagForIdentifier('atst'));
    ASSERT_TRUE(tag->hasSubTagForIdentifier('btst'));
    ASSERT_TRUE(tag->hasSubTagForIdentifier('ptst'));

    auto& blobTag = dynamic_cast<const BlobTag&>(tag->subTagForIdentifier('atst'));
    ASSERT_EQ('atst', blobTag.identifier());
    ASSERT_EQ(sizeof(testData), blobTag.value().size());
    ASSERT_EQ(0, memcmp(testData, blobTag.value().data(), sizeof(testData)));
    auto& booleanTag = dynamic_cast<const BooleanTag&>(tag->subTagForIdentifier('btst'));
    ASSERT_EQ('btst', booleanTag.identifier());
    ASSERT_EQ(false, booleanTag.value());
    auto& pathTag = dynamic_cast<const PathTag&>(tag->subTagForIdentifier('ptst'));
    ASSERT_EQ('ptst', pathTag.identifier());
    ASSERT_STREQ(path->toUTF8(), pathTag.value().toUTF8());
}

TEST(SeratoDB_ObjectTag, AddTo_TagWrittenToBlob_WriteCorrectData)
{
    // -- Given.
    auto tags = Tag::Array::array();
    tags->append(BlobTag::tagWithIdentifierAndValue('atst', testBlob));
    tags->append(BooleanTag::tagWithIdentifierAndValue('btst', true));
    tags->append(PathTag::tagWithIdentifierAndValue('ptst', path));
    tags->append(TextTag::tagWithIdentifierAndValue('ttst', text));
    tags->append(UInteger16Tag::tagWithIdentifierAndValue('stst', 0xBEEF));
    tags->append(UInteger32Tag::tagWithIdentifierAndValue('utst', 0xDEADBEEF));
    tags->append(VersionTag::tagWithIdentifierAndValue('vtst', version));
    auto tag = ObjectTag::tagWithIdentifierAndValue('otst', tags);
    auto destination = Blob::blob();

    // -- When.
    tag->addTo(destination);

    // -- Then.
    auto data = destination->data();
    constexpr byte expectedData[] = {
        0x6F, 0x74, 0x73, 0x74, 0x00, 0x00, 0x00, 0x9D, 0x61, 0x74, 0x73, 0x74, 0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x02, 0x03, 0x04,
        0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x62, 0x74, 0x73, 0x74, 0x00, 0x00, 0x00, 0x01, 0x01, 0x70,
        0x74, 0x73, 0x74, 0x00, 0x00, 0x00, 0x24, 0x00, 0x53, 0x00, 0x6F, 0x00, 0x6D, 0x00, 0x65, 0x00, 0x2F, 0x00, 0x43, 0x00, 0x6F,
        0x00, 0x6F, 0x00, 0x6C, 0x00, 0x2F, 0x00, 0x70, 0x00, 0x61, 0x00, 0x74, 0x00, 0x68, 0x00, 0x2E, 0x00, 0x6D, 0x00, 0x70, 0x00,
        0x33, 0x73, 0x74, 0x73, 0x74, 0x00, 0x00, 0x00, 0x02, 0xBE, 0xEF, 0x74, 0x74, 0x73, 0x74, 0x00, 0x00, 0x00, 0x12, 0x00, 0x53,
        0x00, 0x6F, 0x00, 0x6D, 0x00, 0x65, 0x00, 0x20, 0x00, 0x54, 0x00, 0x65, 0x00, 0x78, 0x00, 0x74, 0x75, 0x74, 0x73, 0x74, 0x00,
        0x00, 0x00, 0x04, 0xDE, 0xAD, 0xBE, 0xEF, 0x76, 0x74, 0x73, 0x74, 0x00, 0x00, 0x00, 0x18, 0x00, 0x43, 0x00, 0x6F, 0x00, 0x6F,
        0x00, 0x6C, 0x00, 0x20, 0x00, 0x56, 0x00, 0x65, 0x00, 0x72, 0x00, 0x73, 0x00, 0x69, 0x00, 0x6F, 0x00, 0x6E
    };
    ASSERT_EQ(sizeof(expectedData), destination->size());
    ASSERT_EQ(0, ::memcmp(expectedData, data, sizeof(expectedData)));
}
