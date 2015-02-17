//
//  NxASeratoTrackEntry.h
//  SeratoDB
//
//  Created by Didier Malenfant on 2/5/15.
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

#include <SeratoDB/NxASeratoTag.h>
#include <SeratoDB/NxASeratoDbUtility.h>

#include <vector>

namespace NxA {
    #pragma mark Forward declarations
    class SeratoTrackEntry;

    #pragma mark Containers
    typedef std::auto_ptr<const SeratoTrackEntry> SeratoTrackEntryAutoPtr;
    typedef std::vector<SeratoTrackEntryAutoPtr> SeratoTrackEntryVector;

    #pragma mark Class Declaration
    class SeratoTrackEntry
    {
    private:
        #pragma mark Private Instance Variable
        SeratoTagAutoPtr p_trackTag;

        StringVector p_trackPaths;

        #pragma mark Private Instance Methods
        bool p_containsAValidTag(void) const;

    public:
        #pragma mark Constructors
        SeratoTrackEntry(const SeratoTag* trackTag) :
                         p_trackTag(SeratoTagAutoPtr(trackTag)) { };

        #pragma mark Instance Methods
        StringAutoPtr trackFilePath(void) const;
    };
}

#endif /* defined(__SeratoDB__NxASeratoTrackEntry__) */
