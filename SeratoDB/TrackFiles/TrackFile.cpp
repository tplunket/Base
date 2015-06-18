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
#include "Base64.hpp"

#include <Base/File.hpp>
#include <Base/Platform.hpp>

#include <taglib/tpropertymap.h>
#include <taglib/generalencapsulatedobjectframe.h>

using namespace NxA;
using namespace NxA::Serato;
using namespace TagLib;
using namespace std;

#pragma mark Constants

static const string emptyString("");

#pragma mark Structures

typedef struct {
    unsigned char majorVersion;
    unsigned char minorVersion;
    unsigned char data[0];
} SeratoMarkerHeaderStruct;

#pragma mark Utility Functions

static const char* p_nextTagPositionAfterTagNamed(const string tagName, const char* currentTagPosition)
{
    const char* parserPosition = currentTagPosition;

    parserPosition += tagName.length() + 1;
    uint32_t tagSize = Platform::bigEndianUInt32ValueAt(parserPosition);
    parserPosition += 4 + tagSize;

    return parserPosition;
}

static Blob::Pointer p_markerV2TagDataFrom(const char* tagStart)
{
    const string tagName(tagStart);
    size_t sizeOfNameField = tagName.length() + 1;
    const size_t sizeOfSizeField = 4;

    const char* sizePosition = tagStart + sizeOfNameField;
    size_t tagSize = Platform::bigEndianUInt32ValueAt(sizePosition) + sizeOfNameField + sizeOfSizeField;

    return Blob::blobWithMemoryAndSize(tagStart, tagSize);
}

#pragma mark Instance Methods

void TrackFile::p_readMarkersV2FromBase64Data(const char* markerV2Data, size_t totalSize)
{
    if (!totalSize) {
        return;
    }

    auto decodedData = Base64::decodeBlock(markerV2Data, totalSize);

    const SeratoMarkerHeaderStruct* markerStruct = (const SeratoMarkerHeaderStruct*)(decodedData->data());
    if ((markerStruct->majorVersion != 1) || (markerStruct->minorVersion != 1)) {
        return;
    }

    // -- FIXME: The data written by Serato sometimes contains an extra byte at the end.
    // -- This might be a bug in the decode method but since marker data is always more
    // -- than one byte long, we substract one here to make sure we don't go over the end.
    const char* markerDataEnd = (const char*)markerStruct + decodedData->size() - 1;
    const char* tagStart = (const char*)markerStruct->data;

    while (tagStart < markerDataEnd) {
        string tagName(tagStart);

        if (tagName == "CUE") {
            this->p_cueMarkers->append(CueMarker::cueMarkerWith(tagStart));
        }
        else if (tagName == "LOOP") {
            this->p_loopMarkers->append(LoopMarker::loopMarkerWith(tagStart));
        }
        else {
            this->p_otherTags->append(p_markerV2TagDataFrom(tagStart));
        }

        tagStart = p_nextTagPositionAfterTagNamed(tagName, tagStart);
    }
}

void TrackFile::p_addGridMarker(GridMarker::ConstPointer const& gridMarker)
{
    this->p_gridMarkers->append(GridMarker::gridMarkerWith(gridMarker));
}

void TrackFile::p_readGridMarkersFrom(const char* gridMarkerData, size_t totalSize)
{
    if (!totalSize) {
        return;
    }

    uint32_t numberOfMarkers = Platform::bigEndianUInt32ValueAt(gridMarkerData);
    gridMarkerData += 4;

    for (uint32_t index = 0; index < numberOfMarkers; ++index) {
        this->p_addGridMarker(GridMarker::gridMarkerWith(gridMarkerData));

        gridMarkerData = GridMarker::nextGridMarkerAfter(gridMarkerData);
    }
}

Blob::Pointer TrackFile::p_base64DataFromMarkersV2(void)
{
    auto decodedData = Blob::blob();

    SeratoMarkerHeaderStruct markersHeader;
    markersHeader.majorVersion = 1;
    markersHeader.minorVersion = 1;

    auto headerData = Blob::blobWithMemoryAndSize(reinterpret_cast<character*>(&markersHeader), sizeof(SeratoMarkerHeaderStruct));
    decodedData->append(headerData);

    for (auto& marker : *this->p_cueMarkers) {
        marker->addId3TagTo(decodedData);
    }

    for (auto& marker : *this->p_loopMarkers) {
        marker->addId3TagTo(decodedData);
    }

    for (auto& tagData : *this->p_otherTags) {
        decodedData->append(tagData);
    }

    auto encodedData = Base64::encodeBlock(decodedData->data(), decodedData->size());
    return encodedData;
}

Blob::Pointer TrackFile::p_gridMarkerDataFromGridMarkers(void)
{
    auto data = Blob::blob();

    uinteger32 numberOfMarkers;
    Platform::writeBigEndianUInt32ValueAt(this->p_gridMarkers->length(), &numberOfMarkers);

    auto numberOfMarkersData = Blob::blobWithMemoryAndSize(reinterpret_cast<character*>(&numberOfMarkers), sizeof(numberOfMarkers));
    data->append(numberOfMarkersData);

    for (auto& marker : *(this->p_gridMarkers)) {
        marker->addDataTo(data);
    }

    return move(data);
}

