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

#include <SeratoDB/Track.hpp>
#include <SeratoDB/Crate.hpp>
#include <SeratoDB/Tags/Tag.hpp>
#include <SeratoDB/Tags/ObjectTag.hpp>

#include <SeratoDb/DebugFlags.hpp>

#include <Base/Base.hpp>
#include <Base/Internal/InternalObject.hpp>

NXA_ENTER_NAMESPACE(NxA);
NXA_ENTER_NAMESPACE(Serato);

struct InternalDatabase : public InternalObject {
    NXA_GENERATED_DESTRUCTOR_FOR_CLASS(InternalDatabase);

    #pragma mark Constructors & Destructors
    InternalDatabase(const String& pathForLocalSeratoFolder,
                     const String::ArrayOfConst& pathsForExternalSeratoFolders);

    #pragma mark Class Methods
    static NxA::Pointer<String> pathForCrateOrderFileInSeratoFolder(const String& seratoFolderPath);

    static void addCratesFoundInSeratoFolderOnVolumeToRootFolder(const String& seratoFolderPath,
                                                                 const String& volumePath,
                                                                 Crate& rootFolder,
                                                                 String::ArrayOfConst& SmartCrateNames);

    static void saveContentOfRootFolderIfModifiedAndOnVolumeAndSmartCrateNamesToSeratoFolder(const Serato::Crate& rootFolder,
                                                                                             const String& volumePath,
                                                                                             const String::ArrayOfConst& smartCrateNames,
                                                                                             const String& seratoFolderPath);

    static void setDatabaseFilesInSeratoFolderAsModifedOnDateInSecondsSince1970(const String& folderPath, timestamp dateModified);

    static void addCratesNamesAtTheStartOfUnlessAlreadyThere(String::ArrayOfConst& cratesToAddTo,
                                                             const String::ArrayOfConst& cratesToAdd);

#if NXA_PRINT_SERATO_DEBUG_INFO
    static void debugListCrate(Serato::Crate& crate,
                               const String& spacing);
#endif

    #pragma mark Constants
    static const char* databaseFileCurrentVersionString;

    #pragma mark Instance Variables
    NxA::Pointer<Crate> rootFolder;
    NxA::Pointer<Track::Array> tracks;

    NxA::Pointer<String::ArrayOfConst> pathsForSeratoDirectories;
    NxA::Pointer<String::ArrayOfConst> volumePathsPerPath;
    NxA::Pointer<Tag::ArrayOfConst::Array> otherTagsPerPath;
    NxA::Pointer<String::ArrayOfConst::Array> smartCrateNamesPerPath;

    boolean databaseIsValid;
    boolean databaseTracksWereModified;

    #pragma mark Instance Methods
    NxA::Pointer<Tag::ArrayOfConst> parseDatabaseFileAtLocatedOnVolumeAndReturnOtherTags(const String& databasePath,
                                                                                         const String& volumePath);
    void parseAnyDatabaseFilesIn(const String& pathForLocalSeratoFolder,
                                 const String::ArrayOfConst& pathsForExternalSeratoFolders);

    void storeTrackTagLocatedOnVolume(ObjectTag& tag, const String& volumePath);
};

NXA_EXIT_NAMESPACE;
NXA_EXIT_NAMESPACE;
