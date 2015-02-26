//
//  NxASeratoCrate.h
//  SeratoDB
//
//  Created by Didier Malenfant on 2/4/15.
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
    typedef std::auto_ptr<SeratoCrate> SeratoCrateAutoPtr;
    typedef std::vector<SeratoCrateAutoPtr> SeratoCrateVector;
    typedef std::auto_ptr<SeratoCrateVector> SeratoCrateVectorAutoPtr;

    #pragma mark Class Declaration
    class SeratoCrate
    {
    private:
        #pragma mark Private Instance Variables
        StringAutoPtr p_crateName;
        StringAutoPtr p_crateFullName;
        StringAutoPtr p_rootVolumePath;
        StringAutoPtr p_crateFilePath;

        bool p_tracksWereModified;
        bool p_cratesWereModified;

        SeratoCrate* p_parentCrate;
        SeratoCrateVectorAutoPtr p_childrenCrates;
        SeratoTrackEntryVectorAutoPtr p_trackEntries;

        ConstSeratoTagAutoPtr p_versionTag;
        ConstSeratoTagVector p_otherTags;

        #pragma mark Private Instance Methods
        void p_storeVersionTag(const SeratoTag* tag);
        void p_storeTrackTag(const SeratoTag* tag);
        void p_storeOtherTag(const SeratoTag* tag);

        void p_markCratesAsModified();

    public:
        #pragma mark Constructors
        SeratoCrate(const char* crateFullName,
                    const char* inSeratoFolderPath,
                    const char* locatedOnVolumePath);

        #pragma mark Class Methods
        static bool isAValidCrateName(const char* crateFullName, const char* seratoFolderPath);

        #pragma mark Instance Methods
        StringAutoPtr versionAsString(void) const;

        const std::string& crateName(void) const;
        const std::string& crateFullName(void) const;
        void addFullCrateNameWithPrefixAndRecurseToChildren(std::string& destination, const char* prefix) const;

        const SeratoTrackEntryVector& trackEntries(void) const;
        const SeratoCrateVector& crates(void) const;

        void resetModificationFlags();

        void loadFromFile(void);
        void saveIfModifiedAndRecurseToChildren(void) const;
        bool childrenCratesWereModified(void) const;

        void addTrackEntry(SeratoTrackEntryAutoPtr trackEntry);
        void addChildCrate(SeratoCrateAutoPtr crate);

        SeratoTrackEntryVectorAutoPtr removeAndReturnTrackEntries(void);
        SeratoCrateVectorAutoPtr removeAndReturnChildrenCrates(void);
    };
}

#endif /* defined(__SeratoDB__NxASeratoCrate__) */
