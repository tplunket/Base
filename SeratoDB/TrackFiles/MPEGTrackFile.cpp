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

#include "TrackFiles/MPEGTrackFile.hpp"

#include <taglib/mpegproperties.h>

using namespace NxA::Serato;
using namespace TagLib;
using namespace std;

#pragma mark Constructors

MPEGTrackFile::MPEGTrackFile(const char* trackFilePath) : ID3TrackFile(trackFilePath)
{
    TaglibFilePtr file = make_unique<MPEG::File>(trackFilePath);
    if (!file->isValid()) {
        this->p_file = TaglibFilePtr();
        this->p_parsedFileTag = NULL;
        this->p_audioProperties = NULL;
        return;
    }

    this->p_parsedFileTag = (TagLib::Tag*)(((MPEG::File*)file.get())->ID3v2Tag());
    this->p_audioProperties = file->audioProperties();
    this->p_properties = file->properties();
    this->p_file = move(file);

    this->p_readMarkers();
}
