//
//  NxASeratoDatabaseTrack.h
//  SeratoDB
//
//  Created by Didier Malenfant on 2/3/15.
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

#ifndef __SeratoDB__NxASeratoDatabaseTrack__
#define __SeratoDB__NxASeratoDatabaseTrack__

#include <SeratoDB/NxASeratoTag.h>

#include <vector>

namespace NxA {
    #pragma mark Forward declarations
    class SeratoDatabaseTrack;

    #pragma mark Containers
    typedef std::auto_ptr<const SeratoDatabaseTrack> SeratoDatabaseTrackAutoPtr;
    typedef std::vector<SeratoDatabaseTrackAutoPtr> SeratoDatabaseTrackVector;

    #pragma mark Class Declaration
    class SeratoDatabaseTrack
    {
    private:
        #pragma mark Private Instance Variable
        SeratoTagAutoPtr p_trackTag;

        #pragma mark Private Instance Methods
        bool p_containsAValidTag(void) const;
        StringAutoPtr p_stringForSubTagIdentifierOrEmptyIfNotFound(uint32_t identifier) const;
        StringAutoPtr p_pathForSubTagIdentifierOrEmptyIfNotFound(uint32_t identifier) const;
        uint32_t p_uint32ForSubTagIdentifierOrZeroIfNotFound(uint32_t identifier) const;
        uint32_t p_uint32ForStringSubTagIdentifierOrZeroIfNotFound(uint32_t identifier) const;

    public:
        #pragma mark Constructors
        SeratoDatabaseTrack(const SeratoTag* trackTag) :
                            p_trackTag(SeratoTagAutoPtr(trackTag)) { };

        #pragma mark Instance Methods
        StringAutoPtr title(void) const;
        StringAutoPtr artist(void) const;
        StringAutoPtr trackFilePath(void) const;
        StringAutoPtr album(void) const;
        StringAutoPtr genre(void) const;
        StringAutoPtr comments(void) const;
        StringAutoPtr grouping(void) const;
        StringAutoPtr remix(void) const;
        StringAutoPtr recordLabel(void) const;
        StringAutoPtr composer(void) const;
        StringAutoPtr key(void) const;
        StringAutoPtr  trackLength(void) const;
        size_t trackSizeInBytes(void) const;
        StringAutoPtr trackBitRate(void) const;
        StringAutoPtr trackSampleRate(void) const;
        StringAutoPtr trackBpm(void) const;
        StringAutoPtr trackYear(void) const;
        uint32_t trackNumber(void) const;
        uint32_t trackDiscNumber(void) const;
        uint32_t trackDateModifiedInSecondsSinceJanuary1st1970(void) const;
        uint32_t trackDateAddedInSecondsSinceJanuary1st1970(void) const;
    };
}

#endif /* defined(__SeratoDB__NxASeratoDatabaseTrack__) */
