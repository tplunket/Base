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

#include "SeratoDB/Tags/TagFactory.hpp"
#include "SeratoDB/Tags/BooleanTag.hpp"
#include "SeratoDB/Tags/PathTag.hpp"
#include "SeratoDB/Tags/TextTag.hpp"
#include "SeratoDB/Tags/VersionTag.hpp"
#include "SeratoDB/Tags/BlobTag.hpp"
#include "SeratoDB/Tags/ObjectTag.hpp"
#include "SeratoDB/Tags/UInteger16Tag.hpp"
#include "SeratoDB/Tags/UInteger32Tag.hpp"

using namespace NxA::Serato;
using namespace std;

#pragma mark Class Methods

NxA::Pointer<Tag> TagFactory::tagForTagAt(const byte *tagAddress, const String &source)
{
    uinteger32 identifier = Tag::identifierForTagAt(tagAddress);
    char typeIdentifier = (identifier >> 24) & 0xff;

    switch (typeIdentifier) {
        case 'b': {
            return NxA::Pointer<NxA::Serato::Tag>::dynamicCastFrom(BooleanTag::tagWithMemoryAt(tagAddress));
        }
        case 's': {
            return NxA::Pointer<NxA::Serato::Tag>::dynamicCastFrom(UInteger16Tag::tagWithMemoryAt(tagAddress));
        }
        case 'u': {
            return NxA::Pointer<NxA::Serato::Tag>::dynamicCastFrom(UInteger32Tag::tagWithMemoryAt(tagAddress));
        }
        case 'v': {
            return NxA::Pointer<NxA::Serato::Tag>::dynamicCastFrom(VersionTag::tagWithMemoryAt(tagAddress));
        }
        case 't': {
            return NxA::Pointer<NxA::Serato::Tag>::dynamicCastFrom(TextTag::tagWithMemoryAt(tagAddress));
        }
        case 'p': {
            return NxA::Pointer<NxA::Serato::Tag>::dynamicCastFrom(PathTag::tagWithMemoryAt(tagAddress));
        }
        case 'a': {
            return NxA::Pointer<NxA::Serato::Tag>::dynamicCastFrom(BlobTag::tagWithMemoryAt(tagAddress));
        }
        case 'o': {
            return NxA::Pointer<NxA::Serato::Tag>::dynamicCastFrom(ObjectTag::tagWithMemoryAt(tagAddress));
        }
    }

    NXA_ALOG("Illegal Serato tag type in '%s'. identifier:0x%08x, (char)0x%02x == '%c'.", source.toUTF8(), identifier, typeIdentifier, typeIdentifier);
}

NxA::Pointer<Tag::Array> TagFactory::parseTagsAt(const byte *firstTagAddress, count sizeFromFirstTag, const String &source)
{
    const byte* tagAddress = firstTagAddress;
    const byte* endOfTagsAddress = firstTagAddress + sizeFromFirstTag;

    auto newTags = Tag::Array::array();

    while (tagAddress < endOfTagsAddress) {
        if (Tag::dataSizeForTagAt(tagAddress) > 0) {
            auto tag = TagFactory::tagForTagAt(tagAddress, source);
            newTags->append(tag);
        }

        tagAddress = Tag::nextTagAfterTagAt(tagAddress);
    }

    return newTags;
}
