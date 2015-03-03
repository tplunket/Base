//
//  NxASeratoTrackFileFactory.cpp
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

#include "SeratoDB/NxASeratoTrackFileFactory.h"
#include "SeratoDB/NxASeratoAIFFTrackFile.h"
#include "SeratoDB/NxASeratoMPEGTrackFile.h"
#include "SeratoDB/NxASeratoMP4TrackFile.h"
#include "SeratoDB/NxASeratoWAVTrackFile.h"
#include "SeratoDB/NxASeratoOGGTrackFile.h"
#include "SeratoDB/NxASeratoFLACTrackFile.h"

using namespace NxA;
using namespace std;

#pragma mark Utility Functions

bool p_hasSuffix(const string& str, const string& suffix)
{
    return str.size() >= suffix.size() &&
    str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

#pragma mark Class Methods

SeratoTrackFilePtr SeratoTrackFileFactory::trackFileForPath(const char* trackFilePath)
{
    string path(trackFilePath);
    SeratoTrackFile* file = NULL;

    if (p_hasSuffix(path, ".aiff") || p_hasSuffix(path, ".aif")) {
        file = new SeratoAIFFTrackFile(trackFilePath);
    }
    else if (p_hasSuffix(trackFilePath, ".mp3")) {
        file = new SeratoMPEGTrackFile(trackFilePath);
    }
    else if (p_hasSuffix(trackFilePath, ".m4a")) {
        file = new SeratoMP4TrackFile(trackFilePath);
    }
    else if (p_hasSuffix(trackFilePath, ".flac")) {
        file = new SeratoFLACTrackFile(trackFilePath);
    }
    else if (p_hasSuffix(trackFilePath, ".ogg")) {
        file = new SeratoOGGTrackFile(trackFilePath);
    }
    else if (p_hasSuffix(trackFilePath, ".wav")) {
        file = new SeratoWAVTrackFile(trackFilePath);
    }

    return SeratoTrackFilePtr(file);
}
