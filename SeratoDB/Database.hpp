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
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR_CLASS(Database);

    class Crate;
    class Track;
    class TrackEntry;

    class Database : public Object {
        NXA_GENERATED_OPERATOR_EQUAL_DECLARATION_IN_NAMESPACE_FOR_CLASS(NxA::Serato, Database);
        NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_CLASS(NxA::Serato, Database);

    public:
        #pragma mark Factory Methods
        static Database::Pointer databaseWithFileAndVolume(const String& seratoFolderPath,
                                                           const String& volume);
        static Database::Pointer databaseWithFileVolumeAndRootCrate(const String& seratoFolderPath,
                                                                    const String& volume,
                                                                    Crate& rootCrate);

        #pragma mark Class Methods
        static String::Pointer versionAsStringForDatabaseIn(const String& seratoFolderPath);
        static String::Pointer seratoFolderPathForFolder(const String& folderPath);
        static String::Pointer databaseFilePathForSeratoFolder(const String& seratoFolderPath);
        static boolean containsAValidSeratoFolder(const String& folderPath);

        #pragma mark Instance Methods
        timestamp databaseModificationDateInSecondsSince1970(void) const;
        timestamp rootCrateModificationDateInSecondsSince1970(void) const;

        Crate& rootCrate(void) const;
        const NxA::ArrayContainer<Track>& tracks(void) const;
        NxA::Pointer<NxA::ArrayContainer<Track>> removeAndReturnTracks(void);
        NxA::String::PointerToConst volume(void) const;

        void removeTrackEntry(TrackEntry& trackEntry);
        void removeCrate(Crate& crate);

        void addTrack(Track& track);
        void removeTrack(Track& track);

        void saveIfModified(void) const;
    };
} }
