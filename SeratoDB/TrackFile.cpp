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

#include "SeratoDB/TrackFile.hpp"
#include "SeratoDB/Base64.hpp"

#include <taglib/tpropertymap.h>
#include <taglib/generalencapsulatedobjectframe.h>

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
    uint32_t tagSize = bigEndianUInt32ValueAt(parserPosition);
    parserPosition += 4 + tagSize;

    return parserPosition;
}

static CharVectorPtr p_markerV2TagDataFrom(const char* tagStart)
{
    const string tagName(tagStart);
    size_t sizeOfNameField = tagName.length() + 1;
    const size_t sizeOfSizeField = 4;

    const char* sizePosition = tagStart + sizeOfNameField;
    size_t tagSize = bigEndianUInt32ValueAt(sizePosition) + sizeOfNameField + sizeOfSizeField;

    CharVectorPtr data(make_unique<CharVector>(tagStart, tagStart + tagSize));
    return move(data);
}

#pragma mark Instance Methods

void TrackFile::p_readMarkersV2FromBase64Data(const char* markerV2Data, size_t sizeInBytes)
{
    if (!sizeInBytes) {
        return;
    }

    CharVectorPtr decodedData = Base64::decodeBlock(markerV2Data, sizeInBytes);

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
            this->p_cueMarkers->push_back(make_unique<CueMarker>(tagStart));
        }
        else if (tagName == "LOOP") {
            this->p_loopMarkers->push_back(make_unique<LoopMarker>(tagStart));
        }
        else {
            this->p_otherTags.push_back(p_markerV2TagDataFrom(tagStart));
        }

        tagStart = p_nextTagPositionAfterTagNamed(tagName, tagStart);
    }
}

void TrackFile::p_addGridMarker(GridMarkerPtr gridMarker)
{
    this->p_gridMarkers->push_back(move(gridMarker));
}

void TrackFile::p_readGridMarkersFrom(const char* gridMarkerData, size_t sizeInBytes)
{
    if (!sizeInBytes) {
        return;
    }

    uint32_t numberOfMarkers = bigEndianUInt32ValueAt(gridMarkerData);
    gridMarkerData += 4;

    for (uint32_t index = 0; index < numberOfMarkers; ++index) {
        this->p_addGridMarker(make_unique<GridMarker>(gridMarkerData));

        gridMarkerData = GridMarker::nextGridMarkerAfter(gridMarkerData);
    }
}

CharVectorPtr TrackFile::p_base64DataFromMarkersV2(void)
{
    CharVector decodedData;

    SeratoMarkerHeaderStruct markersHeader;
    markersHeader.majorVersion = 1;
    markersHeader.minorVersion = 1;

    CharVector headerData((char*)&markersHeader, (char*)&markersHeader.data);
    decodedData.insert(decodedData.end(), headerData.begin(), headerData.end());

    for (auto& marker : *this->p_cueMarkers) {
        marker->addId3TagTo(decodedData);
    }

    for (auto& marker : *this->p_loopMarkers) {
        marker->addId3TagTo(decodedData);
    }

    for (auto& tagData : this->p_otherTags) {
        decodedData.insert(decodedData.end(), tagData->begin(), tagData->end());
    }

    CharVectorPtr encodedData = Base64::encodeBlock(decodedData.data(), decodedData.size());
    return move(encodedData);
}

CharVectorPtr TrackFile::p_gridMarkerDataFromGridMarkers(void)
{
    CharVectorPtr data(make_unique<CharVector>());

    uint32_t numberOfMarkers;
    writeBigEndianUInt32ValueAt(this->p_gridMarkers->size(), &numberOfMarkers);

    CharVector numberOfMarkersData((char*)&numberOfMarkers, (char*)&numberOfMarkers + sizeof(numberOfMarkers));
    data->insert(data->end(), numberOfMarkersData.begin(), numberOfMarkersData.end());

    for (auto& marker : *(this->p_gridMarkers)) {
        marker->addDataTo(*data);
    }

    return move(data);
}

