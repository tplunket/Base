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

    class Track : public NxA::Object {
        NXA_GENERATED_DECLARATIONS_FOR(NxA::Serato, Track);

    public:
        #pragma mark Factory Methods
        static Track::Pointer trackWithTagOnVolume(NxA::Pointer<Tag> const& trackTag, String::ConstPointer const& locatedOnVolumePath);
        static Track::Pointer trackWithFileAtOnVolume(String::ConstPointer const& trackFilePath, String::ConstPointer const& locatedOnVolumePath);

        #pragma mark Instance Methods
        String::Pointer trackFilePath(void) const;

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
        count size(void) const;
        String::ConstPointer const& bitRate(void) const;
        String::ConstPointer const& sampleRate(void) const;
        String::ConstPointer const& bpm(void) const;
        String::ConstPointer const& year(void) const;
        count trackNumber(void) const;
        count discNumber(void) const;
        timestamp dateModifiedInSecondsSinceJanuary1st1970(void) const;
        timestamp dateAddedInSecondsSinceJanuary1st1970(void) const;

        CueMarker::Array::ConstPointer const& cueMarkers(void) const;
        LoopMarker::Array::ConstPointer const& loopMarkers(void) const;
        GridMarker::Array::ConstPointer const& gridMarkers(void) const;

        void setTitle(String::ConstPointer const& title);
        void setArtist(String::ConstPointer const& artist);
        void setAlbum(String::ConstPointer const& album);
        void setGenre(String::ConstPointer const& genre);
        void setComments(String::ConstPointer const& comments);
        void setGrouping(String::ConstPointer const& grouping);
        void setRemixer(String::ConstPointer const& remixer);
        void setRecordLabel(String::ConstPointer const& recordLabel);
        void setComposer(String::ConstPointer const& composer);
        void setKey(String::ConstPointer const& key);
        void setBpm(String::ConstPointer const& bpm);
        void setYear(String::ConstPointer const& year);
        void setTrackNumber(count trackNumber);
        void setDiscNumber(count discNumber);
        void setDateModifiedInSecondsSinceJanuary1st1970(timestamp dateModified);
        void setDateAddedInSecondsSinceJanuary1st1970(timestamp dateAdded);

        void setCueMarkers(CueMarker::Array::Pointer const& markers);
        void setLoopMarkers(LoopMarker::Array::Pointer const& markers);
        void setGridMarkers(GridMarker::Array::Pointer const& markers);

        bool wasModified(void) const;
        void addTo(Blob::Pointer const& destination) const;
        void saveToTrackFile(void);

        void destroy(void);
    };
} }
