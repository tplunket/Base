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

#ifndef __SeratoDB__NxASeratoTrackEntry__
#define __SeratoDB__NxASeratoTrackEntry__

#include "SeratoDB/NxASeratoObjectTag.h"
#include <SeratoDB/NxASeratoDbUtility.h>

#include <vector>

namespace NxA {
    #pragma mark Forward declarations
    class SeratoTrackEntry;

    #pragma mark Containers
    typedef std::unique_ptr<SeratoTrackEntry> SeratoTrackEntryPtr;
    typedef std::vector<SeratoTrackEntryPtr> SeratoTrackEntryVector;
    typedef std::unique_ptr<SeratoTrackEntryVector> SeratoTrackEntryVectorPtr;

    #pragma mark Class Declaration
    class SeratoTrackEntry
    {
    private:
        #pragma mark Private Instance Variable
        SeratoTagPtr p_trackEntryTag;
        ConstStringPtr p_rootVolumePath;

        #pragma mark Private Instance Methods
        bool p_containsAValidTrackEntryTag(void) const;

    public:
        #pragma mark Constructors
        explicit SeratoTrackEntry(SeratoTagPtr trackEntryTag, const char* locatedOnVolumePath) :
                                  p_trackEntryTag(std::move(trackEntryTag)),
                                  p_rootVolumePath(std::make_unique<std::string>(locatedOnVolumePath)) { };
        explicit SeratoTrackEntry(const char* trackPath, const char* locatedOnVolumePath);

        #pragma mark Instance Methods
        ConstStringPtr trackFilePath(void) const;

        const SeratoTag& tagForEntry(void) const;
    };
}

#endif /* defined(__SeratoDB__NxASeratoTrackEntry__) */
