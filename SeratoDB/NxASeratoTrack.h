//
//  NxASeratoTrack.h
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

#ifndef __SeratoDB__NxASeratoTrack__
#define __SeratoDB__NxASeratoTrack__

#include <SeratoDB/NxASeratoTag.h>
#include <SeratoDB/NxASeratoTrackFile.h>

#include <vector>

namespace NxA {
    #pragma mark Forward declarations
    class SeratoTrack;

    #pragma mark Containers
    typedef std::auto_ptr<SeratoTrack> SeratoTrackAutoPtr;
    typedef std::vector<SeratoTrackAutoPtr> SeratoTrackVector;

    #pragma mark Class Declaration
    class SeratoTrack
    {
    private:
        #pragma mark Private Instance Variables
        SeratoTagAutoPtr p_trackTag;
        SeratoTrackFileAutoPtr p_trackFile;
        StringAutoPtr p_rootFolder;

        SeratoCueMarkerVectorAutoPtr p_cueMarkers;
        SeratoLoopMarkerVectorAutoPtr p_loopMarkers;

        #pragma mark Private Instance Methods
        bool p_containsAValidTag(void) const;
        StringAutoPtr p_stringForSubTagIdentifierOrEmptyIfNotFound(uint32_t identifier) const;
        StringAutoPtr p_pathForSubTagIdentifierOrEmptyIfNotFound(uint32_t identifier) const;
        uint32_t p_uint32ForSubTagIdentifierOrZeroIfNotFound(uint32_t identifier) const;
        uint32_t p_uint32ForStringSubTagIdentifierOrZeroIfNotFound(uint32_t identifier) const;

        void p_loadTrackFile(void);
        void p_unloadTrackFile(void);
        void p_readMarkersIfNotAlreadyRead(void);

    public:
        #pragma mark Constructors
        SeratoTrack(const SeratoTag* trackTag, const char* rootDirectoryPath);

        #pragma mark Instance Methods
        StringAutoPtr trackFilePath(void) const;
        StringAutoPtr title(void) const;
        StringAutoPtr artist(void) const;
        StringAutoPtr album(void) const;
        StringAutoPtr genre(void) const;
        StringAutoPtr comments(void) const;
        StringAutoPtr grouping(void) const;
        StringAutoPtr remixer(void) const;
        StringAutoPtr recordLabel(void) const;
        StringAutoPtr composer(void) const;
        StringAutoPtr key(void) const;
        StringAutoPtr  length(void) const;
        size_t sizeInBytes(void) const;
        StringAutoPtr bitRate(void) const;
        StringAutoPtr sampleRate(void) const;
        StringAutoPtr bpm(void) const;
        StringAutoPtr year(void) const;
        uint32_t trackNumber(void) const;
        uint32_t discNumber(void) const;
        uint32_t dateModifiedInSecondsSinceJanuary1st1970(void) const;
        uint32_t dateAddedInSecondsSinceJanuary1st1970(void) const;

        const SeratoCueMarkerVector& cueMarkers(void);
        const SeratoLoopMarkerVector& loopMarkers(void);
    };
}

#endif /* defined(__SeratoDB__NxASeratoTrack__) */
