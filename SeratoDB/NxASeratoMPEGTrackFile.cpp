//
//  NxASeratoTrackFile.cpp
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

#include "SeratoDB/NxASeratoMPEGTrackFile.h"

#include <taglib/mpegproperties.h>

using namespace NxA;
using namespace TagLib;
using namespace std;

#pragma mark Constructors

SeratoMPEGTrackFile::SeratoMPEGTrackFile(const char* trackFilePath) : SeratoID3TrackFile(trackFilePath)
{
    MPEG::File* file = new MPEG::File(trackFilePath);
    if (!file->isValid()) {
        this->p_file = TaglibFilePtr();
        this->p_parsedFileTag = NULL;
        this->p_audioProperties = NULL;
        return;
    }

    this->p_file = TaglibFilePtr((TagLib::File*)file);
    this->p_parsedFileTag = (TagLib::Tag*)file->ID3v2Tag();
    this->p_audioProperties = file->audioProperties();
    this->p_properties = file->properties();

    this->p_readMarkersV2();
}
