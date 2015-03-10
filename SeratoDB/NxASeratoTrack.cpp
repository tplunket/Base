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

SeratoTrack::SeratoTrack(SeratoTagPtr trackTag, const char* locatedOnVolumePath) :
                         p_wasModified(false),
                         p_trackTag(move(trackTag)),
                         p_rootFolder(make_unique<string>(locatedOnVolumePath)),
                         p_cueMarkers(make_unique<SeratoCueMarkerVector>()),
                         p_loopMarkers(make_unique<SeratoLoopMarkerVector>())
{
    this->p_loadTrackFile();

    this->p_readMarkers();

#if PRINT_DEBUG_INFO
    p_debugPrintComparaison(this, this->p_trackFile.get());
#endif

    this->p_unloadTrackFile();
}

SeratoTrack::SeratoTrack(const char* trackFilePath, const char* locatedOnVolumePath) :
    p_wasModified(true),
    p_rootFolder(make_unique<string>(locatedOnVolumePath)),
    p_cueMarkers(make_unique<SeratoCueMarkerVector>()),
    p_loopMarkers(make_unique<SeratoLoopMarkerVector>())
{
    ConstStringPtr relativePath = removePrefixFromPath(locatedOnVolumePath, trackFilePath);
    SeratoTagVectorPtr tags(make_unique<SeratoTagVector>());
    tags->push_back(make_unique<SeratoPathTag>(NxASeratoTrackFilePathTag, relativePath->c_str()));

    this->p_trackTag = make_unique<SeratoObjectTag>(NxASeratoTrackObjectTag, move(tags));

    this->p_loadTrackFile();

    // TODO: Output correct values:
    // '06:20.16'
    // bitrate '128.0kbps' '256kbps' '901kbps' '854kbps' '1411.2kbps'
    // samplerate '44.1k'
    // 68.1MB 17.3MB 9.6MB

    this->p_setStringForSubTagForIdentifier("test", NxASeratoTrackLengthTag);
    this->p_setStringForSubTagForIdentifier("test", NxASeratoTrackBitrateTag);
    this->p_setStringForSubTagForIdentifier("test", NxASeratoTrackSampleRateTag);
    this->p_setStringForSubTagForIdentifier("test", NxASeratoTrackSizeStringTag);
    this->p_setUInt32ForSubTagForIdentifier(23, NxASeratoTrackSizeTag);

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

void SeratoTrack::p_setStringForSubTagForIdentifier(const char* value, const uint32_t& identifier)
{
    if (!this->p_containsAValidTrackTag()) {
        return;
    }

    SeratoObjectTag* trackObjectTag = dynamic_cast<SeratoObjectTag*>(this->p_trackTag.get());
    if (!trackObjectTag->hasSubTagForIdentifier(identifier)) {
        trackObjectTag->addSubTag(std::make_unique<SeratoTextTag>(identifier, value));
    }
    else {
        SeratoTextTag& textTag = dynamic_cast<SeratoTextTag&>(trackObjectTag->subTagForIdentifier(identifier));
        textTag.value() = string(value);
    }

    this->p_wasModified = true;
}

void SeratoTrack::p_setPathForSubTagForIdentifier(const char* value, const uint32_t& identifier)
{
    if (!this->p_containsAValidTrackTag()) {
        return;
    }

    SeratoObjectTag* trackObjectTag = dynamic_cast<SeratoObjectTag*>(this->p_trackTag.get());
    if (!trackObjectTag->hasSubTagForIdentifier(identifier)) {
        trackObjectTag->addSubTag(std::make_unique<SeratoPathTag>(identifier, value));
    }
    else {
        SeratoPathTag& pathTag = dynamic_cast<SeratoPathTag&>(trackObjectTag->subTagForIdentifier(identifier));
        pathTag.value() = string(value);
    }

    this->p_wasModified = true;
}

void SeratoTrack::p_setUInt32ForSubTagForIdentifier(const uint32_t& value, const uint32_t& identifier)
{
    if (!this->p_containsAValidTrackTag()) {
        return;
    }

    SeratoObjectTag* trackObjectTag = dynamic_cast<SeratoObjectTag*>(this->p_trackTag.get());
    if (!trackObjectTag->hasSubTagForIdentifier(identifier)) {
        trackObjectTag->addSubTag(std::make_unique<SeratoUInt32Tag>(identifier, value));
    }
    else {
        SeratoUInt32Tag& uint32Tag = dynamic_cast<SeratoUInt32Tag&>(trackObjectTag->subTagForIdentifier(identifier));
        uint32Tag.value() = value;
    }

    this->p_wasModified = true;
}

void SeratoTrack::p_loadTrackFile(void)
{
    this->p_trackFile = SeratoTrackFileFactory::trackFileForPath(this->trackFilePath()->c_str());
}

void SeratoTrack::p_saveTrackFile(void)
{
    this->p_trackFile->saveChanges();
}

void SeratoTrack::p_unloadTrackFile(void)
{
    this->p_trackFile = SeratoTrackFilePtr();
}

void SeratoTrack::p_readMarkers(void)
{
    const SeratoCueMarkerVector& cueMarkers = this->p_trackFile->cueMarkers();
    for (auto& marker : cueMarkers) {
        this->p_cueMarkers->push_back(make_unique<SeratoCueMarker>(*marker));
    }

    const SeratoLoopMarkerVector& loopMarkers = this->p_trackFile->loopMarkers();
    for (auto& marker : loopMarkers) {
        this->p_loopMarkers->push_back(make_unique<SeratoLoopMarker>(*marker));
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
    return *(this->p_cueMarkers);
}

const SeratoLoopMarkerVector& SeratoTrack::loopMarkers(void) const
{
    return *(this->p_loopMarkers);
}

void SeratoTrack::setTitle(const char* title)
{
    this->p_setStringForSubTagForIdentifier(title, NxASeratoTrackTitleTag);
}

void SeratoTrack::setArtist(const char* artist)
{
    this->p_setStringForSubTagForIdentifier(artist, NxASeratoTrackArtistTag);
}

void SeratoTrack::setAlbum(const char* album)
{
    this->p_setStringForSubTagForIdentifier(album, NxASeratoTrackAlbumTag);
}

void SeratoTrack::setGenre(const char* genre)
{
    this->p_setStringForSubTagForIdentifier(genre, NxASeratoTrackGenreTag);
}

void SeratoTrack::setComments(const char* comments)
{
    this->p_setStringForSubTagForIdentifier(comments, NxASeratoTrackCommentsTag);
}

void SeratoTrack::setGrouping(const char* grouping)
{
    this->p_setStringForSubTagForIdentifier(grouping, NxASeratoTrackGroupingTag);
}

void SeratoTrack::setRemixer(const char* remixer)
{
    this->p_setStringForSubTagForIdentifier(remixer, NxASeratoTrackRemixerTag);
}

void SeratoTrack::setRecordLabel(const char* recordLabel)
{
    this->p_setStringForSubTagForIdentifier(recordLabel, NxASeratoTrackLabelTag);
}

void SeratoTrack::setComposer(const char* composer)
{
    this->p_setStringForSubTagForIdentifier(composer, NxASeratoTrackComposerTag);
}

void SeratoTrack::setKey(const char* key)
{
    this->p_setStringForSubTagForIdentifier(key, NxASeratoTrackKeyTag);
}

void SeratoTrack::setBpm(const char* bpm)
{
    this->p_setStringForSubTagForIdentifier(bpm, NxASeratoTrackBpmTag);
}

void SeratoTrack::setYear(const char* year)
{
    this->p_setStringForSubTagForIdentifier(year, NxASeratoTrackYearTag);
}

void SeratoTrack::setTrackNumber(const uint32_t& trackNumber)
{
    this->p_setUInt32ForSubTagForIdentifier(trackNumber, NxASeratoTrackNumberTag);
}

void SeratoTrack::setDiscNumber(const uint32_t& discNumber)
{
    this->p_setUInt32ForSubTagForIdentifier(discNumber, NxASeratoTrackDiscNumberTag);
}

void SeratoTrack::setDateModifiedInSecondsSinceJanuary1st1970(const uint32_t& dateModified)
{
    this->p_setUInt32ForSubTagForIdentifier(dateModified, NxASeratoTrackDateModifiedTag);
}

void SeratoTrack::setDateAddedInSecondsSinceJanuary1st1970(const uint32_t& dateAdded)
{
    this->p_setUInt32ForSubTagForIdentifier(dateAdded, NxASeratoTrackDateAddedTag);
}

void SeratoTrack::setCueMarkers(SeratoCueMarkerVectorPtr markers)
{
    this->p_cueMarkers = move(markers);
    this->p_wasModified = true;
}

void SeratoTrack::setLoopMarkers(SeratoLoopMarkerVectorPtr markers)
{
    this->p_loopMarkers = move(markers);
    this->p_wasModified = true;
}

bool SeratoTrack::wasModified(void) const
{
    return this->p_wasModified;
}

void SeratoTrack::addTo(CharVector& destination) const
{
    this->p_trackTag->addTo(destination);
}

void SeratoTrack::saveToTrackFile(void)
{
    this->p_loadTrackFile();

    SeratoTrackFile& trackFile = *(this->p_trackFile);
    trackFile.setTitle(this->title().c_str());
    trackFile.setArtist(this->artist().c_str());
    trackFile.setAlbum(this->album().c_str());
    trackFile.setGenre(this->genre().c_str());
    trackFile.setComments(this->comments().c_str());
    trackFile.setGrouping(this->grouping().c_str());
    trackFile.setRemixer(this->remixer().c_str());
    trackFile.setRecordLabel(this->recordLabel().c_str());
    trackFile.setComposer(this->composer().c_str());
    trackFile.setKey(this->key().c_str());
    trackFile.setBpm(this->bpm().c_str());
    trackFile.setYearReleased(this->year().c_str());
    trackFile.setTrackNumber(this->trackNumber());

    SeratoCueMarkerVectorPtr cueMarkers = make_unique<SeratoCueMarkerVector>();
    for (auto& marker : *(this->p_cueMarkers)) {
        SeratoCueMarkerPtr markerCopy = make_unique<SeratoCueMarker>(*marker);
        cueMarkers->push_back(move(markerCopy));
    }
    trackFile.setCueMarkers(move(cueMarkers));

    SeratoLoopMarkerVectorPtr loopMarkers = make_unique<SeratoLoopMarkerVector>();
    for (auto& marker : *(this->p_loopMarkers)) {
        SeratoLoopMarkerPtr markerCopy = make_unique<SeratoLoopMarker>(*marker);
        loopMarkers->push_back(move(markerCopy));
    }
    trackFile.setLoopMarkers(move(loopMarkers));

    this->p_saveTrackFile();

    this->p_unloadTrackFile();
}
