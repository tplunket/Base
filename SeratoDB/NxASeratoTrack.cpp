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

#define PRINT_DEBUG_INFO            0

#pragma mark Utility Functions

#if PRINT_DEBUG_INFO
static void p_debugPrint(StringAutoPtr string, const char* name)
{
    printf("%s '%s'\n", name, string.get() ? string->c_str() : "");
}

static void p_debugPrintUint(uint32_t value, const char* name)
{
    printf("%s '%d'\n", name, value);
}

static void p_debugPrintTimeFromMilliseconds(uint32_t value, const char* name)
{
    uint32_t minutes = value / 60000;
    uint32_t milliseconds = value % 1000;
    uint32_t seconds = (value - milliseconds - (minutes * 60000)) / 1000;

    milliseconds *= 60;
    milliseconds /= 1000;

    printf("%s '%02d:%02d.%02d'\n", name, minutes, seconds, milliseconds);
}

static void p_debugPrintDate(time_t value, const char* name)
{
    char* stringVersion = ctime(&value);
    printf("%s %s", name, stringVersion);
}

static void p_debugPrintComparaison(const SeratoTrack* track, const SeratoTrackFile* trackFile)
{
    printf("----------------------------------------\n");
    p_debugPrintUint((uint32_t)track->sizeInBytes(), "size");
    p_debugPrintUint((uint32_t)trackFile->sizeInBytes(), "size");
    p_debugPrintDate(track->dateModifiedInSecondsSinceJanuary1st1970(), "datemodified");
    p_debugPrintDate(track->dateAddedInSecondsSinceJanuary1st1970(), "dateadded");

    p_debugPrint(track->title(), "title");
    p_debugPrint(trackFile->title(), "title");
    p_debugPrint(track->artist(), "artist");
    p_debugPrint(trackFile->artist(), "artist");
    p_debugPrint(track->album(), "album");
    p_debugPrint(trackFile->album(), "album");
    p_debugPrint(track->comments(), "comments");
    p_debugPrint(trackFile->comments(), "comments");
    p_debugPrint(track->genre(), "genre");
    p_debugPrint(trackFile->genre(), "genre");
    p_debugPrint(track->grouping(), "grouping");
    p_debugPrint(trackFile->grouping(), "grouping");
    p_debugPrint(track->recordLabel(), "recordlabel");
    if (trackFile->hasRecordLabel()) {
        p_debugPrint(trackFile->recordLabel(), "recordLabel");
    }
    else {
        printf("No record label.\n");
    }
    p_debugPrint(track->remixer(), "remixer");
    if (trackFile->hasRecordLabel()) {
        p_debugPrint(trackFile->remixer(), "remixer");
    }
    else {
        printf("No remixer.\n");
    }
    p_debugPrint(track->composer(), "composer");
    p_debugPrint(trackFile->composer(), "composer");
    p_debugPrint(track->key(), "key");
    if (trackFile->hasKey()) {
        p_debugPrint(trackFile->key(), "key");
    }
    else {
        printf("No key.\n");
    }
    p_debugPrint(track->year(), "year");
    p_debugPrint(trackFile->yearReleased(), "year");
    p_debugPrint(track->length(), "length");
    p_debugPrintTimeFromMilliseconds(trackFile->lengthInMilliseconds(), "length");
    p_debugPrint(track->bitRate(), "bitrate");
    p_debugPrintUint(trackFile->bitRateInKiloBitsPerSecond(), "bitrate");
    p_debugPrint(track->sampleRate(), "samplerate");
    p_debugPrintUint(trackFile->sampleRateInSamplesPerSecond(), "samplerate");
    p_debugPrint(track->bpm(), "bpm");
    p_debugPrint(trackFile->bpm(), "bpm");
    p_debugPrintUint(track->trackNumber(), "tracknumber");
    p_debugPrintUint(trackFile->trackNumber(), "tracknumber");

    p_debugPrintUint(track->discNumber(), "discnumber");

    printf("Found %ld cue markers and %ld loop markers.\n", trackFile->cueMarkers().size(), trackFile->loopMarkers().size());
}
#endif

#pragma mark Constructors

SeratoTrack::SeratoTrack(const SeratoTag* trackTag, const char* rootDirectoryPath) :
                         p_trackTag(SeratoTagAutoPtr(trackTag)),
                         p_rootFolder(StringAutoPtr(new string(rootDirectoryPath)))
{
#if PRINT_DEBUG_INFO
    this->p_loadTrackFile();
    p_debugPrintComparaison(this, this->p_trackFile.get());
    this->p_unloadTrackFile();
#endif
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

void SeratoTrack::p_loadTrackFile(void)
{
    this->p_trackFile = SeratoTrackFileFactory::trackFileForPath(this->trackFilePath()->c_str());
}

void SeratoTrack::p_unloadTrackFile(void)
{
    this->p_trackFile = SeratoTrackFileAutoPtr(NULL);
}

void SeratoTrack::p_readMarkersIfNotAlreadyRead(void)
{
    if (!this->p_cueMarkers.get() || !this->p_loopMarkers.get()) {
        this->p_loadTrackFile();
        this->p_cueMarkers = SeratoCueMarkerVectorAutoPtr(new SeratoCueMarkerVector(this->p_trackFile->cueMarkers()));
        this->p_loopMarkers = SeratoLoopMarkerVectorAutoPtr(new SeratoLoopMarkerVector(this->p_trackFile->loopMarkers()));
        this->p_unloadTrackFile();
    }
}

StringAutoPtr SeratoTrack::trackFilePath(void) const
{
    StringAutoPtr pathFromRootFolder = this->p_pathForSubTagIdentifierOrEmptyIfNotFound(NxASeratoTrackFilePathTag);
    StringAutoPtr trackFilePath = joinPaths(this->p_rootFolder->c_str(), pathFromRootFolder->c_str());
    return trackFilePath;
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

const SeratoCueMarkerVector& SeratoTrack::cueMarkers(void)
{
    this->p_readMarkersIfNotAlreadyRead();

    return *(this->p_cueMarkers);
}

const SeratoLoopMarkerVector& SeratoTrack::loopMarkers(void)
{
    this->p_readMarkersIfNotAlreadyRead();

    return *(this->p_loopMarkers);
}
