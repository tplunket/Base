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
    printf("Reading track ----------------------------------------\n");
    Track::debugPrint(*newTrack);
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
    newTrack->internal->setBooleanForSubTagForIdentifier(false, trackBeatGridIsLockedTagIdentifier);
    newTrack->internal->setBooleanForSubTagForIdentifier(false, trackIsCorruptedTagIdentifier);
    newTrack->internal->setBooleanForSubTagForIdentifier(true, trackMetadataWasReadTagIdentifier);
    newTrack->internal->setBooleanForSubTagForIdentifier(false, trackIsReadOnlyTagIdentifier);
    newTrack->internal->setBooleanForSubTagForIdentifier(false, trackIsFromItunesTagIdentifier);
    newTrack->internal->setBooleanForSubTagForIdentifier(false, trackLoopingTagIdentifier);
    newTrack->internal->setBooleanForSubTagForIdentifier(false, trackFileIsMissingTagIdentifier);
    newTrack->internal->setBooleanForSubTagForIdentifier(false, trackWasRecentlyPlayedTagIdentifier);
    newTrack->internal->setBooleanForSubTagForIdentifier(false, trackFileTypeIsUnsuportedTagIdentifier);
    newTrack->internal->setBooleanForSubTagForIdentifier(false, trackIsWhiteLabelTagIdentifier);
    newTrack->internal->setBooleanForSubTagForIdentifier(false, trackIsAccessControlledWhiteLabelTagIdentifier);
    newTrack->internal->setUInt32ForSubTagForIdentifier(0, trackLabelColorTagIdentifier);

    const character *fileTypeString = nullptr;

    auto fileType = TrackFileFactory::audioFileTypeForPath(trackFilePath);
    switch (fileType) {
        case TrackFileFactory::AIFF: {
            fileTypeString = "aiff";
            break;
        }
        case TrackFileFactory::MP3: {
            fileTypeString = "mp3";
            break;
        }
        case TrackFileFactory::WAV: {
            fileTypeString = "wave";
            break;
        }
        case TrackFileFactory::OGG: {
            fileTypeString = "oggvorbis";
            break;
        }
        case TrackFileFactory::AAC:
        case TrackFileFactory::ALAC: {
            fileTypeString = "quicktime";
            break;
        }
        case TrackFileFactory::FLAC: {
            fileTypeString = "flac";
            break;
        }
        default: {
            fileTypeString = "unknown";
        }
    }

    newTrack->internal->setStringForSubTagForIdentifier(String::stringWith(fileTypeString), trackFileTypeTagIdentifier);

    newTrack->internal->needsToUpdateDatabaseFile = true;

    return newTrack;
}

#pragma mark Class Methods

#if NXA_PRINT_DEBUG_INFO
void Track::debugPrintString(const String& text, const String& name)
{
    printf("%s '%s'\n", name.toUTF8(), text.toUTF8());
}

void Track::debugPrintUint(uinteger32 value, const String& name)
{
    printf("%s '%d'\n", name.toUTF8(), value);
}

void Track::debugPrintDate(timestamp value, const String& name)
{
    char* stringVersion = ctime(&value);
    printf("%s %s", name.toUTF8(), stringVersion);
}

