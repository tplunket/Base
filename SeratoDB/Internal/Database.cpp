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
                   otherCrateNamesPerPath(String::ArrayOfConst::Array::array()),
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
                                                               String::ArrayOfConst& unknownCratesNames)
{
    auto crateOrderFilePath = Internal::Database::pathForCrateOrderFileInSeratoFolder(seratoFolderPath);
    auto cratesInOrder = Internal::Database::readCratesNamesInCrateOrderFile(crateOrderFilePath);

    auto subCratesDirectory = NxA::Serato::Crate::subCratesDirectoryPathInSeratoFolder(seratoFolderPath);
    auto subCratesFound = Internal::Database::cratesInSubCratesDirectory(subCratesDirectory);
    Internal::Database::addCratesNamesAtTheStartOfUnlessAlreadyThere(*cratesInOrder, *subCratesFound);

    auto it = cratesInOrder->begin();
    Internal::Database::addChildrenCratesOfCrateNamedUsingNameList(rootCrate,
                                                                         String::string(),
                                                                         it,
                                                                         cratesInOrder->end(),
                                                                         seratoFolderPath,
                                                                         volumePath,
                                                                         unknownCratesNames);

    rootCrate.resetModificationFlags();
}

void Database::saveContentOfRootCrateIfModifiedAndOnVolumeAndUnknownCrateNamesToSeratoFolder(const Serato::Crate& rootCrate,
                                                                                             const String& volumePath,
                                                                                             const String::ArrayOfConst& unknownCratesNames,
                                                                                             const String& seratoFolderPath)
{
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
    rootCrate.addFullCrateNameWithPrefixAndRecurseToChildren(result, "[crate]");
    for (auto& crateName : unknownCratesNames) {
        result->append("[crate]");
        result->append(crateName);
        result->append("\n");
    }
    result->append("[end record]\n");

    auto crateOrderFilePath = Database::pathForCrateOrderFileInSeratoFolder(seratoFolderPath);
    File::writeBlobToFileAt(result->toUTF16(), crateOrderFilePath);
}

NxA::String::Pointer Database::crateNameIfValidCrateOrEmptyIfNot(const String& name)
{
    auto result = String::string();

    if (name.hasPrefix("[crate]")) {
        result = name.subString(7);
    }

    return result;
}

NxA::boolean Database::filenameIsAValidCrateName(const String& fileName)
{
    return !fileName.hasPrefix(".") && fileName.hasPostfix(".crate");
}

NxA::String::Pointer Database::crateNameFromFilename(const String& fileName)
{
    return fileName.subString(0, fileName.length() - 6);
}

NxA::String::ArrayOfConst::Pointer Database::cratesInSubCratesDirectory(const String& directory)
{
    auto cratePathsFound = File::pathsForFilesInDirectory(directory);
    auto crateNamesFound = String::ArrayOfConst::array();

    for (auto& path : *cratePathsFound) {
        auto fileName = path->subString(directory.length() + 1, path->length());

        if (Database::filenameIsAValidCrateName(fileName)) {
            crateNamesFound->append(Database::crateNameFromFilename(fileName));
        }
    }

    return crateNamesFound;
}

