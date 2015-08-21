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

#pragma once

#include <SeratoDb/TrackFiles/TrackFile.hpp>
#include <Base/Base.hpp>

namespace NxA { namespace Serato {
    class TrackFileFactory : public Uncopyable
    {
    public:
        #pragma mark Constants
        enum AudioFileType {
            Unknown,
            AIFF,
            MP3,
            WAV,
            OGG,
            AAC,
            FLAC,
            ALAC,
            WMA,
        };
        
        #pragma mark Constructors & Destructors
        TrackFileFactory() = delete;

        #pragma mark Class Methods
        static AudioFileType audioFileTypeForPath(const String& trackFilePath);

        static NxA::Serato::TrackFile::Pointer trackFileForPath(const String& trackFilePath);
    };
} }
