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

#include <SeratoDB/TrackEntry.hpp>

#include <Base/Base.hpp>

namespace NxA { namespace Serato {
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR_CLASS(Crate);

    class Database;

    class Crate : public Object {
        NXA_GENERATED_OPERATOR_EQUAL_DECLARATION_IN_NAMESPACE_FOR_CLASS(NxA::Serato, Crate);
        NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_CLASS(NxA::Serato, Crate);

    public:
        #pragma mark Factory Methods
        static Crate::Pointer crateWithFullName(const String& fullCrateName);

        #pragma mark Class Methods
        static String::Pointer subCratesDirectoryPathInSeratoFolder(const String& seratoFolderPath);
        static String::ArrayOfConst::Pointer readCratesNamesInCrateOrderFile(const String& crateOrderFilePath);
        static boolean filenameIsAValidCrateName(const String& fileName);
        static String::Pointer crateNameFromFilename(const String& fileName);
        static String::ArrayOfConst::Pointer cratesInSubCratesDirectory(const String& directory);
        static boolean isAnExistingFullCrateName(const String& fullCrateName,
                                                 const String& seratoFolderPath);
        static boolean isAnExistingFullSmartCrateName(const String& fullCrateName,
                                                      const String& seratoFolderPath);
        static void parseCratesInSeratoFolderOnVolumeAddToCrateAndSaveUnknownCrateNamesIn(String::ArrayOfConst& cratesInOrder,
                                                                                          const String& seratoFolderPath,
                                                                                          const String& volumePath,
                                                                                          Serato::Crate& parentCrate,
                                                                                          String::ArrayOfConst& unknownCratesNames);

        #pragma mark Instance Methods
        const String& crateName(void) const;
        void addFullCrateNameWithPrefixForCratesOnVolumeAndRecurseToChildren(String& destination,
                                                                             const char* prefix,
                                                                             const String& volumePath) const;

        TrackEntry::Array::Pointer trackEntries(void) const;
        const Crate::Array& crates(void) const;

        void addCrate(Crate& crate);
        void removeCrate(Crate::Pointer& crate);
        Crate::Pointer findOrAddCrateWithRelativeNameAndFullName(const String& relativeName, const String& fullCrateName);

        void addTrackEntry(TrackEntry& trackEntry);
        void removeTrackEntry(TrackEntry::Pointer& trackEntry);

        boolean hasParentCrate(void) const;
        Crate& parentCrate(void);

        void resetModificationFlags();

        void readFromFolderInVolume(const String& seratoFolderPath, const String& volumePath);
        void saveIfOnVolumeAndRecurseToChildren(const String& volumePath, const String& seratoFolderPath) const;
        boolean childrenCratesWereModified(void) const;

        TrackEntry::Array::Pointer removeAndReturnTrackEntries(void);
        Crate::Array::Pointer removeAndReturnChildrenCrates(void);

        #pragma mark Overridden Object Instance Methods
        virtual String::Pointer description(void) const;
    };
} }
