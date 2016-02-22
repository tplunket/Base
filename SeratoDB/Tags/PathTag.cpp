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
#include "Tags/Internal/InternalPathTag.hpp"

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, PathTag, TextTag);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Factory Methods

PathTag::Pointer PathTag::tagWithMemoryAt(const byte* tagAddress)
{
    count size = Tag::dataSizeForTagAt(tagAddress);
    auto text = String::stringWithUTF16(Blob::blobWithMemoryAndSize(InternalTag::dataForTagAt(tagAddress), size));

    return PathTag::tagWithIdentifierAndValue(Tag::identifierForTagAt(tagAddress), text);
}

PathTag::Pointer PathTag::tagWithIdentifierAndValue(uinteger32 identifier, const String& value)
{
    NXA_ASSERT_EQ((identifier & 0xFF000000) >> 24, 'p');

    auto newTag = PathTag::makeShared();
    newTag->internal->identifier = identifier;
    newTag->internal->value = value.pointer();

    return newTag;
}

#pragma mark Operators

bool PathTag::operator==(const PathTag& other) const
{
    if (this == &other) {
        return true;
    }

    return this->value() == other.value();
}
