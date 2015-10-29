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

#include "Tags/ObjectTag.hpp"

#include <Base/Base.hpp>

namespace NxA { namespace Serato {
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR_CLASS(Track);

    class Tag;

    class Track : public Object {
        NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_CLASS(NxA::Serato, Track);

    public:
        #pragma mark Factory Methods
        static Track::Pointer trackWithTagOnVolume(ObjectTag& trackTag, const String& locatedOnVolumePath);
        static Track::Pointer trackWithFileAtOnVolume(const String& trackFilePath, const String& locatedOnVolumePath);

        #pragma mark Class Methods
#if NXA_PRINT_DEBUG_INFO
        static void debugPrintString(const String& text, const String& name);
        static void debugPrintUint(uinteger32 value, const String& name);
        static void debugPrintTimeFromMilliseconds(uinteger32 value, const String& name);
        static void debugPrintDate(timestamp value, const String& name);
        static void debugPrint(const Serato::Track& track);
#endif

        #pragma mark Operators
        bool operator==(const Track& other) const;

        #pragma mark Instance Methods
        String::Pointer trackFilePath(void) const;
        timestamp trackFileModificationDateInSecondsSince1970(void) const;

        const String& title(void) const;
        const String& artist(void) const;
        const String& album(void) const;
        const String& genre(void) const;
        const String& comments(void) const;
        const String& grouping(void) const;
        const String& remixer(void) const;
        const String& recordLabel(void) const;
        const String& composer(void) const;
        const String& key(void) const;
        count size(void) const;
        const String& length(void) const;
        const String& bitRate(void) const;
        const String& sampleRate(void) const;

        const String& bpm(void) const;
        const String& year(void) const;
        count trackNumber(void) const;
        count discNumber(void) const;
        timestamp dateModifiedInSecondsSinceJanuary1st1970(void) const;
        timestamp dateAddedInSecondsSinceJanuary1st1970(void) const;

        const CueMarker::ArrayOfConst& cueMarkers(void) const;
        const LoopMarker::ArrayOfConst& loopMarkers(void) const;
        const GridMarker::ArrayOfConst& gridMarkers(void) const;

        void setTitle(const String& title);
        void setArtist(const String& artist);
        void setAlbum(const String& album);
        void setGenre(const String& genre);
        void setComments(const String& comments);
        void setGrouping(const String& grouping);
        void setRemixer(const String& remixer);
        void setRecordLabel(const String& recordLabel);
        void setComposer(const String& composer);
        void setKey(const String& key);
        void setBpm(const String& bpm);
        void setYear(const String& year);
        void setTrackNumber(count trackNumber);
        void setDiscNumber(count discNumber);
        void setDateModifiedInSecondsSinceJanuary1st1970(timestamp dateModified);
        void setDateAddedInSecondsSinceJanuary1st1970(timestamp dateAdded);

        void setSizeInBytes(count size);
        void setLengthInMilliseconds(uinteger32 length);
        void setBitRateInKiloBitsPerSecond(uinteger32 bitRate);
        void setSampleRateInSamplesPerSecond(uinteger32 sampleRate);
        void setArtwork(const Blob& artwork);

        void setCueMarkersWhichWereModifiedOn(CueMarker::ArrayOfConst& markers, timestamp modificationDateInSecondsSince1970);
        void setLoopMarkersWhichWereModifiedOn(LoopMarker::ArrayOfConst& markers, timestamp modificationDateInSecondsSince1970);
        void setGridMarkersWhichWereModifiedOn(GridMarker::ArrayOfConst& markers, timestamp modificationDateInSecondsSince1970);

        boolean needsToUpdateTrackFile(void) const;
        boolean needsToUpdateDatabaseFile(void) const;

        void addTo(Blob& destination) const;
        void saveToTrackFile(void) const;
    };
} }
