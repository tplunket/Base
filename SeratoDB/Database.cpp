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

#include "Database.hpp"
#include "Tags/DatabaseV2Tags.hpp"
#include "Tags/TagFactory.hpp"
#include "Tags/TextTag.hpp"

#include <Base/File.hpp>

using namespace NxA;
using namespace NxA::Serato;
using namespace std;

#define PRINT_DEBUG_INFO        0

#pragma mark Constants

static const char* databaseFileCurrentVersionString = "2.0/Serato Scratch LIVE Database";

#pragma mark Utility Functions

#if PRINT_DEBUG_INFO
static void p_debugListCrate(const Crate* crate, std::string spacing)
{
    const CrateVector& crates = crate->crates();
    for (const CratePtr& crate : crates) {
        const string& crateName = crate->crateName();
        printf("%sCrate '%s'\n", spacing.c_str(), crateName.c_str());

        const TrackEntryVector& crateTracks = crate->trackEntries();
        for (const TrackEntryPtr& trackEntry : crateTracks) {
            printf("%s   Track '%s'\n", spacing.c_str(), trackEntry->trackFilePath()->c_str());
        }

        p_debugListCrate(crate.get(), (spacing + "   "));
    }
}
#endif

#pragma mark Constructors

Database::Database(const char* seratoFolderPath) :
                   p_databaseFilePath(databaseFilePathForSeratoFolder(seratoFolderPath)),
                   p_tracks(make_unique<TrackVector>()),
                   p_databaseIsValid(false)
{
    auto databaseFile = File::readFileAt(this->p_databaseFilePath);

    TagVectorPtr tags(TagFactory::parseTagsAt(databaseFile->data(), databaseFile->size()));
    for (auto& tag : *tags) {
        switch (tag->identifier()) {
            case NxASeratoTrackObjectTag: {
                this->p_storeTrackTag(move(tag));
                break;
            }
            case NxASeratoDatabaseVersionTag: {
                auto& versionText = (dynamic_cast<TextTag&>(*tag)).value();
                if (!versionText->isEqualTo(databaseFileCurrentVersionString)) {
                    this->p_tracks->clear();
                    this->p_otherTags.clear();
                    return;
                }
                break;
            }
            default: {
                this->p_storeOtherTag(move(tag));
                break;
            }
        }
    }

    // -- TODO: This will eventually select the root folder based on where the database is.
    this->p_crateOrderFile = make_unique<CrateOrderFile>(seratoFolderPath, "", *this);

#if PRINT_DEBUG_INFO
    p_debugListCrate(this->rootCrate(), "");
#endif

    this->p_databaseIsValid = true;
}

#pragma mark Class Methods

String::Pointer Database::versionAsStringForDatabaseIn(const char* seratoFolderPath)
{
    auto databaseFilePath = databaseFilePathForSeratoFolder(seratoFolderPath);
    auto databaseFile = File::readFileAt(databaseFilePath);

    TagVectorPtr tags(TagFactory::parseTagsAt(databaseFile->data(), databaseFile->size()));
    for (const TagPtr& tag : *(tags)) {
        if (tag->identifier() == NxASeratoDatabaseVersionTag) {
            const TextTag* textTag = dynamic_cast<const TextTag*>(tag.get());
            return String::stringWith(textTag->value());
        }
    }

    return String::string();
}

#pragma mark Instance Methods

void Database::p_storeTrackTag(TagPtr tag)
{
    // -- TODO: This will eventually select the root folder based on where the database is.
    this->p_tracks->push_back(make_unique<Track>(move(tag), ""));
}

void Database::p_storeOtherTag(TagPtr tag)
{
    this->p_otherTags.push_back(move(tag));
}

time_t Database::databaseModificationDateInSecondsSince1970(void) const
{
    return File::modificationDateInSecondsSince1970ForFile(this->p_databaseFilePath);
}

time_t Database::rootCrateModificationDateInSecondsSince1970(void) const
{
    return this->p_crateOrderFile->modificationDateInSecondsSince1970();
}

const Crate* Database::rootCrate(void) const
{
    return this->p_crateOrderFile->rootCrate();
}

const TrackVector& Database::tracks(void) const
{
    return *this->p_tracks;
}

TrackVectorPtr Database::removeAndReturnTracks(void)
{
    TrackVectorPtr tracks = move(this->p_tracks);
    this->p_tracks = TrackVectorPtr(make_unique<TrackVector>());

    return tracks;
}

void Database::addCrateFileToDelete(String::ConstPointer const& path)
{
    this->p_crateFilesToDelete.push_back(path);
}

void Database::addTrack(TrackPtr track)
{
    this->p_tracks->push_back(move(track));
}

void Database::saveIfModified(void) const
{
    if (!this->p_databaseIsValid) {
        return;
    }

    for (auto& path : this->p_crateFilesToDelete) {
        File::deleteFileAt(path);
    }

    this->p_crateOrderFile->saveIfModified();

    bool someTracksWereModified = false;
    for (auto& track : *this->p_tracks) {
        if (!track->wasModified()) {
            continue;
        }

        printf("Saving modifications to Serato track '%s'.\n", track->title()->toUTF8());
        track->saveToTrackFile();
        someTracksWereModified = true;
    }

    if (someTracksWereModified) {
        auto outputData = Blob::blob();

        TagPtr versionTag(make_unique<TextTag>(NxASeratoDatabaseVersionTag, databaseFileCurrentVersionString));
        versionTag->addTo(outputData);

        for (auto& track : *this->p_tracks) {
            track->addTo(outputData);
        }

        for (auto& tag : this->p_otherTags) {
            tag->addTo(outputData);
        }

        File::writeBlobToFileAt(outputData, this->p_databaseFilePath);
    }
}
