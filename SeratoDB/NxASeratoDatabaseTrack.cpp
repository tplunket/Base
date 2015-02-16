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

using namespace NxA;
using namespace std;

#pragma mark Instance Methods

bool SeratoDatabaseTrack::p_containsAValidTag(void) const
{
    return this->p_trackTag.get() != NULL;
}

StringAutoPtr SeratoDatabaseTrack::p_stringForSubTagIdentifierOrEmptyIfNotFound(uint32_t identifier) const
{
    if (this->p_containsAValidTag()) {
        const SeratoTag* tag = this->p_trackTag->subTagWithIdentifierOrNilIfDoesNotExist(identifier);
        if (tag != NULL) {
            return tag->dataAsString();
        }
    }

    return StringAutoPtr(new string(""));
}

StringAutoPtr SeratoDatabaseTrack::p_pathForSubTagIdentifierOrEmptyIfNotFound(uint32_t identifier) const
{
    if (this->p_containsAValidTag()) {
        const SeratoTag* tag = this->p_trackTag->subTagWithIdentifierOrNilIfDoesNotExist(identifier);
        if (tag != NULL) {
            return tag->dataAsPath();
        }
    }

    return StringAutoPtr(new string(""));
}

uint32_t SeratoDatabaseTrack::p_uint32ForSubTagIdentifierOrZeroIfNotFound(uint32_t identifier) const
{
    if (this->p_containsAValidTag()) {
        const SeratoTag* tag = this->p_trackTag->subTagWithIdentifierOrNilIfDoesNotExist(identifier);
        if (tag != NULL) {
            return tag->dataAsUInt32();
        }
    }

    return 0;
}

StringAutoPtr SeratoDatabaseTrack::title(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoDatabaseTrackTitleTag);
}

StringAutoPtr SeratoDatabaseTrack::artist(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoDatabaseTrackArtistTag);
}

StringAutoPtr SeratoDatabaseTrack::trackFilePath(void) const
{
    return this->p_pathForSubTagIdentifierOrEmptyIfNotFound(NxASeratoDatabaseTrackFilePathTag);
}

StringAutoPtr SeratoDatabaseTrack::album(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoDatabaseTrackAlbumTag);
}

StringAutoPtr SeratoDatabaseTrack::genre(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoDatabaseTrackGenreTag);
}

StringAutoPtr SeratoDatabaseTrack::comments(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoDatabaseTrackCommentsTag);
}

StringAutoPtr SeratoDatabaseTrack::grouping(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoDatabaseTrackGroupingTag);
}

StringAutoPtr SeratoDatabaseTrack::remixer(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoDatabaseTrackRemixerTag);
}

StringAutoPtr SeratoDatabaseTrack::recordLabel(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoDatabaseTrackLabelTag);
}

StringAutoPtr SeratoDatabaseTrack::composer(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoDatabaseTrackComposerTag);
}

StringAutoPtr SeratoDatabaseTrack::key(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoDatabaseTrackKeyTag);
}

StringAutoPtr SeratoDatabaseTrack::length(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoDatabaseTrackLengthTag);
}

size_t SeratoDatabaseTrack::sizeInBytes(void) const
{
    return this->p_uint32ForSubTagIdentifierOrZeroIfNotFound(NxASeratoDatabaseTrackSizeTag);
}

StringAutoPtr SeratoDatabaseTrack::bitRate(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoDatabaseTrackBitrateTag);
}

StringAutoPtr SeratoDatabaseTrack::sampleRate(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoDatabaseTrackSampleRateTag);
}

StringAutoPtr SeratoDatabaseTrack::bpm(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoDatabaseTrackBpmTag);
}

StringAutoPtr SeratoDatabaseTrack::year(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoDatabaseTrackYearTag);
}

uint32_t SeratoDatabaseTrack::trackNumber(void) const
{
    return this->p_uint32ForSubTagIdentifierOrZeroIfNotFound(NxASeratoDatabaseTrackNumberTag);
}

uint32_t SeratoDatabaseTrack::discNumber(void) const
{
    return this->p_uint32ForSubTagIdentifierOrZeroIfNotFound(NxASeratoDatabaseTrackDiscNumberTag);
}

uint32_t SeratoDatabaseTrack::dateModifiedInSecondsSinceJanuary1st1970(void) const
{
    return this->p_uint32ForSubTagIdentifierOrZeroIfNotFound(NxASeratoDatabaseTrackDateModifiedTag);
}

uint32_t SeratoDatabaseTrack::dateAddedInSecondsSinceJanuary1st1970(void) const
{
    return this->p_uint32ForSubTagIdentifierOrZeroIfNotFound(NxASeratoDatabaseTrackDateAddedTag);
}
