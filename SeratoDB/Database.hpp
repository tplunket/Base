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

#include <SeratoDB/Tags/Tag.hpp>
#include <SeratoDB/Track.hpp>
#include <SeratoDB/Crate.hpp>
#include <SeratoDB/CrateOrderFile.hpp>
#include <SeratoDB/Utility.hpp>

namespace NxA { namespace Serato {
    #pragma mark Class Declaration
    class Database
    {
    private:
        #pragma mark Private Instance Variables
        ConstStringPtr p_databaseFilePath;
        TrackVectorPtr p_tracks;
        ConstTagVector p_otherTags;

        StringVector p_crateFilesToDelete;

        CrateOrderFilePtr p_crateOrderFile;

        bool p_databaseIsValid;

        #pragma mark Private Instance Methods
        void p_storeTrackTag(TagPtr tag);
        void p_storeOtherTag(TagPtr tag);

    public:
        #pragma mark Constructors
        explicit Database(const char* seratoFolderPath);

        #pragma mark Class Methods
        static ConstStringPtr versionAsStringForDatabaseIn(const char* seratoFolderPath);

        #pragma mark Instance Methods
        time_t databaseModificationDateInSecondsSince1970(void) const;
        time_t rootCrateModificationDateInSecondsSince1970(void) const;

        const Crate* rootCrate(void) const;
        const TrackVector& tracks(void) const;

        TrackVectorPtr removeAndReturnTracks(void);

        void addCrateFileToDelete(const std::string& path);

        void addTrack(TrackPtr track);

        void saveIfModified(void) const;
    };
} }
