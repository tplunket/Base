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

#include "SeratoDB/ObjectTag.hpp"
#include <SeratoDB/DbUtility.hpp>

#include <vector>

namespace NxA { namespace Serato {
    #pragma mark Forward declarations
    class TrackEntry;

    #pragma mark Containers
    typedef std::unique_ptr<TrackEntry> TrackEntryPtr;
    typedef std::vector<TrackEntryPtr> TrackEntryVector;
    typedef std::unique_ptr<TrackEntryVector> TrackEntryVectorPtr;

    #pragma mark Class Declaration
    class TrackEntry
    {
    private:
        #pragma mark Private Instance Variable
        TagPtr p_trackEntryTag;
        ConstStringPtr p_rootVolumePath;

        #pragma mark Private Instance Methods
        bool p_containsAValidTrackEntryTag(void) const;

    public:
        #pragma mark Constructors
        explicit TrackEntry(TagPtr trackEntryTag, const char* locatedOnVolumePath) :
                            p_trackEntryTag(std::move(trackEntryTag)),
                            p_rootVolumePath(std::make_unique<std::string>(locatedOnVolumePath)) { };
        explicit TrackEntry(const char* trackPath, const char* locatedOnVolumePath);

        #pragma mark Instance Methods
        ConstStringPtr trackFilePath(void) const;

        const Tag& tagForEntry(void) const;

        void destroy(void);
    };
} }
