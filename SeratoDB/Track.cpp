//
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

#include "Track.hpp"
#include "TrackFiles/TrackFileFactory.hpp"
#include "Tags/DatabaseV2Tags.hpp"
#include "Tags/ObjectTag.hpp"
#include "Tags/TextTag.hpp"
#include "Tags/PathTag.hpp"
#include "Tags/UInt32Tag.hpp"

#include "Base/File.hpp"

using namespace NxA;
using namespace NxA::Serato;
using namespace std;

#pragma mark Debug macros

#define PRINT_DEBUG_INFO            0

#pragma mark Constants

static const String::Pointer emptyString = String::string();
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

static void p_debugPrintComparaison(const Track* track, const TrackFile* trackFile)
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

    printf("Found %ld cue markers, %ld grid markers and %ld loop markers.\n",
           trackFile->cueMarkers().size(),
           trackFile->gridMarkers().size(),
           trackFile->loopMarkers().size());
}
#endif

#pragma mark Constructors

Track::Track(TagPtr trackTag, const char* locatedOnVolumePath) :
             p_wasModified(false),
             p_trackTag(move(trackTag)),
             p_rootFolder(String::stringWithUTF8(locatedOnVolumePath)),
             p_cueMarkers(make_unique<CueMarkerVector>()),
             p_loopMarkers(make_unique<LoopMarkerVector>()),
             p_gridMarkers(make_unique<GridMarkerVector>())
{
    this->p_loadTrackFile();

    this->p_readMarkers();

#if PRINT_DEBUG_INFO
    p_debugPrintComparaison(this, this->p_trackFile.get());
    p_debugPrint(this->p_stringForSubTagForIdentifier(NxATrackSizeStringTag), "stringsize");
#endif

    this->p_unloadTrackFile();
}

