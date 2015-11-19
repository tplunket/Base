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

#include <Base/Internal/Object.hpp>

#include "TrackFiles/TrackFile.hpp"

#include <Base/Base.hpp>

#include <tfile.h>
#include <tpropertymap.h>

namespace NxA { namespace Serato { namespace Internal {
    using TagLibFilePointer = NxA::Pointer<TagLib::File>;

    struct TrackFile : public NxA::Internal::Object {
        NXA_GENERATED_INTERNAL_DECLARATIONS_WITHOUT_CONSTRUCTORS_FOR(NxA::Serato, TrackFile);

        #pragma mark Constructor & Destructors
        TrackFile(const String& path, const TagLibFilePointer& newFile);

        #pragma mark Class Methods
        static const byte* nextTagPositionAfterTagNamed(const String& tagName, const byte* currentTagPosition);
        static Blob::Pointer markerV2TagDataFrom(const byte* tagStart);

        #pragma mark Instance Variables
        String::PointerToConst trackFilePath;
        TagLibFilePointer file;
        TagLib::Tag* tag;
        boolean markersWereIgnored;
        boolean markersWereModified;
        boolean metadataWasModified;

        Serato::CueMarker::Array::Pointer cueMarkers;
        Serato::LoopMarker::Array::Pointer loopMarkers;
        Serato::GridMarker::Array::Pointer gridMarkers;
        Blob::Array::Pointer otherTags;

        #pragma mark Instance Methods
        const byte* readMarkerAtAndAdvanceToNextTag(const byte* tagStart);
        void readMarkersV2FromBase64String(const byte* markerV2Data, count totalSize);
        void readGridMarkersFrom(const byte* gridMarkerData);
        String::Pointer base64StringFromMarkersV2(void);
        Blob::Pointer gridMarkerDataFromGridMarkersAndFixGridMarkersToMatchData(void);
        virtual void writeMarkers(void) = 0;
    };
} } }
