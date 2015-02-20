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
        StringAutoPtr p_crateFullPathName;
        StringAutoPtr p_rootVolumePath;

        SeratoCrateVector p_childrenCrates;

        SeratoTagAutoPtr p_versionTag;
        SeratoTrackEntryVector p_trackEntries;
        SeratoTagVector p_otherTags;

        #pragma mark Private Instance Methods
        void p_storeVersionTag(const SeratoTag* tag);
        void p_storeTrackTag(const SeratoTag* tag);
        void p_storeOtherTag(const SeratoTag* tag);

    public:
        #pragma mark Constructors
        SeratoCrate(const char* crateFullPathName);
        SeratoCrate(const char* crateFullPathName,
                    const char* readItFromSeratoFolderPath,
                    const char* locatedOnVolumePath);

        #pragma mark Class Methods
        static bool isAValidCrateName(const char* crateFullPathName, const char* seratoFolderPath);

        #pragma mark Instance Methods
        StringAutoPtr versionAsString(void) const;

        const std::string& crateName(void) const;
        const std::string& crateFullPathName(void) const;

        const SeratoTrackEntryVector& trackEntries(void) const;
        const SeratoCrateVector& crates(void) const;

        void addChildCrate(SeratoCrateAutoPtr crate);
    };
}

#endif /* defined(__SeratoDB__NxASeratoCrate__) */
