//
//  NxASeratoAIFFTrackFile.cpp
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

#include "SeratoDB/NxASeratoAIFFTrackFile.h"

#include <taglib/aifffile.h>

using namespace std;
using namespace NxA;
using namespace TagLib;

#pragma mark Constructors

SeratoAIFFTrackFile::SeratoAIFFTrackFile(const char* trackFilePath) : SeratoID3TrackFile()
{
    RIFF::AIFF::File* file = new RIFF::AIFF::File(trackFilePath);
    this->p_file = auto_ptr<TagLib::File>((TagLib::File*)file);
    if (!file) {
        this->p_parsedFileTag = NULL;
        this->p_audioProperties = NULL;
        return;
    }

    this->p_parsedFileTag = file->tag();
    this->p_audioProperties = file->audioProperties();
    this->p_properties = file->properties();

    this->p_readMarkersV2();
}

#pragma mark Instance Variables

uint32_t SeratoAIFFTrackFile::lengthInMilliseconds(void) const
{
    if (this->p_audioProperties) {
        RIFF::AIFF::Properties* audioProperties = (RIFF::AIFF::Properties*)this->p_audioProperties;

        uint32_t numberOfFrames = audioProperties->sampleFrames();
        uint32_t sampleRate = audioProperties->sampleRate();
        uint32_t numberOfMilliseconds = ((double)numberOfFrames * 1000.0) / (double)sampleRate;

        return numberOfMilliseconds;
    }

    return 0;
}
