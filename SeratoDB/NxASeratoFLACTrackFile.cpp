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

#include "SeratoDB/NxASeratoFLACTrackFile.h"
#include "SeratoDB/NxASeratoBase64.h"

#include <taglib/flacfile.h>

using namespace NxA;
using namespace TagLib;
using namespace std;

#pragma mark Constants

static const string emptyString("");

#pragma mark Structures

typedef struct {
    unsigned char mimeType[25];
    unsigned char filename[1];
    unsigned char description[16];
    unsigned char majorVersion;
    unsigned char minorVersion;
    unsigned char data[0];
} SeratoFLACMarkersHeaderStruct;

#pragma mark Constructors

SeratoFLACTrackFile::SeratoFLACTrackFile(const char* trackFilePath) : SeratoTrackFile(trackFilePath)
{
    TaglibFilePtr file = make_unique<FLAC::File>(trackFilePath);
    if (!file->isValid()) {
        this->p_file = TaglibFilePtr();
        this->p_parsedFileTag = NULL;
        this->p_audioProperties = NULL;
        return;
    }

    this->p_parsedFileTag = file->tag();
    this->p_audioProperties = file->audioProperties();
    this->p_properties = file->properties();
    this->p_file = move(file);

    this->p_readMarkers();
}

#pragma mark Instance Methods

void SeratoFLACTrackFile::p_readMarkers(void)
{
    const String markersEncodedData = this->p_properties["SERATO_MARKERS_V2"].toString();
    uint32_t encodedDataSize = markersEncodedData.size();
    if (encodedDataSize) {
        CharVectorPtr decodedMarkersData = SeratoBase64::decodeBlock((const char*)markersEncodedData.data(String::UTF8).data(),
                                                                     encodedDataSize);

        const SeratoFLACMarkersHeaderStruct* headerStruct = (const SeratoFLACMarkersHeaderStruct*)decodedMarkersData->data();
        this->p_readMarkersV2FromBase64Data((const char*)headerStruct->data, decodedMarkersData->size() - sizeof(SeratoFLACMarkersHeaderStruct));
    }

    const String beatGridEncodedData = this->p_properties["SERATO_BEATGRID"].toString();
    uint32_t encodedBeatGridDataSize = beatGridEncodedData.size();
    if (encodedBeatGridDataSize) {
        CharVectorPtr decodedGridMarkersData = SeratoBase64::decodeBlock((const char*)beatGridEncodedData.data(String::UTF8).data(),
                                                                         encodedBeatGridDataSize);
        const SeratoFLACMarkersHeaderStruct* headerStruct = (const SeratoFLACMarkersHeaderStruct*)decodedGridMarkersData->data();
        if ((headerStruct->majorVersion == 1) && (headerStruct->minorVersion == 0)) {
            this->p_readGridMarkersFrom((const char*)headerStruct->data, decodedGridMarkersData->size() - sizeof(SeratoFLACMarkersHeaderStruct));
        }
    }
}

