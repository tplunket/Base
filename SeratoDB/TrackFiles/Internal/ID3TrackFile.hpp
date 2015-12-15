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

#include <tag.h>
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
        NXA_GENERATED_DESTRUCTOR_FOR_CLASS(ID3TrackFile);

        #pragma mark Constructor & Destructors
        ID3TrackFile(const String& path);

        #pragma mark Class Methods
        static boolean isAValidGeobFrame(const TagLib::ID3v2::GeneralEncapsulatedObjectFrame& frame);
        static TagLib::ID3v2::FrameList::Iterator frameInListWithDescription(TagLib::ID3v2::FrameList& list,
                                                                             const String& description);
        static String::Pointer stringValueForFrameNamedInTag(const character* name, const TagLib::ID3v2::Tag& tag);
        static integer integerValueForFrameNamedInTag(const character* name, const TagLib::ID3v2::Tag& tag);
        static integer ratingValueForRatingFrameInTag(const TagLib::ID3v2::Tag& tag);
        static void setStringValueForFrameNamedInTag(const String& value, const character* name, TagLib::ID3v2::Tag& tag);
        static void setIntegerValueForFrameNamedInTag(integer value, const character* name, TagLib::ID3v2::Tag& tag);
        static void setRatingValueForRatingFrameInTag(integer value, TagLib::ID3v2::Tag& tag);
        static void removeGEOBFrameNamedInTag(const String& name, TagLib::ID3v2::Tag& tag);
        static void parseMarkersInTagToTrackFile(const TagLib::ID3v2::Tag& tag, TrackFile& trackFile);
        static void replaceMarkersFrameInTagWithEmptyFrame(TagLib::ID3v2::Tag& tag);
        static void replaceMarkersV2FrameInTagWith(TagLib::ID3v2::Tag& tag, const String& base64MarkersData);
        static void replaceGridMarkersFrameInTagWith(TagLib::ID3v2::Tag& tag, const Blob& gridMarkersData);
        static void updateMarkersInTagFromTrackFile(TagLib::ID3v2::Tag& tag, const TrackFile& trackFile);
        static String::Pointer releaseDateFromTag(const TagLib::ID3v2::Tag& tag);
        static void setReleaseDateInTag(const String& date, TagLib::ID3v2::Tag& tag);
        static Blob::Pointer artworkInTag(const TagLib::ID3v2::Tag& tag);
        static void removeArtworkInTag(TagLib::ID3v2::Tag& tag);

        #pragma mark Instance Methods
        void parseTag(const TagLib::ID3v2::Tag& tag);
        void updateArtworkInTag(TagLib::ID3v2::Tag& tag) const;
        void updateTag(TagLib::ID3v2::Tag& tag) const;
    };
} } }
