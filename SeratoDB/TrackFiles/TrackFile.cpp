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

#include "TrackFiles/TrackFile.hpp"
#include "TrackFiles/Internal/TrackFile.hpp"

#include <tpropertymap.h>
#include <generalencapsulatedobjectframe.h>
#include <audioproperties.h>

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_PURE_VIRTUAL_CLASS_WITH_PARENT(NxA::Serato, TrackFile, Object);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Instance Methods

String::Pointer TrackFile::title(void) const
{
    return String::stringWith(internal->tag->title().toCString());
}

String::Pointer TrackFile::artist(void) const
{
    return String::stringWith(internal->tag->artist().toCString());
}

String::Pointer TrackFile::genre(void) const
{
    return String::stringWith(internal->tag->genre().toCString());
}

String::Pointer TrackFile::comments(void) const
{
    return String::stringWith(internal->tag->comment().toCString());
}

String::Pointer TrackFile::album(void) const
{
    return String::stringWith(internal->tag->album().toCString());
}

count TrackFile::trackNumber(void) const
{
    return internal->tag->track();
}

count TrackFile::size(void) const
{
    return File::sizeOfFileAt(internal->trackFilePath);
}

uinteger32 TrackFile::lengthInMilliseconds(void) const
{
    auto audioProperties = internal->file->audioProperties();
    NXA_ASSERT_NOT_NULL(audioProperties);

    return audioProperties->length() * 1000;
}

uinteger32 TrackFile::bitRateInKiloBitsPerSecond(void) const
{
    auto audioProperties = internal->file->audioProperties();
    NXA_ASSERT_NOT_NULL(audioProperties);

    return audioProperties->bitrate();
}

boolean TrackFile::hasBitDepth(void) const
{
    return false;
}

uinteger32 TrackFile::bitDepthInBits(void) const
{
    return 0;
}

uinteger32 TrackFile::sampleRateInSamplesPerSecond(void) const
{
    TagLib::AudioProperties* audioProperties = internal->file->audioProperties();
    NXA_ASSERT_NOT_NULL(audioProperties);

    return audioProperties->sampleRate();
}

String::Pointer TrackFile::yearReleased(void) const
{
    return String::stringWithFormat("%04d", internal->file->tag()->year());
}

const CueMarker::Array& TrackFile::cueMarkers(void) const
{
    return internal->cueMarkers;
}

const LoopMarker::Array& TrackFile::loopMarkers(void) const
{
    return internal->loopMarkers;
}

const GridMarker::Array& TrackFile::gridMarkers(void) const
{
    return internal->gridMarkers;
}

void TrackFile::setTitle(const String& title)
{
    internal->tag->setTitle(TagLib::String(title.toUTF8()));
}

void TrackFile::setArtist(const String& artist)
{
    internal->tag->setArtist(TagLib::String(artist.toUTF8()));
}

void TrackFile::setGenre(const String& genre)
{
    internal->tag->setGenre(TagLib::String(genre.toUTF8()));
}

void TrackFile::setComments(const String& comments)
{
    internal->tag->setComment(TagLib::String(comments.toUTF8()));
}

void TrackFile::setAlbum(const String& album)
{
    internal->tag->setAlbum(TagLib::String(album.toUTF8()));
}

void TrackFile::setTrackNumber(count trackNumber)
{
    internal->tag->setTrack(trackNumber);
}

void TrackFile::setYearReleased(const String& year)
{
    internal->tag->setYear(::atoi(year.toUTF8()));
}

void TrackFile::setCueMarkers(CueMarker::Array& markers)
{
    NXA_ASSERT_FALSE(internal->markersWereIgnored);

    internal->cueMarkers = markers.pointer();
}

void TrackFile::setLoopMarkers(LoopMarker::Array& markers)
{
    NXA_ASSERT_FALSE(internal->markersWereIgnored);

    internal->loopMarkers = markers.pointer();
}

void TrackFile::setGridMarkers(GridMarker::Array& markers)
{
    NXA_ASSERT_FALSE(internal->markersWereIgnored);
    
    internal->gridMarkers = markers.pointer();
}

void TrackFile::saveChanges(void)
{
    if (!internal->markersWereIgnored) {
        internal->writeMarkers();
    }

    internal->file->save();
}