string TrackFile::title(void) const
{
    if (this->p_parsedFileTag) {
        auto text = this->p_parsedFileTag->title();
        if (text != TagLib::String::null) {
            return text.to8Bit();
        }
    }

    return emptyString;
}

string TrackFile::artist(void) const
{
    if (this->p_parsedFileTag) {
        auto text = this->p_parsedFileTag->artist();
        if (text != TagLib::String::null) {
            return text.to8Bit();
        }
    }

    return emptyString;
}

string TrackFile::genre(void) const
{
    if (this->p_parsedFileTag) {
        auto text = this->p_parsedFileTag->genre();
        if (text != TagLib::String::null) {
            return text.to8Bit();
        }
    }

    return emptyString;
}

string TrackFile::comments(void) const
{
    if (this->p_parsedFileTag) {
        auto text = this->p_parsedFileTag->comment();
        if (text != TagLib::String::null) {
            return text.to8Bit();
        }
    }

    return emptyString;
}

string TrackFile::album(void) const
{
    if (this->p_parsedFileTag) {
        auto text = this->p_parsedFileTag->album();
        if (text != TagLib::String::null) {
            return text.to8Bit();
        }
    }

    return emptyString;
}

string TrackFile::composer(void) const
{
    auto text = this->p_properties["COMPOSER"].toString();
    if (text != TagLib::String::null) {
        return text.to8Bit();
    }

    return emptyString;
}

string TrackFile::bpm(void) const
{
    auto text = this->p_properties["BPM"].toString();
    if (text != TagLib::String::null) {
        return text.to8Bit();
    }

    return emptyString;
}

size_t TrackFile::size(void) const
{
    return File::sizeOfFileAt(String::stringWith(this->p_trackFilePath->c_str()));
}

uint32_t TrackFile::lengthInMilliseconds(void) const
{
    if (this->p_audioProperties) {
        return this->p_audioProperties->length() * 1000;
    }

    return 0;
}

uint32_t TrackFile::bitRateInKiloBitsPerSecond(void) const
{
    if (this->p_audioProperties) {
        return this->p_audioProperties->bitrate();
    }

    return 0;
}

uint32_t TrackFile::bitDepthInBitsOrZeroIfNotApplicable(void) const
{
    return 0;
}

uint32_t TrackFile::sampleRateInSamplesPerSecond(void) const
{
    if (this->p_audioProperties) {
        return this->p_audioProperties->sampleRate();
    }

    return 0;
}

uint32_t TrackFile::trackNumber(void) const
{
    if (this->p_parsedFileTag) {
        return this->p_parsedFileTag->track();
    }

    return 0;
}

CueMarker::Array::ConstPointer const& TrackFile::cueMarkers(void) const
{
    return this->p_cueMarkers;
}

LoopMarker::Array::ConstPointer const& TrackFile::loopMarkers(void) const
{
    return this->p_loopMarkers;
}

GridMarker::Array::ConstPointer const& TrackFile::gridMarkers(void) const
{
    return this->p_gridMarkers;
}

void TrackFile::setTitle(const char* title)
{
    if (this->p_parsedFileTag) {
        this->p_parsedFileTag->setTitle(TagLib::String(title));
    }
}

void TrackFile::setArtist(const char* artist)
{
    if (this->p_parsedFileTag) {
        this->p_parsedFileTag->setArtist(TagLib::String(artist));
    }
}

void TrackFile::setGenre(const char* genre)
{
    if (this->p_parsedFileTag) {
        this->p_parsedFileTag->setGenre(TagLib::String(genre));
    }
}

void TrackFile::setComments(const char* comments)
{
    if (this->p_parsedFileTag) {
        this->p_parsedFileTag->setComment(TagLib::String(comments));
    }
}

void TrackFile::setAlbum(const char* album)
{
    if (this->p_parsedFileTag) {
        this->p_parsedFileTag->setAlbum(TagLib::String(album));
    }
}

void TrackFile::setComposer(const char* composer)
{
    this->p_properties["COMPOSER"] = TagLib::String(composer);
}

void TrackFile::setBpm(const char* bpm)
{
    this->p_properties["BPM"] = TagLib::String(bpm);
}

void TrackFile::setTrackNumber(const uint32_t& trackNumber)
{
    if (this->p_parsedFileTag) {
        this->p_parsedFileTag->setTrack(trackNumber);
    }
}

void TrackFile::setCueMarkers(CueMarker::Array::Pointer const& markers)
{
    this->p_cueMarkers = markers;
}

void TrackFile::setLoopMarkers(LoopMarker::Array::Pointer const& markers)
{
    this->p_loopMarkers = markers;
}

void TrackFile::setGridMarkers(GridMarker::Array::Pointer const& markers)
{
    this->p_gridMarkers = markers;
}

void TrackFile::saveChanges(void)
{
    this->p_writeMarkers();

    this->p_file->save();
}
