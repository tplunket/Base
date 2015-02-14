//
//  NxASeratoCrateFile.cpp
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

#include "SeratoDB/NxASeratoCrateFile.h"
#include "SeratoDB/NxASeratoCrateV1Tags.h"

using namespace NxA;

#pragma mark Constructors

SeratoCrateFile::SeratoCrateFile(const void* startOfFile, unsigned long lengthInBytes)
{
    SeratoTagVectorAutoPtr tags(SeratoTag::parseTagsIn(startOfFile, lengthInBytes));
    for(SeratoTagVector::iterator it = tags->begin(); it != tags->end(); ++it) {
        const SeratoTag* tag = it->release();

        switch (tag->identifier()) {
            case NxASeratoCrateVersionTag: {
                this->p_storeVersionTag(tag);
                break;
            }
            case NxASeratoCrateTrackTag: {
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

void SeratoCrateFile::p_storeVersionTag(const SeratoTag* tag)
{
    if (this->p_versionTag.get()) {
        return;
    }

    this->p_versionTag = SeratoTagAutoPtr(tag);
}

void SeratoCrateFile::p_storeTrackTag(const SeratoTag* tag)
{
    SeratoCrateTrack* newTrack = new SeratoCrateTrack(tag);
    this->p_tracks.push_back(SeratoCrateTrackAutoPtr(newTrack));
}

void SeratoCrateFile::p_storeOtherTag(const SeratoTag* tag)
{
    this->p_otherTags.push_back(SeratoTagAutoPtr(tag));
}

StringAutoPtr SeratoCrateFile::versionAsString(void) const
{
    if (this->p_versionTag.get()) {
        return this->p_versionTag->dataAsString();
    }

    return StringAutoPtr();
}

const SeratoCrateTrackVector& SeratoCrateFile::tracks(void)
{
    return this->p_tracks;
}
