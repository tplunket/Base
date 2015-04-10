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

#ifndef __SeratoDB__NxASeratoCrate__
#define __SeratoDB__NxASeratoCrate__

#include <SeratoDB/NxASeratoTag.h>
#include <SeratoDB/NxASeratoTrackEntry.h>
#include <SeratoDB/NxASeratoDbUtility.h>

namespace NxA {
    #pragma mark Forward declarations
    class SeratoCrate;

    #pragma mark Containers
    typedef std::unique_ptr<SeratoCrate> SeratoCratePtr;
    typedef std::vector<SeratoCratePtr> SeratoCrateVector;
    typedef std::unique_ptr<SeratoCrateVector> SeratoCrateVectorPtr;

    #pragma mark Class Declaration
    class SeratoCrate
    {
    private:
        #pragma mark Private Instance Variables
        ConstStringPtr p_crateName;
        ConstStringPtr p_crateFullName;
        ConstStringPtr p_rootVolumePath;
        ConstStringPtr p_crateFilePath;

        bool p_tracksWereModified;
        bool p_cratesWereModified;

        SeratoCrate* p_parentCrate;
        SeratoCrateVectorPtr p_childrenCrates;
        SeratoTrackEntryVectorPtr p_trackEntries;

        ConstSeratoTagVector p_otherTags;

        #pragma mark Private Instance Methods
        void p_storeTrackTag(SeratoTagPtr tag);
        void p_storeOtherTag(SeratoTagPtr tag);

        void p_markCratesAsModified();

    public:
        #pragma mark Constructors
        explicit SeratoCrate(const char* crateFullName,
                             const char* inSeratoFolderPath,
                             const char* locatedOnVolumePath);

        #pragma mark Class Methods
        static bool isAValidCrateName(const char* crateFullName, const char* seratoFolderPath);
        static bool isASmartCrateName(const char* crateFullName, const char* seratoFolderPath);

        #pragma mark Instance Methods
        const std::string& crateName(void) const;
        const std::string& crateFullName(void) const;
        void addFullCrateNameWithPrefixAndRecurseToChildren(std::string& destination, const char* prefix) const;

        const SeratoTrackEntryVector& trackEntries(void) const;
        const SeratoCrateVector& crates(void) const;

        void resetModificationFlags();

        void loadFromFile(void);
        void deleteCrateFile(void);
        void saveIfModifiedAndRecurseToChildren(void) const;
        bool childrenCratesWereModified(void) const;

        void addTrackEntry(SeratoTrackEntryPtr trackEntry);
        void addChildCrate(SeratoCratePtr crate);

        SeratoTrackEntryVectorPtr removeAndReturnTrackEntries(void);
        SeratoCrateVectorPtr removeAndReturnChildrenCrates(void);
    };
}

#endif /* defined(__SeratoDB__NxASeratoCrate__) */
