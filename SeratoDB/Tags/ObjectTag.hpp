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

#pragma once

#include <SeratoDb/Tags/Tag.hpp>

#include <Base/Base.hpp>

#define NXA_PRINT_DEBUG_INFO                0
#define NXA_PRINT_TAG_DEBUG_INFO            0
#define NXA_PRINT_TRACK_DEBUG_INFO          0

namespace NxA { namespace Serato {
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR_CLASS(ObjectTag);

    class ObjectTag : public Tag {
        NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_CLASS(NxA::Serato, ObjectTag);
        
    public:
        #pragma mark Factory Methods
        static ObjectTag::Pointer tagWithMemoryAt(const byte* tagAddress);
        static ObjectTag::Pointer tagWithIdentifierAndValue(uinteger32 identifier, const Tag::Array& content);

        #pragma mark Operators
        bool operator==(const ObjectTag& other) const;

        #pragma mark Instance Methods
#if NXA_PRINT_TAG_DEBUG_INFO
        void debugPrint(void) const;
#endif

        boolean hasSubTagForIdentifier(uinteger32 identifier) const;

        const Tag& subTagForIdentifier(uinteger32 identifier) const;
        Tag& subTagForIdentifier(uinteger32 identifier);

        void setSubTag(Tag& tag);

        count numberOfSubTags(void) const;

        void addInSeratoTrackOrderTo(Blob& destination) const;
        void addTo(Blob& destination) const;
    };
} }
