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

#include <string>
#include <vector>

namespace NxA {
    #pragma mark Forward declarations
    class SeratoDatabaseTrack;

    #pragma mark Containers
    typedef std::vector<SeratoDatabaseTrack*> SeratoDatabaseTrackVector;

    #pragma mark Class Declaration
    class SeratoDatabaseTrack
    {
    private:
        #pragma mark Private Instance Variable
        SeratoTag* p_trackTag;

        #pragma mark Private Instance Methods
        const std::string* p_stringForSubTagIdentifierOrEmptyIfNotFound(uint32_t identifier) const;
        const std::string* p_pathForSubTagIdentifierOrEmptyIfNotFound(uint32_t identifier) const;
        uint32_t p_uint32ForSubTagIdentifierOrZeroIfNotFound(uint32_t identifier) const;
        uint32_t p_uint32ForStringSubTagIdentifierOrZeroIfNotFound(uint32_t identifier) const;

    public:
        #pragma mark Constructors
        SeratoDatabaseTrack(SeratoTag* trackTag) : p_trackTag(trackTag) { };

        #pragma mark Destructor
        ~SeratoDatabaseTrack();

        #pragma mark Instance Methods
        const std::string* title(void) const;
        const std::string* artist(void) const;
        const std::string* filePath(void) const;
        const std::string* album(void) const;
        const std::string* genre(void) const;
        const std::string* comments(void) const;
        const std::string* grouping(void) const;
        const std::string* remix(void) const;
        const std::string* recordLabel(void) const;
        const std::string* composer(void) const;
        const std::string* key(void) const;
        const std::string* trackLength(void) const;
        size_t trackSizeInBytes(void) const;
        const std::string* trackBitRate(void) const;
        const std::string* trackSampleRate(void) const;
        const std::string* trackBpm(void) const;
        const std::string* trackYear(void) const;
        uint32_t trackNumber(void) const;
        uint32_t trackDiscNumber(void) const;
        uint32_t trackDateModifiedInSecondsSinceJanuary1st1970(void) const;
        uint32_t trackDateAddedInSecondsSinceJanuary1st1970(void) const;
    };
}

#endif /* defined(__SeratoDB__NxASeratoDatabaseTrack__) */
