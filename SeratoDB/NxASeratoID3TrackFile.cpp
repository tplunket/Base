//
//  NxASeratoID3TrackFile.cpp
//  SeratoDB
//
//  Created by Didier Malenfant on 2/12/15.
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

#include <taglib/generalencapsulatedobjectframe.h>

#include "SeratoDB/NxASeratoID3TrackFile.h"
#include "SeratoDB/NxASeratoBase64.h"

#include <taglib/id3v2tag.h>

using namespace std;
using namespace NxA;
using namespace TagLib;

#pragma mark Structures

typedef struct {
    unsigned char dummy;
    unsigned char mimeType[25];
    unsigned char filename;
    unsigned char description[16];
    unsigned char majorVersion;
    unsigned char minorVersion;
} SeratoGeobBodyHeaderStruct;

typedef struct {
    unsigned char majorVersion;
    unsigned char minorVersion;
    unsigned char data[0];
} SeratoGeobObjectStruct;

#pragma mark Utility Functions

static bool p_isAValidGeobFrame(const ID3v2::GeneralEncapsulatedObjectFrame* frame)
{
    String frameID(frame->frameID().data());
    if (frameID != "GEOB") {
        return false;
    }

    String mimeType = frame->mimeType();
    return mimeType == "application/octet-stream";
}

#pragma mark Instance Methods

void SeratoID3TrackFile::p_readMarkersV2(void)
{
    ID3v2::Tag* tag = (ID3v2::Tag*)this->p_parsedFileTag;
    if (!tag) {
        return;
    }

    ID3v2::FrameListMap frameListMap = tag->frameListMap();
    if (!frameListMap.contains("GEOB")) {
        return;
    }

    ID3v2::FrameList geobFrames = frameListMap["GEOB"];
    for (ID3v2::FrameList::Iterator it = geobFrames.begin(); it != geobFrames.end(); ++it) {
        const ID3v2::GeneralEncapsulatedObjectFrame* frame = (ID3v2::GeneralEncapsulatedObjectFrame*)(*it);
        if (!p_isAValidGeobFrame(frame)) {
            continue;
        }

        const String description = frame->description();
        if (description == "Serato Markers2") {
            uint32_t encodedDataSize = frame->size() - sizeof(SeratoGeobBodyHeaderStruct);
            const ByteVector frameObject = frame->object();

            SeratoGeobObjectStruct* objectData = (SeratoGeobObjectStruct*)frameObject.data();
            this->p_readMarkersV2FromBase64Data((char*)objectData->data, encodedDataSize);
        }
    }
}

bool SeratoID3TrackFile::hasKey(void) const
{
    return true;
}

StringAutoPtr SeratoID3TrackFile::key(void) const
{
    string* result = NULL;

    String text = this->p_properties["INITIALKEY"].toString();
    if (text != String::null) {
        result = new string(text.to8Bit());
    }

    return StringAutoPtr(result);
}

StringAutoPtr SeratoID3TrackFile::grouping(void) const
{
    string* result = NULL;

    String text = this->p_properties["CONTENTGROUP"].toString();
    if (text != String::null) {
        result = new string(text.to8Bit());
    }

    return StringAutoPtr(result);
}

bool SeratoID3TrackFile::hasRecordLabel(void) const
{
    return true;
}

StringAutoPtr SeratoID3TrackFile::recordLabel(void) const
{
    string* result = NULL;

    String text = this->p_properties["LABEL"].toString();
    if (text != String::null) {
        result = new string(text.to8Bit());
    }

    return StringAutoPtr(result);
}

bool SeratoID3TrackFile::hasRemixer(void) const
{
    return true;
}

StringAutoPtr SeratoID3TrackFile::remixer(void) const
{
    string* result = NULL;

    String text = this->p_properties["REMIXER"].toString();
    if (text != String::null) {
        result = new string(text.to8Bit());
    }

    return StringAutoPtr(result);
}

bool SeratoID3TrackFile::hasDateReleased(void) const
{
    return true;
}

StringAutoPtr SeratoID3TrackFile::dateReleased(void) const
{
    string* result = NULL;

    String text = this->p_properties["DATE"].toString();
    if (text != String::null) {
        result = new string(text.to8Bit());
    }

    return StringAutoPtr(result);
}

bool SeratoID3TrackFile::hasYearReleased(void) const
{
    return false;
}

StringAutoPtr SeratoID3TrackFile::yearReleased(void) const
{
    return StringAutoPtr(NULL);
}
