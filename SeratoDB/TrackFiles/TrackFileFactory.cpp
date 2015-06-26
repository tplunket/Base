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

#pragma mark Class Methods

TrackFile::Pointer TrackFileFactory::trackFileForPath(const String& trackFilePath)
{
    if (trackFilePath.hasPostfix(".aiff") || trackFilePath.hasPostfix(".aif")) {
        return TrackFile::Pointer::dynamicCastFrom(AIFFTrackFile::fileWithFileAt(trackFilePath));
    }
    else if (trackFilePath.hasPostfix(".mp3")) {
        return TrackFile::Pointer::dynamicCastFrom(MPEGTrackFile::fileWithFileAt(trackFilePath));
    }
    else if (trackFilePath.hasPostfix(".m4a")) {
        return TrackFile::Pointer::dynamicCastFrom(MP4TrackFile::fileWithFileAt(trackFilePath));
    }
    else if (trackFilePath.hasPostfix(".flac")) {
        return TrackFile::Pointer::dynamicCastFrom(FLACTrackFile::fileWithFileAt(trackFilePath));
    }
    else if (trackFilePath.hasPostfix(".ogg")) {
        return TrackFile::Pointer::dynamicCastFrom(OGGTrackFile::fileWithFileAt(trackFilePath));
    }
    else if (trackFilePath.hasPostfix(".wav")) {
        return TrackFile::Pointer::dynamicCastFrom(WAVTrackFile::fileWithFileAt(trackFilePath));
    }

    NXA_ALOG("Unknown file extension.");
}
