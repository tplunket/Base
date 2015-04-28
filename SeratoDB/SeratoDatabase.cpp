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

#include "SeratoDB/SeratoDatabase.h"
#include "SeratoDB/SeratoDatabaseV2Tags.h"
#include "SeratoDB/SeratoTagFactory.h"
#include "SeratoDB/SeratoTextTag.h"

using namespace NxA;
using namespace std;

#define PRINT_DEBUG_INFO        0

#pragma mark Constants

static const char* NxASeratoDatabaseFileCurrentVersion = "2.0/Serato Scratch LIVE Database";

#pragma Utility Functions

#if PRINT_DEBUG_INFO
static void p_debugListCrate(const SeratoCrate* crate, std::string spacing)
{
    const SeratoCrateVector& crates = crate->crates();
    for (const SeratoCratePtr& crate : crates) {
        const string& crateName = crate->crateName();
        printf("%sCrate '%s'\n", spacing.c_str(), crateName.c_str());

        const SeratoTrackEntryVector& crateTracks = crate->trackEntries();
        for (const SeratoTrackEntryPtr& trackEntry : crateTracks) {
            printf("%s   Track '%s'\n", spacing.c_str(), trackEntry->trackFilePath()->c_str());
        }

        p_debugListCrate(crate.get(), (spacing + "   "));
    }
}
#endif

#pragma mark Constructors

SeratoDatabase::SeratoDatabase(const char* seratoFolderPath) :
    p_tracks(make_unique<SeratoTrackVector>()),
    p_databaseIsValid(false)
{
    this->p_databaseFilePath = databaseFilePathForSeratoFolder(seratoFolderPath);
    CharVectorPtr databaseFile = readFileAt(this->p_databaseFilePath->c_str());

    SeratoTagVectorPtr tags(SeratoTagFactory::parseTagsAt(databaseFile->data(), databaseFile->size()));
    for (auto& tag : *tags) {
        switch (tag->identifier()) {
            case NxASeratoTrackObjectTag: {
                this->p_storeTrackTag(move(tag));
                break;
            }
            case NxASeratoDatabaseVersionTag: {
                string& versionText = (dynamic_cast<SeratoTextTag&>(*tag)).value();
                if (versionText != NxASeratoDatabaseFileCurrentVersion) {
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
    this->p_crateOrderFile = make_unique<SeratoCrateOrderFile>(seratoFolderPath, "", *this);

#if PRINT_DEBUG_INFO
    p_debugListCrate(this->rootCrate(), "");
#endif

    this->p_databaseIsValid = true;
}

#pragma mark Class Methods

ConstStringPtr SeratoDatabase::versionAsStringForDatabaseIn(const char* seratoFolderPath)
{
    ConstStringPtr databaseFilePath = databaseFilePathForSeratoFolder(seratoFolderPath);
    CharVectorPtr databaseFile = readFileAt(databaseFilePath->c_str());

    SeratoTagVectorPtr tags(SeratoTagFactory::parseTagsAt(databaseFile->data(), databaseFile->size()));
    for (const SeratoTagPtr& tag : *(tags)) {
        if (tag->identifier() == NxASeratoDatabaseVersionTag) {
            const SeratoTextTag* textTag = dynamic_cast<const SeratoTextTag*>(tag.get());
            return make_unique<string>(textTag->value());
        }
    }

    return make_unique<string>("");
}

#pragma mark Instance Methods

void SeratoDatabase::p_storeTrackTag(SeratoTagPtr tag)
{
    // -- TODO: This will eventually select the root folder based on where the database is.
    this->p_tracks->push_back(make_unique<SeratoTrack>(move(tag), ""));
}

void SeratoDatabase::p_storeOtherTag(SeratoTagPtr tag)
{
    this->p_otherTags.push_back(move(tag));
}

time_t SeratoDatabase::databaseModificationDateInSecondsSince1970(void) const
{
    return modificationDateInSecondsSince1970ForFile(this->p_databaseFilePath->c_str());
}

time_t SeratoDatabase::rootCrateModificationDateInSecondsSince1970(void) const
{
    return this->p_crateOrderFile->modificationDateInSecondsSince1970();
}

const SeratoCrate* SeratoDatabase::rootCrate(void) const
{
    return this->p_crateOrderFile->rootCrate();
}

const SeratoTrackVector& SeratoDatabase::tracks(void) const
{
    return *this->p_tracks;
}

SeratoTrackVectorPtr SeratoDatabase::removeAndReturnTracks(void)
{
    SeratoTrackVectorPtr tracks = move(this->p_tracks);
    this->p_tracks = SeratoTrackVectorPtr(make_unique<SeratoTrackVector>());

    return tracks;
}

void SeratoDatabase::addCrateFileToDelete(const std::string& path)
{
    this->p_crateFilesToDelete.push_back(StringPtr(make_unique<string>(path)));
}

void SeratoDatabase::addTrack(SeratoTrackPtr track)
{
    this->p_tracks->push_back(move(track));
}

void SeratoDatabase::saveIfModified(void) const
{
    if (!this->p_databaseIsValid) {
        return;
    }

    for (auto& path : this->p_crateFilesToDelete) {
        ::deleteFileAt(path->c_str());
    }

    this->p_crateOrderFile->saveIfModified();

    bool someTracksWereModified = false;
    for (auto& track : *this->p_tracks) {
        if (!track->wasModified()) {
            continue;
        }

        printf("Saving modifications to Serato track '%s'.\n", track->title().c_str());
        track->saveToTrackFile();
        someTracksWereModified = true;
    }

    if (someTracksWereModified) {
        CharVectorPtr outputData = make_unique<CharVector>();

        SeratoTagPtr versionTag(make_unique<SeratoTextTag>(NxASeratoDatabaseVersionTag, NxASeratoDatabaseFileCurrentVersion));
        versionTag->addTo(*outputData);

        for (auto& track : *this->p_tracks) {
            track->addTo(*outputData);
        }

        for (auto& tag : this->p_otherTags) {
            tag->addTo(*outputData);
        }

        writeToFile(this->p_databaseFilePath->c_str(), *outputData);
    }
}
