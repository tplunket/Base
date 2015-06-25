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

NXA_GENERATED_IMPLEMENTATION_FOR(NxA::Serato, Track);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Constructors & Destructors

Track::Track(NxA::Internal::Object::Pointer const& initial_internal) :
             Object(initial_internal),
             internal(initial_internal) { }

#pragma mark Factory Methods

Track::Pointer Track::trackWithTagOnVolume(Tag::Pointer const& trackTag, String::ConstPointer const& locatedOnVolumePath)
{
    auto internalObject = Internal::Track::Pointer(std::make_shared<Internal::Track>(trackTag, locatedOnVolumePath));
    auto newTrack = Track::makeSharedWithInternal(internalObject);

    auto trackFile = TrackFileFactory::trackFileForPath(newTrack->trackFilePath());
    newTrack->internal->readMarkersFrom(trackFile);

#if PRINT_DEBUG_INFO
    Internal::Track::debugPrintComparaison(newTrack, trackFile);
    Internal::Track::debugPrint(newTrack->internal->stringForSubTagForIdentifier(trackSizeTagIdentifier),
                                String::stringWith("stringsize"));
#endif

    return newTrack;
}

Track::Pointer Track::trackWithFileAtOnVolume(String::ConstPointer const& trackFilePath, String::ConstPointer const& locatedOnVolumePath)
{
    auto relativePath = File::removePrefixFromPath(String::stringWith(locatedOnVolumePath),
                                                   String::stringWith(trackFilePath));
    auto tags = Tag::Array::array();
    tags->append(PathTag::tagWithIdentifierAndValue(trackFilePathTagIdentifier, relativePath));

    auto trackTag = ObjectTag::tagWithIdentifierAndValue(trackObjectTagIdentifier, tags);
    auto internalObject = Internal::Track::Pointer(std::make_shared<Internal::Track>(trackTag, locatedOnVolumePath));
    auto newTrack = Track::makeSharedWithInternal(internalObject);
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

String::ConstPointer const& Track::title(void) const
{
    return internal->stringForSubTagForIdentifier(trackTitleTagIdentifier);
}

String::ConstPointer const& Track::artist(void) const
{
    return internal->stringForSubTagForIdentifier(trackArtistTagIdentifier);
}

String::ConstPointer const& Track::album(void) const
{
    return internal->stringForSubTagForIdentifier(trackAlbumTagIdentifier);
}

String::ConstPointer const& Track::genre(void) const
{
    return internal->stringForSubTagForIdentifier(trackGenreTagIdentifier);
}

String::ConstPointer const& Track::comments(void) const
{
    return internal->stringForSubTagForIdentifier(trackCommentsTagIdentifier);
}

String::ConstPointer const& Track::grouping(void) const
{
    return internal->stringForSubTagForIdentifier(trackGroupingTagIdentifier);
}

String::ConstPointer const& Track::remixer(void) const
{
    return internal->stringForSubTagForIdentifier(trackRemixerTagIdentifier);
}

String::ConstPointer const& Track::recordLabel(void) const
{
    return internal->stringForSubTagForIdentifier(trackLabelTagIdentifier);
}

String::ConstPointer const& Track::composer(void) const
{
    return internal->stringForSubTagForIdentifier(trackComposerTagIdentifier);
}

String::ConstPointer const& Track::key(void) const
{
    return internal->stringForSubTagForIdentifier(trackKeyTagIdentifier);
}

String::ConstPointer const& Track::length(void) const
{
    return internal->stringForSubTagForIdentifier(trackLengthTagIdentifier);
}

count Track::size(void) const
{
    return internal->uint32ForSubTagForIdentifier(trackSizeTagIdentifier);
}

String::ConstPointer const& Track::bitRate(void) const
{
    return internal->stringForSubTagForIdentifier(trackBitrateTagIdentifier);
}

String::ConstPointer const& Track::sampleRate(void) const
{
    return internal->stringForSubTagForIdentifier(trackSampleRateTagIdentifier);
}

String::ConstPointer const& Track::bpm(void) const
{
    return internal->stringForSubTagForIdentifier(trackBpmTagIdentifier);
}

String::ConstPointer const& Track::year(void) const
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

CueMarker::Array::ConstPointer const& Track::cueMarkers(void) const
{
    return internal->cueMarkers;
}

LoopMarker::Array::ConstPointer const& Track::loopMarkers(void) const
{
    return internal->loopMarkers;
}

GridMarker::Array::ConstPointer const& Track::gridMarkers(void) const
{
    return internal->gridMarkers;
}

void Track::setTitle(String::ConstPointer const& title)
{
    internal->setStringForSubTagForIdentifier(title, trackTitleTagIdentifier);
}

void Track::setArtist(String::ConstPointer const& artist)
{
    internal->setStringForSubTagForIdentifier(artist, trackArtistTagIdentifier);
}

void Track::setAlbum(String::ConstPointer const& album)
{
    internal->setStringForSubTagForIdentifier(album, trackAlbumTagIdentifier);
}

void Track::setGenre(String::ConstPointer const& genre)
{
    internal->setStringForSubTagForIdentifier(genre, trackGenreTagIdentifier);
}

void Track::setComments(String::ConstPointer const& comments)
{
    internal->setStringForSubTagForIdentifier(comments, trackCommentsTagIdentifier);
}

void Track::setGrouping(String::ConstPointer const& grouping)
{
    internal->setStringForSubTagForIdentifier(grouping, trackGroupingTagIdentifier);
}

void Track::setRemixer(String::ConstPointer const& remixer)
{
    internal->setStringForSubTagForIdentifier(remixer, trackRemixerTagIdentifier);
}

void Track::setRecordLabel(String::ConstPointer const& recordLabel)
{
    internal->setStringForSubTagForIdentifier(recordLabel, trackLabelTagIdentifier);
}

void Track::setComposer(String::ConstPointer const& composer)
{
    internal->setStringForSubTagForIdentifier(composer, trackComposerTagIdentifier);
}

void Track::setKey(String::ConstPointer const& key)
{
    internal->setStringForSubTagForIdentifier(key, trackKeyTagIdentifier);
}

void Track::setBpm(String::ConstPointer const& bpm)
{
    internal->setStringForSubTagForIdentifier(bpm, trackBpmTagIdentifier);
}

void Track::setYear(String::ConstPointer const& year)
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

void Track::addTo(Blob::Pointer const& destination) const
{
    internal->trackTag->addTo(destination);
}

void Track::saveToTrackFile(void)
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
