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

#include "SeratoDB/Database.hpp"
#include "SeratoDB/Exceptions.hpp"
#include "SeratoDB/Internal/InternalDatabase.hpp"
#include "SeratoDB/Tags/DatabaseV2Tags.hpp"
#include "SeratoDB/Tags/TagFactory.hpp"
#include "SeratoDB/Tags/VersionTag.hpp"

// -- Generated internal implementation ommitted because this class does not use the default contructor.

using namespace NxA::Serato;

#pragma mark Constants

const char* InternalDatabase::databaseFileCurrentVersionString = "2.0/Serato Scratch LIVE Database";

#pragma mark Constructors & Destructors

InternalDatabase::InternalDatabase(const String& pathForLocalSeratoFolder,
                                   const String::ArrayOfConst& pathsForExternalSeratoFolders) :
    rootFolder(Crate::crateWithName(String::string())),
    tracks(Track::Array::array()),
    pathsForSeratoDirectories(String::ArrayOfConst::array()),
    volumePathsPerPath(String::ArrayOfConst::array()),
    otherTagsPerPath(Tag::ArrayOfConst::Array::array()),
    smartCrateNamesPerPath(String::ArrayOfConst::Array::array()),
    databaseIsValid(false),
    databaseTracksWereModified(false)
{
    this->parseAnyDatabaseFilesIn(pathForLocalSeratoFolder, pathsForExternalSeratoFolders);
}

#pragma mark Class Methods

NxA::Pointer<NxA::String> InternalDatabase::pathForCrateOrderFileInSeratoFolder(const String& seratoFolderPath)
{
    auto joinedPath = File::joinPaths(seratoFolderPath, String::stringWith("neworder.pref"));
    return joinedPath;
}

void InternalDatabase::addCratesFoundInSeratoFolderOnVolumeToRootFolder(const String& seratoFolderPath,
                                                                        const String& volumePath,
                                                                        Crate& rootFolder,
                                                                        String::ArrayOfConst& smartCrateNames)
{
    auto crateOrderFilePath = InternalDatabase::pathForCrateOrderFileInSeratoFolder(seratoFolderPath);
    auto cratesInOrder = Crate::readCratesNamesInCrateOrderFile(crateOrderFilePath);

    auto subCratesDirectory = Crate::subCratesDirectoryPathInSeratoFolder(seratoFolderPath);
    auto subCratesFound = Crate::cratesInSubCratesDirectory(subCratesDirectory);
    InternalDatabase::addCratesNamesAtTheStartOfUnlessAlreadyThere(*cratesInOrder, *subCratesFound);

    Crate::parseCratesInSeratoFolderOnVolumeAddToCrateAndSaveSmartCrateNamesIn(cratesInOrder,
                                                                               seratoFolderPath,
                                                                               volumePath,
                                                                               rootFolder,
                                                                               smartCrateNames);

    rootFolder.resetModificationFlags();
}

void InternalDatabase::saveContentOfRootFolderIfModifiedAndOnVolumeAndSmartCrateNamesToSeratoFolder(const Crate& rootFolder,
                                                                                                    const String& volumePath,
                                                                                                    const String::ArrayOfConst& smartCrateNames,
                                                                                                    const String& seratoFolderPath)
{
    try {
        if (!rootFolder.childrenCratesWereModified()) {
            return;
        }

        auto subCratesDirectory = Crate::subCratesDirectoryPathInSeratoFolder(seratoFolderPath);
        if (File::directoryExistsAt(subCratesDirectory)) {
            auto cratePathsFound = File::pathsForFilesInDirectory(subCratesDirectory);
            for (auto& path : *cratePathsFound) {
                File::deleteFileAt(path);
            }
        }

        auto result = String::string();
        result->append("[begin record]\n");

        for (auto& crate : rootFolder.crates()) {
            crate->saveIfOnVolumeAndRecurseToChildren(volumePath, seratoFolderPath);
            crate->addFullCrateNameWithPrefixForCratesOnVolumeAndRecurseToChildren(result, "[crate]", volumePath);
        }

        for (auto& crateName : smartCrateNames) {
            result->append("[crate]");
            result->append(crateName);
            result->append("\n");
        }

        result->append("[end record]\n");

        auto crateOrderFilePath = InternalDatabase::pathForCrateOrderFileInSeratoFolder(seratoFolderPath);
        File::writeBlobToFileAt(result->toUTF16(), crateOrderFilePath);
    }
    catch (FileError& e) {
        // -- This should be logged.
    }
}

