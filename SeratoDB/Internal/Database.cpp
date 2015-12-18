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

#include "SeratoDB/Database.hpp"
#include "Internal/Database.hpp"
#include "Tags/DatabaseV2Tags.hpp"
#include "Tags/TagFactory.hpp"
#include "Tags/VersionTag.hpp"

// -- Generated internal implementation ommitted because this class does not use the default contructor.

using namespace NxA::Serato::Internal;

#pragma mark Constants

const char* Database::databaseFileCurrentVersionString = "2.0/Serato Scratch LIVE Database";

#pragma mark Constructors & Destructors

Database::Database(const String& pathForLocalSeratoFolder,
                   const String::ArrayOfConst& pathsForExternalSeratoFolders) :
                   rootCrate(Serato::Crate::crateWithName(String::string())),
                   tracks(Serato::Track::Array::array()),
                   pathsForSeratoDirectories(String::ArrayOfConst::array()),
                   volumePathsPerPath(String::ArrayOfConst::array()),
                   otherTagsPerPath(Serato::Tag::ArrayOfConst::Array::array()),
                   smartCrateNamesPerPath(String::ArrayOfConst::Array::array()),
                   databaseIsValid(false)
{
    this->parseAnyDatabaseFilesIn(pathForLocalSeratoFolder, pathsForExternalSeratoFolders);
}

#pragma mark Class Methods

NxA::String::Pointer Database::pathForCrateOrderFileInSeratoFolder(const String& seratoFolderPath)
{
    auto joinedPath = File::joinPaths(seratoFolderPath, String::stringWith("neworder.pref"));
    return joinedPath;
}

void Database::addCratesFoundInSeratoFolderOnVolumeToRootCrate(const String& seratoFolderPath,
                                                               const String& volumePath,
                                                               Serato::Crate& rootCrate,
                                                               String::ArrayOfConst& smartCrateNames)
{
    auto crateOrderFilePath = Internal::Database::pathForCrateOrderFileInSeratoFolder(seratoFolderPath);
    auto cratesInOrder = Serato::Crate::readCratesNamesInCrateOrderFile(crateOrderFilePath);

    auto subCratesDirectory = NxA::Serato::Crate::subCratesDirectoryPathInSeratoFolder(seratoFolderPath);
    auto subCratesFound = Serato::Crate::cratesInSubCratesDirectory(subCratesDirectory);
    Internal::Database::addCratesNamesAtTheStartOfUnlessAlreadyThere(*cratesInOrder, *subCratesFound);

    Serato::Crate::parseCratesInSeratoFolderOnVolumeAddToCrateAndSaveSmartCrateNamesIn(cratesInOrder,
                                                                                       seratoFolderPath,
                                                                                       volumePath,
                                                                                       rootCrate,
                                                                                       smartCrateNames);

    rootCrate.resetModificationFlags();
}

void Database::saveContentOfRootCrateIfModifiedAndOnVolumeAndSmartCrateNamesToSeratoFolder(const Serato::Crate& rootCrate,
                                                                                           const String& volumePath,
                                                                                           const String::ArrayOfConst& smartCrateNames,
                                                                                           const String& seratoFolderPath)
{
    try {
        if (!rootCrate.childrenCratesWereModified()) {
            return;
        }

        auto subCratesDirectory = Serato::Crate::subCratesDirectoryPathInSeratoFolder(seratoFolderPath);
        if (File::directoryExistsAt(subCratesDirectory)) {
            auto cratePathsFound = File::pathsForFilesInDirectory(subCratesDirectory);
            for (auto& path : *cratePathsFound) {
                File::deleteFileAt(path);
            }
        }

        rootCrate.saveIfOnVolumeAndRecurseToChildren(volumePath, seratoFolderPath);

        auto result = String::string();
        result->append("[begin record]\n");
        rootCrate.addFullCrateNameWithPrefixForCratesOnVolumeAndRecurseToChildren(result, "[crate]", volumePath);
        for (auto& crateName : smartCrateNames) {
            result->append("[crate]");
            result->append(crateName);
            result->append("\n");
        }
        result->append("[end record]\n");

        auto crateOrderFilePath = Database::pathForCrateOrderFileInSeratoFolder(seratoFolderPath);
        File::writeBlobToFileAt(result->toUTF16(), crateOrderFilePath);
    }
    catch (FileError& e) {
        // -- This should be logged.
    }
}

void Database::setDatabaseFilesInSeratoFolderAsModifedOnDateInSecondsSince1970(const String& folderPath, timestamp dateModified)
{
    auto databaseFilePath = Serato::Database::databaseFilePathForSeratoFolder(folderPath);
    File::setModificationDateInSecondsSince1970ForFile(dateModified, databaseFilePath);

    auto crateOrderFilePath = Internal::Database::pathForCrateOrderFileInSeratoFolder(folderPath);
    File::setModificationDateInSecondsSince1970ForFile(dateModified, crateOrderFilePath);

    auto subCratesDirectory = Serato::Crate::subCratesDirectoryPathInSeratoFolder(folderPath);
    if (File::directoryExistsAt(subCratesDirectory)) {
        auto cratePathsFound = File::pathsForFilesInDirectory(subCratesDirectory);
        for (auto& path : *cratePathsFound) {
            File::setModificationDateInSecondsSince1970ForFile(dateModified, path);
        }
    }
}

