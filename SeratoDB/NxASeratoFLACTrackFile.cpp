//
//  NxASeratoFLACTrackFile.cpp
//  SeratoDB
//
//  Created by Didier Malenfant on 2/13/15.
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

using namespace std;
using namespace NxA;
using namespace TagLib;

#pragma mark Structures

typedef struct {
    unsigned char mimeType[25];
    unsigned char filename[1];
    unsigned char description[16];
    unsigned char majorVersion;
    unsigned char minorVersion;
    unsigned char data[0];
} SeratoFLACMarkerHeaderStruct;

#pragma mark Constructors

SeratoFLACTrackFile::SeratoFLACTrackFile(const char* trackFilePath) : SeratoTrackFile(trackFilePath)
{
    const FLAC::File* file = new FLAC::File(trackFilePath);
    if (!file->isValid()) {
        this->p_file = TaglibFileAutoPtr(NULL);
        this->p_parsedFileTag = NULL;
        this->p_audioProperties = NULL;
        return;
    }

    this->p_file = TaglibFileAutoPtr((TagLib::File*)file);
    this->p_parsedFileTag = file->tag();
    this->p_audioProperties = file->audioProperties();
    this->p_properties = file->properties();

    this->p_readMarkersV2();
}

#pragma mark Instance Methods

void SeratoFLACTrackFile::p_readMarkersV2(void)
{
    const String markersEncodedData = this->p_properties["SERATO_MARKERS_V2"].toString();
    uint32_t encodedDataSize = markersEncodedData.size();
    if (!encodedDataSize) {
        return;
    }

    CharVectorAutoPtr decodedData = SeratoBase64::decodeBlock((const char*)markersEncodedData.data(String::UTF8).data(),
                                                                      encodedDataSize);

    const SeratoFLACMarkerHeaderStruct* headerStruct = (const SeratoFLACMarkerHeaderStruct*)decodedData->data();
    this->p_readMarkersV2FromBase64Data((const char*)headerStruct->data, decodedData->size() - sizeof(SeratoFLACMarkerHeaderStruct));
}

bool SeratoFLACTrackFile::hasKey(void) const
{
    return true;
}

StringAutoPtr SeratoFLACTrackFile::key(void) const
{
    string* result = NULL;

    String text = this->p_properties["INITIALKEY"].toString();
    if (text != String::null) {
        result = new string(text.to8Bit());
    }

    return StringAutoPtr(result);
}

StringAutoPtr SeratoFLACTrackFile::grouping(void) const
{
    string* result = NULL;

    String text = this->p_properties["GROUPING"].toString();
    if (text != String::null) {
        result = new string(text.to8Bit());
    }

    return StringAutoPtr(result);
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

StringAutoPtr SeratoFLACTrackFile::recordLabel(void) const
{
    return StringAutoPtr(NULL);
}

bool SeratoFLACTrackFile::hasRemixer(void) const
{
    return false;
}

StringAutoPtr SeratoFLACTrackFile::remixer(void) const
{
    return StringAutoPtr(NULL);
}

bool SeratoFLACTrackFile::hasDateReleased(void) const
{
    return true;
}

StringAutoPtr SeratoFLACTrackFile::dateReleased(void) const
{
    string* result = NULL;

    String text = this->p_properties["DATE"].toString();
    if (text != String::null) {
        result = new string(text.to8Bit());
    }

    return StringAutoPtr(result);
}

bool SeratoFLACTrackFile::hasYearReleased(void) const
{
    return false;
}

StringAutoPtr SeratoFLACTrackFile::yearReleased(void) const
{
    return StringAutoPtr(NULL);
}

CharVectorAutoPtr SeratoFLACTrackFile::artwork(void) const
{
    // -- TODO: To be implemented.
    return CharVectorAutoPtr(NULL);
}
