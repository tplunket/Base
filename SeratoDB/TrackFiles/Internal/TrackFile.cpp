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

#include "TrackFiles/Internal/TrackFile.hpp"
#include "Base64.hpp"

//NXA_GENERATED_INTERNAL_IMPLEMENTATION_FOR(NxA::Serato, TrackFile);

using namespace NxA;
using namespace NxA::Serato::Internal;

#pragma mark Structures

namespace NxA { namespace Serato { namespace Internal {
    typedef struct {
        unsigned char majorVersion;
        unsigned char minorVersion;
        unsigned char data[0];
    } MarkerHeaderStruct;
} } }

#pragma mark Constructors & Destructors

TrackFile::TrackFile(const String& path, const TagLibFilePointer& newFile) :
                     trackFilePath(path.pointerToConst()),
                     file(newFile),
                     cueMarkers(Serato::CueMarker::Array::array()),
                     loopMarkers(Serato::LoopMarker::Array::array()),
                     gridMarkers(Serato::GridMarker::Array::array()),
                     otherTags(Blob::Array::array()) { }

#pragma mark Class Methods

const byte* TrackFile::nextTagPositionAfterTagNamed(const String& tagName, const byte* currentTagPosition)
{
    auto parserPosition = currentTagPosition;
    parserPosition += tagName.length() + 1;

    auto tagSize = Platform::bigEndianUInteger32ValueAt(parserPosition);
    parserPosition += 4 + tagSize;

    return parserPosition;
}

Blob::Pointer TrackFile::markerV2TagDataFrom(const byte* tagStart)
{
    auto tagName = String::stringWith(reinterpret_cast<const char*>(tagStart));
    count sizeOfNameField = tagName->length() + 1;
    constexpr count sizeOfSizeField = 4;

    auto sizePosition = tagStart + sizeOfNameField;
    count tagSize = Platform::bigEndianUInteger32ValueAt(sizePosition) + sizeOfNameField + sizeOfSizeField;

    return Blob::blobWithMemoryAndSize(tagStart, tagSize);
}

#pragma mark Instance Methods

void TrackFile::readMarkersV2FromBase64Data(const byte* markerV2Data, count totalSize)
{
    if (!totalSize) {
        return;
    }

    auto decodedData = Base64::decodeBlock(markerV2Data, totalSize);

    auto markerStruct = reinterpret_cast<MarkerHeaderStruct*>(decodedData->data());
    if ((markerStruct->majorVersion != 1) || (markerStruct->minorVersion != 1)) {
        return;
    }

    // -- FIXME: The data written by Serato sometimes contains an extra byte at the end.
    // -- This might be a bug in the decode method but since marker data is always more
    // -- than one byte long, we substract one here to make sure we don't go over the end.
    const byte* markerDataEnd = (const byte*)markerStruct + decodedData->size() - 1;
    const byte* tagStart = (const byte*)markerStruct->data;

    while (tagStart < markerDataEnd) {
        auto tagName = String::stringWith(reinterpret_cast<const character*>(tagStart));

        if (tagName->isEqualTo("CUE")) {
            this->cueMarkers->append(Serato::CueMarker::markerWithMemoryAt(tagStart));
        }
        else if (tagName->isEqualTo("LOOP")) {
            this->loopMarkers->append(Serato::LoopMarker::markerWithMemoryAt(tagStart));
        }
        else {
            this->otherTags->append(markerV2TagDataFrom(tagStart));
        }

        tagStart = nextTagPositionAfterTagNamed(tagName, tagStart);
    }
}

void TrackFile::addGridMarker(Serato::GridMarker& gridMarker)
{
    this->gridMarkers->append(gridMarker.pointer());
}

void TrackFile::readGridMarkersFrom(const byte* gridMarkerData, size_t totalSize)
{
    if (!totalSize) {
        return;
    }

    auto numberOfMarkers = Platform::bigEndianUInteger32ValueAt(gridMarkerData);
    gridMarkerData += 4;

    for (uint32_t index = 0; index < numberOfMarkers; ++index) {
        this->addGridMarker(Serato::GridMarker::markerWithMemoryAt(gridMarkerData));

        gridMarkerData = Serato::GridMarker::nextGridMarkerAfter(gridMarkerData);
    }
}

Blob::Pointer TrackFile::base64DataFromMarkersV2(void)
{
    auto decodedData = Blob::blob();

    MarkerHeaderStruct markersHeader;
    markersHeader.majorVersion = 1;
    markersHeader.minorVersion = 1;

    auto headerData = Blob::blobWithMemoryAndSize(reinterpret_cast<byte*>(&markersHeader), sizeof(MarkerHeaderStruct));
    decodedData->append(headerData);

    for (auto& marker : *this->cueMarkers) {
        marker->addId3TagTo(decodedData);
    }

    for (auto& marker : *this->loopMarkers) {
        marker->addId3TagTo(decodedData);
    }

    for (auto& tagData : *this->otherTags) {
        decodedData->append(tagData);
    }

    auto encodedData = Base64::encodeBlock(decodedData->data(), decodedData->size());
    return encodedData;
}

Blob::Pointer TrackFile::gridMarkerDataFromGridMarkers(void)
{
    auto data = Blob::blob();

    uinteger32 numberOfMarkers;
    Platform::writeBigEndianUInteger32ValueAt(this->gridMarkers->length(), reinterpret_cast<byte*>(&numberOfMarkers));

    auto numberOfMarkersData = Blob::blobWithMemoryAndSize(reinterpret_cast<byte*>(&numberOfMarkers), sizeof(numberOfMarkers));
    data->append(numberOfMarkersData);

    for (auto& marker : *(this->gridMarkers)) {
        marker->addDataTo(data);
    }
    
    return data;
}
