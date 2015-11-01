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

#include "SeratoDB/Database.hpp"
#include "SeratoDb/Internal/Database.hpp"
#include "SeratoDB/CrateOrderFile.hpp"
#include "Tags/DatabaseV2Tags.hpp"
#include "Tags/TagFactory.hpp"
#include "Tags/VersionTag.hpp"

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, Database, Object);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Constants

static const char* databaseFileCurrentVersionString = "2.0/Serato Scratch LIVE Database";

#pragma mark Factory Methods

Database::Pointer Database::databaseWithFileAt(const String& seratoFolderPath)
{
    auto crateOrderFile = CrateOrderFile::fileWithSeratoFolderInRootFolder(seratoFolderPath,
                                                                           String::string());
    auto internalObject = Internal::Database::Pointer(std::make_shared<Internal::Database>(databaseFilePathForSeratoFolder(seratoFolderPath),
                                                                                           crateOrderFile));
    auto newDatabase = Database::makeSharedWithInternal(NxA::Internal::Object::Pointer::dynamicCastFrom(internalObject));

    auto databaseFile = File::readFileAt(newDatabase->internal->databaseFilePath);

    auto tags = TagFactory::parseTagsAt(databaseFile->data(), databaseFile->size());
    for (auto& tag : *tags) {
        switch (tag->identifier()) {
            case trackObjectTagIdentifier: {
                newDatabase->internal->storeTrackTag(dynamic_cast<ObjectTag&>(*tag));
                break;
            }
            case databaseVersionTagIdentifier: {
                auto& versionText = dynamic_cast<VersionTag&>(*tag).value();
                if (versionText != databaseFileCurrentVersionString) {
                    newDatabase->internal->tracks->emptyAll();
                    newDatabase->internal->otherTags->emptyAll();
                    return newDatabase;
                }
                break;
            }
            default: {
                newDatabase->internal->storeOtherTag(tag);
                break;
            }
        }
    }

#if NXA_PRINT_DEBUG_INFO
    Internal::Database::debugListCrate(newDatabase->rootCrate(), String::string());
#endif

    newDatabase->internal->databaseIsValid = true;

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
    auto seratoFolderPath = seratoFolderPathForFolder(folderPath);
    auto databaseFilePath = databaseFilePathForSeratoFolder(seratoFolderPath);
    return File::fileExistsAt(databaseFilePath);
}

void Database::setDatabaseFilesInSeratoFolderAsModifedOnDateInSecondsSince1970(const String& folderPath, timestamp dateModified)
{
    auto databaseFilePath = databaseFilePathForSeratoFolder(folderPath);
    File::setModificationDateInSecondsSince1970ForFile(dateModified, databaseFilePath);

    auto crateOrderFilePath = CrateOrderFile::pathForFileInSeratoFolder(folderPath);
    File::setModificationDateInSecondsSince1970ForFile(dateModified, crateOrderFilePath);
}

#pragma mark Instance Methods

timestamp Database::databaseModificationDateInSecondsSince1970(void) const
{
    return File::modificationDateInSecondsSince1970ForFile(internal->databaseFilePath);
}

timestamp Database::rootCrateModificationDateInSecondsSince1970(void) const
{
    return internal->crateOrderFile->modificationDateInSecondsSince1970();
}

Crate& Database::rootCrate(void) const
{
    return internal->crateOrderFile->rootCrate();
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

void Database::removeTrackEntry(TrackEntry& trackEntry)
{
    if (trackEntry.hasParentCrate()) {
        trackEntry.removeFromParentCrate();
    }
}

void Database::removeCrate(Crate& crate)
{
    internal->crateFilesToDelete->append(String::stringWith(crate.crateFilePath()));

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

    for (auto& path : *(internal->crateFilesToDelete)) {
        File::deleteFileAt(path);
    }

    internal->crateOrderFile->saveIfModified();

    boolean needsToUpdateDatabaseFile = false;
    for (auto& track : *(internal->tracks)) {
        if (track->needsToUpdateDatabaseFile()) {
            needsToUpdateDatabaseFile = true;
        }

        if (track->needsToUpdateTrackFile()) {
            track->saveToTrackFile();
        }
    }

    if (needsToUpdateDatabaseFile) {
        auto outputData = Blob::blob();

        auto versionTag = VersionTag::tagWithIdentifierAndValue(databaseVersionTagIdentifier,
                                                                String::stringWith(databaseFileCurrentVersionString));
        versionTag->addTo(outputData);

        for (auto& track : *(internal->tracks)) {
            track->addTo(outputData);
        }

        for (auto& tag : *(internal->otherTags)) {
            tag->addTo(outputData);
        }

        File::writeBlobToFileAt(outputData, internal->databaseFilePath);
    }
}
