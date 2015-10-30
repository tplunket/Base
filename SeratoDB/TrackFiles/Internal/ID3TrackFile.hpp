//
//  Copyright (c) 2015 Next Audio Labs, LLC. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy of
//  this software and associated documentation files (the "Software"), to deal in the
//  Software without restriction, including without limitation the rights to use, copy,
//  modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
//  and to permit persons to whom the Software is furnished to do so, subject to the
//  following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
//  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
//  PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
//  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#pragma once

#include "TrackFiles/ID3TrackFile.hpp"
#include "TrackFiles/Internal/TrackFile.hpp"

#include <Base/Base.hpp>

#include <id3v2tag.h>
#include <generalencapsulatedobjectframe.h>

namespace NxA { namespace Serato { namespace Internal {
    #pragma mark Constants
    constexpr const character* id3KeyFrameName = "TKEY";
    constexpr const character* id3ComposerFrameName = "TCOM";
    constexpr const character* id3GroupingFrameName = "TIT1";
    constexpr const character* id3BpmFrameName = "TBPM";
    constexpr const character* id3RecordLabelFrameName = "TPUB";
    constexpr const character* id3RemixerFrameName = "TPE4";
    constexpr const character* id3RatingFrameName = "POPM";
    constexpr const character* id3ArtworkFrameName = "APIC";
    constexpr const character* id3OriginalReleaseTimeFrameName = "TDOR";
    constexpr const character* id3RecordingTimeFrameName = "TDRC";
    constexpr const character* id3ReleaseTimeFrameName = "TDRL";

    constexpr const character* id3MarkersFrameDescription = "Serato Markers_";

    struct ID3TrackFile : public TrackFile {
        NXA_GENERATED_INTERNAL_DECLARATIONS_WITHOUT_CONSTRUCTORS_FOR(NxA::Serato, ID3TrackFile);

        #pragma mark Constructor & Destructors
        ID3TrackFile(const String& path, const TagLibFilePointer& newFile);

        #pragma mark Class Methods
        static boolean isAValidGeobFrame(const TagLib::ID3v2::GeneralEncapsulatedObjectFrame* frame);
        static TagLib::ID3v2::FrameList::Iterator frameInListWithDescription(TagLib::ID3v2::FrameList& list,
                                                                             const String& description);
        static String::Pointer stringValueForFrameNamedInTag(const character* name, const TagLib::ID3v2::Tag* id3v2Tag);
        static integer integerValueForFrameNamedInTag(const character* name, const TagLib::ID3v2::Tag* id3v2Tag);
        static void setStringValueForFrameNamedInTag(const String& value, const character* name, TagLib::ID3v2::Tag* id3v2Tag);
        static void setIntegerValueForFrameNamedInTag(integer value, const character* name, TagLib::ID3v2::Tag* id3v2Tag);
        static void removeGEOBFrameNamedInTag(const String& name, TagLib::ID3v2::Tag* id3v2Tag);
        static void replaceMarkersV2FrameInTagWith(TagLib::ID3v2::Tag* id3v2Tag, const String& base64MarkersData);
        static void replaceGridMarkersFrameInTagWith(TagLib::ID3v2::Tag* id3v2Tag, const Blob& gridMarkersData);
        static String::Pointer releaseDateFromTag(TagLib::ID3v2::Tag* id3v2Tag);
        static void setReleaseDateForTag(const String& date, TagLib::ID3v2::Tag* id3v2Tag);

        #pragma mark Instance Variables
        TagLib::ID3v2::Tag* id3v2Tag;

        #pragma mark Instance Methods
        String::Pointer stringValueForFrameNamed(const character* name) const;
        integer integerValueForFrameNamed(const character* name) const;
        void setStringValueForFrameNamed(const String& value, const character* name);
        void setIntegerValueForFrameNamed(integer value, const character* name);

        void removeArtwork(void);

        void readMarkers(void);
        void writeMarkers(void);
    };
} } }