void Database::addCratesNamesAtTheStartOfUnlessAlreadyThere(String::ArrayOfConst& cratesToAddTo,
                                                            const String::ArrayOfConst& cratesToAdd)
{
    count insertionIndex = 0;
    for (auto& crateName : cratesToAdd) {
        boolean alreadyHaveThisCrate = cratesToAddTo.contains(crateName);
        if (alreadyHaveThisCrate) {
            continue;
        }

        cratesToAddTo.insertAt(*crateName, cratesToAddTo.begin() + insertionIndex);

        ++insertionIndex;
    }
}

#if NXA_PRINT_SERATO_DEBUG_INFO
void Database::debugListCrate(Serato::Crate& crate,
                              const String& spacing)
{
    auto& crates = crate.crates();
    for (auto& subCrate : crates) {
        auto& crateName = subCrate->name();
        printf("%sCrate '%s'\n", spacing.toUTF8(), crateName.toUTF8());

        auto crateTracks = subCrate->trackEntries();
        for (auto& trackEntry : *crateTracks) {
            printf("%s   Track '%s'\n", spacing.toUTF8(), trackEntry->trackFilePath()->toUTF8());
        }

        String::Pointer newSpacing = String::stringWith(spacing);
        newSpacing->append("   ");

        Database::debugListCrate(subCrate, newSpacing);
    }
}
#endif

#pragma mark Instance Methods

NxA::Serato::Tag::ArrayOfConst::Pointer Database::parseDatabaseFileAtLocatedOnVolumeAndReturnOtherTags(const String& databasePath,
                                                                                                       const String& volumePath)
{
    auto otherTags = Serato::Tag::ArrayOfConst::array();

    try {
        auto databaseFile = File::readFileAt(databasePath);

        auto tags = TagFactory::parseTagsAt(databaseFile->data(), databaseFile->size());
        for (auto& tag : *tags) {
            switch (tag->identifier()) {
                case trackObjectTagIdentifier: {
                    this->storeTrackTagLocatedOnVolume(dynamic_cast<Serato::ObjectTag&>(*tag), volumePath);
                    break;
                }
                case databaseVersionTagIdentifier: {
                    auto& versionText = dynamic_cast<Serato::VersionTag&>(*tag).value();
                    if (versionText != databaseFileCurrentVersionString) {
                        throw Serato::DatabaseError::exceptionWith("Illegal database version for file at '%s'.", databasePath.toUTF8());
                    }
                    break;
                }
                default: {
                    otherTags->append(tag);
                    break;
                }
            }
        }
    }
    catch (Serato::DatabaseError& e) {
        // -- This should log something.
    }
    catch (FileError& e) {
        // -- This should log something.
    }

    return otherTags;
}

void Database::parseAnyDatabaseFilesIn(const String& pathForLocalSeratoFolder,
                                       const String::ArrayOfConst& pathsForExternalSeratoFolders)
{
    this->pathsForSeratoDirectories->append(pathForLocalSeratoFolder);
    this->pathsForSeratoDirectories->append(pathsForExternalSeratoFolders);

    boolean firstPath = true;

#if NXA_PRINT_SERATO_DEBUG_INFO
    count previousTrackCount = 0;
    count previousOtherTagsCount = 0;
#endif

    for (auto& path : *(this->pathsForSeratoDirectories)) {
        auto smartCrateNames = String::ArrayOfConst::array();
        String::PointerToConst volumePath(path);

        if (Serato::Database::containsAValidSeratoFolder(path)) {
            auto seratoFolderPath = Serato::Database::seratoFolderPathForFolder(path);
            auto databasePath = Serato::Database::databaseFilePathForSeratoFolder(*seratoFolderPath);

            if (firstPath) {
                volumePath = String::stringWith("/");
                firstPath = false;
            }
            else {
                volumePath = path;
            }

            auto otherTags = Database::parseDatabaseFileAtLocatedOnVolumeAndReturnOtherTags(databasePath, volumePath);
            this->otherTagsPerPath->append(otherTags);

            Database::addCratesFoundInSeratoFolderOnVolumeToRootCrate(seratoFolderPath,
                                                                      volumePath,
                                                                      this->rootCrate,
                                                                      smartCrateNames);
        }
        else {
            this->otherTagsPerPath->append(NxA::Serato::Tag::ArrayOfConst::array());
        }

#if NXA_PRINT_SERATO_DEBUG_INFO
        printf("  found %ld tracks and %ld other tags.\n",
               this->tracks->length() - previousTrackCount,
               this->otherTagsPerPath->length() - previousOtherTagsCount);
        previousTrackCount = this->tracks->length();
        previousOtherTagsCount = this->otherTagsPerPath->length();
#endif

        NXA_ASSERT_TRUE(volumePath->length() != 0);
        
        this->volumePathsPerPath->append(volumePath);
        this->smartCrateNamesPerPath->append(*smartCrateNames);
    }

#if NXA_PRINT_SERATO_DEBUG_INFO
    debugListCrate(this->rootCrate, String::string());
#endif

    databaseIsValid = true;
}

void Database::storeTrackTagLocatedOnVolume(Serato::ObjectTag& tag, const String& volumePath)
{
    auto track = Serato::Track::trackWithTagLocatedOnVolume(tag, volumePath);
    this->tracks->append(track);
}
