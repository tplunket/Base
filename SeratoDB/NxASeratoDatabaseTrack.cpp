//
//  NxASeratoDatabaseTrack.cpp
//  SeratoDB
//
//  Created by Didier Malenfant on 2/3/15.
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

#include "SeratoDB/NxASeratoDatabaseTrack.h"
#include "SeratoDB/NxASeratoDatabaseV2Tags.h"

#include <stdlib.h>

using namespace NxA;

#pragma mark Destructor

SeratoDatabaseTrack::~SeratoDatabaseTrack()
{
    delete this->p_trackTag;
    this->p_trackTag = NULL;
}

#pragma mark Instance Methods

const std::string* SeratoDatabaseTrack::p_stringForSubTagIdentifierOrEmptyIfNotFound(uint32_t identifier) const
{
    if (this->p_trackTag) {
        SeratoDatabaseTag* tag = this->p_trackTag->subTagWithIdentifier(identifier);
        if (tag) {
            return tag->dataAsString();
        }
    }

    return new std::string("");
}

const std::string* SeratoDatabaseTrack::p_pathForSubTagIdentifierOrEmptyIfNotFound(uint32_t identifier) const
{
    if (this->p_trackTag) {
        SeratoDatabaseTag* tag = this->p_trackTag->subTagWithIdentifier(identifier);
        if (tag) {
            return tag->dataAsPath();
        }
    }

    return new std::string("");
}

uint32_t SeratoDatabaseTrack::p_uint32ForSubTagIdentifierOrZeroIfNotFound(uint32_t identifier) const
{
    if (this->p_trackTag) {
        SeratoDatabaseTag* tag = this->p_trackTag->subTagWithIdentifier(identifier);
        if (tag) {
            return tag->dataAsUInt32();
        }
    }

    return 0;
}

const std::string* SeratoDatabaseTrack::title(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoDatabaseTrackTitleTag);
}

const std::string* SeratoDatabaseTrack::artist(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoDatabaseTrackArtistTag);
}

const std::string* SeratoDatabaseTrack::filePath(void) const
{
    return this->p_pathForSubTagIdentifierOrEmptyIfNotFound(NxASeratoDatabaseAudioFilePathTag);
}

const std::string* SeratoDatabaseTrack::album(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoDatabaseTrackAlbumTag);
}

const std::string* SeratoDatabaseTrack::genre(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoDatabaseTrackGenreTag);
}

const std::string* SeratoDatabaseTrack::comments(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoDatabaseTrackCommentsTag);
}

const std::string* SeratoDatabaseTrack::grouping(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoDatabaseTrackGroupingTag);
}

const std::string* SeratoDatabaseTrack::remix(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoDatabaseTrackRemixTag);
}

const std::string* SeratoDatabaseTrack::recordLabel(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoDatabaseTrackLabelTag);
}

const std::string* SeratoDatabaseTrack::composer(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoDatabaseTrackComposerTag);
}

const std::string* SeratoDatabaseTrack::key(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoDatabaseTrackKeyTag);
}

const std::string* SeratoDatabaseTrack::trackLength(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoDatabaseTrackLengthTag);
}

size_t SeratoDatabaseTrack::trackSizeInBytes(void) const
{
    return this->p_uint32ForSubTagIdentifierOrZeroIfNotFound(NxASeratoDatabaseTrackSizeTag);
}

const std::string* SeratoDatabaseTrack::trackBitRate(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoDatabaseTrackBitrateTag);
}

const std::string* SeratoDatabaseTrack::trackSampleRate(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoDatabaseTrackSampleRateTag);
}

const std::string* SeratoDatabaseTrack::trackBpm(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoDatabaseTrackBpmTag);
}

const std::string* SeratoDatabaseTrack::trackYear(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoDatabaseTrackYearTag);
}

uint32_t SeratoDatabaseTrack::trackNumber(void) const
{
    return this->p_uint32ForSubTagIdentifierOrZeroIfNotFound(NxASeratoDatabaseTrackNumberTag);
}

uint32_t SeratoDatabaseTrack::trackDiscNumber(void) const
{
    return this->p_uint32ForSubTagIdentifierOrZeroIfNotFound(NxASeratoDatabaseTrackDiscNumberTag);
}

uint32_t SeratoDatabaseTrack::trackDateModifiedInSecondsSinceJanuary1st1970(void) const
{
    return this->p_uint32ForSubTagIdentifierOrZeroIfNotFound(NxASeratoDatabaseTrackDateModifiedTag);
}

uint32_t SeratoDatabaseTrack::trackDateAddedInSecondsSinceJanuary1st1970(void) const
{
    return this->p_uint32ForSubTagIdentifierOrZeroIfNotFound(NxASeratoDatabaseTrackDateAddedTag);
}
