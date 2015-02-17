//
//  NxASeratoTrack.cpp
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

#include "SeratoDB/NxASeratoTrack.h"
#include "SeratoDB/NxASeratoDatabaseV2Tags.h"
#include "SeratoDB/NxASeratoTrackFileFactory.h"

using namespace NxA;
using namespace std;

#pragma mark Constructors

SeratoTrack::SeratoTrack(const SeratoTag* trackTag) : p_trackTag(SeratoTagAutoPtr(trackTag))
{
    string trackFilePath("/");
    trackFilePath += *(this->p_trackFilePath());
    
    this->p_trackFile = SeratoTrackFileFactory::trackFileForPath(trackFilePath.c_str());
}

#pragma mark Instance Methods

bool SeratoTrack::p_containsAValidTag(void) const
{
    return this->p_trackTag.get() != NULL;
}

StringAutoPtr SeratoTrack::p_stringForSubTagIdentifierOrEmptyIfNotFound(uint32_t identifier) const
{
    if (this->p_containsAValidTag()) {
        const SeratoTag* tag = this->p_trackTag->subTagWithIdentifierOrNilIfDoesNotExist(identifier);
        if (tag != NULL) {
            return tag->dataAsString();
        }
    }

    return StringAutoPtr(new string(""));
}

StringAutoPtr SeratoTrack::p_pathForSubTagIdentifierOrEmptyIfNotFound(uint32_t identifier) const
{
    if (this->p_containsAValidTag()) {
        const SeratoTag* tag = this->p_trackTag->subTagWithIdentifierOrNilIfDoesNotExist(identifier);
        if (tag != NULL) {
            return tag->dataAsPath();
        }
    }

    return StringAutoPtr(new string(""));
}

uint32_t SeratoTrack::p_uint32ForSubTagIdentifierOrZeroIfNotFound(uint32_t identifier) const
{
    if (this->p_containsAValidTag()) {
        const SeratoTag* tag = this->p_trackTag->subTagWithIdentifierOrNilIfDoesNotExist(identifier);
        if (tag != NULL) {
            return tag->dataAsUInt32();
        }
    }

    return 0;
}

StringAutoPtr SeratoTrack::p_trackFilePath(void) const
{
    return this->p_pathForSubTagIdentifierOrEmptyIfNotFound(NxASeratoTrackFilePathTag);
}

StringAutoPtr SeratoTrack::title(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoTrackTitleTag);
}

StringAutoPtr SeratoTrack::artist(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoTrackArtistTag);
}

StringAutoPtr SeratoTrack::album(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoTrackAlbumTag);
}

StringAutoPtr SeratoTrack::genre(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoTrackGenreTag);
}

StringAutoPtr SeratoTrack::comments(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoTrackCommentsTag);
}

StringAutoPtr SeratoTrack::grouping(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoTrackGroupingTag);
}

StringAutoPtr SeratoTrack::remixer(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoTrackRemixerTag);
}

StringAutoPtr SeratoTrack::recordLabel(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoTrackLabelTag);
}

StringAutoPtr SeratoTrack::composer(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoTrackComposerTag);
}

StringAutoPtr SeratoTrack::key(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoTrackKeyTag);
}

StringAutoPtr SeratoTrack::length(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoTrackLengthTag);
}

size_t SeratoTrack::sizeInBytes(void) const
{
    return this->p_uint32ForSubTagIdentifierOrZeroIfNotFound(NxASeratoTrackSizeTag);
}

StringAutoPtr SeratoTrack::bitRate(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoTrackBitrateTag);
}

StringAutoPtr SeratoTrack::sampleRate(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoTrackSampleRateTag);
}

StringAutoPtr SeratoTrack::bpm(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoTrackBpmTag);
}

StringAutoPtr SeratoTrack::year(void) const
{
    return this->p_stringForSubTagIdentifierOrEmptyIfNotFound(NxASeratoTrackYearTag);
}

uint32_t SeratoTrack::trackNumber(void) const
{
    return this->p_uint32ForSubTagIdentifierOrZeroIfNotFound(NxASeratoTrackNumberTag);
}

uint32_t SeratoTrack::discNumber(void) const
{
    return this->p_uint32ForSubTagIdentifierOrZeroIfNotFound(NxASeratoTrackDiscNumberTag);
}

uint32_t SeratoTrack::dateModifiedInSecondsSinceJanuary1st1970(void) const
{
    return this->p_uint32ForSubTagIdentifierOrZeroIfNotFound(NxASeratoTrackDateModifiedTag);
}

uint32_t SeratoTrack::dateAddedInSecondsSinceJanuary1st1970(void) const
{
    return this->p_uint32ForSubTagIdentifierOrZeroIfNotFound(NxASeratoTrackDateAddedTag);
}
