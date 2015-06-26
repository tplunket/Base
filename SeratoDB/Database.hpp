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

#include <Base/Base.hpp>

namespace NxA { namespace Serato {
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR(Database);

    class Crate;
    class Track;
    class TrackEntry;

    class Database : public Object, NXA_INHERITANCE_FOR(Database) {
        NXA_GENERATED_DECLARATIONS_FOR(NxA::Serato, Database);

    public:
        #pragma mark Factory Methods
        static Database::Pointer databaseWithFileAt(String::Pointer const& seratoFolderPath);

        #pragma mark Class Methods
        static String::Pointer versionAsStringForDatabaseIn(String::Pointer const& seratoFolderPath);

        #pragma mark Instance Methods
        timestamp databaseModificationDateInSecondsSince1970(void) const;
        timestamp rootCrateModificationDateInSecondsSince1970(void) const;

        NxA::Pointer<Crate> const& rootCrate(void) const;
        NxA::Pointer<NxA::Array<Track>> const& tracks(void) const;
        NxA::Pointer<NxA::Array<Track>> removeAndReturnTracks(void);

        void deleteTrackEntry(NxA::Pointer<TrackEntry>& crate);
        void deleteCrate(NxA::Pointer<Crate>& crate);

        void addTrack(NxA::Pointer<Track> const& track);
        void deleteTrack(NxA::Pointer<Track>& track);

        void saveIfModified(void) const;
    };
} }
