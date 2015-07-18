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

#include "SeratoDB/Track.hpp"
#include "SeratoDB/Internal/Track.hpp"
#include "TrackFiles/TrackFileFactory.hpp"
#include "Tags/PathTag.hpp"
#include "Tags/DatabaseV2Tags.hpp"

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, Track, Object);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Factory Methods

Track::Pointer Track::trackWithTagOnVolume(ObjectTag& trackTag, const String& locatedOnVolumePath)
{
    auto internalObject = Internal::Track::Pointer(std::make_shared<Internal::Track>(trackTag, locatedOnVolumePath));
    auto newTrack = Track::makeSharedWithInternal(NxA::Internal::Object::Pointer::dynamicCastFrom(internalObject));

    auto trackFile = TrackFileFactory::trackFileForPath(newTrack->trackFilePath());
    newTrack->internal->readMarkersFrom(trackFile);

#if PRINT_DEBUG_INFO
    Internal::Track::debugPrintComparaison(newTrack, trackFile);
#endif

    return newTrack;
}

Track::Pointer Track::trackWithFileAtOnVolume(const String& trackFilePath, const String& locatedOnVolumePath)
{
    auto relativePath = File::removePrefixFromPath(String::stringWith(locatedOnVolumePath),
                                                   String::stringWith(trackFilePath));
    auto tags = Tag::Array::array();
    tags->append(Tag::Pointer::dynamicCastFrom(PathTag::tagWithIdentifierAndValue(trackFilePathTagIdentifier, relativePath)));

    auto trackTag = ObjectTag::tagWithIdentifierAndValue(trackObjectTagIdentifier, tags);
    auto internalObject = Internal::Track::Pointer(std::make_shared<Internal::Track>(trackTag, locatedOnVolumePath));
    auto newTrack = Track::makeSharedWithInternal(NxA::Internal::Object::Pointer::dynamicCastFrom(internalObject));
    newTrack->internal->wasModified = true;

    // TODO: Output correct values:
    // '06:20.16'
    // bitrate '128.0kbps' '256kbps' '901kbps' '854kbps' '1411.2kbps'
    // samplerate '44.1k'
    // 68.1MB 17.3MB 9.6MB

    newTrack->internal->setStringForSubTagForIdentifier(String::stringWith("test"), trackLengthTagIdentifier);
    newTrack->internal->setStringForSubTagForIdentifier(String::stringWith("test"), trackBitrateTagIdentifier);
    newTrack->internal->setStringForSubTagForIdentifier(String::stringWith("test"), trackSampleRateTagIdentifier);
    newTrack->internal->setStringForSubTagForIdentifier(String::stringWith("test"), trackSizeStringTagIdentifier);
    newTrack->internal->setUInt32ForSubTagForIdentifier(23, trackSizeTagIdentifier);

    return newTrack;
}

#pragma mark Operators

bool Track::operator==(const Track& other) const
{
    if (this == &other) {
        return true;
    }
    else if (*this->trackFilePath() != *other.trackFilePath()) {
        return false;
    }

    NXA_ASSERT_TRUE(this->title() == other.title() &&
                    this->artist() == other.artist() &&
                    this->album() == other.album() &&
                    this->genre() == other.genre() &&
                    this->comments() == other.comments() &&
                    this->grouping() == other.grouping() &&
                    this->remixer() == other.remixer() &&
                    this->recordLabel() == other.recordLabel() &&
                    this->composer() == other.composer() &&
                    this->key() == other.key() &&
                    this->length() == other.length() &&
                    this->size() == other.size() &&
                    this->bitRate() == other.bitRate() &&
                    this->sampleRate() == other.sampleRate() &&
                    this->bpm() == other.bpm() &&
                    this->year() == other.year() &&
                    this->trackNumber() == other.trackNumber() &&
                    this->discNumber() == other.discNumber() &&
                    this->dateModifiedInSecondsSinceJanuary1st1970() == other.dateModifiedInSecondsSinceJanuary1st1970() &&
                    this->dateAddedInSecondsSinceJanuary1st1970() == other.dateAddedInSecondsSinceJanuary1st1970() &&
                    this->cueMarkers() == other.cueMarkers() &&
                    this->gridMarkers() == other.gridMarkers() &&
                    this->loopMarkers() == other.loopMarkers());

    return true;
}

