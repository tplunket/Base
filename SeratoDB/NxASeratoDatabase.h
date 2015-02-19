//
//  NxASeratoDatabase.h
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

#ifndef __SeratoDB__NxASeratoDatabase__
#define __SeratoDB__NxASeratoDatabase__

#include <SeratoDB/NxASeratoTag.h>
#include <SeratoDB/NxASeratoTrack.h>
#include <SeratoDB/NxASeratoCrate.h>
#include <SeratoDB/NxASeratoCrateOrderFile.h>
#include <SeratoDB/NxASeratoDBUtility.h>

namespace NxA {
    #pragma mark Class Declaration
    class SeratoDatabase
    {
    private:
        #pragma mark Private Instance Variables
        SeratoTrackVector p_tracks;
        SeratoTagVector p_otherTags;

        SeratoCrateOrderFileAutoPtr p_crateOrderFile;

        #pragma mark Private Instance Methods
        void p_storeVersionTag(const SeratoTag* tag);
        void p_storeTrackTag(const SeratoTag* tag);
        void p_storeOtherTag(const SeratoTag* tag);

    public:
        #pragma mark Constructors
        SeratoDatabase(const char* seratoFolderPath);

        #pragma mark Class Methods
        static StringAutoPtr versionAsStringForDatabaseIn(const char* seratoFolderPath);

        #pragma mark Instance Methods
        const SeratoCrate* rootCrate(void) const;
        const SeratoTrackVector& tracks(void) const;
    };
}

#endif /* defined(__SeratoDB__NxASeratoDatabase__) */
