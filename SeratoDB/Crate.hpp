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

#include <SeratoDB/Tag.hpp>
#include <SeratoDB/TrackEntry.hpp>
#include <SeratoDB/DbUtility.hpp>

namespace NxA {
    namespace Serato {
    #pragma mark Forward declarations
    class Crate;
    class Database;

    #pragma mark Containers
    typedef std::unique_ptr<Crate> CratePtr;
    typedef std::vector<CratePtr> CrateVector;
    typedef std::unique_ptr<CrateVector> CrateVectorPtr;

    #pragma mark Class Declaration
    class Crate
    {
    private:
        #pragma mark Private Instance Variables
        ConstStringPtr p_crateName;
        ConstStringPtr p_crateFullName;
        ConstStringPtr p_rootVolumePath;
        ConstStringPtr p_crateFilePath;

        bool p_tracksWereModified;
        bool p_cratesWereModified;

        Database& p_parentDatabase;

        Crate* p_parentCrate;
        CrateVectorPtr p_childrenCrates;
        TrackEntryVectorPtr p_trackEntries;

        ConstTagVector p_otherTags;

        #pragma mark Private Instance Methods
        void p_storeTrackTag(TagPtr tag);
        void p_storeOtherTag(TagPtr tag);

        void p_markCratesAsModified();

    public:
        #pragma mark Constructors
        explicit Crate(const char* crateFullName,
                             const char* inSeratoFolderPath,
                             const char* locatedOnVolumePath,
                             Database& database);

        #pragma mark Class Methods
        static bool isAValidCrateName(const char* crateFullName, const char* seratoFolderPath);
        static bool isASmartCrateName(const char* crateFullName, const char* seratoFolderPath);

        #pragma mark Instance Methods
        const std::string& crateName(void) const;
        const std::string& crateFullName(void) const;
        void addFullCrateNameWithPrefixAndRecurseToChildren(std::string& destination, const char* prefix) const;

        const TrackEntryVector& trackEntries(void) const;
        const CrateVector& crates(void) const;

        bool hasParentCrate(void) const;
        Crate& parentCrate(void) const;

        void resetModificationFlags();

        void loadFromFile(void);
        void saveIfModifiedAndRecurseToChildren(void) const;
        bool childrenCratesWereModified(void) const;

        void addTrackEntry(TrackEntryPtr trackEntry);
        void addChildCrate(CratePtr crate);

        CratePtr removeAndReturnChildrenCrate(Crate& crate);
        TrackEntryVectorPtr removeAndReturnTrackEntries(void);
        CrateVectorPtr removeAndReturnChildrenCrates(void);

        void destroy(void);
    };
}
}
