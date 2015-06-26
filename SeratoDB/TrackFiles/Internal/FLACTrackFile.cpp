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

#include "TrackFiles/Internal/FLACTrackFile.hpp"
#include "Base64.hpp"

// -- Generated internal implementation ommitted because this class does not use the default contructor.

using namespace NxA;
using namespace NxA::Serato::Internal;

#pragma mark Structures

namespace NxA { namespace Serato { namespace Internal {
    typedef struct {
        unsigned char mimeType[25];
        unsigned char filename[1];
        unsigned char description[16];
        unsigned char majorVersion;
        unsigned char minorVersion;
        unsigned char data[0];
    } FLACMarkersHeaderStruct;
} } }

#pragma mark Constructors & Destructors

FLACTrackFile::FLACTrackFile(String::Pointer const& path, TagLibFilePointer const& newFile) :
                             TrackFile(path, newFile) { }

#pragma mark Instance Methods

void FLACTrackFile::readMarkers(void)
{
    auto markersEncodedData = this->properties["SERATO_MARKERS_V2"].toString();
    auto encodedDataSize = markersEncodedData.size();
    if (encodedDataSize) {
        auto decodedMarkersData = Base64::decodeBlock(reinterpret_cast<const char*>(markersEncodedData.data(TagLib::String::UTF8).data()),
                                                      encodedDataSize);

        auto headerStruct = reinterpret_cast<const FLACMarkersHeaderStruct*>(decodedMarkersData->data());
        this->readMarkersV2FromBase64Data(headerStruct->data, decodedMarkersData->size() - sizeof(FLACMarkersHeaderStruct));
    }

    auto beatGridEncodedData = this->properties["SERATO_BEATGRID"].toString();
    auto encodedBeatGridDataSize = beatGridEncodedData.size();
    if (encodedBeatGridDataSize) {
        auto decodedGridMarkersData = Base64::decodeBlock(reinterpret_cast<const char*>(beatGridEncodedData.data(TagLib::String::UTF8).data()),
                                                          encodedBeatGridDataSize);
        auto headerStruct = reinterpret_cast<const FLACMarkersHeaderStruct*>(decodedGridMarkersData->data());
        if ((headerStruct->majorVersion == 1) && (headerStruct->minorVersion == 0)) {
            this->readGridMarkersFrom(headerStruct->data, decodedGridMarkersData->size() - sizeof(FLACMarkersHeaderStruct));
        }
    }
}

void FLACTrackFile::writeMarkers(void)
{
    if (this->cueMarkers->length() || this->loopMarkers->length()) {
        auto decodedData = Blob::blob();

        FLACMarkersHeaderStruct header;
        memcpy(header.mimeType, "application/octet-stream", 25);
        header.filename[0] = 0;
        memcpy(header.description, "Serato Markers2", 16);
        header.majorVersion = 1;
        header.minorVersion = 1;

        auto headerData = Blob::blobWithMemoryAndSize(reinterpret_cast<const byte*>(&header), sizeof(header));
        decodedData->append(headerData);

        auto base64Data = this->base64DataFromMarkersV2();
        decodedData->append(base64Data);

        auto encodedData = Base64::encodeBlock(decodedData->data(), decodedData->size());
        encodedData->append(static_cast<character>(0));

        TagLib::StringList newList;
        newList.append(TagLib::String(reinterpret_cast<character*>(encodedData->data())));
        this->properties["SERATO_MARKERS_V2"] = newList;
    }
    else {
        this->properties.erase("SERATO_MARKERS_V2");
    }

    if (this->gridMarkers->length()) {
        auto decodedData = Blob::blob();

        FLACMarkersHeaderStruct header;
        memcpy(header.mimeType, "application/octet-stream", 25);
        header.filename[0] = 0;
        memcpy(header.description, "Serato Beatgrid", 16);
        header.majorVersion = 1;
        header.minorVersion = 0;

        auto headerData = Blob::blobWithMemoryAndSize(reinterpret_cast<const byte*>(&header), sizeof(header));
        decodedData->append(headerData);

        auto base64Data = this->gridMarkerDataFromGridMarkers();
        decodedData->append(base64Data);

        auto encodedData = Base64::encodeBlock(decodedData->data(), decodedData->size());
        encodedData->append(static_cast<character>(0));

        TagLib::StringList newList;
        newList.append(TagLib::String(reinterpret_cast<character*>(encodedData->data())));
        this->properties["SERATO_BEATGRID"] = newList;
    }
    else {
        this->properties.erase("SERATO_BEATGRID");
    }
}

