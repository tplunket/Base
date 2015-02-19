//
//  NxASeratoDatabase.cpp
//  SeratoDB
//
//  Created by Didier Malenfant on 1/30/15.
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

#include "SeratoDB/NxASeratoDatabase.h"
#include "SeratoDB/NxASeratoDatabaseV2Tags.h"

using namespace NxA;
using namespace std;

#define PRINT_DEBUG_INFO        0

#pragma Utility Functions

#if PRINT_DEBUG_INFO
static void p_debugListCrate(const SeratoCrate* crate, std::string spacing)
{
    const SeratoCrateVector& crates = crate->crates();

    for(SeratoCrateVector::const_iterator it = crates.begin(); it != crates.end(); ++it) {
        const SeratoCrate* crate = it->get();
        const string& crateName = crate->crateName();
        printf("%sCrate '%s'\n", spacing.c_str(), crateName.c_str());

        const SeratoTrackEntryVector& crateTracks = crate->tracks();
        for(SeratoTrackEntryVector::const_iterator cit = crateTracks.begin(); cit != crateTracks.end(); ++cit) {
            printf("%s   Track '%s'\n", spacing.c_str(), cit->get()->trackFilePath()->c_str());
        }

        p_debugListCrate(crate, (spacing + "   "));
    }
}
#endif

#pragma mark Constructors

SeratoDatabase::SeratoDatabase(const char* seratoFolderPath)
{
    StringAutoPtr databaseFilePath = databaseFilePathForSeratoFolder(seratoFolderPath);
    CharVectorAutoPtr databaseFile = readFileAt(databaseFilePath->c_str());

    SeratoTagVectorAutoPtr tags = SeratoTag::parseTagsIn(databaseFile);
    for(SeratoTagVector::iterator it = tags->begin(); it != tags->end(); ++it) {
        const SeratoTag* tag = it->release();

        switch (tag->identifier()) {
            case NxASeratoTrackObjectTag: {
                this->p_storeTrackTag(tag);
                break;
            }
            default: {
                this->p_storeOtherTag(tag);
                break;
            }
        }
    }

    this->p_crateOrderFile = SeratoCrateOrderFileAutoPtr(new SeratoCrateOrderFile(seratoFolderPath));

#if PRINT_DEBUG_INFO
    p_debugListCrate(this->rootCrate(), "");
#endif
}

#pragma mark Class Methods

StringAutoPtr SeratoDatabase::versionAsStringForDatabaseIn(const char* seratoFolderPath)
{
    StringAutoPtr databaseFilePath = databaseFilePathForSeratoFolder(seratoFolderPath);
    CharVectorAutoPtr databaseFile = readFileAt(databaseFilePath->c_str());

    SeratoTagVectorAutoPtr tags = SeratoTag::parseTagsIn(databaseFile);
    for(SeratoTagVector::iterator it = tags->begin(); it != tags->end(); ++it) {
        const SeratoTag* tag = it->release();

        if (tag->identifier() == NxASeratoDatabaseVersionTag) {
            return tag->dataAsString();
        }
    }

    return StringAutoPtr(new string(""));
}

#pragma mark Instance Methods

void SeratoDatabase::p_storeTrackTag(const SeratoTag* tag)
{
    SeratoTrack* newTrack = new SeratoTrack(tag);
    this->p_tracks.push_back(SeratoTrackAutoPtr(newTrack));
}

void SeratoDatabase::p_storeOtherTag(const SeratoTag* tag)
{
    this->p_otherTags.push_back(SeratoTagAutoPtr(tag));
}

const SeratoCrate* SeratoDatabase::rootCrate(void) const
{
    return this->p_crateOrderFile->rootCrate();
}

const SeratoTrackVector& SeratoDatabase::tracks(void) const
{
    return this->p_tracks;
}
