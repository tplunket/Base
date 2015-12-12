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

#include <SeratoDb/Markers/CueMarker.hpp>
#include <SeratoDb/Markers/LoopMarker.hpp>
#include <SeratoDb/Markers/GridMarker.hpp>

#include <Base/Base.hpp>

namespace NxA { namespace Serato {
    NXA_EXCEPTION_NAMED_WITH_PARENT(TrackFileError, NxA::Exception);

    NXA_GENERATED_FORWARD_DECLARATIONS_FOR_CLASS(TrackFile);

    class TrackFile : public Object {
        NXA_GENERATED_OPERATOR_EQUAL_DECLARATION_IN_NAMESPACE_FOR_CLASS(NxA::Serato, TrackFile);
        NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_PURE_VIRTUAL_CLASS(NxA::Serato, TrackFile);

    public:
        #pragma mark Constants
        enum Flags {
            NoFlag          = 0x0,
            IgnoreMarkers   = 0x1<<0
        };

        #pragma mark Instance Methods
        String::PointerToConst filePath(void) const;

        virtual String::Pointer title(void) const;
        virtual String::Pointer artist(void) const;
        virtual String::Pointer genre(void) const;
        virtual boolean hasKey(void) const = 0;
        virtual String::Pointer key(void) const = 0;
        virtual String::Pointer comments(void) const;
        virtual String::Pointer album(void) const;
        virtual String::Pointer composer(void) const = 0;
        virtual String::Pointer grouping(void) const = 0;
        virtual String::Pointer bpm(void) const = 0;
        virtual count trackNumber(void) const;

        virtual count audioDataSizeInBytes(void) const;
        virtual uinteger32 lengthInMilliseconds(void) const;
        virtual uinteger32 bitRateInKiloBitsPerSecond(void) const;
        virtual boolean hasBitDepth(void) const;
        virtual uinteger32 bitDepthInBits(void) const;
        virtual uinteger32 sampleRateInSamplesPerSecond(void) const;
        virtual Blob::Pointer artwork(void) const = 0;

        // -- TODO: Look into adding discNumber and numberOfAlbumTracks if they are available.

        virtual boolean hasRecordLabel(void) const = 0;
        virtual String::Pointer recordLabel(void) const = 0;
        virtual boolean hasRemixer(void) const = 0;
        virtual String::Pointer remixer(void) const = 0;
        virtual boolean hasRating(void) const = 0;
        virtual integer rating(void) const = 0;
        virtual String::Pointer releaseDate(void) const;

        const CueMarker::Array& cueMarkers(void) const;
        const LoopMarker::Array& loopMarkers(void) const;
        const GridMarker::Array& gridMarkers(void) const;

        virtual void setTitle(const String& title);
        virtual void setArtist(const String& artist);
        virtual void setGenre(const String& genre);
        virtual void setKey(const String& key) = 0;
        virtual void setComments(const String& comments);
        virtual void setAlbum(const String& album);
        virtual void setComposer(const String& composer) = 0;
        virtual void setGrouping(const String& grouping) = 0;
        virtual void setBpm(const String& bpm) = 0;
        virtual void setTrackNumber(count trackNumber);
        virtual void setRecordLabel(const String& recordLabel) = 0;
        virtual void setRemixer(const String& remixer) = 0;
        virtual void setRating(integer rating) = 0;
        virtual void setReleaseDate(const String& date);

        virtual void setArtwork(const Blob& artwork) = 0;

        void setCueMarkers(CueMarker::Array& markers);
        void setLoopMarkers(LoopMarker::Array& markers);
        void setGridMarkers(GridMarker::Array& markers);

        void saveChangesIfAny(void);
    };
} }
