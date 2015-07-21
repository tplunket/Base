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

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_PURE_VIRTUAL_CLASS_WITH_PARENT(NxA::Serato, TrackFile, Object);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Instance Methods

String::Pointer TrackFile::title(void) const
{
    auto text = internal->parsedFileTag->title();
    if (text != TagLib::String::null) {
        return String::stringWith(text.toCString());
    }

    return String::string();
}

String::Pointer TrackFile::artist(void) const
{
    auto text = internal->parsedFileTag->artist();
    if (text != TagLib::String::null) {
        return String::stringWith(text.toCString());
    }

    return String::string();
}

String::Pointer TrackFile::genre(void) const
{
    auto text = internal->parsedFileTag->genre();
    if (text != TagLib::String::null) {
        return String::stringWith(text.toCString());
    }

    return String::string();
}

String::Pointer TrackFile::comments(void) const
{
    auto text = internal->parsedFileTag->comment();
    if (text != TagLib::String::null) {
        return String::stringWith(text.toCString());
    }

    return String::string();
}

String::Pointer TrackFile::album(void) const
{
    auto text = internal->parsedFileTag->album();
    if (text != TagLib::String::null) {
        return String::stringWith(text.toCString());
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
    return File::sizeOfFileAt(internal->trackFilePath);
}

uinteger32 TrackFile::lengthInMilliseconds(void) const
{
    return internal->audioProperties->length() * 1000;
}

uinteger32 TrackFile::bitRateInKiloBitsPerSecond(void) const
{
    return internal->audioProperties->bitrate();
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
    return internal->audioProperties->sampleRate();
}

count TrackFile::trackNumber(void) const
{
    return internal->parsedFileTag->track();
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
    internal->parsedFileTag->setTitle(TagLib::String(title.toUTF8()));
}

void TrackFile::setArtist(const String& artist)
{
    internal->parsedFileTag->setArtist(TagLib::String(artist.toUTF8()));
}

void TrackFile::setGenre(const String& genre)
{
    internal->parsedFileTag->setGenre(TagLib::String(genre.toUTF8()));
}

void TrackFile::setComments(const String& comments)
{
    internal->parsedFileTag->setComment(TagLib::String(comments.toUTF8()));
}

void TrackFile::setAlbum(const String& album)
{
    internal->parsedFileTag->setAlbum(TagLib::String(album.toUTF8()));
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
    internal->parsedFileTag->setTrack(trackNumber);
}

void TrackFile::setCueMarkers(CueMarker::Array& markers)
{
    internal->cueMarkers = markers.pointer();
}

void TrackFile::setLoopMarkers(LoopMarker::Array& markers)
{
    internal->loopMarkers = markers.pointer();
}

void TrackFile::setGridMarkers(GridMarker::Array& markers)
{
    internal->gridMarkers = markers.pointer();
}

void TrackFile::saveChanges(void)
{
    internal->writeMarkers();
    internal->file->setProperties(internal->properties);
    internal->file->save();
}
