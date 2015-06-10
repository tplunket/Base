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

#include <SeratoDB/ID3TrackFile.hpp>

#include <taglib/mpegfile.h>

namespace NxA {
    namespace Serato {
    #pragma mark Class Declaration
    class MPEGTrackFile : public ID3TrackFile
    {
    public:
        #pragma mark Constructors
        explicit MPEGTrackFile(const char* trackFilePath);
    };
}
}