#pragma mark Instance Methods

String::Pointer Track::trackFilePath(void) const
{
    auto& pathFromRootFolder = internal->pathForSubTagForIdentifier(trackFilePathTagIdentifier);
    return File::joinPaths(internal->rootFolder, pathFromRootFolder);
}

const String& Track::title(void) const
{
    return internal->stringForSubTagForIdentifier(trackTitleTagIdentifier);
}

const String& Track::artist(void) const
{
    return internal->stringForSubTagForIdentifier(trackArtistTagIdentifier);
}

const String& Track::album(void) const
{
    return internal->stringForSubTagForIdentifier(trackAlbumTagIdentifier);
}

const String& Track::genre(void) const
{
    return internal->stringForSubTagForIdentifier(trackGenreTagIdentifier);
}

const String& Track::comments(void) const
{
    return internal->stringForSubTagForIdentifier(trackCommentsTagIdentifier);
}

const String& Track::grouping(void) const
{
    return internal->stringForSubTagForIdentifier(trackGroupingTagIdentifier);
}

const String& Track::remixer(void) const
{
    return internal->stringForSubTagForIdentifier(trackRemixerTagIdentifier);
}

const String& Track::recordLabel(void) const
{
    return internal->stringForSubTagForIdentifier(trackLabelTagIdentifier);
}

const String& Track::composer(void) const
{
    return internal->stringForSubTagForIdentifier(trackComposerTagIdentifier);
}

const String& Track::key(void) const
{
    return internal->stringForSubTagForIdentifier(trackKeyTagIdentifier);
}

const String& Track::length(void) const
{
    return internal->stringForSubTagForIdentifier(trackLengthTagIdentifier);
}

count Track::size(void) const
{
    return internal->uint32ForSubTagForIdentifier(trackSizeTagIdentifier);
}

const String& Track::bitRate(void) const
{
    return internal->stringForSubTagForIdentifier(trackBitrateTagIdentifier);
}

const String& Track::sampleRate(void) const
{
    return internal->stringForSubTagForIdentifier(trackSampleRateTagIdentifier);
}

const String& Track::bpm(void) const
{
    return internal->stringForSubTagForIdentifier(trackBpmTagIdentifier);
}

const String& Track::year(void) const
{
    return internal->stringForSubTagForIdentifier(trackYearTagIdentifier);
}

count Track::trackNumber(void) const
{
    return internal->uint32ForSubTagForIdentifier(trackNumberTagIdentifier);
}

count Track::discNumber(void) const
{
    return internal->uint32ForSubTagForIdentifier(trackDiscNumberTagIdentifier);
}

timestamp Track::dateModifiedInSecondsSinceJanuary1st1970(void) const
{
    return internal->uint32ForSubTagForIdentifier(trackDateModifiedTagIdentifier);
}

timestamp Track::dateAddedInSecondsSinceJanuary1st1970(void) const
{
    return internal->uint32ForSubTagForIdentifier(trackDateAddedTagIdentifier);
}

const CueMarker::ArrayOfConst& Track::cueMarkers(void) const
{
    return internal->cueMarkers;
}

const LoopMarker::ArrayOfConst& Track::loopMarkers(void) const
{
    return internal->loopMarkers;
}

const GridMarker::ArrayOfConst& Track::gridMarkers(void) const
{
    return internal->gridMarkers;
}

void Track::setTitle(const String& title)
{
    internal->setStringForSubTagForIdentifier(title, trackTitleTagIdentifier);
}

void Track::setArtist(const String& artist)
{
    internal->setStringForSubTagForIdentifier(artist, trackArtistTagIdentifier);
}

void Track::setAlbum(const String& album)
{
    internal->setStringForSubTagForIdentifier(album, trackAlbumTagIdentifier);
}

void Track::setGenre(const String& genre)
{
    internal->setStringForSubTagForIdentifier(genre, trackGenreTagIdentifier);
}

void Track::setComments(const String& comments)
{
    internal->setStringForSubTagForIdentifier(comments, trackCommentsTagIdentifier);
}

void Track::setGrouping(const String& grouping)
{
    internal->setStringForSubTagForIdentifier(grouping, trackGroupingTagIdentifier);
}

