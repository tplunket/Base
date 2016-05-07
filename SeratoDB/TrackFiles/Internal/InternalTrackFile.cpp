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

#include "SeratoDB/TrackFiles/Internal/InternalTrackFile.hpp"
#include "SeratoDB/Exceptions.hpp"

NXA_ENTER_NAMESPACE(NxA);
NXA_ENTER_NAMESPACE(Serato);

#pragma mark Structures

typedef struct {
    byte majorVersion;
    byte minorVersion;
    byte data[0];
} MarkerV2HeaderStruct;

typedef struct {
    byte markerCount[4];
    byte data[0];
} MarkerV1HeaderStruct;

typedef struct {
    byte footer[4];
} MarkerV1FooterStruct;

typedef struct {
    byte tag[8];
    byte size[4];
    byte locked;
} SeratoBpmLockTagStruct;

NXA_EXIT_NAMESPACE;
NXA_EXIT_NAMESPACE;

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Constructors & Destructors

InternalTrackFile::InternalTrackFile(const String& path) :
    filePath(path.pointer()),
    markersWereIgnored(false),
    markersWereModified(false),
    metadataWasModified(false),
    title(String::string()),
    artist(String::string()),
    genre(String::string()),
    key(String::string()),
    comments(String::string()),
    album(String::string()),
    composer(String::string()),
    grouping(String::string()),
    bpm(String::string()),
    trackNumber(0),
    audioDataSizeInBytes(0),
    lengthInMilliseconds(0),
    bitRateInKiloBitsPerSecond(0),
    bitDepthInBits(0),
    sampleRateInSamplesPerSecond(0),
    artwork(Blob::blob()),
    recordLabel(String::string()),
    remixer(String::string()),
    rating(0),
    releaseDate(String::string()),
    cueMarkers(CueMarker::Array::array()),
    loopMarkers(LoopMarker::Array::array()),
    gridMarkers(GridMarker::Array::array()),
    otherTags(Blob::Array::array())
{
}

#pragma mark Class Methods

const byte* InternalTrackFile::nextTagPositionAfterTagNamed(const String& tagName, const byte* currentTagPosition)
{
    auto parserPosition = currentTagPosition;
    parserPosition += tagName.length() + 1;

    auto tagSize = Platform::bigEndianUInteger32ValueAt(parserPosition);
    parserPosition += 4 + tagSize;

    return parserPosition;
}

Blob::Pointer InternalTrackFile::markerV2TagDataFrom(const byte* tagStart)
{
    auto tagName = String::stringWith(reinterpret_cast<const char*>(tagStart));
    count sizeOfNameField = tagName->length() + 1;
    constexpr count sizeOfSizeField = 4;

    auto sizePosition = tagStart + sizeOfNameField;
    count tagSize = Platform::bigEndianUInteger32ValueAt(sizePosition) + sizeOfNameField + sizeOfSizeField;
    return Blob::blobWithMemoryAndSize(tagStart, tagSize);
}

#pragma mark Instance Methods

void InternalTrackFile::parseAudioProperties(const TagLib::AudioProperties& properties)
{
    this->lengthInMilliseconds = properties.lengthInMilliseconds();
    this->bitRateInKiloBitsPerSecond = properties.bitrate();
    this->sampleRateInSamplesPerSecond = properties.sampleRate();

    this->audioDataSizeInBytes = ((uinteger64)this->lengthInMilliseconds * (uinteger64)this->bitRateInKiloBitsPerSecond * 1024) / 8 / 1000;
}

void InternalTrackFile::parseTag(const TagLib::Tag& tag)
{
    this->title = String::stringWith(tag.title().to8Bit(true).c_str());
    this->artist = String::stringWith(tag.artist().to8Bit(true).c_str());
    this->genre = String::stringWith(tag.genre().to8Bit(true).c_str());
    this->comments = String::stringWith(tag.comment().to8Bit(true).c_str());
    this->album = String::stringWith(tag.album().to8Bit(true).c_str());
    this->trackNumber = tag.track();

    integer year = tag.year();
    this->releaseDate = year ? String::stringWithFormat("%04d-01-01", year) : String::string();
}

void InternalTrackFile::updateTag(TagLib::Tag& tag) const
{
    tag.setTitle(TagLib::String(this->title->toUTF8()));
    tag.setArtist(TagLib::String(this->artist->toUTF8()));
    tag.setGenre(TagLib::String(this->genre->toUTF8()));
    tag.setComment(TagLib::String(this->comments->toUTF8()));
    tag.setAlbum(TagLib::String(this->album->toUTF8()));
    tag.setTrack(trackNumber);

    auto components = this->releaseDate->splitBySeperator('-');
    tag.setYear(components->length() ? components->firstObject().integerValue() : 0);
}

