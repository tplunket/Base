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

#include <SeratoDb/Markers/CueMarker.hpp>
#include <SeratoDb/Markers/LoopMarker.hpp>
#include <SeratoDb/Markers/GridMarker.hpp>

#include <Base/Base.hpp>

namespace NxA { namespace Serato {
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR(TrackFile);

    class TrackFile : public Object {
        NXA_GENERATED_DECLARATIONS_FOR(NxA::Serato, TrackFile);

    public:
        #pragma mark Instance Methods
        virtual String::Pointer title(void) const;
        virtual String::Pointer artist(void) const;
        virtual String::Pointer genre(void) const;
        virtual bool hasKey(void) const = 0;
        virtual String::Pointer key(void) const = 0;
        virtual String::Pointer comments(void) const;
        virtual String::Pointer album(void) const;
        virtual String::Pointer composer(void) const;
        virtual String::Pointer grouping(void) const = 0;
        virtual String::Pointer bpm(void) const;

        virtual count size(void) const;
        virtual uinteger32 lengthInMilliseconds(void) const;
        virtual uinteger32 bitRateInKiloBitsPerSecond(void) const;
        virtual uinteger32 bitDepthInBitsOrZeroIfNotApplicable(void) const;
        virtual uinteger32 sampleRateInSamplesPerSecond(void) const;
        virtual count trackNumber(void) const;

        // -- TODO: Look into adding discNumber and numberOfAlbumTracks if they are available.

        virtual bool hasRecordLabel(void) const = 0;
        virtual String::Pointer recordLabel(void) const = 0;
        virtual bool hasRemixer(void) const = 0;
        virtual String::Pointer remixer(void) const = 0;
        virtual String::Pointer yearReleased(void) const = 0;

        virtual Blob::Pointer artwork(void) const = 0;
        
        CueMarker::Array::ConstPointer const& cueMarkers(void) const;
        LoopMarker::Array::ConstPointer const& loopMarkers(void) const;
        GridMarker::Array::ConstPointer const& gridMarkers(void) const;

        virtual void setTitle(String::ConstPointer const& title);
        virtual void setArtist(String::ConstPointer const& artist);
        virtual void setGenre(String::ConstPointer const& genre);
        virtual void setKey(String::ConstPointer const& key) = 0;
        virtual void setComments(String::ConstPointer const& comments);
        virtual void setAlbum(String::ConstPointer const& album);
        virtual void setComposer(String::ConstPointer const& composer);
        virtual void setGrouping(String::ConstPointer const& grouping) = 0;
        virtual void setBpm(String::ConstPointer const& bpm);
        virtual void setTrackNumber(count trackNumber);
        virtual void setRecordLabel(String::ConstPointer const& recordLabel) = 0;
        virtual void setRemixer(String::ConstPointer const& remixer) = 0;
        virtual void setYearReleased(String::ConstPointer const& year) = 0;

        virtual void setArtwork(Blob::ConstPointer const& artwork) = 0;

        void setCueMarkers(CueMarker::Array::Pointer const& markers);
        void setLoopMarkers(LoopMarker::Array::Pointer const& markers);
        void setGridMarkers(GridMarker::Array::Pointer const& markers);

        void saveChanges(void);
    };
} }