void Track::setRemixer(const String& remixer)
{
    internal->setStringForSubTagForIdentifier(remixer, trackRemixerTagIdentifier);
}

void Track::setRecordLabel(const String& recordLabel)
{
    internal->setStringForSubTagForIdentifier(recordLabel, trackLabelTagIdentifier);
}

void Track::setComposer(const String& composer)
{
    internal->setStringForSubTagForIdentifier(composer, trackComposerTagIdentifier);
}

void Track::setKey(const String& key)
{
    internal->setStringForSubTagForIdentifier(key, trackKeyTagIdentifier);
}

void Track::setBpm(const String& bpm)
{
    internal->setStringForSubTagForIdentifier(bpm, trackBpmTagIdentifier);
}

void Track::setYear(const String& year)
{
    internal->setStringForSubTagForIdentifier(year, trackYearTagIdentifier);
}

void Track::setTrackNumber(count trackNumber)
{
    // TODO: Should make sure the cast doesn't chop off anything.
    internal->setUInt32ForSubTagForIdentifier(static_cast<uinteger32>(trackNumber), trackNumberTagIdentifier);
}

void Track::setDiscNumber(count discNumber)
{
    // TODO: Should make sure the cast doesn't chop off anything.
    internal->setUInt32ForSubTagForIdentifier(static_cast<uinteger32>(discNumber), trackDiscNumberTagIdentifier);
}

void Track::setDateModifiedInSecondsSinceJanuary1st1970(timestamp dateModified)
{
    // TODO: Should make sure the cast doesn't chop off anything.
    internal->setUInt32ForSubTagForIdentifier(static_cast<uinteger32>(dateModified), trackDateModifiedTagIdentifier);
}

void Track::setDateAddedInSecondsSinceJanuary1st1970(timestamp dateAdded)
{
    // TODO: Should make sure the cast doesn't chop off anything.
    internal->setUInt32ForSubTagForIdentifier(static_cast<uinteger32>(dateAdded), trackDateAddedTagIdentifier);
}

void Track::setCueMarkers(CueMarker::ArrayOfConst& markers)
{
    internal->cueMarkers = markers.pointer();
    internal->wasModified = true;
}

void Track::setLoopMarkers(LoopMarker::ArrayOfConst& markers)
{
    internal->loopMarkers = markers.pointer();
    internal->wasModified = true;
}

void Track::setGridMarkers(GridMarker::ArrayOfConst& markers)
{
    internal->gridMarkers = markers.pointer();
    internal->wasModified = true;
}

boolean Track::wasModified(void) const
{
    return internal->wasModified;
}

void Track::addTo(Blob& destination) const
{
    internal->trackTag->addInSeratoTrackOrderTo(destination);
}

void Track::saveToTrackFile(void) const
{
    auto trackFile = TrackFileFactory::trackFileForPath(this->trackFilePath());

    trackFile->setTitle(this->title());
    trackFile->setArtist(this->artist());
    trackFile->setAlbum(this->album());
    trackFile->setGenre(this->genre());
    trackFile->setComments(this->comments());
    trackFile->setGrouping(this->grouping());
    trackFile->setRemixer(this->remixer());
    trackFile->setRecordLabel(this->recordLabel());
    trackFile->setComposer(this->composer());
    trackFile->setKey(this->key());
    trackFile->setBpm(this->bpm());
    trackFile->setYearReleased(this->year());
    trackFile->setTrackNumber(this->trackNumber());

    auto cueMarkers = CueMarker::Array::array();
    for (auto& marker : this->cueMarkers()) {
        auto markerCopy = CueMarker::markerWith(marker);
        cueMarkers->append(markerCopy);
    }
    trackFile->setCueMarkers(cueMarkers);

    auto loopMarkers = LoopMarker::Array::array();
    for (auto& marker : this->loopMarkers()) {
        auto markerCopy = LoopMarker::markerWith(marker);
        loopMarkers->append(markerCopy);
    }
    trackFile->setLoopMarkers(loopMarkers);

    auto gridMarkers = GridMarker::Array::array();
    for (auto& marker : this->gridMarkers()) {
        auto markerCopy = GridMarker::markerWith(marker);
        gridMarkers->append(markerCopy);
    }
    trackFile->setGridMarkers(gridMarkers);

    trackFile->saveChanges();
}