NxA::String::ArrayOfConst::Pointer Database::readCratesNamesInCrateOrderFile(const String& crateOrderFilePath)
{
    auto cratesInOrder = String::ArrayOfConst::array();

    if (File::fileExistsAt(crateOrderFilePath)) {
        auto crateOrderFile = File::readFileAt(crateOrderFilePath);
        if (crateOrderFile->size()) {
            auto textAsString = String::stringWithUTF16(crateOrderFile);
            auto lines = textAsString->splitBySeperator('\n');
            for (auto& crateLine : *lines) {
                auto fullCrateName = Database::crateNameIfValidCrateOrEmptyIfNot(crateLine);
                if (fullCrateName->isEmpty()) {
                    continue;
                }

                cratesInOrder->append(fullCrateName);
            }
        }
    }

    return cratesInOrder;
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

void Database::addChildrenCratesOfCrateNamedUsingNameList(Serato::Crate& parentCrate,
                                                                const String& name,
                                                                String::ArrayOfConst::iterator& it,
                                                                const String::ArrayOfConst::iterator& end,
                                                                const String& seratoFolderPath,
                                                                const String& volumePath,
                                                                String::ArrayOfConst& unknownCratesNames)
{
    while (it != end) {
        auto fullCrateName = *it;
        if (name.length() && !fullCrateName->hasPrefix(name)) {
            break;
        }

        if (Serato::Crate::isASmartCrateName(fullCrateName, seratoFolderPath) ||
            !Serato::Crate::isAValidCrateName(fullCrateName, seratoFolderPath)) {
            unknownCratesNames.append(fullCrateName);
            ++it;
            continue;
        }

        auto newCrate = parentCrate.findOrAddCrateWithName(fullCrateName);
        newCrate->readFromFolderInVolume(seratoFolderPath, volumePath);

        ++it;

        auto crateNameWithSeperator = String::stringWith(fullCrateName);
        crateNameWithSeperator->append("%%");

        Database::addChildrenCratesOfCrateNamedUsingNameList(newCrate,
                                                                   crateNameWithSeperator,
                                                                   it,
                                                                   end,
                                                                   seratoFolderPath,
                                                                   volumePath,
                                                                   unknownCratesNames);
        
        newCrate->resetModificationFlags();
    }
}

#if NXA_PRINT_DEBUG_INFO
void Database::debugListCrate(Serato::Crate& crate,
                              const String& spacing)
{
    auto& crates = crate.crates();
    for (auto& subCrate : crates) {
        auto& crateName = subCrate->crateName();
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

void Database::parseAnyDatabaseFilesIn(const String& pathForLocalSeratoFolder,
                                       const String::ArrayOfConst& pathsForExternalSeratoFolders)
{
    this->pathsForSeratoDirectories->append(pathForLocalSeratoFolder);
    this->pathsForSeratoDirectories->append(pathsForExternalSeratoFolders);

    boolean firstPath = true;

    count previousTrackCount = 0;

    for (auto& path : *(this->pathsForSeratoDirectories)) {
        auto otherTags = Serato::Tag::ArrayOfConst::array();
        auto otherCrateNames = String::ArrayOfConst::array();
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

            auto databaseFile = File::readFileAt(databasePath);

            auto tags = TagFactory::parseTagsAt(databaseFile->data(), databaseFile->size());
            for (auto& tag : *tags) {
                switch (tag->identifier()) {
                    case trackObjectTagIdentifier: {
                        storeTrackTagLocatedOnVolume(dynamic_cast<Serato::ObjectTag&>(*tag), volumePath);
                        break;
                    }
                    case databaseVersionTagIdentifier: {
                        auto& versionText = dynamic_cast<Serato::VersionTag&>(*tag).value();
                        if (versionText != databaseFileCurrentVersionString) {
                            throw NxA::Serato::DatabaseError::exceptionWith("Illegal database version for file at '%s'.", databasePath->toUTF8());
                            return;
                        }
                        break;
                    }
                    default: {
                        otherTags->append(tag);
                        break;
                    }
                }
            }

            Internal::Database::addCratesFoundInSeratoFolderOnVolumeToRootCrate(seratoFolderPath,
                                                                                volumePath,
                                                                                this->rootCrate,
                                                                                otherCrateNames);
        }

        printf("  found %ld tracks and %ld other tags.\n", this->tracks->length() - previousTrackCount, otherTags->length());

        previousTrackCount = this->tracks->length();

        NXA_ASSERT_TRUE(volumePath->length() != 0);
        
        this->volumePathsPerPath->append(volumePath);
        this->otherTagsPerPath->append(*otherTags);
        this->otherCrateNamesPerPath->append(*otherCrateNames);
    }

#if NXA_PRINT_DEBUG_INFO
    debugListCrate(this->rootCrate, String::string());
#endif

    databaseIsValid = true;
}

void Database::storeTrackTagLocatedOnVolume(Serato::ObjectTag& tag, const String& volumePath)
{
    auto track = Serato::Track::trackWithTagLocatedOnVolume(tag, volumePath);
    this->tracks->append(track);
}