const byte* InternalTrackFile::parseMarkerAtAndAdvanceToNextTag(const byte* tagStart)
{
    NXA_ASSERT_FALSE(this->markersWereIgnored);

    auto tagName = String::stringWith(reinterpret_cast<const character*>(tagStart));

    try {
        if (*tagName == "CUE") {
            this->cueMarkers->append(CueMarker::markerWithMemoryAt(tagStart));
        }
        else if (*tagName == "LOOP") {
            this->loopMarkers->append(LoopMarker::markerWithMemoryAt(tagStart));
        }
        else if (*tagName == "BPMLOCK") {
            SeratoBpmLockTagStruct* bpmLockData = (SeratoBpmLockTagStruct *)tagStart;
            this->beatGridIsLocked = (bpmLockData->locked != 0);
        }
        else {
            this->otherTags->append(markerV2TagDataFrom(tagStart));
        }
    }
    catch (MarkerError exception) {
        // TODO: This should be a logging call instead.
    }

    return nextTagPositionAfterTagNamed(tagName, tagStart);
}

void InternalTrackFile::parseMarkersV1FromRawByteArray(const byte* markerData, count totalSize)
{
    NXA_ASSERT_FALSE(this->markersWereIgnored);

    if(totalSize != sizeof(MarkerV1HeaderStruct) +
       CueMarker::sizeOfV1RawMarker() * 5 +
       LoopMarker::sizeOfV1RawMarker() * 9 +
       sizeof(MarkerV1FooterStruct)) {
        throw TrackFileError::exceptionWith("Invalid raw V1 marker data in '%s'.", this->filePath->toUTF8());
    }

    auto markerPos = markerData + sizeof(MarkerV1HeaderStruct);

    for (int i = 0; i < 5; ++i) {
        if (CueMarker::isValidV1RawMarker(markerPos)) {
            this->cueMarkers->append(CueMarker::markerV1WithIndexAndRawMemoryAt(i, markerPos));
        }
        
        markerPos += CueMarker::sizeOfV1RawMarker();
    }
    
    for (int i = 0; i < 9; ++i) {
        if (LoopMarker::isValidV1RawMarker(markerPos)) {
            this->loopMarkers->append(LoopMarker::markerV1WithIndexAndRawMemoryAt(i, markerPos));
        }
        
        markerPos += LoopMarker::sizeOfV1RawMarker();
    }
}

void InternalTrackFile::parseMarkersV1FromEncodedByteArray(const byte* markerData, count totalSize)
{
    NXA_ASSERT_FALSE(this->markersWereIgnored);

    if (totalSize != sizeof(MarkerV1HeaderStruct) +
        CueMarker::sizeOfV1EncodedMarker() * 5 +
        LoopMarker::sizeOfV1EncodedMarker() * 9 +
        sizeof(MarkerV1FooterStruct)) {
        throw TrackFileError::exceptionWith("Invalid encoded V1 marker data in '%s'.", this->filePath->toUTF8());
    }

    auto markerPos = markerData + sizeof(MarkerV1HeaderStruct);

    for (int i = 0; i < 5; ++i) {
        if (CueMarker::isValidV1EncodedMarker(markerPos)) {
            this->cueMarkers->append(CueMarker::markerV1WithIndexAndEncodedMemoryAt(i, markerPos));
        }
        
        markerPos += CueMarker::sizeOfV1EncodedMarker();
    }
    
    for (int i = 0; i < 9; ++i) {
        if (LoopMarker::isValidV1EncodedMarker(markerPos)) {
            this->loopMarkers->append(LoopMarker::markerV1WithIndexAndEncodedMemoryAt(i, markerPos));
        }
        
        markerPos += LoopMarker::sizeOfV1EncodedMarker();
    }
}

void InternalTrackFile::parseMarkersV2FromBase64String(const byte* markerV2Data, count totalSize)
{
    NXA_ASSERT_FALSE(this->markersWereIgnored);

    if (!totalSize) {
        return;
    }

    auto decodedData = Blob::blobWithBase64String(String::stringWith(reinterpret_cast<const character*>(markerV2Data),
                                                                     totalSize));

    auto markerStruct = reinterpret_cast<MarkerV2HeaderStruct*>(decodedData->data());
    if ((markerStruct->majorVersion != 1) || (markerStruct->minorVersion != 1)) {
        return;
    }

    auto markerDataEnd = (const byte*)markerStruct + decodedData->size();
    auto tagStart = (const byte*)markerStruct->data;

    while ((tagStart < markerDataEnd) && *tagStart) {
        tagStart = this->parseMarkerAtAndAdvanceToNextTag(tagStart);
    }
}

void InternalTrackFile::parseGridMarkersFrom(const byte* gridMarkerData)
{
    NXA_ASSERT_FALSE(this->markersWereIgnored);

    this->gridMarkers = GridMarker::markersWithMemoryAt(gridMarkerData);
}

