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

#include "mp4file.h"

using namespace NxA::Serato;

#pragma mark Class Methods

TrackFileFactory::AudioFileType TrackFileFactory::audioFileTypeForPath(const String& trackFilePath)
{
    auto lowerCaseString = trackFilePath.lowerCaseString();
    
    if (lowerCaseString->hasPostfix(".aiff") || lowerCaseString->hasPostfix(".aif")) {
        return AudioFileType::AIFF;
    }
    else if (lowerCaseString->hasPostfix(".mp3")) {
        return AudioFileType::MP3;
    }
    else if (lowerCaseString->hasPostfix(".m4a") || lowerCaseString->hasPostfix(".mp4")) {
        auto file = std::make_shared<TagLib::MP4::File>(lowerCaseString->toUTF8());
        if (!file->isValid()) {
            return AudioFileType::Unknown;
        }

        return file->audioProperties()->codec() == TagLib::MP4::Properties::Codec::ALAC ? AudioFileType::ALAC : AudioFileType::AAC;
    }
    else if (lowerCaseString->hasPostfix(".flac")) {
        return AudioFileType::FLAC;
    }
    else if (lowerCaseString->hasPostfix(".ogg")) {
        return AudioFileType::OGG;
    }
    else if (lowerCaseString->hasPostfix(".wav")) {
        return AudioFileType::WAV;
    }
    else {
        return AudioFileType::Unknown;
    }
}

TrackFile::Pointer TrackFileFactory::trackFileForPath(const String& trackFilePath, TrackFile::Flags flags)
{
    switch (TrackFileFactory::audioFileTypeForPath(trackFilePath)) {
        case AudioFileType::AIFF: {
            return TrackFile::Pointer::dynamicCastFrom(AIFFTrackFile::fileWithFileAt(trackFilePath, flags));
        }
        case AudioFileType::MP3: {
            return TrackFile::Pointer::dynamicCastFrom(MPEGTrackFile::fileWithFileAt(trackFilePath, flags));
        }
        case AudioFileType::ALAC:
        case AudioFileType::AAC: {
            return TrackFile::Pointer::dynamicCastFrom(MP4TrackFile::fileWithFileAt(trackFilePath, flags));
        }
        case AudioFileType::FLAC: {
            return TrackFile::Pointer::dynamicCastFrom(FLACTrackFile::fileWithFileAt(trackFilePath, flags));
        }
        case AudioFileType::OGG: {
            return TrackFile::Pointer::dynamicCastFrom(OGGTrackFile::fileWithFileAt(trackFilePath, flags));
        }
        case AudioFileType::WAV: {
            return TrackFile::Pointer::dynamicCastFrom(WAVTrackFile::fileWithFileAt(trackFilePath, flags));
        }
        default: {
            NXA_ALOG("Unknown file extension for file '%s'.", trackFilePath.toUTF8());
        }
    }
}
