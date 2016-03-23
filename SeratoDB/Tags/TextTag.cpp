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

#include "SeratoDB/Tags/TextTag.hpp"
#include "SeratoDB/Tags/Internal/InternalTextTag.hpp"

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, TextTag, Tag);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Factory Methods

TextTag::Pointer TextTag::tagWithMemoryAt(const byte* tagAddress)
{
    count size = Tag::dataSizeForTagAt(tagAddress);
    auto text = size ? String::stringWithUTF16(Blob::blobWithMemoryAndSize(InternalTag::dataForTagAt(tagAddress), size)) : String::string();

    return TextTag::tagWithIdentifierAndValue(Tag::identifierForTagAt(tagAddress), text);
}

TextTag::Pointer TextTag::tagWithIdentifierAndValue(uinteger32 identifier, const String& value)
{
    NXA_ASSERT_EQ((identifier & 0xFF000000) >> 24, 't');

    auto newTag = TextTag::makeShared();
    newTag->internal->identifier = identifier;
    newTag->internal->value = value.pointer();

    return newTag;
}

#pragma mark Operators

bool TextTag::operator==(const TextTag& other) const
{
    if (this == &other) {
        return true;
    }

    return this->value() == other.value();
}

#pragma mark Instance Methods

const String& TextTag::value(void) const
{
    return internal->value;
}

void TextTag::setValue(const String& value)
{
    internal->value = value.pointer();
}

void TextTag::addTo(Blob& destination) const
{
    auto utf16Value = internal->value->toUTF16();
    count dataSize = utf16Value->size();
    auto memoryRepresentation = Blob::blobWithCapacity(InternalTag::memoryNeededForTagHeader());

    byte* tagAddress = memoryRepresentation->data();
    InternalTag::setIdentifierForTagAt(this->identifier(), tagAddress);
    InternalTag::setDataSizeForTagAt(dataSize, tagAddress);

    destination.append(memoryRepresentation);
    destination.append(utf16Value);
}
