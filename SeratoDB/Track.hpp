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

#include <SeratoDb/Tags/Tag.hpp>
#include <SeratoDb/TrackFiles/TrackFile.hpp>

#include <Base/String.hpp>

#include <string>
#include <vector>

namespace NxA { namespace Serato {
    #pragma mark Forward declarations
    class Track;

    #pragma mark Containers
    typedef std::unique_ptr<Track> TrackPtr;
    typedef std::vector<TrackPtr> TrackVector;
    typedef std::unique_ptr<TrackVector> TrackVectorPtr;

    #pragma mark Class Declaration
    class Track
    {
    private:
        #pragma mark Private Instance Variables
        TagPtr p_trackTag;
        TrackFilePtr p_trackFile;
        String::ConstPointer p_rootFolder;

        bool p_wasModified;

        CueMarkerVectorPtr p_cueMarkers;
        LoopMarkerVectorPtr p_loopMarkers;
        GridMarkerVectorPtr p_gridMarkers;

        #pragma mark Private Instance Methods
        inline bool p_containsAValidTrackTag(void) const;

        String::ConstPointer const& p_stringForSubTagForIdentifier(const uint32_t& identifier) const;
        String::ConstPointer const& p_pathForSubTagForIdentifier(const uint32_t& identifier) const;
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
        explicit Track(TagPtr trackTag, const char* locatedOnVolumePath);
        explicit Track(const char* trackFilePath, const char* locatedOnVolumePath);

        #pragma mark Instance Methods
        String::ConstPointer trackFilePath(void) const;

        String::ConstPointer const& title(void) const;
        String::ConstPointer const& artist(void) const;
        String::ConstPointer const& album(void) const;
        String::ConstPointer const& genre(void) const;
        String::ConstPointer const& comments(void) const;
        String::ConstPointer const& grouping(void) const;
        String::ConstPointer const& remixer(void) const;
        String::ConstPointer const& recordLabel(void) const;
        String::ConstPointer const& composer(void) const;
        String::ConstPointer const& key(void) const;
        String::ConstPointer const& length(void) const;
        const uint32_t& sizeInBytes(void) const;
        String::ConstPointer const& bitRate(void) const;
        String::ConstPointer const& sampleRate(void) const;
        String::ConstPointer const& bpm(void) const;
        String::ConstPointer const& year(void) const;
        const uint32_t& trackNumber(void) const;
        const uint32_t& discNumber(void) const;
        const uint32_t& dateModifiedInSecondsSinceJanuary1st1970(void) const;
        const uint32_t& dateAddedInSecondsSinceJanuary1st1970(void) const;

        const CueMarkerVector& cueMarkers(void) const;
        const LoopMarkerVector& loopMarkers(void) const;
        const GridMarkerVector& gridMarkers(void) const;

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

        void setCueMarkers(CueMarkerVectorPtr markers);
        void setLoopMarkers(LoopMarkerVectorPtr markers);
        void setGridMarkers(GridMarkerVectorPtr markers);

        bool wasModified(void) const;
        void addTo(Blob::Pointer const& destination) const;
        void saveToTrackFile(void);

        void destroy(void);
    };
} }

