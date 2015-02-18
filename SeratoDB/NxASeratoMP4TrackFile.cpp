//
//  NxASeratoMP4TrackFile.cpp
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

#include "SeratoDB/NxASeratoMP4TrackFile.h"
#include "SeratoDB/NxASeratoBase64.h"

#include <taglib/mp4tag.h>

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
} SeratoMP4MarkerHeaderStruct;

#pragma mark Constructors

SeratoMP4TrackFile::SeratoMP4TrackFile(const char* trackFilePath) : SeratoTrackFile(trackFilePath)
{
    MP4::File* file = new MP4::File(trackFilePath);
    if (!file->isValid()) {
        this->p_file = TaglibFileAutoPtr(NULL);
        this->p_itemListMap = NULL;
        this->p_audioProperties = NULL;
        this->p_parsedFileTag = NULL;
        return;
    }

    this->p_file = TaglibFileAutoPtr(file);
    MP4::Tag* tag = file->tag();
    this->p_parsedFileTag = tag;
    this->p_itemListMap = &(tag->itemListMap());
    this->p_audioProperties = file->audioProperties();
    this->p_properties = file->properties();

    this->p_readMarkersV2();
}

#pragma mark Instance Methods

void SeratoMP4TrackFile::p_readMarkersV2(void)
{
    if (!this->p_itemListMap) {
        return;
    }

    MP4::Item item = (*this->p_itemListMap)["----:com.serato.dj:markersv2"];
    if (!item.isValid()) {
        return;
    }

    const String markersEncodedData = item.toStringList().toString();
    uint32_t encodedDataSize = markersEncodedData.size();
    if (!encodedDataSize) {
        return;
    }

    CharVectorAutoPtr decodedData = SeratoBase64::decodeBlock((const char*)markersEncodedData.data(String::UTF8).data(),
                                                                      encodedDataSize);

    const SeratoMP4MarkerHeaderStruct* headerStruct = (const SeratoMP4MarkerHeaderStruct*)decodedData->data();
    this->p_readMarkersV2FromBase64Data((const char*)headerStruct->data, decodedData->size() - sizeof(SeratoMP4MarkerHeaderStruct));
}

bool SeratoMP4TrackFile::hasKey(void) const
{
    return true;
}

StringAutoPtr SeratoMP4TrackFile::key(void) const
{
    string* result = NULL;

    MP4::Item item = (*this->p_itemListMap)["----:com.apple.iTunes:initialkey"];
    if (item.isValid()) {
        String text = item.toStringList().front();
        if (text != String::null) {
            result = new string(text.to8Bit());
        }
    }

    return StringAutoPtr(result);
}

StringAutoPtr SeratoMP4TrackFile::grouping(void) const
{
    string* result = NULL;

    String text = this->p_properties["GROUPING"].toString();
    if (text != String::null) {
        result = new string(text.to8Bit());
    }

    return StringAutoPtr(result);
}

bool SeratoMP4TrackFile::hasRecordLabel(void) const
{
    return false;
}

StringAutoPtr SeratoMP4TrackFile::recordLabel(void) const
{
    return StringAutoPtr(NULL);
}

bool SeratoMP4TrackFile::hasRemixer(void) const
{
    return false;
}

StringAutoPtr SeratoMP4TrackFile::remixer(void) const
{
    return StringAutoPtr(NULL);
}

bool SeratoMP4TrackFile::hasDateReleased(void) const
{
    return true;
}

StringAutoPtr SeratoMP4TrackFile::dateReleased(void) const
{
    string* result = NULL;

    String text = this->p_properties["purd"].toString();
    if (text != String::null) {
        result = new string(text.to8Bit());
    }

    return StringAutoPtr(result);
}

bool SeratoMP4TrackFile::hasYearReleased(void) const
{
    return true;
}

StringAutoPtr SeratoMP4TrackFile::yearReleased(void) const
{
    string* result = NULL;

    String text = this->p_properties["DATE"].toString();
    if (text != String::null) {
        result = new string(text.to8Bit());
    }

    return StringAutoPtr(result);
}

CharVectorAutoPtr SeratoMP4TrackFile::artwork(void) const
{
    CharVector* result = NULL;

    MP4::Item item = (*this->p_itemListMap)["covr"];
    if (item.isValid()) {
        MP4::CoverArtList coverArtList = item.toCoverArtList();
        MP4::CoverArt coverArt = coverArtList.front();
        if (coverArt.data().size()) {
            char* data = coverArt.data().data();
            result = new CharVector(data, data + coverArt.data().size());
        }
    }

    return CharVectorAutoPtr(result);
}
