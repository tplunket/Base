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
        NXA_GENERATED_OPERATOR_EQUAL_DECLARATION_IN_NAMESPACE_FOR_CLASS(NxA::Serato, Crate);
        NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_CLASS(NxA::Serato, Crate);

    public:
        #pragma mark Factory Methods
        static Crate::Pointer crateWithName(const String& crateFullName);

        #pragma mark Class Methods
        static String::Pointer subCratesDirectoryPathInSeratoFolder(const String& seratoFolderPath);
        static boolean isAValidCrateName(const String& crateFullName,
                                      const String& seratoFolderPath);
        static boolean isASmartCrateName(const String& crateFullName,
                                      const String& seratoFolderPath);

        #pragma mark Instance Methods
        const String& crateName(void) const;
        const String& crateFullName(void) const;
        void addFullCrateNameWithPrefixAndRecurseToChildren(String& destination, const char* prefix) const;

        TrackEntry::Array& trackEntries(void);
        Crate::Array& crates(void);

        void addCrate(Crate& crate);
        void removeCrate(Crate& crate);
        Crate::Pointer findOrAddCrateWithName(const String& crateName);

        void addTrackEntry(TrackEntry& trackEntry);
        void removeTrackEntry(TrackEntry& trackEntry);

        boolean hasParentCrate(void) const;
        Crate& parentCrate(void);
        void removeFromParentCrate(void);

        const String::Array& crateFilePaths(void) const;

        void resetModificationFlags();

        void readFromFolderInVolume(const String& seratoFolderPath, const String& volume);
        void saveIfModifiedAndRecurseToChildren(void) const;
        boolean childrenCratesWereModified(void) const;

        TrackEntry::Array::Pointer removeAndReturnTrackEntries(void);
        Crate::Array::Pointer removeAndReturnChildrenCrates(void);
    };
} }
