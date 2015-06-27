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

#include <SeratoDB/TrackEntry.hpp>

#include <Base/Base.hpp>

namespace NxA { namespace Serato {
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR(Crate);

    class Database;

    class Crate : public Object {
        NXA_GENERATED_DECLARATIONS_FOR(NxA::Serato, Crate);

    public:
        #pragma mark Factory Methods
        static Crate::Pointer crateWithNameInFolderOnVolume(const String& crateFullName,
                                                            const String& seratoFolderPath,
                                                            const String& volumePath);

        #pragma mark Class Methods
        static bool isAValidCrateName(const String& crateFullName,
                                      const String& seratoFolderPath);
        static bool isASmartCrateName(const String& crateFullName,
                                      const String& seratoFolderPath);
        static void addCrateAsChildOfCrate(Crate::Pointer& crate, Crate::Pointer& parentCrate);
        static void destroy(Crate::Pointer const& crate);

        #pragma mark Instance Methods
        const String& crateName(void) const;
        const String& crateFullName(void) const;
        void addFullCrateNameWithPrefixAndRecurseToChildren(String& destination, const char* prefix) const;

        TrackEntry::ArrayOfConst::Pointer const& trackEntries(void) const;
        Crate::Array::Pointer const& crates(void) const;

        void addTrackEntry(Serato::TrackEntry::Pointer const& trackEntry);

        bool hasParentCrate(void) const;
        Crate::Pointer parentCrate(void) const;

        const String& crateFilePath(void) const;

        void resetModificationFlags();

        void loadFromFile(void);
        void saveIfModifiedAndRecurseToChildren(void) const;
        bool childrenCratesWereModified(void) const;

        void removeChildrenCrate(Crate::Pointer const& crate);
        TrackEntry::ArrayOfConst::Pointer removeAndReturnTrackEntries(void);
        Crate::Array::Pointer removeAndReturnChildrenCrates(void);
    };
} }