void Track::debugPrint(const Serato::Track& track)
{
    track.internal->trackTag->debugPrint();

    Track::debugPrintUint(static_cast<uinteger32>(track.size()), String::stringWith("size"));
    Track::debugPrintDate(track.dateModifiedInSecondsSinceJanuary1st1970(), String::stringWith("datemodified"));
    Track::debugPrintDate(track.dateAddedInSecondsSinceJanuary1st1970(), String::stringWith("dateadded"));

    Track::debugPrintString(track.title(), String::stringWith("title"));
    Track::debugPrintString(track.artist(), String::stringWith("artist"));
    Track::debugPrintString(track.album(), String::stringWith("album"));
    Track::debugPrintString(track.comments(), String::stringWith("comments"));
    Track::debugPrintString(track.genre(), String::stringWith("genre"));
    Track::debugPrintString(track.grouping(), String::stringWith("grouping"));
    Track::debugPrintString(track.recordLabel(), String::stringWith("recordlabel"));
    Track::debugPrintString(track.remixer(), String::stringWith("remixer"));
    Track::debugPrintString(track.composer(), String::stringWith("composer"));
    Track::debugPrintString(track.key(), String::stringWith("key"));
    Track::debugPrintString(track.year(), String::stringWith("year"));
    Track::debugPrintString(track.length(), String::stringWith("length"));
    Track::debugPrintString(track.bitRate(), String::stringWith("bitrate"));
    Track::debugPrintString(track.sampleRate(), String::stringWith("samplerate"));
    Track::debugPrintString(track.bpm(), String::stringWith("bpm"));
    Track::debugPrintUint((uinteger32)track.trackNumber(), String::stringWith("tracknumber"));
    Track::debugPrintUint((uinteger32)track.discNumber(), String::stringWith("discnumber"));

    printf("Found %ld cue markers, %ld grid markers and %ld loop markers.\n",
           track.cueMarkers().length(),
           track.gridMarkers().length(),
           track.loopMarkers().length());
}
#endif

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

count Track::size(void) const
{
    return internal->uint32ForSubTagForIdentifier(trackSizeTagIdentifier);
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
    if (!internal->trackFilemarkersWereRead) {
        internal->readMarkers();
    }

    return internal->cueMarkers;
}

const LoopMarker::ArrayOfConst& Track::loopMarkers(void) const
{
    if (!internal->trackFilemarkersWereRead) {
        internal->readMarkers();
    }

    return internal->loopMarkers;
}

const GridMarker::ArrayOfConst& Track::gridMarkers(void) const
{
    if (!internal->trackFilemarkersWereRead) {
        internal->readMarkers();
    }

    return internal->gridMarkers;
}

void Track::setTitle(const String& title)
{
    internal->setStringForSubTagForIdentifier(title, trackTitleTagIdentifier);
    internal->needsToUpdateDatabaseFile = true;
}

void Track::setArtist(const String& artist)
{
    internal->setStringForSubTagForIdentifier(artist, trackArtistTagIdentifier);
    internal->needsToUpdateDatabaseFile = true;
}

void Track::setAlbum(const String& album)
{
    internal->setStringForSubTagForIdentifier(album, trackAlbumTagIdentifier);
    internal->needsToUpdateDatabaseFile = true;
}

void Track::setGenre(const String& genre)
{
    internal->setStringForSubTagForIdentifier(genre, trackGenreTagIdentifier);
    internal->needsToUpdateDatabaseFile = true;
}

void Track::setComments(const String& comments)
{
    internal->setStringForSubTagForIdentifier(comments, trackCommentsTagIdentifier);
    internal->needsToUpdateDatabaseFile = true;
}

void Track::setGrouping(const String& grouping)
{
    internal->setStringForSubTagForIdentifier(grouping, trackGroupingTagIdentifier);
    internal->needsToUpdateDatabaseFile = true;
}

void Track::setRemixer(const String& remixer)
{
    internal->setStringForSubTagForIdentifier(remixer, trackRemixerTagIdentifier);
    internal->needsToUpdateDatabaseFile = true;
}

void Track::setRecordLabel(const String& recordLabel)
{
    internal->setStringForSubTagForIdentifier(recordLabel, trackLabelTagIdentifier);
    internal->needsToUpdateDatabaseFile = true;
}

void Track::setComposer(const String& composer)
{
    internal->setStringForSubTagForIdentifier(composer, trackComposerTagIdentifier);
    internal->needsToUpdateDatabaseFile = true;
}

void Track::setKey(const String& key)
{
    internal->setStringForSubTagForIdentifier(key, trackKeyTagIdentifier);
    internal->needsToUpdateDatabaseFile = true;
}

void Track::setSizeInBytes(count size)
{
    internal->setUInt32ForSubTagForIdentifier(static_cast<uinteger32>(size), trackSizeTagIdentifier);

    decimal sizeInMb = static_cast<decimal>(size) / (1024.0f * 1024.0f);
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

    auto lengthString = String::stringWithFormat("%02d:%02d.%02d", minutes, seconds, milliseconds);
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
    internal->setStringForSubTagForIdentifier(bpm, trackBpmTagIdentifier);
    internal->needsToUpdateDatabaseFile = true;
}

