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

    class Crate : public NxA::Object {
        NXA_GENERATED_DECLARATIONS_FOR(NxA::Serato, Crate);

    public:
        #pragma mark Factory Methods
        static Crate::Pointer crateWithNameInFolderOnVolume(String::ConstPointer const& crateFullName,
                                                            String::ConstPointer const& seratoFolderPath,
                                                            String::ConstPointer const& volumePath);

        #pragma mark Class Methods
        static bool isAValidCrateName(String::ConstPointer const& crateFullName,
                                      String::ConstPointer const& seratoFolderPath);
        static bool isASmartCrateName(String::ConstPointer const& crateFullName,
                                      String::ConstPointer const& seratoFolderPath);
        static void addCrateAsChildOfCrate(Crate::Pointer const& crate, Crate::Pointer const& parentCrate);
        static void destroy(Crate::Pointer const& crate);

        #pragma mark Instance Methods
        String::ConstPointer const& crateName(void) const;
        String::ConstPointer const& crateFullName(void) const;
        void addFullCrateNameWithPrefixAndRecurseToChildren(String::Pointer const& destination, const char* prefix) const;

        TrackEntry::Array::ConstPointer const& trackEntries(void) const;
        Crate::Array::ConstPointer const& crates(void) const;

        void addTrackEntry(Serato::TrackEntry::Pointer const& trackEntry);

        bool hasParentCrate(void) const;
        Crate::Pointer parentCrate(void) const;

        String::ConstPointer const& crateFilePath(void) const;

        void resetModificationFlags();

        void loadFromFile(void);
        void saveIfModifiedAndRecurseToChildren(void) const;
        bool childrenCratesWereModified(void) const;

        void removeChildrenCrate(Crate::Pointer const& crate);
        TrackEntry::Array::Pointer removeAndReturnTrackEntries(void);
        Crate::Array::Pointer removeAndReturnChildrenCrates(void);
    };
} }