Track::Track(const char* trackFilePath, const char* locatedOnVolumePath) :
             p_wasModified(true),
             p_rootFolder(String::stringWithUTF8(locatedOnVolumePath)),
             p_cueMarkers(make_unique<CueMarkerVector>()),
             p_loopMarkers(make_unique<LoopMarkerVector>()),
             p_gridMarkers(make_unique<GridMarkerVector>())
{
    String::Pointer relativePath = File::removePrefixFromPath(String::stringWithUTF8(locatedOnVolumePath),
                                                              String::stringWithUTF8(trackFilePath));
    TagVectorPtr tags(make_unique<TagVector>());
    tags->push_back(make_unique<PathTag>(NxASeratoTrackFilePathTag, relativePath->toUTF8()));

    this->p_trackTag = make_unique<ObjectTag>(NxASeratoTrackObjectTag, move(tags));

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

bool Track::p_containsAValidTrackTag(void) const
{
    return this->p_trackTag.get() != NULL;
}

const String::Pointer& Track::p_stringForSubTagForIdentifier(const uint32_t& identifier) const
{
    if (this->p_containsAValidTrackTag()) {
        const ObjectTag* trackObjectTag = dynamic_cast<const ObjectTag*>(this->p_trackTag.get());
        if (trackObjectTag->hasSubTagForIdentifier(identifier)) {
            const TextTag& textTag = dynamic_cast<const TextTag&>(trackObjectTag->subTagForIdentifier(identifier));
            return textTag.value();
        }
    }

    return emptyString;
}

const String::Pointer& Track::p_pathForSubTagForIdentifier(const uint32_t& identifier) const
{
    if (this->p_containsAValidTrackTag()) {
        const ObjectTag* trackObjectTag = dynamic_cast<const ObjectTag*>(this->p_trackTag.get());
        if (trackObjectTag->hasSubTagForIdentifier(identifier)) {
            const PathTag& pathTag = dynamic_cast<const PathTag&>(trackObjectTag->subTagForIdentifier(identifier));
            return pathTag.value();
        }
    }

    return emptyString;
}

const uint32_t& Track::p_uint32ForSubTagForIdentifier(const uint32_t& identifier) const
{
    if (this->p_containsAValidTrackTag()) {
        const ObjectTag* trackObjectTag = dynamic_cast<const ObjectTag*>(this->p_trackTag.get());
        if (trackObjectTag->hasSubTagForIdentifier(identifier)) {
            const UInt32Tag& uintTag = dynamic_cast<const UInt32Tag&>(trackObjectTag->subTagForIdentifier(identifier));
            return uintTag.value();
        }
    }

    return zeroValue;
}

void Track::p_setStringForSubTagForIdentifier(const char* value, const uint32_t& identifier)
{
    if (!this->p_containsAValidTrackTag()) {
        return;
    }

    ObjectTag* trackObjectTag = dynamic_cast<ObjectTag*>(this->p_trackTag.get());
    if (!trackObjectTag->hasSubTagForIdentifier(identifier)) {
        trackObjectTag->addSubTag(std::make_unique<TextTag>(identifier, value));
    }
    else {
        TextTag& textTag = dynamic_cast<TextTag&>(trackObjectTag->subTagForIdentifier(identifier));
        textTag.value() = String::stringWithUTF8(value);
    }

    this->p_wasModified = true;
}

void Track::p_setPathForSubTagForIdentifier(const char* value, const uint32_t& identifier)
{
    if (!this->p_containsAValidTrackTag()) {
        return;
    }

    ObjectTag* trackObjectTag = dynamic_cast<ObjectTag*>(this->p_trackTag.get());
    if (!trackObjectTag->hasSubTagForIdentifier(identifier)) {
        trackObjectTag->addSubTag(std::make_unique<PathTag>(identifier, value));
    }
    else {
        PathTag& pathTag = dynamic_cast<PathTag&>(trackObjectTag->subTagForIdentifier(identifier));
        pathTag.value() = String::stringWithUTF8(value);
    }

    this->p_wasModified = true;
}

void Track::p_setUInt32ForSubTagForIdentifier(const uint32_t& value, const uint32_t& identifier)
{
    if (!this->p_containsAValidTrackTag()) {
        return;
    }

    ObjectTag* trackObjectTag = dynamic_cast<ObjectTag*>(this->p_trackTag.get());
    if (!trackObjectTag->hasSubTagForIdentifier(identifier)) {
        trackObjectTag->addSubTag(std::make_unique<UInt32Tag>(identifier, value));
    }
    else {
        UInt32Tag& uint32Tag = dynamic_cast<UInt32Tag&>(trackObjectTag->subTagForIdentifier(identifier));
        uint32Tag.value() = value;
    }

    this->p_wasModified = true;
}

void Track::p_loadTrackFile(void)
{
    this->p_trackFile = TrackFileFactory::trackFileForPath(this->trackFilePath()->toUTF8());
}

void Track::p_saveTrackFile(void)
{
    this->p_trackFile->saveChanges();
}

void Track::p_unloadTrackFile(void)
{
    this->p_trackFile = TrackFilePtr();
}

void Track::p_readMarkers(void)
{
    const CueMarkerVector& cueMarkers = this->p_trackFile->cueMarkers();
    for (auto& marker : cueMarkers) {
        this->p_cueMarkers->push_back(make_unique<CueMarker>(*marker));
    }

    const LoopMarkerVector& loopMarkers = this->p_trackFile->loopMarkers();
    for (auto& marker : loopMarkers) {
        this->p_loopMarkers->push_back(make_unique<LoopMarker>(*marker));
    }

    const GridMarkerVector& gridMarkers = this->p_trackFile->gridMarkers();
    for (auto& marker : gridMarkers) {
        this->p_gridMarkers->push_back(make_unique<GridMarker>(*marker));
    }
}

String::Pointer Track::trackFilePath(void) const
{
    const String::Pointer& pathFromRootFolder = this->p_pathForSubTagForIdentifier(NxASeratoTrackFilePathTag);
    String::Pointer trackFilePath = File::joinPaths(this->p_rootFolder, pathFromRootFolder);
    return trackFilePath;
}

const String::Pointer& Track::title(void) const
{
    return this->p_stringForSubTagForIdentifier(NxASeratoTrackTitleTag);
}

const String::Pointer& Track::artist(void) const
{
    return this->p_stringForSubTagForIdentifier(NxASeratoTrackArtistTag);
}

const String::Pointer& Track::album(void) const
{
    return this->p_stringForSubTagForIdentifier(NxASeratoTrackAlbumTag);
}

const String::Pointer& Track::genre(void) const
{
    return this->p_stringForSubTagForIdentifier(NxASeratoTrackGenreTag);
}

const String::Pointer& Track::comments(void) const
{
    return this->p_stringForSubTagForIdentifier(NxASeratoTrackCommentsTag);
}

const String::Pointer& Track::grouping(void) const
{
    return this->p_stringForSubTagForIdentifier(NxASeratoTrackGroupingTag);
}

const String::Pointer& Track::remixer(void) const
{
    return this->p_stringForSubTagForIdentifier(NxASeratoTrackRemixerTag);
}

const String::Pointer& Track::recordLabel(void) const
{
    return this->p_stringForSubTagForIdentifier(NxASeratoTrackLabelTag);
}

const String::Pointer& Track::composer(void) const
{
    return this->p_stringForSubTagForIdentifier(NxASeratoTrackComposerTag);
}

const String::Pointer& Track::key(void) const
{
    return this->p_stringForSubTagForIdentifier(NxASeratoTrackKeyTag);
}

const String::Pointer& Track::length(void) const
{
    return this->p_stringForSubTagForIdentifier(NxASeratoTrackLengthTag);
}

const uint32_t& Track::sizeInBytes(void) const
{
    return this->p_uint32ForSubTagForIdentifier(NxASeratoTrackSizeTag);
}

const String::Pointer& Track::bitRate(void) const
{
    return this->p_stringForSubTagForIdentifier(NxASeratoTrackBitrateTag);
}

const String::Pointer& Track::sampleRate(void) const
{
    return this->p_stringForSubTagForIdentifier(NxASeratoTrackSampleRateTag);
}

const String::Pointer& Track::bpm(void) const
{
    return this->p_stringForSubTagForIdentifier(NxASeratoTrackBpmTag);
}

const String::Pointer& Track::year(void) const
{
    return this->p_stringForSubTagForIdentifier(NxASeratoTrackYearTag);
}

const uint32_t& Track::trackNumber(void) const
{
    return this->p_uint32ForSubTagForIdentifier(NxASeratoTrackNumberTag);
}

const uint32_t& Track::discNumber(void) const
{
    return this->p_uint32ForSubTagForIdentifier(NxASeratoTrackDiscNumberTag);
}

const uint32_t& Track::dateModifiedInSecondsSinceJanuary1st1970(void) const
{
    return this->p_uint32ForSubTagForIdentifier(NxASeratoTrackDateModifiedTag);
}

const uint32_t& Track::dateAddedInSecondsSinceJanuary1st1970(void) const
{
    return this->p_uint32ForSubTagForIdentifier(NxASeratoTrackDateAddedTag);
}

const CueMarkerVector& Track::cueMarkers(void) const
{
    return *(this->p_cueMarkers);
}

const LoopMarkerVector& Track::loopMarkers(void) const
{
    return *(this->p_loopMarkers);
}

const GridMarkerVector& Track::gridMarkers(void) const
{
    return *(this->p_gridMarkers);
}

void Track::setTitle(const char* title)
{
    this->p_setStringForSubTagForIdentifier(title, NxASeratoTrackTitleTag);
}

void Track::setArtist(const char* artist)
{
    this->p_setStringForSubTagForIdentifier(artist, NxASeratoTrackArtistTag);
}

void Track::setAlbum(const char* album)
{
    this->p_setStringForSubTagForIdentifier(album, NxASeratoTrackAlbumTag);
}

void Track::setGenre(const char* genre)
{
    this->p_setStringForSubTagForIdentifier(genre, NxASeratoTrackGenreTag);
}

void Track::setComments(const char* comments)
{
    this->p_setStringForSubTagForIdentifier(comments, NxASeratoTrackCommentsTag);
}

void Track::setGrouping(const char* grouping)
{
    this->p_setStringForSubTagForIdentifier(grouping, NxASeratoTrackGroupingTag);
}

void Track::setRemixer(const char* remixer)
{
    this->p_setStringForSubTagForIdentifier(remixer, NxASeratoTrackRemixerTag);
}

void Track::setRecordLabel(const char* recordLabel)
{
    this->p_setStringForSubTagForIdentifier(recordLabel, NxASeratoTrackLabelTag);
}

void Track::setComposer(const char* composer)
{
    this->p_setStringForSubTagForIdentifier(composer, NxASeratoTrackComposerTag);
}

void Track::setKey(const char* key)
{
    this->p_setStringForSubTagForIdentifier(key, NxASeratoTrackKeyTag);
}

void Track::setBpm(const char* bpm)
{
    this->p_setStringForSubTagForIdentifier(bpm, NxASeratoTrackBpmTag);
}

void Track::setYear(const char* year)
{
    this->p_setStringForSubTagForIdentifier(year, NxASeratoTrackYearTag);
}

void Track::setTrackNumber(const uint32_t& trackNumber)
{
    this->p_setUInt32ForSubTagForIdentifier(trackNumber, NxASeratoTrackNumberTag);
}

void Track::setDiscNumber(const uint32_t& discNumber)
{
    this->p_setUInt32ForSubTagForIdentifier(discNumber, NxASeratoTrackDiscNumberTag);
}

void Track::setDateModifiedInSecondsSinceJanuary1st1970(const uint32_t& dateModified)
{
    this->p_setUInt32ForSubTagForIdentifier(dateModified, NxASeratoTrackDateModifiedTag);
}

void Track::setDateAddedInSecondsSinceJanuary1st1970(const uint32_t& dateAdded)
{
    this->p_setUInt32ForSubTagForIdentifier(dateAdded, NxASeratoTrackDateAddedTag);
}

void Track::setCueMarkers(CueMarkerVectorPtr markers)
{
    this->p_cueMarkers = move(markers);
    this->p_wasModified = true;
}

void Track::setLoopMarkers(LoopMarkerVectorPtr markers)
{
    this->p_loopMarkers = move(markers);
    this->p_wasModified = true;
}

void Track::setGridMarkers(GridMarkerVectorPtr markers)
{
    this->p_gridMarkers = move(markers);
    this->p_wasModified = true;
}

bool Track::wasModified(void) const
{
    return this->p_wasModified;
}

void Track::addTo(Blob::Pointer& destination) const
{
    this->p_trackTag->addTo(destination);
}

void Track::saveToTrackFile(void)
{
    this->p_loadTrackFile();

    TrackFile& trackFile = *(this->p_trackFile);
    trackFile.setTitle(this->title()->toUTF8());
    trackFile.setArtist(this->artist()->toUTF8());
    trackFile.setAlbum(this->album()->toUTF8());
    trackFile.setGenre(this->genre()->toUTF8());
    trackFile.setComments(this->comments()->toUTF8());
    trackFile.setGrouping(this->grouping()->toUTF8());
    trackFile.setRemixer(this->remixer()->toUTF8());
    trackFile.setRecordLabel(this->recordLabel()->toUTF8());
    trackFile.setComposer(this->composer()->toUTF8());
    trackFile.setKey(this->key()->toUTF8());
    trackFile.setBpm(this->bpm()->toUTF8());
    trackFile.setYearReleased(this->year()->toUTF8());
    trackFile.setTrackNumber(this->trackNumber());

    CueMarkerVectorPtr cueMarkers = make_unique<CueMarkerVector>();
    for (auto& marker : *(this->p_cueMarkers)) {
        CueMarkerPtr markerCopy = make_unique<CueMarker>(*marker);
        cueMarkers->push_back(move(markerCopy));
    }
    trackFile.setCueMarkers(move(cueMarkers));

    LoopMarkerVectorPtr loopMarkers = make_unique<LoopMarkerVector>();
    for (auto& marker : *(this->p_loopMarkers)) {
        LoopMarkerPtr markerCopy = make_unique<LoopMarker>(*marker);
        loopMarkers->push_back(move(markerCopy));
    }
    trackFile.setLoopMarkers(move(loopMarkers));

    GridMarkerVectorPtr gridMarkers = make_unique<GridMarkerVector>();
    for (auto& marker : *(this->p_gridMarkers)) {
        GridMarkerPtr markerCopy = make_unique<GridMarker>(*marker);
        gridMarkers->push_back(move(markerCopy));
    }
    trackFile.setGridMarkers(move(gridMarkers));

    this->p_saveTrackFile();

    this->p_unloadTrackFile();
}

void Track::destroy(void)
{
    delete this;
}
