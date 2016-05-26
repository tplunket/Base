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

#pragma once

#include <SeratoDB/TrackEntry.hpp>

#include <Base/Base.hpp>

NXA_ENTER_NAMESPACE(NxA);
NXA_ENTER_NAMESPACE(Serato);

NXA_GENERATED_FORWARD_DECLARATIONS_FOR_CLASS(Crate);

class Database;

class Crate : public Object {
    NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_CLASS(NxA::Serato, Crate);

public:
    #pragma mark Factory Methods
    static NxA::Pointer<Crate> crateWithName(const String& crateName);

    #pragma mark Class Methods
    static NxA::Pointer<String> subCratesDirectoryPathInSeratoFolder(const String& seratoFolderPath);
    static NxA::Pointer<String::ArrayOfConst> readCratesNamesInCrateOrderFile(const String& crateOrderFilePath);
    static boolean filenameIsAValidCrateName(const String& fileName);
    static NxA::Pointer<String::ArrayOfConst> cratesInSubCratesDirectory(const String& directory);
    static boolean isAnExistingFullCrateName(const String& fullCrateName,
                                             const String& seratoFolderPath);
    static boolean isAnExistingFullSmartCrateName(const String& fullCrateName,
                                                  const String& seratoFolderPath);
    static void parseCratesInSeratoFolderOnVolumeAddToCrateAndSaveSmartCrateNamesIn(String::ArrayOfConst& cratesInOrder,
                                                                                    const String& seratoFolderPath,
                                                                                    const String& volumePath,
                                                                                    Serato::Crate& parentCrate,
                                                                                    String::ArrayOfConst& smartCrateNames);

    #pragma mark Instance Methods
    const String& name(void) const;
    NxA::Pointer<String> fullCrateName(void) const;

    void addFullCrateNameWithPrefixForCratesOnVolumeAndRecurseToChildren(String& destination,
                                                                         const char* prefix,
                                                                         const String& volumePath) const;

    NxA::Pointer<TrackEntry::Array> trackEntries(void) const;
    const Crate::Array& crates(void) const;

    void addCrate(Crate& crate);
    void removeCrate(NxA::Pointer<Crate>& crate);

    NxA::Pointer<Crate> findOrAddCrateWithRelativeNameAndFullName(const String& relativeName, const String& fullCrateName);
    bool crateOrChildrenCrateContainsTracks(void) const;

    void addTrackEntry(TrackEntry& trackEntry);
    void removeTrackEntry(NxA::Pointer<TrackEntry>& trackEntry);

    boolean hasParentCrate(void) const;
    Crate& parentCrate(void);

    boolean isEmpty(void) const;

    void resetModificationFlags();

    void readFromFolderInVolume(const String& seratoFolderPath, const String& volumePath);
    void saveIfOnVolumeAndRecurseToChildren(const String& volumePath, const String& seratoFolderPath) const;
    boolean childrenCratesWereModified(void) const;

    NxA::Pointer<TrackEntry::Array> removeAndReturnTrackEntries(void);
    NxA::Pointer<Crate::Array> removeAndReturnChildrenCrates(void);

    #pragma mark Overridden Object Instance Methods
    virtual NxA::Pointer<String> description(void) const override;
};

NXA_EXIT_NAMESPACE;
NXA_EXIT_NAMESPACE;
