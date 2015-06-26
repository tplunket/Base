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
#include "Tags/ObjectTag.hpp"
#include "Tags/DatabaseV2Tags.hpp"

NXA_GENERATED_IMPLEMENTATION_FOR(NxA::Serato, Track, Object);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Factory Methods

Track::Pointer Track::trackWithTagOnVolume(Tag::Pointer const& trackTag, String::Pointer const& locatedOnVolumePath)
{
    auto internalObject = Internal::Track::Pointer(std::make_shared<Internal::Track>(trackTag, locatedOnVolumePath));
    auto newTrack = Track::makeSharedWithInternal(NxA::Internal::Object::Pointer::dynamicCastFrom(internalObject));

    auto trackFile = TrackFileFactory::trackFileForPath(newTrack->trackFilePath());
    newTrack->internal->readMarkersFrom(trackFile);

#if PRINT_DEBUG_INFO
    Internal::Track::debugPrintComparaison(newTrack, trackFile);
    Internal::Track::debugPrint(newTrack->internal->stringForSubTagForIdentifier(trackSizeTagIdentifier),
                                String::stringWith("stringsize"));
#endif

    return newTrack;
}

Track::Pointer Track::trackWithFileAtOnVolume(String::Pointer const& trackFilePath, String::Pointer const& locatedOnVolumePath)
{
    auto relativePath = File::removePrefixFromPath(String::stringWith(locatedOnVolumePath),
                                                   String::stringWith(trackFilePath));
    auto tags = Tag::Array::array();
    tags->append(Tag::Pointer::dynamicCastFrom(PathTag::tagWithIdentifierAndValue(trackFilePathTagIdentifier, relativePath)));

    auto trackTag = Tag::Pointer::dynamicCastFrom(ObjectTag::tagWithIdentifierAndValue(trackObjectTagIdentifier, tags));
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

#pragma mark Instance Methods

String::Pointer Track::trackFilePath(void) const
{
    auto pathFromRootFolder = internal->pathForSubTagForIdentifier(trackFilePathTagIdentifier);
    return File::joinPaths(internal->rootFolder, pathFromRootFolder);
}

String::Pointer const& Track::title(void) const
{
    return internal->stringForSubTagForIdentifier(trackTitleTagIdentifier);
}

String::Pointer const& Track::artist(void) const
{
    return internal->stringForSubTagForIdentifier(trackArtistTagIdentifier);
}

String::Pointer const& Track::album(void) const
{
    return internal->stringForSubTagForIdentifier(trackAlbumTagIdentifier);
}

String::Pointer const& Track::genre(void) const
{
    return internal->stringForSubTagForIdentifier(trackGenreTagIdentifier);
}

String::Pointer const& Track::comments(void) const
{
    return internal->stringForSubTagForIdentifier(trackCommentsTagIdentifier);
}

String::Pointer const& Track::grouping(void) const
{
    return internal->stringForSubTagForIdentifier(trackGroupingTagIdentifier);
}

String::Pointer const& Track::remixer(void) const
{
    return internal->stringForSubTagForIdentifier(trackRemixerTagIdentifier);
}

String::Pointer const& Track::recordLabel(void) const
{
    return internal->stringForSubTagForIdentifier(trackLabelTagIdentifier);
}

String::Pointer const& Track::composer(void) const
{
    return internal->stringForSubTagForIdentifier(trackComposerTagIdentifier);
}

String::Pointer const& Track::key(void) const
{
    return internal->stringForSubTagForIdentifier(trackKeyTagIdentifier);
}

String::Pointer const& Track::length(void) const
{
    return internal->stringForSubTagForIdentifier(trackLengthTagIdentifier);
}

count Track::size(void) const
{
    return internal->uint32ForSubTagForIdentifier(trackSizeTagIdentifier);
}

String::Pointer const& Track::bitRate(void) const
{
    return internal->stringForSubTagForIdentifier(trackBitrateTagIdentifier);
}

String::Pointer const& Track::sampleRate(void) const
{
    return internal->stringForSubTagForIdentifier(trackSampleRateTagIdentifier);
}

String::Pointer const& Track::bpm(void) const
{
    return internal->stringForSubTagForIdentifier(trackBpmTagIdentifier);
}

String::Pointer const& Track::year(void) const
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

CueMarker::Array::Pointer const& Track::cueMarkers(void) const
{
    return internal->cueMarkers;
}

LoopMarker::Array::Pointer const& Track::loopMarkers(void) const
{
    return internal->loopMarkers;
}

GridMarker::Array::Pointer const& Track::gridMarkers(void) const
{
    return internal->gridMarkers;
}

void Track::setTitle(String::Pointer const& title)
{
    internal->setStringForSubTagForIdentifier(title, trackTitleTagIdentifier);
}

void Track::setArtist(String::Pointer const& artist)
{
    internal->setStringForSubTagForIdentifier(artist, trackArtistTagIdentifier);
}

void Track::setAlbum(String::Pointer const& album)
{
    internal->setStringForSubTagForIdentifier(album, trackAlbumTagIdentifier);
}

void Track::setGenre(String::Pointer const& genre)
{
    internal->setStringForSubTagForIdentifier(genre, trackGenreTagIdentifier);
}

void Track::setComments(String::Pointer const& comments)
{
    internal->setStringForSubTagForIdentifier(comments, trackCommentsTagIdentifier);
}

void Track::setGrouping(String::Pointer const& grouping)
{
    internal->setStringForSubTagForIdentifier(grouping, trackGroupingTagIdentifier);
}

void Track::setRemixer(String::Pointer const& remixer)
{
    internal->setStringForSubTagForIdentifier(remixer, trackRemixerTagIdentifier);
}

void Track::setRecordLabel(String::Pointer const& recordLabel)
{
    internal->setStringForSubTagForIdentifier(recordLabel, trackLabelTagIdentifier);
}

void Track::setComposer(String::Pointer const& composer)
{
    internal->setStringForSubTagForIdentifier(composer, trackComposerTagIdentifier);
}

void Track::setKey(String::Pointer const& key)
{
    internal->setStringForSubTagForIdentifier(key, trackKeyTagIdentifier);
}

void Track::setBpm(String::Pointer const& bpm)
{
    internal->setStringForSubTagForIdentifier(bpm, trackBpmTagIdentifier);
}

void Track::setYear(String::Pointer const& year)
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

void Track::setCueMarkers(CueMarker::Array::Pointer const& markers)
{
    internal->cueMarkers = markers;
    internal->wasModified = true;
}

void Track::setLoopMarkers(LoopMarker::Array::Pointer const& markers)
{
    internal->loopMarkers = markers;
    internal->wasModified = true;
}

void Track::setGridMarkers(GridMarker::Array::Pointer const& markers)
{
    internal->gridMarkers = markers;
    internal->wasModified = true;
}

bool Track::wasModified(void) const
{
    return internal->wasModified;
}

void Track::addTo(Blob::Pointer& destination) const
{
    internal->trackTag->addTo(destination);
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
    for (auto& marker : *(this->cueMarkers())) {
        auto markerCopy = CueMarker::markerWith(marker);
        cueMarkers->append(markerCopy);
    }
    trackFile->setCueMarkers(cueMarkers);

    auto loopMarkers = LoopMarker::Array::array();
    for (auto& marker : *(this->loopMarkers())) {
        auto markerCopy = LoopMarker::markerWith(marker);
        loopMarkers->append(markerCopy);
    }
    trackFile->setLoopMarkers(loopMarkers);

    auto gridMarkers = GridMarker::Array::array();
    for (auto& marker : *(this->gridMarkers())) {
        auto markerCopy = GridMarker::markerWith(marker);
        gridMarkers->append(markerCopy);
    }
    trackFile->setGridMarkers(gridMarkers);

    trackFile->saveChanges();
}

void Track::destroy(void)
{
    delete this;
}
