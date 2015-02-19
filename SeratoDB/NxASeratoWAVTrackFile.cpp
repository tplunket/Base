//
//  NxASeratoWAVTrackFile.cpp
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

#include "SeratoDB/NxASeratoWAVTrackFile.h"

#include <taglib/wavfile.h>

using namespace NxA;
using namespace TagLib;
using namespace std;

#pragma mark Constructors

SeratoWAVTrackFile::SeratoWAVTrackFile(const char* trackFilePath) : SeratoID3TrackFile(trackFilePath)
{
    RIFF::WAV::File* file = new RIFF::WAV::File(trackFilePath);
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

#pragma mark Instance Variables

uint32_t SeratoWAVTrackFile::lengthInMilliseconds(void) const
{
    if (this->p_audioProperties) {
        const RIFF::WAV::Properties* audioProperties = (RIFF::WAV::Properties*)this->p_audioProperties;

        uint32_t numberOfFrames = audioProperties->sampleFrames();
        uint32_t sampleRate = audioProperties->sampleRate();
        uint32_t numberOfMilliseconds = ((double)numberOfFrames * 1000.0) / (double)sampleRate;

        return numberOfMilliseconds;
    }
    
    return 0;
}

uint32_t SeratoWAVTrackFile::bitDepthInBitsOrZeroIfNotApplicable(void) const
{
    if (this->p_audioProperties) {
        const RIFF::WAV::Properties* audioProperties = (RIFF::WAV::Properties*)this->p_audioProperties;

        return audioProperties->sampleWidth();
    }

    return 0;
}
