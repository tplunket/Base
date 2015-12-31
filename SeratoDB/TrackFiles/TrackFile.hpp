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
        NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_PURE_VIRTUAL_CLASS(NxA::Serato, TrackFile);

    public:
        #pragma mark Constants
        enum Flags {
            NoFlag          = 0x0,
            IgnoreMarkers   = 0x1<<0
        };

        #pragma mark Instance Methods
        const String& filePath(void) const;

        const String& title(void) const;
        void setTitle(const String& title);

        const String& artist(void) const;
        void setArtist(const String& artist);

        const String& genre(void) const;
        void setGenre(const String& genre);

        const String& comments(void) const;
        void setComments(const String& comments);

        const String& album(void) const;
        void setAlbum(const String& album);

        count trackNumber(void) const;
        void setTrackNumber(count trackNumber);

        const String& releaseDate(void) const;
        virtual void setReleaseDate(const String& date);

        virtual boolean hasKey(void) const;
        const String& key(void) const;
        void setKey(const String& key);

        const String& composer(void) const;
        void setComposer(const String& composer);

        const String& grouping(void) const;
        void setGrouping(const String& grouping);

        const String& bpm(void) const;
        virtual void setBpm(const String& bpm);

        virtual boolean hasRecordLabel(void) const;
        const String& recordLabel(void) const;
        void setRecordLabel(const String& recordLabel);

        virtual boolean hasRemixer(void) const;
        const String& remixer(void) const;
        void setRemixer(const String& remixer);

        virtual boolean hasRating(void) const;
        integer rating(void) const;
        void setRating(integer rating);

        const Blob& artwork(void) const;
        void setArtwork(const Blob& artwork);

        count audioDataSizeInBytes(void) const;
        uinteger32 lengthInMilliseconds(void) const;
        uinteger32 bitRateInKiloBitsPerSecond(void) const;
        virtual boolean hasBitDepth(void) const;
        uinteger32 bitDepthInBits(void) const;
        uinteger32 sampleRateInSamplesPerSecond(void) const;

        boolean beatGridIsLocked(void) const;
        void setBeatGridAsLocked(boolean locked);

        const CueMarker::Array& cueMarkers(void) const;
        void setCueMarkers(const CueMarker::Array& markers);

        const LoopMarker::Array& loopMarkers(void) const;
        void setLoopMarkers(const LoopMarker::Array& markers);

        const GridMarker::Array& gridMarkers(void) const;
        void setGridMarkers(const GridMarker::Array& markers);
        
        // -- TODO: Look into adding discNumber and numberOfAlbumTracks if they are available.

        boolean hasModificationsToSave(void);
        boolean saveIfModified(void);
    };
} }
