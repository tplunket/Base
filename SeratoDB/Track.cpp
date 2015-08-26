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

#include <cstdlib>

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, Track, Object);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Factory Methods

Track::Pointer Track::trackWithTagOnVolume(ObjectTag& trackTag, const String& locatedOnVolumePath)
{
    auto internalObject = Internal::Track::Pointer(std::make_shared<Internal::Track>(trackTag, locatedOnVolumePath));
    auto newTrack = Track::makeSharedWithInternal(NxA::Internal::Object::Pointer::dynamicCastFrom(internalObject));

#if NXA_PRINT_DEBUG_INFO
    printf("----------------------------------------\n");
    auto trackFile = TrackFileFactory::trackFileForPath(newTrack->internal->trackFilePath());
    Internal::Track::debugPrint(*newTrack, *trackFile);
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
    newTrack->internal->needsToUpdateTrackFile = true;
    newTrack->internal->lastMarkersModificationDate = lastModificationDateInSecondsSince1970;
    newTrack->internal->needsToUpdateDatabaseFile = true;

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
                    this->size() == other.size() &&
                    this->length() == other.length() &&
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
    return internal->trackFilePath();
}

timestamp Track::trackFileModificationDateInSecondsSince1970(void) const
{
    auto filePath = internal->trackFilePath();
    return File::modificationDateInSecondsSince1970ForFile(*filePath);
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

const String& Track::size(void) const
{
    return internal->stringForSubTagForIdentifier(trackSizeTagIdentifier);
}

const String& Track::length(void) const
{
    return internal->stringForSubTagForIdentifier(trackLengthTagIdentifier);
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
    if (!internal->trackFileWasRead) {
        internal->readMarkers();
    }

    return internal->cueMarkers;
}

const LoopMarker::ArrayOfConst& Track::loopMarkers(void) const
{
    if (!internal->trackFileWasRead) {
        internal->readMarkers();
    }

    return internal->loopMarkers;
}

const GridMarker::ArrayOfConst& Track::gridMarkers(void) const
{
    if (!internal->trackFileWasRead) {
        internal->readMarkers();
    }

    return internal->gridMarkers;
}

void Track::setTitle(const String& title)
{
#if NXA_OUTPUT_DEBUG_METADATA
    internal->setStringForSubTagForIdentifier(String::stringWithFormat("!%s", title.toUTF8()), trackTitleTagIdentifier);
#else
    internal->setStringForSubTagForIdentifier(title, trackTitleTagIdentifier);
#endif
    internal->needsToUpdateDatabaseFile = true;
}

void Track::setArtist(const String& artist)
{
#if NXA_OUTPUT_DEBUG_METADATA
    internal->setStringForSubTagForIdentifier(String::stringWithFormat("!%s", artist.toUTF8()), trackArtistTagIdentifier);
#else
    internal->setStringForSubTagForIdentifier(artist, trackArtistTagIdentifier);
#endif
    internal->needsToUpdateDatabaseFile = true;
}

void Track::setAlbum(const String& album)
{
#if NXA_OUTPUT_DEBUG_METADATA
    internal->setStringForSubTagForIdentifier(String::stringWithFormat("!%s", album.toUTF8()), trackAlbumTagIdentifier);
#else
    internal->setStringForSubTagForIdentifier(album, trackAlbumTagIdentifier);
#endif
    internal->needsToUpdateDatabaseFile = true;
}

void Track::setGenre(const String& genre)
{
#if NXA_OUTPUT_DEBUG_METADATA
    internal->setStringForSubTagForIdentifier(String::stringWithFormat("!%s", genre.toUTF8()), trackGenreTagIdentifier);
#else
    internal->setStringForSubTagForIdentifier(genre, trackGenreTagIdentifier);
#endif
    internal->needsToUpdateDatabaseFile = true;
}

void Track::setComments(const String& comments)
{
#if NXA_OUTPUT_DEBUG_METADATA
    internal->setStringForSubTagForIdentifier(String::stringWithFormat("!%s", comments.toUTF8()), trackCommentsTagIdentifier);
#else
    internal->setStringForSubTagForIdentifier(comments, trackCommentsTagIdentifier);
#endif
    internal->needsToUpdateDatabaseFile = true;
}

void Track::setGrouping(const String& grouping)
{
#if NXA_OUTPUT_DEBUG_METADATA
    internal->setStringForSubTagForIdentifier(String::stringWithFormat("!%s", grouping.toUTF8()), trackGroupingTagIdentifier);
#else
    internal->setStringForSubTagForIdentifier(grouping, trackGroupingTagIdentifier);
#endif
    internal->needsToUpdateDatabaseFile = true;
}

void Track::setRemixer(const String& remixer)
{
#if NXA_OUTPUT_DEBUG_METADATA
    internal->setStringForSubTagForIdentifier(String::stringWithFormat("!%s", remixer.toUTF8()), trackRemixerTagIdentifier);
#else
    internal->setStringForSubTagForIdentifier(remixer, trackRemixerTagIdentifier);
#endif
    internal->needsToUpdateDatabaseFile = true;
}

void Track::setRecordLabel(const String& recordLabel)
{
#if NXA_OUTPUT_DEBUG_METADATA
    internal->setStringForSubTagForIdentifier(String::stringWithFormat("!%s", recordLabel.toUTF8()), trackLabelTagIdentifier);
#else
    internal->setStringForSubTagForIdentifier(recordLabel, trackLabelTagIdentifier);
#endif
    internal->needsToUpdateDatabaseFile = true;
}

void Track::setComposer(const String& composer)
{
#if NXA_OUTPUT_DEBUG_METADATA
    internal->setStringForSubTagForIdentifier(String::stringWithFormat("!%s", composer.toUTF8()), trackComposerTagIdentifier);
#else
    internal->setStringForSubTagForIdentifier(composer, trackComposerTagIdentifier);
#endif
    internal->needsToUpdateDatabaseFile = true;
}

void Track::setKey(const String& key)
{
#if NXA_OUTPUT_DEBUG_METADATA
    internal->setStringForSubTagForIdentifier(String::stringWithFormat("!%s", key.toUTF8()), trackKeyTagIdentifier);
#else
    internal->setStringForSubTagForIdentifier(key, trackKeyTagIdentifier);
#endif
    internal->needsToUpdateDatabaseFile = true;
}

void Track::setSizeInBytes(count size)
{
    internal->setUInt32ForSubTagForIdentifier(static_cast<uinteger32>(size), trackSizeTagIdentifier);

    decimal sizeInMb = static_cast<decimal>(size) / 1024.0f;
    auto sizeString = String::stringWithFormat("%.1fMB", sizeInMb);
    internal->setStringForSubTagForIdentifier(sizeString, trackSizeStringTagIdentifier);

    internal->needsToUpdateDatabaseFile = true;
}

void Track::setLengthInMilliseconds(uinteger32 length)
{
    uinteger32 minutes = length / 60000;
    uinteger32 milliseconds = length % 1000;
    uinteger32 seconds = (length - milliseconds - (minutes * 60000)) / 1000;

    milliseconds *= 60;
    milliseconds /= 1000;

    auto lengthString = String::stringWithFormat("%02d:%02d.%02d'\n", minutes, seconds, milliseconds);
    internal->setStringForSubTagForIdentifier(lengthString, trackLengthTagIdentifier);

    internal->needsToUpdateDatabaseFile = true;
}

void Track::setBitRateInKiloBitsPerSecond(uinteger32 bitRate)
{
    auto bitrateString = String::stringWithFormat("%ld.0kbps", bitRate);
    internal->setStringForSubTagForIdentifier(bitrateString, trackBitrateTagIdentifier);

    internal->needsToUpdateDatabaseFile = true;
}

void Track::setSampleRateInSamplesPerSecond(uinteger32 sampleRate)
{
    decimal sampleRateInThousandOfSamplesPerSecond = static_cast<decimal>(sampleRate) / 1000.0f;
    auto sampleRateString = String::stringWithFormat("%.1fk", sampleRateInThousandOfSamplesPerSecond);
    internal->setStringForSubTagForIdentifier(sampleRateString, trackSampleRateTagIdentifier);

    internal->needsToUpdateDatabaseFile = true;
}

void Track::setBpm(const String& bpm)
{
#if NXA_OUTPUT_DEBUG_METADATA
    internal->setStringForSubTagForIdentifier(String::stringWithFormat("%.2f", ::atof(bpm.toUTF8()) + 1.0f), trackBpmTagIdentifier);
#else
    internal->setStringForSubTagForIdentifier(bpm, trackBpmTagIdentifier);
#endif
    internal->needsToUpdateDatabaseFile = true;
}

void Track::setYear(const String& year)
{
#if NXA_OUTPUT_DEBUG_METADATA
    internal->setStringForSubTagForIdentifier(String::stringWithFormat("%4d", ::atoi(year.toUTF8()) + 1), trackYearTagIdentifier);
#else
    internal->setStringForSubTagForIdentifier(year, trackYearTagIdentifier);
#endif
    internal->needsToUpdateDatabaseFile = true;
}

void Track::setTrackNumber(count trackNumber)
{
#if NXA_OUTPUT_DEBUG_METADATA
    internal->setUInt32ForSubTagForIdentifier(static_cast<uinteger32>(this->trackNumber() + 10), trackNumberTagIdentifier);
#else
    // TODO: Should make sure the cast doesn't chop off anything.
    internal->setUInt32ForSubTagForIdentifier(static_cast<uinteger32>(trackNumber), trackNumberTagIdentifier);
#endif
    internal->needsToUpdateDatabaseFile = true;
}

void Track::setDiscNumber(count discNumber)
{
#if NXA_OUTPUT_DEBUG_METADATA
    internal->setUInt32ForSubTagForIdentifier(static_cast<uinteger32>(this->discNumber() + 10), trackDiscNumberTagIdentifier);
#else
    // TODO: Should make sure the cast doesn't chop off anything.
    internal->setUInt32ForSubTagForIdentifier(static_cast<uinteger32>(discNumber), trackDiscNumberTagIdentifier);
#endif
    internal->needsToUpdateDatabaseFile = true;
}

void Track::setDateModifiedInSecondsSinceJanuary1st1970(timestamp dateModified)
{
    // TODO: Should make sure the cast doesn't chop off anything.
    internal->setUInt32ForSubTagForIdentifier(static_cast<uinteger32>(dateModified), trackDateModifiedTagIdentifier);
    internal->needsToUpdateDatabaseFile = true;
}

void Track::setDateAddedInSecondsSinceJanuary1st1970(timestamp dateAdded)
{
    // TODO: Should make sure the cast doesn't chop off anything.
    internal->setUInt32ForSubTagForIdentifier(static_cast<uinteger32>(dateAdded), trackDateAddedTagIdentifier);
    internal->needsToUpdateDatabaseFile = true;
}

void Track::setCueMarkersWhichWereModifiedOn(CueMarker::ArrayOfConst& markers, timestamp modificationDateInSecondsSince1970)
{
#if NXA_OUTPUT_DEBUG_METADATA
    auto cueMarkers = CueMarker::ArrayOfConst::array();
    for (auto& marker : markers) {
        auto markerCopy = CueMarker::markerWithLabelPositionAndIndex(marker->label(),
                                                                     marker->positionInMilliseconds() + 1000,
                                                                     marker->index());
        cueMarkers->append(markerCopy);
    }
    internal->cueMarkers = cueMarkers;
#else
    internal->cueMarkers = markers.pointer();
#endif
    internal->needsToUpdateTrackFile = true;

    if (internal->lastMarkersModificationDate < modificationDateInSecondsSince1970) {
        internal->lastMarkersModificationDate = modificationDateInSecondsSince1970;
    }
}

void Track::setLoopMarkersWhichWereModifiedOn(LoopMarker::ArrayOfConst& markers, timestamp modificationDateInSecondsSince1970)
{
#if NXA_OUTPUT_DEBUG_METADATA
    auto loopMarkers = LoopMarker::ArrayOfConst::array();
    for (auto& marker : markers) {
        auto markerCopy = LoopMarker::markerWithLabelStartEndPositionsAndIndex(marker->label(),
                                                                               marker->startPositionInMilliseconds() + 1000,
                                                                               marker->endPositionInMilliseconds() + 1000,
                                                                               marker->index());
        loopMarkers->append(markerCopy);
    }
    internal->loopMarkers = loopMarkers;
#else
    internal->loopMarkers = markers.pointer();
#endif
    internal->needsToUpdateTrackFile = true;

    if (internal->lastMarkersModificationDate < modificationDateInSecondsSince1970) {
        internal->lastMarkersModificationDate = modificationDateInSecondsSince1970;
    }
}

void Track::setGridMarkersWhichWereModifiedOn(GridMarker::ArrayOfConst& markers, timestamp modificationDateInSecondsSince1970)
{
#if NXA_OUTPUT_DEBUG_METADATA
    auto gridMarkers = GridMarker::ArrayOfConst::array();
    for (auto& marker : markers) {
        auto markerCopy = GridMarker::markerWithPositionAndBeatsPerMinute(marker->positionInSeconds() + 0.2f,
                                                                          marker->beatsPerMinute() + 1.0f);
        gridMarkers->append(markerCopy);
    }
    internal->gridMarkers = gridMarkers;
#else
    internal->gridMarkers = markers.pointer();
#endif
    internal->needsToUpdateTrackFile = true;

    if (internal->lastMarkersModificationDate < modificationDateInSecondsSince1970) {
        internal->lastMarkersModificationDate = modificationDateInSecondsSince1970;
    }
}

boolean Track::needsToUpdateTrackFile(void) const
{
    return internal->needsToUpdateTrackFile;
}

boolean Track::needsToUpdateDatabaseFile(void) const
{
    return internal->needsToUpdateDatabaseFile;
}

void Track::addTo(Blob& destination) const
{
    internal->trackTag->addInSeratoTrackOrderTo(destination);
}

void Track::saveToTrackFile(void) const
{
    auto trackFile = TrackFileFactory::trackFileForPath(this->trackFilePath());
    NXA_ASSERT_TRUE(internal->lastMarkersModificationDate != 0);

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
