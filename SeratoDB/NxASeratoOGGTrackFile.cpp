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

using namespace NxA;
using namespace TagLib;
using namespace std;

#pragma mark Constants

static const string emptyString("");

#pragma mark Constructors

SeratoOGGTrackFile::SeratoOGGTrackFile(const char* trackFilePath) : SeratoID3TrackFile(trackFilePath)
{
    TaglibFilePtr file = make_unique<Vorbis::File>(trackFilePath);
    if (!file->isValid()) {
        this->p_file = TaglibFilePtr();
        this->p_parsedFileTag = NULL;
        this->p_audioProperties = NULL;
        return;
    }

    this->p_file = move(file);
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

string SeratoOGGTrackFile::key(void) const
{
    return emptyString;
}

string SeratoOGGTrackFile::grouping(void) const
{
    String text = this->p_properties["GROUPING"].toString();
    if (text != String::null) {
        return text.to8Bit();
    }

    return emptyString;
}

bool SeratoOGGTrackFile::hasRecordLabel(void) const
{
    return true;
}

string SeratoOGGTrackFile::recordLabel(void) const
{
    String text = this->p_properties["LABEL"].toString();
    if (text != String::null) {
        return text.to8Bit();
    }

    return emptyString;
}

bool SeratoOGGTrackFile::hasRemixer(void) const
{
    return true;
}

string SeratoOGGTrackFile::remixer(void) const
{
    String text = this->p_properties["REMIXER"].toString();
    if (text != String::null) {
        return text.to8Bit();
    }

    return emptyString;
}

string SeratoOGGTrackFile::yearReleased(void) const
{
    String text = this->p_properties["YEAR"].toString();
    if (text != String::null) {
        return text.to8Bit();
    }

    return emptyString;
}

CharVectorPtr SeratoOGGTrackFile::artwork(void) const
{
    // -- TODO: To be implemented.
    return CharVectorPtr();
}
