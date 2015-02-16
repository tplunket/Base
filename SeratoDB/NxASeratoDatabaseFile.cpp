//
//  NxASeratoDatabaseFile.cpp
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

#include "SeratoDB/NxASeratoDatabaseFile.h"
#include "SeratoDB/NxASeratoDatabaseV2Tags.h"

using namespace NxA;
using namespace std;

#pragma mark Constructors

SeratoDatabaseFile::SeratoDatabaseFile(const char* seratoFolderPath)
{
    StringAutoPtr databaseFilePath = databaseFilePathForSeratoFolder(seratoFolderPath);
    CharVectorAutoPtr databaseFile = readFileAt(databaseFilePath->c_str());

    SeratoTagVectorAutoPtr tags = SeratoTag::parseTagsIn(databaseFile);
    for(SeratoTagVector::iterator it = tags->begin(); it != tags->end(); ++it) {
        const SeratoTag* tag = it->release();

        switch (tag->identifier()) {
            case NxASeratoDatabaseVersionTag: {
                this->p_storeVersionTag(tag);
                break;
            }
            case NxASeratoDatabaseTrackObjectTag: {
                this->p_storeTrackTag(tag);
                break;
            }
            default: {
                this->p_storeOtherTag(tag);
                break;
            }
        }
    }
}

#pragma mark Instance Methods

void SeratoDatabaseFile::p_storeVersionTag(const SeratoTag* tag)
{
    if (this->p_versionTag.get()) {
        return;
    }

    this->p_versionTag = SeratoTagAutoPtr(tag);
}

void SeratoDatabaseFile::p_storeTrackTag(const SeratoTag* tag)
{
    SeratoDatabaseTrack* newTrack = new SeratoDatabaseTrack(tag);
    this->p_tracks.push_back(SeratoDatabaseTrackAutoPtr(newTrack));
}

void SeratoDatabaseFile::p_storeOtherTag(const SeratoTag* tag)
{
    this->p_otherTags.push_back(SeratoTagAutoPtr(tag));
}

StringAutoPtr SeratoDatabaseFile::versionAsString(void) const
{
    if (this->p_versionTag.get()) {
        return this->p_versionTag->dataAsString();
    }

    return StringAutoPtr();
}

const SeratoDatabaseTrackVector& SeratoDatabaseFile::tracks(void)
{
    return this->p_tracks;
}
