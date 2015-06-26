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

#include "Markers/CueMarker.hpp"
#include "Markers/LoopMarker.hpp"
#include "Markers/GridMarker.hpp"

#include <Base/Base.hpp>

namespace NxA { namespace Serato {
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR(Track);

    class Tag;

    class Track : public Object, NXA_INHERITANCE_FOR(Track) {
        NXA_GENERATED_DECLARATIONS_FOR(NxA::Serato, Track);

    public:
        #pragma mark Factory Methods
        static Track::Pointer trackWithTagOnVolume(NxA::Pointer<Tag> const& trackTag, String::Pointer const& locatedOnVolumePath);
        static Track::Pointer trackWithFileAtOnVolume(String::Pointer const& trackFilePath, String::Pointer const& locatedOnVolumePath);

        #pragma mark Instance Methods
        String::Pointer trackFilePath(void) const;

        String::Pointer const& title(void) const;
        String::Pointer const& artist(void) const;
        String::Pointer const& album(void) const;
        String::Pointer const& genre(void) const;
        String::Pointer const& comments(void) const;
        String::Pointer const& grouping(void) const;
        String::Pointer const& remixer(void) const;
        String::Pointer const& recordLabel(void) const;
        String::Pointer const& composer(void) const;
        String::Pointer const& key(void) const;
        String::Pointer const& length(void) const;
        count size(void) const;
        String::Pointer const& bitRate(void) const;
        String::Pointer const& sampleRate(void) const;
        String::Pointer const& bpm(void) const;
        String::Pointer const& year(void) const;
        count trackNumber(void) const;
        count discNumber(void) const;
        timestamp dateModifiedInSecondsSinceJanuary1st1970(void) const;
        timestamp dateAddedInSecondsSinceJanuary1st1970(void) const;

        CueMarker::Array::Pointer const& cueMarkers(void) const;
        LoopMarker::Array::Pointer const& loopMarkers(void) const;
        GridMarker::Array::Pointer const& gridMarkers(void) const;

        void setTitle(String::Pointer const& title);
        void setArtist(String::Pointer const& artist);
        void setAlbum(String::Pointer const& album);
        void setGenre(String::Pointer const& genre);
        void setComments(String::Pointer const& comments);
        void setGrouping(String::Pointer const& grouping);
        void setRemixer(String::Pointer const& remixer);
        void setRecordLabel(String::Pointer const& recordLabel);
        void setComposer(String::Pointer const& composer);
        void setKey(String::Pointer const& key);
        void setBpm(String::Pointer const& bpm);
        void setYear(String::Pointer const& year);
        void setTrackNumber(count trackNumber);
        void setDiscNumber(count discNumber);
        void setDateModifiedInSecondsSinceJanuary1st1970(timestamp dateModified);
        void setDateAddedInSecondsSinceJanuary1st1970(timestamp dateAdded);

        void setCueMarkers(CueMarker::Array::Pointer const& markers);
        void setLoopMarkers(LoopMarker::Array::Pointer const& markers);
        void setGridMarkers(GridMarker::Array::Pointer const& markers);

        bool wasModified(void) const;
        void addTo(Blob::Pointer& destination) const;
        void saveToTrackFile(void) const;

        void destroy(void);
    };
} }
