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

#include <SeratoDB/SeratoTag.h>
#include <SeratoDB/SeratoTrackFile.h>

#include <string>
#include <vector>

namespace NxA {
    #pragma mark Forward declarations
    class SeratoTrack;

    #pragma mark Containers
    typedef std::unique_ptr<SeratoTrack> SeratoTrackPtr;
    typedef std::vector<SeratoTrackPtr> SeratoTrackVector;
    typedef std::unique_ptr<SeratoTrackVector> SeratoTrackVectorPtr;

    #pragma mark Class Declaration
    class SeratoTrack
    {
    private:
        #pragma mark Private Instance Variables
        SeratoTagPtr p_trackTag;
        SeratoTrackFilePtr p_trackFile;
        ConstStringPtr p_rootFolder;

        bool p_wasModified;

        SeratoCueMarkerVectorPtr p_cueMarkers;
        SeratoLoopMarkerVectorPtr p_loopMarkers;
        SeratoGridMarkerVectorPtr p_gridMarkers;

        #pragma mark Private Instance Methods
        inline bool p_containsAValidTrackTag(void) const;

        const std::string& p_stringForSubTagForIdentifier(const uint32_t& identifier) const;
        const std::string& p_pathForSubTagForIdentifier(const uint32_t& identifier) const;
        const uint32_t& p_uint32ForSubTagForIdentifier(const uint32_t& identifier) const;

        void p_setStringForSubTagForIdentifier(const char* value, const uint32_t& identifier);
        void p_setPathForSubTagForIdentifier(const char* value, const uint32_t& identifier);
        void p_setUInt32ForSubTagForIdentifier(const uint32_t& value, const uint32_t& identifier);

        void p_loadTrackFile(void);
        void p_saveTrackFile(void);
        void p_unloadTrackFile(void);

        void p_readMarkers(void);

    public:
        #pragma mark Constructors
        explicit SeratoTrack(SeratoTagPtr trackTag, const char* locatedOnVolumePath);
        explicit SeratoTrack(const char* trackFilePath, const char* locatedOnVolumePath);

        #pragma mark Instance Methods
        ConstStringPtr trackFilePath(void) const;

        const std::string& title(void) const;
        const std::string& artist(void) const;
        const std::string& album(void) const;
        const std::string& genre(void) const;
        const std::string& comments(void) const;
        const std::string& grouping(void) const;
        const std::string& remixer(void) const;
        const std::string& recordLabel(void) const;
        const std::string& composer(void) const;
        const std::string& key(void) const;
        const std::string& length(void) const;
        const uint32_t& sizeInBytes(void) const;
        const std::string& bitRate(void) const;
        const std::string& sampleRate(void) const;
        const std::string& bpm(void) const;
        const std::string& year(void) const;
        const uint32_t& trackNumber(void) const;
        const uint32_t& discNumber(void) const;
        const uint32_t& dateModifiedInSecondsSinceJanuary1st1970(void) const;
        const uint32_t& dateAddedInSecondsSinceJanuary1st1970(void) const;

        const SeratoCueMarkerVector& cueMarkers(void) const;
        const SeratoLoopMarkerVector& loopMarkers(void) const;
        const SeratoGridMarkerVector& gridMarkers(void) const;

        void setTitle(const char* title);
        void setArtist(const char* artist);
        void setAlbum(const char* album);
        void setGenre(const char* genre);
        void setComments(const char* comments);
        void setGrouping(const char* grouping);
        void setRemixer(const char* remixer);
        void setRecordLabel(const char* recordLabel);
        void setComposer(const char* composer);
        void setKey(const char* key);
        void setBpm(const char* bpm);
        void setYear(const char* year);
        void setTrackNumber(const uint32_t& trackNumber);
        void setDiscNumber(const uint32_t& discNumber);
        void setDateModifiedInSecondsSinceJanuary1st1970(const uint32_t& dateModified);
        void setDateAddedInSecondsSinceJanuary1st1970(const uint32_t& dateAdded);

        void setCueMarkers(SeratoCueMarkerVectorPtr markers);
        void setLoopMarkers(SeratoLoopMarkerVectorPtr markers);
        void setGridMarkers(SeratoGridMarkerVectorPtr markers);

        bool wasModified(void) const;
        void addTo(CharVector& destination) const;
        void saveToTrackFile(void);

        void destroy(void);
    };
}