void SeratoFLACTrackFile::p_writeMarkers(void)
{
    if (this->cueMarkers().size() || this->loopMarkers().size()) {
        CharVector decodedData;

        SeratoFLACMarkersHeaderStruct header;
        memcpy(header.mimeType, "application/octet-stream", 25);
        header.filename[0] = 0;
        memcpy(header.description, "Serato Markers2", 16);
        header.majorVersion = 1;
        header.minorVersion = 1;

        CharVector headerData((char*)&header, (char*)&header.data);
        decodedData.insert(decodedData.end(), headerData.begin(), headerData.end());

        CharVectorPtr base64Data(this->p_base64DataFromMarkersV2());
        decodedData.insert(decodedData.end(), base64Data->begin(), base64Data->end());

        CharVectorPtr encodedData = SeratoBase64::encodeBlock(decodedData.data(), decodedData.size());
        encodedData->push_back('\0');

        StringList newList;
        newList.append(String(encodedData->data()));
        this->p_properties["SERATO_MARKERS_V2"] = newList;
    }
    else {
        this->p_properties.erase("SERATO_MARKERS_V2");
    }

    if (this->gridMarkers().size()) {
        CharVector decodedData;

        SeratoFLACMarkersHeaderStruct header;
        memcpy(header.mimeType, "application/octet-stream", 25);
        header.filename[0] = 0;
        memcpy(header.description, "Serato Beatgrid", 16);
        header.majorVersion = 1;
        header.minorVersion = 0;

        CharVector headerData((char*)&header, (char*)&header.data);
        decodedData.insert(decodedData.end(), headerData.begin(), headerData.end());

        CharVectorPtr base64Data(this->p_gridMarkerDataFromGridMarkers());
        decodedData.insert(decodedData.end(), base64Data->begin(), base64Data->end());

        CharVectorPtr encodedData = SeratoBase64::encodeBlock(decodedData.data(), decodedData.size());
        encodedData->push_back('\0');

        StringList newList;
        newList.append(String(encodedData->data()));
        this->p_properties["SERATO_BEATGRID"] = newList;
    }
    else {
        this->p_properties.erase("SERATO_BEATGRID");
    }
}

bool SeratoFLACTrackFile::hasKey(void) const
{
    return true;
}

string SeratoFLACTrackFile::key(void) const
{
    String text = this->p_properties["INITIALKEY"].toString();
    if (text != String::null) {
        return text.to8Bit();
    }

    return emptyString;
}

string SeratoFLACTrackFile::grouping(void) const
{
    String text = this->p_properties["GROUPING"].toString();
    if (text != String::null) {
        return text.to8Bit();
    }

    return emptyString;
}

uint32_t SeratoFLACTrackFile::lengthInMilliseconds(void) const
{
    if (this->p_audioProperties) {
        const FLAC::Properties* audioProperties = (const FLAC::Properties*)this->p_audioProperties;

        uint32_t numberOfFrames = audioProperties->sampleFrames();
        uint32_t sampleRate = audioProperties->sampleRate();
        uint32_t numberOfMilliseconds = ((double)numberOfFrames * 1000.0) / (double)sampleRate;

        return numberOfMilliseconds;
    }

    return 0;
}

uint32_t SeratoFLACTrackFile::bitDepthInBitsOrZeroIfNotApplicable(void) const
{
    if (this->p_audioProperties) {
        const FLAC::Properties* audioProperties = (const FLAC::Properties*)this->p_audioProperties;

        return audioProperties->sampleWidth();
    }

    return 0;
}

bool SeratoFLACTrackFile::hasRecordLabel(void) const
{
    return false;
}

string SeratoFLACTrackFile::recordLabel(void) const
{
    return emptyString;
}

bool SeratoFLACTrackFile::hasRemixer(void) const
{
    return false;
}

string SeratoFLACTrackFile::remixer(void) const
{
    return emptyString;
}

string SeratoFLACTrackFile::yearReleased(void) const
{
    String text = this->p_properties["DATE"].toString();
    if (text != String::null) {
        return text.to8Bit();
    }

    return emptyString;
}

CharVectorPtr SeratoFLACTrackFile::artwork(void) const
{
    // -- TODO: To be implemented.
    return CharVectorPtr();
}

void SeratoFLACTrackFile::setKey(const char* key)
{
    this->p_properties["INITIALKEY"] = String(key);
}

void SeratoFLACTrackFile::setGrouping(const char* grouping)
{
    this->p_properties["GROUPING"] = String(grouping);
}

void SeratoFLACTrackFile::setRecordLabel(const char* recordLabel)
{
    // -- This is not supported by FLAC files.
}

void SeratoFLACTrackFile::setRemixer(const char* remixer)
{
    // -- This is not supported by FLAC files.
}

void SeratoFLACTrackFile::setYearReleased(const char* year)
{
    this->p_properties["DATE"] = String(year);
}

void SeratoFLACTrackFile::setArtwork(CharVectorPtr artwork)
{
    // -- TODO: To be implemented.
}
