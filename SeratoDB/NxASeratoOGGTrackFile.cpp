//
//  NxASeratoOGGTrackFile.cpp
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

#include "SeratoDB/NxASeratoOGGTrackFile.h"

#include <taglib/vorbisfile.h>

using namespace std;
using namespace NxA;
using namespace TagLib;

#pragma mark Constructors

SeratoOGGTrackFile::SeratoOGGTrackFile(const char* trackFilePath) : SeratoID3TrackFile()
{
    Vorbis::File* file = new Vorbis::File(trackFilePath);
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

void SeratoOGGTrackFile::p_readMarkersV2(void)
{
    const String markersEncodedData = this->p_properties["SERATO_MARKERS2"].toString();
    uint32_t encodedDataSize = markersEncodedData.size();

    this->p_readMarkersV2FromBase64Data((char*)markersEncodedData.data(String::UTF8).data(), encodedDataSize);
}

bool SeratoOGGTrackFile::hasKey(void) const
{
    return false;
}

StringAutoPtr SeratoOGGTrackFile::key(void) const
{
    return StringAutoPtr(NULL);
}

StringAutoPtr SeratoOGGTrackFile::grouping(void) const
{
    string* result = NULL;

    String text = this->p_properties["GROUPING"].toString();
    if (text != String::null) {
        result = new string(text.to8Bit());
    }

    return StringAutoPtr(result);
}

bool SeratoOGGTrackFile::hasRecordLabel(void) const
{
    return true;
}

StringAutoPtr SeratoOGGTrackFile::recordLabel(void) const
{
    string* result = NULL;

    String text = this->p_properties["LABEL"].toString();
    if (text != String::null) {
        result = new string(text.to8Bit());
    }

    return StringAutoPtr(result);
}

bool SeratoOGGTrackFile::hasRemixer(void) const
{
    return true;
}

StringAutoPtr SeratoOGGTrackFile::remixer(void) const
{
    string* result = NULL;

    String text = this->p_properties["REMIXER"].toString();
    if (text != String::null) {
        result = new string(text.to8Bit());
    }

    return StringAutoPtr(result);
}

bool SeratoOGGTrackFile::hasDateReleased(void) const
{
    return false;
}

StringAutoPtr SeratoOGGTrackFile::dateReleased(void) const
{
    return StringAutoPtr(NULL);
}

bool SeratoOGGTrackFile::hasYearReleased(void) const
{
    return true;
}

StringAutoPtr SeratoOGGTrackFile::yearReleased(void) const
{
    string* result = NULL;

    String text = this->p_properties["YEAR"].toString();
    if (text != String::null) {
        result = new string(text.to8Bit());
    }

    return StringAutoPtr(result);
}
