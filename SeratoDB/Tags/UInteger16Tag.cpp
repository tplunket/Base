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

#include "SeratoDB/Tags/UInteger16Tag.hpp"
#include "SeratoDB/Tags/Internal/InternalUInteger16Tag.hpp"

#include "Base/Base.hpp"

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, UInteger16Tag, Tag);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Factory Methods

UInteger16Tag::Pointer UInteger16Tag::tagWithMemoryAt(const byte* tagAddress)
{
    const byte* tagData = InternalTag::dataForTagAt(tagAddress);
    NXA_ASSERT_EQ(Tag::dataSizeForTagAt(tagAddress), 2);

    return UInteger16Tag::tagWithIdentifierAndValue(Tag::identifierForTagAt(tagAddress),
                                                    Platform::bigEndianUInteger16ValueAt(tagData));
}

UInteger16Tag::Pointer UInteger16Tag::tagWithIdentifierAndValue(uinteger32 identifier, uinteger16 value)
{
    NXA_ASSERT_EQ((identifier & 0xFF000000) >> 24, 's');

    auto newTag = UInteger16Tag::makeShared();
    newTag->internal->identifier = identifier;
    newTag->internal->value = value;

    return newTag;
}

#pragma mark Operators

bool UInteger16Tag::operator==(const UInteger16Tag& other) const
{
    if (this == &other) {
        return true;
    }

    return this->value() == other.value();
}

#pragma mark Instance Methods

uinteger16 UInteger16Tag::value(void) const
{
    return internal->value;
}

void UInteger16Tag::setValue(uinteger16 value)
{
    this->internal->value = value;
}

void UInteger16Tag::addTo(Blob& destination) const
{
    size_t dataSize = 2;
    auto memoryRepresentation = Blob::blobWithCapacity(InternalTag::memoryNeededForTagHeader() + dataSize);

    byte* tagAddress = memoryRepresentation->data();
    InternalTag::setIdentifierForTagAt(this->identifier(), tagAddress);
    InternalTag::setDataSizeForTagAt(dataSize, tagAddress);
    Platform::writeBigEndianUInteger16ValueAt(this->value(), InternalTag::dataForTagAt(tagAddress));

    destination.append(memoryRepresentation);
}
