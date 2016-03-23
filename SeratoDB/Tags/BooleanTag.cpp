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

#include "SeratoDB/Tags/BooleanTag.hpp"
#include "SeratoDB/Tags/Internal/InternalBooleanTag.hpp"

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, BooleanTag, Tag);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Factory Methods

BooleanTag::Pointer BooleanTag::tagWithMemoryAt(const byte* tagAddress)
{
    const byte* tagData = InternalTag::dataForTagAt(tagAddress);
    NXA_ASSERT_EQ(Tag::dataSizeForTagAt(tagAddress), 1);

    return BooleanTag::tagWithIdentifierAndValue(Tag::identifierForTagAt(tagAddress),
                                                 (*tagData == 1) ? true : false);
}

BooleanTag::Pointer BooleanTag::tagWithIdentifierAndValue(uinteger32 identifier, boolean value)
{
    NXA_ASSERT_EQ((identifier & 0xFF000000) >> 24, 'b');

    auto newTag = BooleanTag::makeShared();
    newTag->internal->identifier = identifier;
    newTag->internal->value = value;

    return newTag;
}

#pragma mark Operators

bool BooleanTag::operator==(const BooleanTag& other) const
{
    if (this == &other) {
        return true;
    }

    return this->value() == other.value();
}

#pragma mark Instance Methods

boolean BooleanTag::value(void) const
{
    return internal->value;
}

void BooleanTag::setValue(boolean newValue)
{
    internal->value = newValue;
}

void BooleanTag::addTo(Blob& destination) const
{
    count dataSize = 1;
    count totalSizeNeeded = InternalTag::memoryNeededForTagHeader() + dataSize;
    auto memoryRepresentation = Blob::blobWithCapacity(totalSizeNeeded);

    byte* tagAddress = memoryRepresentation->data();
    InternalTag::setIdentifierForTagAt(this->identifier(), tagAddress);
    InternalTag::setDataSizeForTagAt(dataSize, tagAddress);
    byte* tagData = InternalTag::dataForTagAt(tagAddress);
    *tagData = this->value() ? 1 : 0;

    destination.append(memoryRepresentation);
}