string TrackFile::title(void) const
{
    if (this->p_parsedFileTag) {
        String text = this->p_parsedFileTag->title();
        if (text != String::null) {
            return text.to8Bit();
        }
    }

    return emptyString;
}

string TrackFile::artist(void) const
{
    if (this->p_parsedFileTag) {
        String text = this->p_parsedFileTag->artist();
        if (text != String::null) {
            return text.to8Bit();
        }
    }

    return emptyString;
}

string TrackFile::genre(void) const
{
    if (this->p_parsedFileTag) {
        String text = this->p_parsedFileTag->genre();
        if (text != String::null) {
            return text.to8Bit();
        }
    }

    return emptyString;
}

string TrackFile::comments(void) const
{
    if (this->p_parsedFileTag) {
        String text = this->p_parsedFileTag->comment();
        if (text != String::null) {
            return text.to8Bit();
        }
    }

    return emptyString;
}

string TrackFile::album(void) const
{
    if (this->p_parsedFileTag) {
        String text = this->p_parsedFileTag->album();
        if (text != String::null) {
            return text.to8Bit();
        }
    }

    return emptyString;
}

string TrackFile::composer(void) const
{
    String text = this->p_properties["COMPOSER"].toString();
    if (text != String::null) {
        return text.to8Bit();
    }

    return emptyString;
}

string TrackFile::bpm(void) const
{
    String text = this->p_properties["BPM"].toString();
    if (text != String::null) {
        return text.to8Bit();
    }

    return emptyString;
}

size_t TrackFile::sizeInBytes(void) const
{
    return sizeOfFileAt(this->p_trackFilePath->c_str());
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

const CueMarkerVector& TrackFile::cueMarkers(void) const
{
    return *(this->p_cueMarkers);
}

const LoopMarkerVector& TrackFile::loopMarkers(void) const
{
    return *(this->p_loopMarkers);
}

const GridMarkerVector& TrackFile::gridMarkers(void) const
{
    return *(this->p_gridMarkers);
}

void TrackFile::setTitle(const char* title)
{
    if (this->p_parsedFileTag) {
        this->p_parsedFileTag->setTitle(String(title));
    }
}

void TrackFile::setArtist(const char* artist)
{
    if (this->p_parsedFileTag) {
        this->p_parsedFileTag->setArtist(String(artist));
    }
}

void TrackFile::setGenre(const char* genre)
{
    if (this->p_parsedFileTag) {
        this->p_parsedFileTag->setGenre(String(genre));
    }
}

void TrackFile::setComments(const char* comments)
{
    if (this->p_parsedFileTag) {
        this->p_parsedFileTag->setComment(String(comments));
    }
}

void TrackFile::setAlbum(const char* album)
{
    if (this->p_parsedFileTag) {
        this->p_parsedFileTag->setAlbum(String(album));
    }
}

void TrackFile::setComposer(const char* composer)
{
    this->p_properties["COMPOSER"] = String(composer);
}

void TrackFile::setBpm(const char* bpm)
{
    this->p_properties["BPM"] = String(bpm);
}

void TrackFile::setTrackNumber(const uint32_t& trackNumber)
{
    if (this->p_parsedFileTag) {
        this->p_parsedFileTag->setTrack(trackNumber);
    }
}

void TrackFile::setCueMarkers(CueMarkerVectorPtr markers)
{
    this->p_cueMarkers = move(markers);
}

void TrackFile::setLoopMarkers(LoopMarkerVectorPtr markers)
{
    this->p_loopMarkers = move(markers);
}

void TrackFile::setGridMarkers(GridMarkerVectorPtr markers)
{
    this->p_gridMarkers = move(markers);
}

void TrackFile::saveChanges(void)
{
    this->p_writeMarkers();

    this->p_file->save();
}
