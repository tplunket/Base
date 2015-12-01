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

#include "SeratoDB/Crate.hpp"
#include "SeratoDB/TrackEntry.hpp"
#include "Tags/Tag.hpp"
#include "Tags/ObjectTag.hpp"

#include <Base/Base.hpp>
#include <Base/Internal/Object.hpp>

namespace NxA { namespace Serato { namespace Internal {
    struct Crate : public NxA::Internal::Object {
        NXA_GENERATED_INTERNAL_DECLARATIONS_WITHOUT_CONSTRUCTORS_FOR(NxA::Serato, Crate);

        #pragma mark Constructors & Destructors
        Crate(const String& crateName);

        #pragma mark Class Methods
        static String::Pointer crateNameIfValidCrateOrEmptyIfNot(const String& name);
        static String::Pointer escapedNameFromCrateName(const String& crateName);
        static String::Pointer crateNameFromEscapedName(const String& escapedName);
        static String::Pointer topParentCrateNameFromFullCrateName(const String& fullCrateName);
        static String::Pointer smartCratesDirectoryPathInSeratoFolder(const String& seratoFolderPath);
        static String::Pointer crateFilePathForFullCrateNameInSeratoFolder(const String& crateName,
                                                                           const String& seratoFolderPath);
        static String::Pointer crateFilePathForFullSmartCrateNameInSeratoFolder(const String& fullCrateName,
                                                                                const String& seratoFolderPath);

        #pragma mark Instance Variables
        String::PointerToConst name;

        boolean tracksWereModified;
        boolean cratesWereModified;

        Serato::Crate::WeakPointer parentCrate;
        Serato::Crate::Array::Pointer childrenCrates;

        String::ArrayOfConst::Pointer volumePaths;
        Serato::TrackEntry::Array::Array::Pointer trackEntriesPerPath;
        Serato::Tag::ArrayOfConst::Array::Pointer otherTagsPerPath;

        #pragma mark Instance Methods
        String::Pointer fullCrateName(void);

        NxA::count indexOfVolumePath(const String& volumePath);
        NxA::count indexOfVolumePathAndAddIfNotPresent(const String& volumePath);

        void markCratesAsModified(void);

        void saveDataToCrateFileInSeratoFolder(const Blob& data, const String& seratoFolderPath);
    };
} } }
