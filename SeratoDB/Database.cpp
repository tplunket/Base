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
#include "SeratoDB/Internal/Database.hpp"
#include "Tags/DatabaseV2Tags.hpp"
#include "Tags/TagFactory.hpp"
#include "Tags/VersionTag.hpp"

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, Database, Object);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Factory Methods

Database::Pointer Database::databaseWithPathsForLocalAndExternalSeratoDirectories(const String& pathForLocalSeratoFolder,
                                                                                  const String::ArrayOfConst& pathsForExternalSeratoFolders)
{
    auto internalObject = Internal::Database::Pointer(std::make_shared<Internal::Database>(pathForLocalSeratoFolder, pathsForExternalSeratoFolders));
    auto newDatabase = Database::makeSharedWithInternal(NxA::Internal::Object::Pointer::dynamicCastFrom(internalObject));
    return newDatabase;
}

#pragma mark Class Methods

String::Pointer Database::versionAsStringForDatabaseIn(const String& seratoFolderPath)
{
    auto databaseFilePath = databaseFilePathForSeratoFolder(seratoFolderPath);
    auto databaseFile = File::readFileAt(databaseFilePath);

    const byte* tagAddress = databaseFile->data();
    const byte* endOfTagsAddress = databaseFile->data() + databaseFile->size();

    while (tagAddress < endOfTagsAddress) {
        if ((Tag::dataSizeForTagAt(tagAddress) > 0) &&
            (Tag::identifierForTagAt(tagAddress) == databaseVersionTagIdentifier)) {
            auto tag = TagFactory::tagForTagAt(tagAddress);
            auto& textTag = dynamic_cast<VersionTag&>(*tag);
            return String::stringWith(textTag.value());
        }

        tagAddress = Tag::nextTagAfterTagAt(tagAddress);
    }

    return String::string();
}

String::Pointer Database::seratoFolderPathForFolder(const String& folderPath)
{
    auto joinedPath = File::joinPaths(folderPath, String::stringWith("_Serato_"));
    return joinedPath;
}

String::Pointer Database::databaseFilePathForSeratoFolder(const String& seratoFolderPath)
{
    auto joinedPath = File::joinPaths(seratoFolderPath, String::stringWith("database V2"));
    return joinedPath;
}

boolean Database::containsAValidSeratoFolder(const String& folderPath)
{
    auto seratoFolderPath = Database::seratoFolderPathForFolder(folderPath);
    auto databaseFilePath = Database::databaseFilePathForSeratoFolder(seratoFolderPath);
    return File::fileExistsAt(databaseFilePath);
}

void Database::createSeratoFolderIfDoesNotExists(const String& seratoFolderPath)
{
    if (!File::directoryExistsAt(seratoFolderPath)) {
        File::createDirectoryAt(seratoFolderPath);
    }
}

void Database::setDatabaseFilesInSeratoFolderAsModifedOnDateInSecondsSince1970(const String& folderPath, timestamp dateModified)
{
    auto databaseFilePath = Database::databaseFilePathForSeratoFolder(folderPath);
    File::setModificationDateInSecondsSince1970ForFile(dateModified, databaseFilePath);

    auto crateOrderFilePath = Internal::Database::pathForCrateOrderFileInSeratoFolder(folderPath);
    File::setModificationDateInSecondsSince1970ForFile(dateModified, crateOrderFilePath);
}

#pragma mark Instance Methods

timestamp Database::databaseModificationDateInSecondsSince1970(void) const
{
    // -- If we don't have a database file yet, we return a very old date.
    timestamp latestTimestamp = 0;

    for (auto& path : *internal->pathsForSeratoDirectories) {
        auto seratoFolderPath = Database::seratoFolderPathForFolder(path);
        auto databaseFilePath = Database::databaseFilePathForSeratoFolder(seratoFolderPath);

        if (!File::fileExistsAt(databaseFilePath)) {
            continue;
        }

        timestamp newTimestamp = File::modificationDateInSecondsSince1970ForFile(databaseFilePath);
        if (newTimestamp > latestTimestamp) {
            latestTimestamp = newTimestamp;
        }
    }

    return latestTimestamp;
}

timestamp Database::rootCrateModificationDateInSecondsSince1970(void) const
{
    // -- If we don't have crate files yet, we return a very old date.
    timestamp latestTimestamp = 0;

    for (auto& path : *internal->pathsForSeratoDirectories) {
        auto seratoFolderPath = Database::seratoFolderPathForFolder(path);
        auto crateOrderFilePath = Internal::Database::pathForCrateOrderFileInSeratoFolder(seratoFolderPath);

        if (!File::fileExistsAt(crateOrderFilePath)) {
            continue;
        }

        timestamp newTimestamp = File::modificationDateInSecondsSince1970ForFile(crateOrderFilePath);
        if (newTimestamp > latestTimestamp) {
            latestTimestamp = newTimestamp;
        }
    }

    return latestTimestamp;
}

