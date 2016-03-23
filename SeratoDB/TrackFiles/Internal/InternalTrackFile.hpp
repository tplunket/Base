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
    static Blob::Pointer markerV2TagDataFrom(const byte* tagStart);

    #pragma mark Instance Variables
    String::PointerToConst filePath;
    boolean markersWereIgnored;
    boolean markersWereModified;
    boolean metadataWasModified;

    String::Pointer title;
    String::Pointer artist;
    String::Pointer genre;
    String::Pointer key;
    String::Pointer comments;
    String::Pointer album;
    String::Pointer composer;
    String::Pointer grouping;
    String::Pointer bpm;
    count trackNumber;

    count audioDataSizeInBytes;
    uinteger32 lengthInMilliseconds;
    uinteger32 bitRateInKiloBitsPerSecond;
    boolean hasBitDepth;
    uinteger32 bitDepthInBits;
    uinteger32 sampleRateInSamplesPerSecond;
    Blob::Pointer artwork;

    String::Pointer recordLabel;
    String::Pointer remixer;
    integer rating;
    String::Pointer releaseDate;

    boolean beatGridIsLocked;

    CueMarker::Array::Pointer cueMarkers;
    LoopMarker::Array::Pointer loopMarkers;
    GridMarker::Array::Pointer gridMarkers;
    Blob::Array::Pointer otherTags;

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
    Blob::Pointer rawBlobFromMarkersV1(void) const;
    Blob::Pointer id3EncodedBlobFromMarkersV1(void) const;
    String::Pointer base64StringFromMarkersV2(void) const;
    Blob::Pointer gridMarkerDataFromGridMarkers(void) const;
};

NXA_EXIT_NAMESPACE;
NXA_EXIT_NAMESPACE;
