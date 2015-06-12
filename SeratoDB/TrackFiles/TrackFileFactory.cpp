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

#include "TrackFiles/TrackFileFactory.hpp"
#include "TrackFiles/AIFFTrackFile.hpp"
#include "TrackFiles/MPEGTrackFile.hpp"
#include "TrackFiles/MP4TrackFile.hpp"
#include "TrackFiles/WavTrackFile.hpp"
#include "TrackFiles/OGGTrackFile.hpp"
#include "TrackFiles/FLACTrackFile.hpp"

using namespace NxA::Serato;
using namespace std;

#pragma mark Utility Functions

bool p_hasSuffix(const string& str, const string& suffix)
{
    return str.size() >= suffix.size() &&
    str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

#pragma mark Class Methods

TrackFilePtr TrackFileFactory::trackFileForPath(const char* trackFilePath)
{
    string path(trackFilePath);
    TrackFilePtr file;

    if (p_hasSuffix(path, ".aiff") || p_hasSuffix(path, ".aif")) {
        file = make_unique<AIFFTrackFile>(trackFilePath);
    }
    else if (p_hasSuffix(trackFilePath, ".mp3")) {
        file = make_unique<MPEGTrackFile>(trackFilePath);
    }
    else if (p_hasSuffix(trackFilePath, ".m4a")) {
        file = make_unique<MP4TrackFile>(trackFilePath);
    }
    else if (p_hasSuffix(trackFilePath, ".flac")) {
        file = make_unique<FLACTrackFile>(trackFilePath);
    }
    else if (p_hasSuffix(trackFilePath, ".ogg")) {
        file = make_unique<OGGTrackFile>(trackFilePath);
    }
    else if (p_hasSuffix(trackFilePath, ".wav")) {
        file = make_unique<SeratoWAVTrackFile>(trackFilePath);
    }

    return move(file);
}
