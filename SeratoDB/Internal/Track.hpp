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

#include <SeratoDB/Track.hpp>
#include <Tags/ObjectTag.hpp>
#include <TrackFiles/TrackFile.hpp>

#include <Base/Base.hpp>
#include <Base/Internal/Object.hpp>

namespace NxA { namespace Serato { namespace Internal {
    struct Track : public NxA::Internal::Object {
        NXA_GENERATED_INTERNAL_DECLARATIONS_WITHOUT_CONSTRUCTORS_FOR(NxA::Serato, Track);

        #pragma mark Constructors & Destructors
        Track(Serato::ObjectTag& tag, const String& rootFolderPath);

        #pragma mark Instance Variables
        Serato::ObjectTag::Pointer trackTag;
        String::PointerToConst rootFolder;

        boolean needsToUpdateDatabaseFile;

        #pragma mark Instance Methods
        const String& stringForSubTagForIdentifier(uinteger32 identifier) const;
        const String& pathForSubTagForIdentifier(uinteger32 identifier) const;
        uinteger32 uint32ForSubTagForIdentifier(uinteger32 identifier) const;

        void setStringForSubTagForIdentifier(const String& value, uinteger32 identifier);
        void setPathForSubTagForIdentifier(const String& value, uinteger32 identifier);
        void setUInt32ForSubTagForIdentifier(uinteger32 value, uinteger32 identifier);
        void setBooleanForSubTagForIdentifier(boolean value, uinteger32 identifier);

        String::Pointer trackFilePath(void) const;
    };
} } }
