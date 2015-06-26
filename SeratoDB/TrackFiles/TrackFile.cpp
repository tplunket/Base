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

NXA_GENERATED_PURE_VIRTUAL_IMPLEMENTATION_FOR(NxA::Serato, TrackFile);
NXA_GENERATED_CONSTRUCTORS_FOR(NxA::Serato, TrackFile, Object);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Instance Methods

String::Pointer TrackFile::title(void) const
{
    if (internal->parsedFileTag) {
        auto text = internal->parsedFileTag->title();
        if (text != TagLib::String::null) {
            return String::stringWith(text.toCString());
        }
    }

    return String::string();
}

String::Pointer TrackFile::artist(void) const
{
    if (internal->parsedFileTag) {
        auto text = internal->parsedFileTag->artist();
        if (text != TagLib::String::null) {
            return String::stringWith(text.toCString());
        }
    }

    return String::string();
}

String::Pointer TrackFile::genre(void) const
{
    if (internal->parsedFileTag) {
        auto text = internal->parsedFileTag->genre();
        if (text != TagLib::String::null) {
            return String::stringWith(text.toCString());
        }
    }

    return String::string();
}

String::Pointer TrackFile::comments(void) const
{
    if (internal->parsedFileTag) {
        auto text = internal->parsedFileTag->comment();
        if (text != TagLib::String::null) {
            return String::stringWith(text.toCString());
        }
    }

    return String::string();
}

String::Pointer TrackFile::album(void) const
{
    if (internal->parsedFileTag) {
        auto text = internal->parsedFileTag->album();
        if (text != TagLib::String::null) {
            return String::stringWith(text.toCString());
        }
    }

    return String::string();
}

String::Pointer TrackFile::composer(void) const
{
    auto text = internal->properties["COMPOSER"].toString();
    if (text != TagLib::String::null) {
        return String::stringWith(text.toCString());
    }

    return String::string();
}

String::Pointer TrackFile::bpm(void) const
{
    auto text = internal->properties["BPM"].toString();
    if (text != TagLib::String::null) {
        return String::stringWith(text.toCString());
    }

    return String::string();
}

count TrackFile::size(void) const
{
    return File::sizeOfFileAt(String::stringWith(internal->trackFilePath->toUTF8()));
}

uinteger32 TrackFile::lengthInMilliseconds(void) const
{
    if (internal->audioProperties) {
        return internal->audioProperties->length() * 1000;
    }

    return 0;
}

uinteger32 TrackFile::bitRateInKiloBitsPerSecond(void) const
{
    if (internal->audioProperties) {
        return internal->audioProperties->bitrate();
    }

    return 0;
}

uinteger32 TrackFile::bitDepthInBitsOrZeroIfNotApplicable(void) const
{
    return 0;
}

uinteger32 TrackFile::sampleRateInSamplesPerSecond(void) const
{
    if (internal->audioProperties) {
        return internal->audioProperties->sampleRate();
    }

    return 0;
}

count TrackFile::trackNumber(void) const
{
    if (internal->parsedFileTag) {
        return internal->parsedFileTag->track();
    }

    return 0;
}

CueMarker::Array::Pointer const& TrackFile::cueMarkers(void) const
{
    return internal->cueMarkers;
}

LoopMarker::Array::Pointer const& TrackFile::loopMarkers(void) const
{
    return internal->loopMarkers;
}

GridMarker::Array::Pointer const& TrackFile::gridMarkers(void) const
{
    return internal->gridMarkers;
}

void TrackFile::setTitle(const String& title)
{
    if (internal->parsedFileTag) {
        internal->parsedFileTag->setTitle(TagLib::String(title.toUTF8()));
    }
}

void TrackFile::setArtist(const String& artist)
{
    if (internal->parsedFileTag) {
        internal->parsedFileTag->setArtist(TagLib::String(artist.toUTF8()));
    }
}

void TrackFile::setGenre(const String& genre)
{
    if (internal->parsedFileTag) {
        internal->parsedFileTag->setGenre(TagLib::String(genre.toUTF8()));
    }
}

void TrackFile::setComments(const String& comments)
{
    if (internal->parsedFileTag) {
        internal->parsedFileTag->setComment(TagLib::String(comments.toUTF8()));
    }
}

void TrackFile::setAlbum(const String& album)
{
    if (internal->parsedFileTag) {
        internal->parsedFileTag->setAlbum(TagLib::String(album.toUTF8()));
    }
}

void TrackFile::setComposer(const String& composer)
{
    internal->properties["COMPOSER"] = TagLib::String(composer.toUTF8());
}

void TrackFile::setBpm(const String& bpm)
{
    internal->properties["BPM"] = TagLib::String(bpm.toUTF8());
}

void TrackFile::setTrackNumber(count trackNumber)
{
    if (internal->parsedFileTag) {
        internal->parsedFileTag->setTrack(trackNumber);
    }
}

void TrackFile::setCueMarkers(CueMarker::Array::Pointer const& markers)
{
    internal->cueMarkers = markers;
}

void TrackFile::setLoopMarkers(LoopMarker::Array::Pointer const& markers)
{
    internal->loopMarkers = markers;
}

void TrackFile::setGridMarkers(GridMarker::Array::Pointer const& markers)
{
    internal->gridMarkers = markers;
}

void TrackFile::saveChanges(void)
{
    internal->writeMarkers();
    internal->file->save();
}
