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

#ifndef __SeratoDB__NxASeratoMPEGTrackFile__
#define __SeratoDB__NxASeratoMPEGTrackFile__

#include <SeratoDB/NxASeratoID3TrackFile.h>

#include <taglib/mpegfile.h>

namespace NxA {
    #pragma mark Class Declaration
    class SeratoMPEGTrackFile : public SeratoID3TrackFile
    {
    public:
        #pragma mark Constructors
        explicit SeratoMPEGTrackFile(const char* trackFilePath);
    };
}

#endif /* defined(__SeratoDB__NxASeratoMPEGTrackFile__) */