Blob::Pointer InternalTrackFile::rawBlobFromMarkersV1(void) const
{
    NXA_ASSERT_FALSE(this->markersWereIgnored);

    auto blobData = Blob::blob();

    MarkerV1HeaderStruct markersHeader;
    markersHeader.markerCount[0] = 0;
    markersHeader.markerCount[1] = 0;
    markersHeader.markerCount[2] = 0;
    markersHeader.markerCount[3] = 14;

    auto headerData = Blob::blobWithMemoryAndSize(reinterpret_cast<byte*>(&markersHeader), sizeof(MarkerV1HeaderStruct));
    blobData->append(headerData);

    for (int i = 0; i < 5; ++i) {
        // Find the cue with this index
        bool cueFound = false;
        for (auto& cue : *this->cueMarkers) {
            if (cue->index() == i) {
                cue->addRawMarkerV1TagTo(blobData);
                cueFound = true;
                break;
            }
        }

        // Otherwise, write an empty cue
        if (!cueFound) {
            CueMarker::addEmptyRawMarkerV1TagTo(blobData);
        }
    }

    for (int i = 0; i < 9; ++i) {
        // Find the loop with this index
        bool loopFound = false;
        for (auto& loop : *this->loopMarkers) {
            if (loop->index() == i) {
                loop->addRawMarkerV1TagTo(blobData);
                loopFound = true;
                break;
            }
        }

        // Otherwise, write an empty loop
        if (!loopFound) {
            LoopMarker::addEmptyRawMarkerV1TagTo(blobData);
        }
    }

    // -- This marks the end of tags.
    blobData->append(0x00);
    blobData->append(0xFF);
    blobData->append(0xFF);
    blobData->append(0xFF);

    return blobData;
}

Blob::Pointer InternalTrackFile::id3EncodedBlobFromMarkersV1(void) const
{
    NXA_ASSERT_FALSE(this->markersWereIgnored);

    auto blobData = Blob::blob();

    MarkerV1HeaderStruct markersHeader;
    markersHeader.markerCount[0] = 0;
    markersHeader.markerCount[1] = 0;
    markersHeader.markerCount[2] = 0;
    markersHeader.markerCount[3] = 14;

    auto headerData = Blob::blobWithMemoryAndSize(reinterpret_cast<byte*>(&markersHeader), sizeof(MarkerV1HeaderStruct));
    blobData->append(headerData);

    for (int i = 0; i < 5; ++i) {
        // Find the cue with this index
        bool cueFound = false;
        for (auto& cue : *this->cueMarkers) {
            if (cue->index() == i) {
                cue->addEncodedMarkerV1TagTo(blobData);
                cueFound = true;
                break;
            }
        }

        // Otherwise, write an empty cue
        if (!cueFound) {
            CueMarker::addEmptyEncodedMarkerV1TagTo(blobData);
        }
    }

    for (int i = 0; i < 9; ++i) {
        // Find the loop with this index
        bool loopFound = false;
        for (auto& loop : *this->loopMarkers) {
            if (loop->index() == i) {
                loop->addEncodedMarkerV1TagTo(blobData);
                loopFound = true;
                break;
            }
        }

        // Otherwise, write an empty loop
        if (!loopFound) {
            LoopMarker::addEmptyEncodedMarkerV1TagTo(blobData);
        }
    }

    // -- This marks the end of tags.
    blobData->append(0x07);
    blobData->append(0x7F);
    blobData->append(0x7F);
    blobData->append(0x7F);
    
    return blobData;
}

String::Pointer InternalTrackFile::base64StringFromMarkersV2(void) const
{
    NXA_ASSERT_FALSE(this->markersWereIgnored);

    if (!this->cueMarkers->length() && !this->loopMarkers->length() && !this->otherTags->length()) {
        return String::string();
    }

    auto decodedData = Blob::blob();

    MarkerV2HeaderStruct markersHeader;
    markersHeader.majorVersion = 1;
    markersHeader.minorVersion = 1;

    auto headerData = Blob::blobWithMemoryAndSize(reinterpret_cast<byte*>(&markersHeader), sizeof(MarkerV2HeaderStruct));
    decodedData->append(headerData);

    for (auto& marker : *this->cueMarkers) {
        marker->addMarkerV2TagTo(decodedData);
    }

    for (auto& marker : *this->loopMarkers) {
        marker->addMarkerV2TagTo(decodedData);
    }

    SeratoBpmLockTagStruct bpmLockStruct;
    memcpy(bpmLockStruct.tag, "BPMLOCK", 8);
    Platform::writeBigEndianUInteger32ValueAt(1, bpmLockStruct.size);
    bpmLockStruct.locked = this->beatGridIsLocked ? 1 : 0;

    auto bpmLockData = Blob::blobWithMemoryAndSize(reinterpret_cast<const byte*>(&bpmLockStruct), sizeof(bpmLockStruct));
    decodedData->append(bpmLockData);
    for (auto& tagData : *this->otherTags) {
        decodedData->append(tagData);
    }

    // -- This marks the end of tags.
    decodedData->append('\0');

    auto encodedData = Blob::base64StringFor(decodedData->data(), decodedData->size());
    return encodedData;
}

Blob::Pointer InternalTrackFile::gridMarkerDataFromGridMarkers(void) const
{
    NXA_ASSERT_FALSE(this->markersWereIgnored);

    auto data = Blob::blob();
    if (!this->gridMarkers->length()) {
        return data;
    }

    GridMarker::addMarkersTo(this->gridMarkers, data);

    return data;
}
