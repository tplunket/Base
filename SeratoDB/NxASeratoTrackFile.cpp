//
//  NxASeratoTrackFile.cpp
//  SeratoDB
//
//  Created by Didier Malenfant on 2/6/15.
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

#include <taglib/tpropertymap.h>
#include <taglib/generalencapsulatedobjectframe.h>

#include "SeratoDB/NxASeratoTrackFile.h"
#include "SeratoDB/NxASeratoDbUtility.h"
#include "SeratoDB/NxASeratoBase64.h"

using namespace std;
using namespace NxA;
using namespace TagLib;

#pragma mark Structures

typedef struct {
    unsigned char majorVersion;
    unsigned char minorVersion;
    unsigned char data[0];
} SeratoMarkerV2Struct;

#pragma mark Utility Functions

const char* p_nextTagPositionAfterTagNamed(const string tagName, const char* currentTagPosition)
{
    const char* parserPosition = currentTagPosition;

    parserPosition += tagName.length() + 1;
    uint32_t tagSize = bigEndianUInt32ValueAt(parserPosition);
    parserPosition += 4 + tagSize;

    return parserPosition;
}

#pragma mark Instance Methods

void SeratoTrackFile::p_readMarkersV2FromBase64Data(const char* markerV2Data, size_t sizeInBytes)
{
    if (!sizeInBytes) {
        return;
    }

    UnsignedCharVectorAutoPtr decodedData = SeratoBase64::decodeBlock(markerV2Data, sizeInBytes);

    const SeratoMarkerV2Struct* markerStruct = (const SeratoMarkerV2Struct*)(decodedData->data());
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
            SeratoCueMarker* newCueMarker = new SeratoCueMarker(tagStart);
            this->p_cueMarkers->push_back(SeratoCueMarkerAutoPtr(newCueMarker));
        }
        else if (tagName == "LOOP") {
            SeratoLoopMarker* newLoopMarker = new SeratoLoopMarker(tagStart);
            this->p_loopMarkers->push_back(SeratoLoopMarkerAutoPtr(newLoopMarker));
        }
        else {
            // -- TODO: Preserve other tags in order to write them back.
        }

        tagStart = p_nextTagPositionAfterTagNamed(tagName, tagStart);
    }
}

StringAutoPtr SeratoTrackFile::title(void) const
{
    string* result = NULL;

    if (this->p_parsedFileTag) {
        String text = this->p_parsedFileTag->title();
        if (text != String::null) {
            result = new string(text.to8Bit());
        }
    }

    return StringAutoPtr(result);
}

StringAutoPtr SeratoTrackFile::artist(void) const
{
    string* result = NULL;

    if (this->p_parsedFileTag) {
        String text = this->p_parsedFileTag->artist();
        if (text != String::null) {
            result = new string(text.to8Bit());
        }
    }

    return StringAutoPtr(result);
}

StringAutoPtr SeratoTrackFile::genre(void) const
{
    string* result = NULL;

    if (this->p_parsedFileTag) {
        String text = this->p_parsedFileTag->genre();
        if (text != String::null) {
            result = new string(text.to8Bit());
        }
    }

    return StringAutoPtr(result);
}

StringAutoPtr SeratoTrackFile::comments(void) const
{
    string* result = NULL;

    if (this->p_parsedFileTag) {
        String text = this->p_parsedFileTag->comment();
        if (text != String::null) {
            result = new string(text.to8Bit());
        }
    }

    return StringAutoPtr(result);
}

StringAutoPtr SeratoTrackFile::album(void) const
{
    string* result = NULL;

    if (this->p_parsedFileTag) {
        String text = this->p_parsedFileTag->album();
        if (text != String::null) {
            result = new string(text.to8Bit());
        }
    }

    return StringAutoPtr(result);
}

StringAutoPtr SeratoTrackFile::composer(void) const
{
    string* result = NULL;

    String text = this->p_properties["COMPOSER"].toString();
    if (text != String::null) {
        result = new string(text.to8Bit());
    }

    return StringAutoPtr(result);
}

StringAutoPtr SeratoTrackFile::bpm(void) const
{
    string* result = NULL;

    String text = this->p_properties["BPM"].toString();
    if (text != String::null) {
        result = new string(text.to8Bit());
    }

    return StringAutoPtr(result);
}

uint32_t SeratoTrackFile::lengthInMilliseconds(void) const
{
    if (this->p_audioProperties) {
        return this->p_audioProperties->length() * 1000;
    }

    return 0;
}

uint32_t SeratoTrackFile::bitRateInKiloBitsPerSecond(void) const
{
    if (this->p_audioProperties) {
        return this->p_audioProperties->bitrate();
    }

    return 0;
}

uint32_t SeratoTrackFile::sampleRateInSamplesPerSecond(void) const
{
    if (this->p_audioProperties) {
        return this->p_audioProperties->sampleRate();
    }

    return 0;
}

uint32_t SeratoTrackFile::trackNumber(void) const
{
    if (this->p_parsedFileTag) {
        return this->p_parsedFileTag->track();
    }

    return 0;
}

const SeratoCueMarkerVector& SeratoTrackFile::cueMarkers(void) const
{
    return *(this->p_cueMarkers);
}

const SeratoLoopMarkerVector& SeratoTrackFile::loopMarkers(void) const
{
    return *(this->p_loopMarkers);
}
