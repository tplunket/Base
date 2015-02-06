//
//  NxASeratoDatabaseParser.h
//  SeratoDB
//
//  Created by Didier Malenfant on 1/30/15.
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

#ifndef __SeratoDB__NxASeratoDatabaseParser__
#define __SeratoDB__NxASeratoDatabaseParser__

#include <SeratoDB/NxASeratoTag.h>
#include <SeratoDB/NxASeratoDatabaseTrack.h>

#include <string>

namespace NxA {
    #pragma mark Class Declaration
    class SeratoDatabaseParser
    {
    private:
        #pragma mark Private Instance Variables
        SeratoTagAutoPtr p_versionTag;
        SeratoDatabaseTrackVector p_tracks;
        SeratoTagVector p_otherTags;

        #pragma mark Private Instance Methods
        void p_storeVersionTag(const SeratoTag* tag);
        void p_storeTrackTag(const SeratoTag* tag);
        void p_storeOtherTag(const SeratoTag* tag);

    public:
        #pragma mark Constructors
        SeratoDatabaseParser(const void* startOfFile, unsigned long lengthInBytes);

        #pragma mark Instance Methods
        StringAutoPtr versionAsString(void) const;

        const SeratoDatabaseTrackVector& tracks(void);
    };
}

#endif /* defined(__SeratoDB__NxASeratoDatabaseParser__) */
