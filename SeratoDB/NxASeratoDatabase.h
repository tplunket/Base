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
        ConstStringPtr p_databaseFilePath;
        SeratoTrackVectorPtr p_tracks;
        ConstSeratoTagVector p_otherTags;

        SeratoCrateOrderFilePtr p_crateOrderFile;

        bool p_databaseIsValid;

        #pragma mark Private Instance Methods
        void p_storeTrackTag(SeratoTagPtr tag);
        void p_storeOtherTag(SeratoTagPtr tag);

    public:
        #pragma mark Constructors
        explicit SeratoDatabase(const char* seratoFolderPath);

        #pragma mark Class Methods
        static ConstStringPtr versionAsStringForDatabaseIn(const char* seratoFolderPath);

        #pragma mark Instance Methods
        time_t databaseModificationDateInSecondsSince1970(void) const;

        const SeratoCrate* rootCrate(void) const;
        const SeratoTrackVector& tracks(void) const;

        SeratoTrackVectorPtr removeAndReturnTracks(void);

        void addTrack(SeratoTrackPtr track);

        void saveIfModified(void) const;
    };
}
