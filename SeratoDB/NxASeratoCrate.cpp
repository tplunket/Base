//
//  NxASeratoCrate.cpp
//  SeratoDB
//
//  Created by Didier Malenfant on 2/4/15.
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

#include "SeratoDB/NxASeratoCrate.h"
#include "SeratoDB/NxASeratoCrateV1Tags.h"

using namespace NxA;
using namespace std;

#pragma mark Constructors

SeratoCrate::SeratoCrate(const char* fullyQualifiedCrateName, const char* seratoFolderPath)
{
    this->p_fullyQualifiedCrateName = StringAutoPtr(new string(fullyQualifiedCrateName));

    string crateName(fullyQualifiedCrateName);
    size_t lastSeperator = crateName.rfind("%%");
    if (lastSeperator != string::npos) {
        this->p_crateName = StringAutoPtr(new string(crateName.substr(lastSeperator + 2)));
    }
    else {
        this->p_crateName = StringAutoPtr(new string(fullyQualifiedCrateName));
    }

    StringAutoPtr crateFilePath = crateFilePathForCrateNameInSeratoFolder(fullyQualifiedCrateName, seratoFolderPath);
    CharVectorAutoPtr crateFileData = readFileAt(crateFilePath->c_str());

    SeratoTagVectorAutoPtr tags(SeratoTag::parseTagsIn(crateFileData));
    for(SeratoTagVector::iterator it = tags->begin(); it != tags->end(); ++it) {
        const SeratoTag* tag = it->release();

        switch (tag->identifier()) {
            case NxASeratoCrateVersionTag: {
                this->p_storeVersionTag(tag);
                break;
            }
            case NxASeratoTrackEntryTag: {
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

#pragma mark Class Methods

bool SeratoCrate::isAValidCrateName(const char* fullyQualifiedCrateName, const char* seratoFolderPath)
{
    StringAutoPtr crateFilePath = crateFilePathForCrateNameInSeratoFolder(fullyQualifiedCrateName, seratoFolderPath);
    return fileExistsAt(crateFilePath->c_str());
}

#pragma mark Instance Methods

void SeratoCrate::p_storeVersionTag(const SeratoTag* tag)
{
    if (this->p_versionTag.get()) {
        return;
    }

    this->p_versionTag = SeratoTagAutoPtr(tag);
}

void SeratoCrate::p_storeTrackTag(const SeratoTag* tag)
{
    SeratoTrackEntry* newTrack = new SeratoTrackEntry(tag);
    this->p_tracks.push_back(SeratoTrackEntryAutoPtr(newTrack));
}

void SeratoCrate::p_storeOtherTag(const SeratoTag* tag)
{
    this->p_otherTags.push_back(SeratoTagAutoPtr(tag));
}

StringAutoPtr SeratoCrate::versionAsString(void) const
{
    if (this->p_versionTag.get()) {
        return this->p_versionTag->dataAsString();
    }

    return StringAutoPtr();
}

const std::string& SeratoCrate::crateName(void) const
{
    return *(this->p_crateName);
}

const SeratoTrackEntryVector& SeratoCrate::tracks(void) const
{
    return this->p_tracks;
}

const SeratoCrateVector& SeratoCrate::crates(void) const
{
    return this->p_childrenCrates;
}

void SeratoCrate::addChildCrate(SeratoCrateAutoPtr crate)
{
    this->p_childrenCrates.push_back(SeratoCrateAutoPtr(crate));
}
