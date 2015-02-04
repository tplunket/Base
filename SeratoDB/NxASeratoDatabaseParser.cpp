//
//  NxASeratoDatabaseParser.cpp
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

#include "SeratoDB/NxASeratoDatabaseParser.h"
#include "SeratoDB/NxASeratoDatabaseV2Tags.h"

using namespace NxA;

#pragma mark Utility Functions

void p_deleteTagVectorAndItsContent(SeratoDatabaseTagVector* tags)
{
    for(SeratoDatabaseTagVector::iterator it = tags->begin(); it != tags->end(); ++it) {
        delete *it;
    }

    delete tags;
}

void p_deleteTrackVectorAndItsContent(SeratoDatabaseTrackVector* tags)
{
    for(SeratoDatabaseTrackVector::iterator it = tags->begin(); it != tags->end(); ++it) {
        delete *it;
    }

    delete tags;
}

#pragma mark Constructors

SeratoDatabaseParser::SeratoDatabaseParser(const void* startOfFile, unsigned long lengthInBytes)
{
    this->p_initTagStorage();

    SeratoDatabaseTagVector* tags = SeratoDatabaseTag::parseTagsIn(startOfFile, lengthInBytes);
    for(SeratoDatabaseTagVector::iterator it = tags->begin(); it != tags->end(); ++it) {
        SeratoDatabaseTag* tag = *it;

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

    delete tags;
}

#pragma mark Destructor

SeratoDatabaseParser::~SeratoDatabaseParser()
{
    this->p_deleteTagStorage();
}

#pragma mark Instance Methods

void SeratoDatabaseParser::p_initTagStorage(void)
{
    this->p_versionTag = NULL;
    this->p_tracks = new SeratoDatabaseTrackVector;
    this->p_otherTags = new SeratoDatabaseTagVector;
}

void SeratoDatabaseParser::p_deleteTagStorage(void)
{
    delete this->p_versionTag;
    this->p_versionTag = NULL;

    p_deleteTrackVectorAndItsContent(this->p_tracks);
    this->p_tracks = NULL;

    p_deleteTagVectorAndItsContent(this->p_otherTags);
    this->p_otherTags = NULL;
}

void SeratoDatabaseParser::p_storeVersionTag(SeratoDatabaseTag* tag)
{
    if (this->p_versionTag) {
        return;
    }

    this->p_versionTag = tag;
}

void SeratoDatabaseParser::p_storeTrackTag(SeratoDatabaseTag* tag)
{
    SeratoDatabaseTrack* newTrack = new SeratoDatabaseTrack(tag);
    this->p_tracks->push_back(newTrack);
}

void SeratoDatabaseParser::p_storeOtherTag(SeratoDatabaseTag* tag)
{
    this->p_otherTags->push_back(tag);
}

const std::string* SeratoDatabaseParser::versionAsString(void) const
{
    if (this->p_versionTag) {
        return this->p_versionTag->dataAsString();
    }

    return NULL;
}

const SeratoDatabaseTrackVector* SeratoDatabaseParser::tracks(void)
{
    return this->p_tracks;
}
