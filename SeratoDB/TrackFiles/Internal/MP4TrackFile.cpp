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

#include "TrackFiles/Internal/MP4TrackFile.hpp"

#include "SeratoDB/Base64.hpp"

// -- Generated internal implementation ommitted because this class does not use the default contructor.

using namespace NxA;
using namespace NxA::Serato::Internal;

#pragma mark Structures

namespace NxA { namespace Serato { namespace Internal {
    typedef struct {
        byte mimeType[25];
        byte filename[1];
        byte description[16];
        byte majorVersion;
        byte minorVersion;
        byte data[0];
    } MP4MarkersHeaderStruct;
} } }

#pragma mark Constructors & Destructors

MP4TrackFile::MP4TrackFile(const String& path, const TagLibFilePointer& newFile) :
                           TrackFile(path, newFile) { }

#pragma mark Instance Methods

void MP4TrackFile::readMarkers(void)
{
    if (!this->itemListMap) {
        return;
    }

    auto markersItem = (*this->itemListMap)["----:com.serato.dj:markersv2"];
    if (markersItem.isValid()) {
        const TagLib::String encodedData = markersItem.toStringList().toString();
        uint32_t encodedDataSize = encodedData.size();
        if (!encodedDataSize) {
            return;
        }

        auto decodedData = Base64::decodeBlock(reinterpret_cast<const char*>(encodedData.data(TagLib::String::UTF8).data()),
                                               encodedDataSize);
        auto headerStruct = reinterpret_cast<const MP4MarkersHeaderStruct*>(decodedData->data());
        this->readMarkersV2FromBase64Data(headerStruct->data, decodedData->size() - sizeof(MP4MarkersHeaderStruct));
    }

    TagLib::MP4::Item beatgridItem = (*this->itemListMap)["----:com.serato.dj:beatgrid"];
    if (beatgridItem.isValid()) {
        const TagLib::String encodedData = beatgridItem.toStringList().toString();
        uint32_t encodedDataSize = encodedData.size();
        if (!encodedDataSize) {
            return;
        }

        auto decodedData = Base64::decodeBlock(reinterpret_cast<const char*>(encodedData.data(TagLib::String::UTF8).data()),
                                               encodedDataSize);

        auto headerStruct = reinterpret_cast<const MP4MarkersHeaderStruct*>(decodedData->data());
        if ((headerStruct->majorVersion == 1) && (headerStruct->minorVersion == 0)) {
            this->readGridMarkersFrom(headerStruct->data, decodedData->size() - sizeof(MP4MarkersHeaderStruct));
        }
    }
}

void MP4TrackFile::writeMarkers(void)
{
    if (this->cueMarkers->length() || this->loopMarkers->length()) {
        auto decodedData = Blob::blob();

        MP4MarkersHeaderStruct header;
        memcpy(header.mimeType, "application/octet-stream", 25);
        header.filename[0] = 0;
        memcpy(header.description, "Serato Markers2", 16);
        header.majorVersion = 1;
        header.minorVersion = 1;

        auto headerData = Blob::blobWithMemoryAndSize(reinterpret_cast<byte*>(&header), sizeof(header));
        decodedData->append(headerData);
        decodedData->append(this->base64DataFromMarkersV2());

        auto encodedData = Base64::encodeBlock(decodedData->data(), decodedData->size());
        encodedData->append(static_cast<character>(0));

        TagLib::StringList newList;
        newList.append(TagLib::String(reinterpret_cast<character*>(encodedData->data())));

        TagLib::MP4::Item newItem(newList);
        (*this->itemListMap)["----:com.serato.dj:markersv2"] = newItem;
    }
    else {
        (*this->itemListMap).erase("----:com.serato.dj:markersv2");
    }

    if (this->gridMarkers->length()) {
        auto decodedData = Blob::blob();

        MP4MarkersHeaderStruct header;
        memcpy(header.mimeType, "application/octet-stream", 25);
        header.filename[0] = 0;
        memcpy(header.description, "Serato Beatgrid", 16);
        header.majorVersion = 1;
        header.minorVersion = 0;

        auto headerData = Blob::blobWithMemoryAndSize(reinterpret_cast<byte*>(&header), sizeof(header));
        decodedData->append(headerData);
        decodedData->append(this->gridMarkerDataFromGridMarkers());

        auto encodedData = Base64::encodeBlock(decodedData->data(), decodedData->size());
        encodedData->append(static_cast<character>(0));

        TagLib::StringList newList;
        newList.append(TagLib::String(reinterpret_cast<character*>(encodedData->data())));

        TagLib::MP4::Item newItem(newList);
        (*this->itemListMap)["----:com.serato.dj:beatgrid"] = newItem;
    }
    else {
        (*this->itemListMap).erase("----:com.serato.dj:beatgrid");
    }
}
