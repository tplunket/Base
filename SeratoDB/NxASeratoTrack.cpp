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
#include "SeratoDB/NxASeratoObjectTag.h"
#include "SeratoDB/NxASeratoTextTag.h"
#include "SeratoDB/NxASeratoPathTag.h"
#include "SeratoDB/NxASeratoUInt32Tag.h"

using namespace NxA;
using namespace std;

#pragma mark Debug macros

#define PRINT_DEBUG_INFO            0

#pragma mark Constants

static const string emptyString("");
static const uint32_t zeroValue = 0;

#pragma mark Utility Functions

#if PRINT_DEBUG_INFO
static void p_debugPrint(const string& text, const char* name)
{
    printf("%s '%s'\n", name, text.c_str());
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

SeratoTrack::SeratoTrack(SeratoTagPtr trackTag, const char* rootDirectoryPath) :
                         p_wasModified(false),
                         p_trackTag(move(trackTag)),
                         p_rootFolder(make_unique<string>(rootDirectoryPath))
{
    this->p_loadTrackFile();

    this->p_readMarkers();

#if PRINT_DEBUG_INFO
    p_debugPrintComparaison(this, this->p_trackFile.get());
#endif

    this->p_unloadTrackFile();
}

#pragma mark Instance Methods

bool SeratoTrack::p_containsAValidTrackTag(void) const
{
    return this->p_trackTag.get() != NULL;
}

const string& SeratoTrack::p_stringForSubTagForIdentifier(const uint32_t& identifier) const
{
    if (this->p_containsAValidTrackTag()) {
        const SeratoObjectTag* trackObjectTag = dynamic_cast<const SeratoObjectTag*>(this->p_trackTag.get());
        if (trackObjectTag->hasSubTagForIdentifier(identifier)) {
            const SeratoTextTag& textTag = dynamic_cast<const SeratoTextTag&>(trackObjectTag->subTagForIdentifier(identifier));
            return textTag.value();
        }
    }

    return emptyString;
}

const string& SeratoTrack::p_pathForSubTagForIdentifier(const uint32_t& identifier) const
{
    if (this->p_containsAValidTrackTag()) {
        const SeratoObjectTag* trackObjectTag = dynamic_cast<const SeratoObjectTag*>(this->p_trackTag.get());
        if (trackObjectTag->hasSubTagForIdentifier(identifier)) {
            const SeratoPathTag& pathTag = dynamic_cast<const SeratoPathTag&>(trackObjectTag->subTagForIdentifier(identifier));
            return pathTag.value();
        }
    }

    return emptyString;
}

const uint32_t& SeratoTrack::p_uint32ForSubTagForIdentifier(const uint32_t& identifier) const
{
    if (this->p_containsAValidTrackTag()) {
        const SeratoObjectTag* trackObjectTag = dynamic_cast<const SeratoObjectTag*>(this->p_trackTag.get());
        if (trackObjectTag->hasSubTagForIdentifier(identifier)) {
            const SeratoUInt32Tag& uintTag = dynamic_cast<const SeratoUInt32Tag&>(trackObjectTag->subTagForIdentifier(identifier));
            return uintTag.value();
        }
    }

    return zeroValue;
}

void SeratoTrack::p_loadTrackFile(void)
{
    this->p_trackFile = SeratoTrackFileFactory::trackFileForPath(this->trackFilePath()->c_str());
}

void SeratoTrack::p_unloadTrackFile(void)
{
    this->p_trackFile = SeratoTrackFilePtr();
}

void SeratoTrack::p_readMarkers(void)
{
    const SeratoCueMarkerVector& cueMarkers = this->p_trackFile->cueMarkers();
    for (auto& marker : cueMarkers) {
        this->p_cueMarkers.push_back(make_unique<SeratoCueMarker>(*marker));
    }

    const SeratoLoopMarkerVector& loopMarkers = this->p_trackFile->loopMarkers();
    for (auto& marker : loopMarkers) {
        this->p_loopMarkers.push_back(make_unique<SeratoLoopMarker>(*marker));
    }
}

ConstStringPtr SeratoTrack::trackFilePath(void) const
{
    const string& pathFromRootFolder = this->p_pathForSubTagForIdentifier(NxASeratoTrackFilePathTag);
    ConstStringPtr trackFilePath = joinPaths(this->p_rootFolder->c_str(), pathFromRootFolder.c_str());
    return trackFilePath;
}

const string& SeratoTrack::title(void) const
{
    return this->p_stringForSubTagForIdentifier(NxASeratoTrackTitleTag);
}

const string& SeratoTrack::artist(void) const
{
    return this->p_stringForSubTagForIdentifier(NxASeratoTrackArtistTag);
}

const string& SeratoTrack::album(void) const
{
    return this->p_stringForSubTagForIdentifier(NxASeratoTrackAlbumTag);
}

const string& SeratoTrack::genre(void) const
{
    return this->p_stringForSubTagForIdentifier(NxASeratoTrackGenreTag);
}

const string& SeratoTrack::comments(void) const
{
    return this->p_stringForSubTagForIdentifier(NxASeratoTrackCommentsTag);
}

const string& SeratoTrack::grouping(void) const
{
    return this->p_stringForSubTagForIdentifier(NxASeratoTrackGroupingTag);
}

const string& SeratoTrack::remixer(void) const
{
    return this->p_stringForSubTagForIdentifier(NxASeratoTrackRemixerTag);
}

const string& SeratoTrack::recordLabel(void) const
{
    return this->p_stringForSubTagForIdentifier(NxASeratoTrackLabelTag);
}

const string& SeratoTrack::composer(void) const
{
    return this->p_stringForSubTagForIdentifier(NxASeratoTrackComposerTag);
}

const string& SeratoTrack::key(void) const
{
    return this->p_stringForSubTagForIdentifier(NxASeratoTrackKeyTag);
}

const string& SeratoTrack::length(void) const
{
    return this->p_stringForSubTagForIdentifier(NxASeratoTrackLengthTag);
}

const uint32_t& SeratoTrack::sizeInBytes(void) const
{
    return this->p_uint32ForSubTagForIdentifier(NxASeratoTrackSizeTag);
}

const string& SeratoTrack::bitRate(void) const
{
    return this->p_stringForSubTagForIdentifier(NxASeratoTrackBitrateTag);
}

const string& SeratoTrack::sampleRate(void) const
{
    return this->p_stringForSubTagForIdentifier(NxASeratoTrackSampleRateTag);
}

const string& SeratoTrack::bpm(void) const
{
    return this->p_stringForSubTagForIdentifier(NxASeratoTrackBpmTag);
}

const string& SeratoTrack::year(void) const
{
    return this->p_stringForSubTagForIdentifier(NxASeratoTrackYearTag);
}

const uint32_t& SeratoTrack::trackNumber(void) const
{
    return this->p_uint32ForSubTagForIdentifier(NxASeratoTrackNumberTag);
}

const uint32_t& SeratoTrack::discNumber(void) const
{
    return this->p_uint32ForSubTagForIdentifier(NxASeratoTrackDiscNumberTag);
}

const uint32_t& SeratoTrack::dateModifiedInSecondsSinceJanuary1st1970(void) const
{
    return this->p_uint32ForSubTagForIdentifier(NxASeratoTrackDateModifiedTag);
}

const uint32_t& SeratoTrack::dateAddedInSecondsSinceJanuary1st1970(void) const
{
    return this->p_uint32ForSubTagForIdentifier(NxASeratoTrackDateAddedTag);
}

const SeratoCueMarkerVector& SeratoTrack::cueMarkers(void) const
{
    return this->p_cueMarkers;
}

const SeratoLoopMarkerVector& SeratoTrack::loopMarkers(void) const
{
    return this->p_loopMarkers;
}
