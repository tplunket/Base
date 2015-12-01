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

#include <Tags/ObjectTag.hpp>

#include <Base/Base.hpp>

namespace NxA { namespace Serato {
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR_CLASS(TrackEntry);

    class Tag;
    class Crate;

    class TrackEntry : public Object {
        NXA_GENERATED_OPERATOR_EQUAL_DECLARATION_IN_NAMESPACE_FOR_CLASS(NxA::Serato, TrackEntry);
        NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_CLASS(NxA::Serato, TrackEntry);

        friend Crate;

    public:
        #pragma mark Factory Methods
        static TrackEntry::Pointer entryWithTagOnVolume(const ObjectTag& tag, const String& volumePath);
        static TrackEntry::Pointer entryWithTrackFileAtOnVolume(const String& path, const String& volumePath);

        #pragma mark Instance Methods
        String::Pointer trackFilePath(void) const;
        const String& volumePath(void) const;

        boolean hasParentCrate(void) const;
        Crate& parentCrate(void);
        
        const Tag& tagForEntry(void) const;
    };
} }