void InternalDatabase::setDatabaseFilesInSeratoFolderAsModifedOnDateInSecondsSince1970(const String& folderPath, timestamp dateModified)
{
    auto databaseFilePath = Database::databaseFilePathForSeratoFolder(folderPath);
    File::setModificationDateInSecondsSince1970ForFile(dateModified, databaseFilePath);

    auto crateOrderFilePath = InternalDatabase::pathForCrateOrderFileInSeratoFolder(folderPath);
    File::setModificationDateInSecondsSince1970ForFile(dateModified, crateOrderFilePath);

    auto subCratesDirectory = Crate::subCratesDirectoryPathInSeratoFolder(folderPath);
    if (File::directoryExistsAt(subCratesDirectory)) {
        auto cratePathsFound = File::pathsForFilesInDirectory(subCratesDirectory);
        for (auto& path : *cratePathsFound) {
            File::setModificationDateInSecondsSince1970ForFile(dateModified, path);
        }
    }
}

void InternalDatabase::addCratesNamesAtTheStartOfUnlessAlreadyThere(String::ArrayOfConst& cratesToAddTo,
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
void InternalDatabase::debugListCrate(Crate& crate,
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

        Pointer<String> newSpacing = String::stringWith(spacing);
        newSpacing->append("   ");

        Database::debugListCrate(subCrate, newSpacing);
    }
}
#endif

#pragma mark Instance Methods

NxA::Pointer<Tag::ArrayOfConst> InternalDatabase::parseDatabaseFileAtLocatedOnVolumeAndReturnOtherTags(const String& databasePath,
                                                                                                  const String& volumePath)
{
    auto otherTags = Tag::ArrayOfConst::array();

    try {
        auto databaseFile = File::readFileAt(databasePath);

        auto tags = TagFactory::parseTagsAt(databaseFile->data(), databaseFile->size(), databasePath);
        for (auto& tag : *tags) {
            switch (tag->identifier()) {
                case trackObjectTagIdentifier: {
                    this->storeTrackTagLocatedOnVolume(dynamic_cast<ObjectTag&>(*tag), volumePath);
                    break;
                }
                case databaseVersionTagIdentifier: {
                    auto& versionText = dynamic_cast<VersionTag&>(*tag).value();
                    if (versionText != databaseFileCurrentVersionString) {
                        throw DatabaseError::exceptionWith("Illegal database version for file at '%s'.", databasePath.toUTF8());
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
    catch (DatabaseError& e) {
        // -- This should log something.
    }
    catch (FileError& e) {
        // -- This should log something.
    }

    return otherTags;
}

void InternalDatabase::parseAnyDatabaseFilesIn(const String& pathForLocalSeratoFolder,
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
        NxA::Pointer<const String> volumePath(path);

        if (Database::containsAValidSeratoFolder(path)) {
            auto seratoFolderPath = Database::seratoFolderPathForFolder(path);
            auto databasePath = Database::databaseFilePathForSeratoFolder(*seratoFolderPath);

            if (firstPath) {
                volumePath = String::stringWith("/");
                firstPath = false;
            }
            else {
                volumePath = path;
            }

            auto otherTags = InternalDatabase::parseDatabaseFileAtLocatedOnVolumeAndReturnOtherTags(databasePath, volumePath);
            this->otherTagsPerPath->append(otherTags);

            InternalDatabase::addCratesFoundInSeratoFolderOnVolumeToRootFolder(seratoFolderPath,
                                                                               volumePath,
                                                                               this->rootFolder,
                                                                               smartCrateNames);
        }
        else {
            this->otherTagsPerPath->append(Tag::ArrayOfConst::array());
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
    debugListCrate(this->rootFolder, String::string());
#endif

    databaseIsValid = true;
}

void InternalDatabase::storeTrackTagLocatedOnVolume(ObjectTag& tag, const String& volumePath)
{
    auto track = Track::trackWithTagLocatedOnVolume(tag, volumePath);
    this->tracks->append(track);
}
