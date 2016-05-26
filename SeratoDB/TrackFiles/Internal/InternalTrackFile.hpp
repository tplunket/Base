//
//  Copyright (c) 2015-2016 Next Audio Labs, LLC. All rights reserved.
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

#include <Base/Internal/InternalObject.hpp>

#include "SeratoDB/TrackFiles/TrackFile.hpp"
#include "SeratoDB/Markers/CueMarker.hpp"
#include "SeratoDB/Markers/LoopMarker.hpp"
#include "SeratoDB/Markers/GridMarker.hpp"

#include <Base/Base.hpp>

#include "tfile.h"
#include "tpropertymap.h"
#include "audioproperties.h"

NXA_ENTER_NAMESPACE(NxA);
NXA_ENTER_NAMESPACE(Serato);

struct InternalTrackFile : public InternalObject {
    NXA_GENERATED_DESTRUCTOR_FOR_CLASS(InternalTrackFile);

    #pragma mark Constructor & Destructors
    InternalTrackFile(const String& path);

    #pragma mark Class Methods
    static const byte* nextTagPositionAfterTagNamed(const String& tagName, const byte* currentTagPosition);
    static NxA::Pointer<Blob> markerV2TagDataFrom(const byte* tagStart);

    #pragma mark Instance Variables
    NxA::Pointer<const String> filePath;
    boolean markersWereIgnored;
    boolean markersWereModified;
    boolean metadataWasModified;

    NxA::Pointer<String> title;
    NxA::Pointer<String> artist;
    NxA::Pointer<String> genre;
    NxA::Pointer<String> key;
    NxA::Pointer<String> comments;
    NxA::Pointer<String> album;
    NxA::Pointer<String> composer;
    NxA::Pointer<String> grouping;
    NxA::Pointer<String> bpm;
    count trackNumber;

    count audioDataSizeInBytes;
    uinteger32 lengthInMilliseconds;
    uinteger32 bitRateInKiloBitsPerSecond;
    boolean hasBitDepth;
    uinteger32 bitDepthInBits;
    uinteger32 sampleRateInSamplesPerSecond;
    NxA::Pointer<Blob> artwork;

    NxA::Pointer<String> recordLabel;
    NxA::Pointer<String> remixer;
    integer rating;
    NxA::Pointer<String> releaseDate;

    boolean beatGridIsLocked;

    NxA::Pointer<CueMarker::Array> cueMarkers;
    NxA::Pointer<LoopMarker::Array> loopMarkers;
    NxA::Pointer<GridMarker::Array> gridMarkers;
    NxA::Pointer<Blob::Array> otherTags;

    #pragma mark Instance Methods
    virtual void loadAndParseFile(void) = 0;
    virtual void updateAndSaveFile(void) const = 0;
    void parseTag(const TagLib::Tag& tag);
    void updateTag(TagLib::Tag& tag) const;
    void parseAudioProperties(const TagLib::AudioProperties& properties);
    const byte* parseMarkerAtAndAdvanceToNextTag(const byte* tagStart);
    void parseMarkersV1FromRawByteArray(const byte* markerData, count totalSize);
    void parseMarkersV1FromEncodedByteArray(const byte* markerData, count totalSize);
    void parseMarkersV2FromBase64String(const byte* markerV2Data, count totalSize);
    void parseGridMarkersFrom(const byte* gridMarkerData);
    NxA::Pointer<Blob> rawBlobFromMarkersV1(void) const;
    NxA::Pointer<Blob> id3EncodedBlobFromMarkersV1(void) const;
    NxA::Pointer<String> base64StringFromMarkersV2(void) const;
    NxA::Pointer<Blob> gridMarkerDataFromGridMarkers(void) const;
};

NXA_EXIT_NAMESPACE;
NXA_EXIT_NAMESPACE;