void Track::setYear(const String& year)
{
    internal->setStringForSubTagForIdentifier(year, trackYearTagIdentifier);
    internal->needsToUpdateDatabaseFile = true;
}

void Track::setTrackNumber(count trackNumber)
{
    // TODO: Should make sure the cast doesn't chop off anything.
    internal->setUInt32ForSubTagForIdentifier(static_cast<uinteger32>(trackNumber), trackNumberTagIdentifier);
    internal->needsToUpdateDatabaseFile = true;
}

void Track::setDiscNumber(count discNumber)
{
    // TODO: Should make sure the cast doesn't chop off anything.
    internal->setUInt32ForSubTagForIdentifier(static_cast<uinteger32>(discNumber), trackDiscNumberTagIdentifier);
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
    auto dateAsString = Date::formattedStringWithTimestampAndFormat(dateAdded, "%d-%m-%Y");

    // TODO: Should make sure the cast doesn't chop off anything.
    internal->setUInt32ForSubTagForIdentifier(static_cast<uinteger32>(dateAdded), trackDateAddedTagIdentifier);
    internal->setStringForSubTagForIdentifier(dateAsString, trackDateAddedStringTagIdentifier);
    internal->needsToUpdateDatabaseFile = true;
}

void Track::setCueLoopAndGridMarkersWhichWereModifiedOn(CueMarker::ArrayOfConst& cueMarkers,
                                                        LoopMarker::ArrayOfConst& loopMarkers,
                                                        GridMarker::ArrayOfConst& gridMarkers,
                                                        timestamp modificationDateInSecondsSince1970)
{
    // -- In case we haven't read the markers and since we're overwriting them anyway, we mark them as read.
    internal->trackFilemarkersWereRead = true;

    this->setCueMarkersWhichWereModifiedOn(cueMarkers, modificationDateInSecondsSince1970);
    this->setLoopMarkersWhichWereModifiedOn(loopMarkers, modificationDateInSecondsSince1970);
    this->setGridMarkersWhichWereModifiedOn(gridMarkers, modificationDateInSecondsSince1970);
}

void Track::setCueMarkersWhichWereModifiedOn(CueMarker::ArrayOfConst& markers, timestamp modificationDateInSecondsSince1970)
{
    if (!internal->trackFilemarkersWereRead) {
        internal->readMarkers();
    }

    internal->cueMarkers = markers.pointer();

    if (internal->lastMarkersModificationDate < modificationDateInSecondsSince1970) {
        internal->lastMarkersModificationDate = modificationDateInSecondsSince1970;
    }
}

void Track::setLoopMarkersWhichWereModifiedOn(LoopMarker::ArrayOfConst& markers, timestamp modificationDateInSecondsSince1970)
{
    if (!internal->trackFilemarkersWereRead) {
        internal->readMarkers();
    }

    internal->loopMarkers = markers.pointer();

    if (internal->lastMarkersModificationDate < modificationDateInSecondsSince1970) {
        internal->lastMarkersModificationDate = modificationDateInSecondsSince1970;
    }
}

void Track::setGridMarkersWhichWereModifiedOn(GridMarker::ArrayOfConst& markers, timestamp modificationDateInSecondsSince1970)
{
    if (!internal->trackFilemarkersWereRead) {
        internal->readMarkers();
    }

    internal->gridMarkers = markers.pointer();

    if (internal->lastMarkersModificationDate < modificationDateInSecondsSince1970) {
        internal->lastMarkersModificationDate = modificationDateInSecondsSince1970;
    }
}

boolean Track::needsToUpdateTrackFile(void) const
{
    return internal->lastMarkersModificationDate != 0;
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
    NXA_ASSERT_TRUE(internal->lastMarkersModificationDate != 0);

    internal->saveToTrackFile();

    File::setModificationDateInSecondsSince1970ForFile(internal->lastMarkersModificationDate,
                                                       internal->trackFilePath());
}
