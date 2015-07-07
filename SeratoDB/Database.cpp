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
#include "SeratoDB/Utility.hpp"
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
                newDatabase->internal->storeTrackTag(tag);
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

#if PRINT_DEBUG_INFO
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

    auto tags(TagFactory::parseTagsAt(databaseFile->data(), databaseFile->size()));
    for (auto& tag : *(tags)) {
        if (tag->identifier() == databaseVersionTagIdentifier) {
            auto& textTag = dynamic_cast<TextTag&>(*tag);
            return String::stringWith(textTag.value());
        }
    }

    return String::string();
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

void Database::deleteTrackEntry(TrackEntry& trackEntry)
{
    trackEntry.destroy();
}

void Database::deleteCrate(Crate& crate)
{
    internal->crateFilesToDelete->append(String::stringWith(crate.crateFilePath()));
    Crate::destroy(crate);
}

void Database::addTrack(Track& track)
{
    internal->tracks->append(track.pointer());
}

void Database::deleteTrack(Track& track)
{
    track.destroy();
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

    boolean someTracksWereModified = false;
    for (auto& track : *(internal->tracks)) {
        if (!track->wasModified()) {
            continue;
        }

        printf("Saving modifications to Serato track '%s'.\n", track->title().toUTF8());
        track->saveToTrackFile();
        someTracksWereModified = true;
    }

    if (someTracksWereModified) {
        auto outputData = Blob::blob();

        auto versionTag = TextTag::tagWithIdentifierAndValue(databaseVersionTagIdentifier,
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