Crate& Database::rootCrate(void) const
{
    return internal->rootCrate;
}

const Track::Array& Database::tracks(void) const
{
    return internal->tracks;
}

Track::Array::Pointer Database::removeAndReturnTracks(void)
{
    auto tracks = internal->tracks;
    internal->tracks = Track::Array::array();

    return tracks;
}

const String& Database::volumePathForTrackFilePath(const String& trackFilePath) const
{
    count numberOfPaths = internal->volumePathsPerPath->length();
    for (count pathIndex = numberOfPaths - 1; pathIndex > 0; --pathIndex) {
        auto& volumePath = (*internal->volumePathsPerPath)[pathIndex];

        if (trackFilePath.hasPrefix(volumePath)) {
            return volumePath;
        }
    }

    NXA_ALOG("Could not find volume for track at '%s'.", trackFilePath.toUTF8());
    return (*internal->volumePathsPerPath)[0];
}

void Database::removeTrackEntry(TrackEntry& trackEntry)
{
    if (trackEntry.hasParentCrate()) {
        trackEntry.removeFromParentCrate();
    }
}

void Database::removeCrate(Crate& crate)
{
    if (crate.hasParentCrate()) {
        crate.removeFromParentCrate();
    }

    for (auto& childrenCrate : crate.crates()) {
        this->removeCrate(childrenCrate);
    }
}

void Database::addTrack(Track& track)
{
    internal->tracks->append(track);
}

void Database::removeTrack(Track& track)
{
    internal->tracks->remove(track);
}

void Database::saveIfModified(void) const
{
    if (!internal->databaseIsValid) {
        return;
    }

    count numberOfPaths = internal->pathsForSeratoDirectories->length();
    for (count pathIndex = 0; pathIndex < numberOfPaths; ++pathIndex) {
        auto& pathsForSeratoDirectory = (*internal->pathsForSeratoDirectories)[pathIndex];
        auto seratoFolderPath = Database::seratoFolderPathForFolder(pathsForSeratoDirectory);
        auto& unknownCrateNames = (*internal->otherCrateNamesPerPath)[pathIndex];
        auto& volumePath = (*internal->volumePathsPerPath)[pathIndex];

        Internal::Database::saveContentOfRootCrateIfModifiedAndOnVolumeAndUnknownCrateNamesToSeratoFolder(internal->rootCrate,
                                                                                                          volumePath,
                                                                                                          unknownCrateNames,
                                                                                                          seratoFolderPath);

        boolean needsToUpdateDatabaseFile = false;
        for (auto& track : *(internal->tracks)) {
            if (track->volumePath() != volumePath) {
                continue;
            }

            if (track->needsToUpdateDatabaseFile()) {
                needsToUpdateDatabaseFile = true;
                break;
            }
        }

        if (needsToUpdateDatabaseFile) {
            auto outputData = Blob::blob();

            auto versionTag = VersionTag::tagWithIdentifierAndValue(databaseVersionTagIdentifier,
                                                                    String::stringWith(Internal::Database::databaseFileCurrentVersionString));
            versionTag->addTo(outputData);

            for (auto& track : *(internal->tracks)) {
                if (track->volumePath() != volumePath) {
                    continue;
                }

                track->addTo(outputData);
            }

            auto& otherTags = (*internal->otherTagsPerPath)[pathIndex];
            for (auto& tag : otherTags) {
                tag->addTo(outputData);
            }

            Database::createSeratoFolderIfDoesNotExists(seratoFolderPath);

            auto databaseFilePath = Database::databaseFilePathForSeratoFolder(seratoFolderPath);
            File::writeBlobToFileAt(outputData, databaseFilePath);
        }
    }
}

void Database::saveIfModifiedAndMarkAsModifiedOn(timestamp modificationTimesSince1970) const
{
    this->saveIfModified();

    count numberOfPaths = internal->pathsForSeratoDirectories->length();
    for (count pathIndex = 0; pathIndex < numberOfPaths; ++pathIndex) {
        auto& pathsForSeratoDirectory = (*internal->pathsForSeratoDirectories)[pathIndex];
        auto seratoFolderPath = Database::seratoFolderPathForFolder(pathsForSeratoDirectory);
        auto databaseFilePath = Database::databaseFilePathForSeratoFolder(seratoFolderPath);

        if (File::fileExistsAt(databaseFilePath)) {
            File::setModificationDateInSecondsSince1970ForFile(modificationTimesSince1970, databaseFilePath);
        }
    }
}
