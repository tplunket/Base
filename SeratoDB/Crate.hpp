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
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR_CLASS(Crate);

    class Database;

    class Crate : public Object {
        NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_CLASS(NxA::Serato, Crate);

    public:
        #pragma mark Factory Methods
        static Crate::Pointer crateWithNameInFolderOnVolume(const String& crateFullName,
                                                            const String& seratoFolderPath,
                                                            const String& volumePath);

        #pragma mark Class Methods
        static boolean isAValidCrateName(const String& crateFullName,
                                      const String& seratoFolderPath);
        static boolean isASmartCrateName(const String& crateFullName,
                                      const String& seratoFolderPath);
        static void destroy(const Crate& crate);

        #pragma mark Instance Methods
        const String& crateName(void) const;
        const String& crateFullName(void) const;
        void addFullCrateNameWithPrefixAndRecurseToChildren(String& destination, const char* prefix) const;

        TrackEntry::Array& trackEntries(void);
        Crate::Array& crates(void);

        void addTrackEntry(Serato::TrackEntry& trackEntry);
        void addCrate(Crate& crate);

        boolean hasParentCrate(void) const;
        Crate& parentCrate(void) const;

        const String& crateFilePath(void) const;

        void resetModificationFlags();

        void loadFromFile(void);
        void saveIfModifiedAndRecurseToChildren(void) const;
        boolean childrenCratesWereModified(void) const;

        void removeChildrenCrate(const Crate& crate);
        TrackEntry::Array::Pointer removeAndReturnTrackEntries(void);
        Crate::Array::Pointer removeAndReturnChildrenCrates(void);
    };
} }
