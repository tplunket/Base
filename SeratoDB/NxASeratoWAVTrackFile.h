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

#include <SeratoDB/NxASeratoID3TrackFile.h>

namespace NxA {
    #pragma mark Class Declaration
    class SeratoWAVTrackFile : public SeratoID3TrackFile
    {
    public:
        #pragma mark Constructors
        SeratoWAVTrackFile(const char* trackFilePath);

        #pragma mark Instance Variables
        virtual uint32_t lengthInMilliseconds(void) const;
        uint32_t bitDepthInBitsOrZeroIfNotApplicable(void) const;
    };
}
