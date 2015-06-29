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

#include "TrackFiles/Internal/ID3TrackFile.hpp"
#include "Base64.hpp"

// -- Generated internal implementation ommitted because this class does not use the default contructor.

using namespace NxA;
using namespace NxA::Serato::Internal;

#pragma mark Structures

namespace NxA { namespace Serato { namespace Internal {
    typedef struct {
        byte dummy;
        byte mimeType[25];
        byte filename;
        byte description[16];
        byte majorVersion;
        byte minorVersion;
    } GeobBodyHeaderStruct;

    typedef struct {
        byte majorVersion;
        byte minorVersion;
        byte data[0];
    } GeobObjectStruct;
} } }

#pragma mark Constructors & Destructors

ID3TrackFile::ID3TrackFile(const String& path, const TagLibFilePointer& newFile) :
                           TrackFile(path, newFile) { }

#pragma mark Class Methods

boolean ID3TrackFile::isAValidGeobFrame(const TagLib::ID3v2::GeneralEncapsulatedObjectFrame* frame)
{
    TagLib::String frameID(reinterpret_cast<char*>(frame->frameID().data()));
    if (frameID != "GEOB") {
        return false;
    }

    TagLib::String mimeType = frame->mimeType();
    return mimeType == "application/octet-stream";
}

TagLib::ID3v2::FrameList::Iterator ID3TrackFile::frameInListWithDescription(TagLib::ID3v2::FrameList& list,
                                                                            const String& description)
{
    for (auto it = list.begin(); it != list.end(); ++it) {
        auto frame = (TagLib::ID3v2::GeneralEncapsulatedObjectFrame*)*it;
        if (!ID3TrackFile::isAValidGeobFrame(frame)) {
            continue;
        }

        if (frame->description() == description) {
            return it;
        }
    }

    return list.end();
}

#pragma mark Instance Methods

void ID3TrackFile::readMarkers(void)
{
    auto tag = reinterpret_cast<TagLib::ID3v2::Tag*>(this->parsedFileTag);
    if (!tag) {
        return;
    }

    auto frameListMap = tag->frameListMap();
    if (!frameListMap.contains("GEOB")) {
        return;
    }

    auto geobFrames = frameListMap["GEOB"];
    for (auto& framePtr : geobFrames) {
        auto frame = reinterpret_cast<const TagLib::ID3v2::GeneralEncapsulatedObjectFrame*>(framePtr);
        if (!isAValidGeobFrame(frame)) {
            continue;
        }

        const TagLib::String description = frame->description();
        if (description == "Serato Markers2") {
            auto encodedDataSize = frame->size() - sizeof(GeobBodyHeaderStruct);
            auto frameObject = frame->object();

            auto objectData = reinterpret_cast<GeobObjectStruct*>(frameObject.data());
            this->readMarkersV2FromBase64Data(objectData->data, encodedDataSize);
        }
        else if (description == "Serato BeatGrid") {
            auto dataSize = frame->size() - sizeof(GeobBodyHeaderStruct);
            auto frameObject = frame->object();

            auto headerStruct = reinterpret_cast<GeobObjectStruct*>(frameObject.data());
            if ((headerStruct->majorVersion == 1) && (headerStruct->minorVersion == 0)) {
                this->readGridMarkersFrom(headerStruct->data, dataSize);
            }
        }
    }
}

void ID3TrackFile::writeMarkersV2Frame(void)
{
    auto decodedData = Blob::blob();

    GeobObjectStruct header;
    header.majorVersion = 1;
    header.minorVersion = 1;
    auto headerData = Blob::blobWithMemoryAndSize(reinterpret_cast<const byte*>(&header), sizeof(header));
    decodedData->append(headerData);
    decodedData->append(this->base64DataFromMarkersV2());

    auto encodedData = Base64::encodeBlock(decodedData->data(), decodedData->size());

    TagLib::ByteVector newData((char*)encodedData->data(), encodedData->size());

    auto newFrame = new TagLib::ID3v2::GeneralEncapsulatedObjectFrame(newData);
    newFrame->setTextEncoding(TagLib::String::Latin1);
    newFrame->setMimeType("application/octet-stream");
    newFrame->setFileName("");
    newFrame->setDescription("Serato Markers2");

    auto tag = reinterpret_cast<TagLib::ID3v2::Tag*>(this->parsedFileTag);
    if (!tag) {
        // -- TODO: This should be asserted against.
        return;
    }

    TagLib::ID3v2::FrameListMap frameListMap = tag->frameListMap();
    TagLib::ID3v2::FrameList geobFrames = frameListMap["GEOB"];
    geobFrames.append(newFrame);
}

void ID3TrackFile::writeGridMarkersFrame(void)
{
    auto data = Blob::blob();

    GeobObjectStruct header;
    header.majorVersion = 1;
    header.minorVersion = 0;
    auto headerData = Blob::blobWithMemoryAndSize(reinterpret_cast<const byte*>(&header), sizeof(header));
    data->append(headerData);
    data->append(this->gridMarkerDataFromGridMarkers());

    TagLib::ByteVector newData((char*)data->data(), data->size());

    auto newFrame = new TagLib::ID3v2::GeneralEncapsulatedObjectFrame(newData);
    newFrame->setTextEncoding(TagLib::String::Latin1);
    newFrame->setMimeType("application/octet-stream");
    newFrame->setFileName("");
    newFrame->setDescription("Serato BeatGrid");

    auto tag = reinterpret_cast<TagLib::ID3v2::Tag*>(this->parsedFileTag);
    if (!tag) {
        // -- TODO: This should be asserted against.
        return;
    }

    auto frameListMap = tag->frameListMap();
    auto geobFrames = frameListMap["GEOB"];
    geobFrames.append(newFrame);
}

void ID3TrackFile::writeMarkers(void)
{
    auto tag = reinterpret_cast<TagLib::ID3v2::Tag*>(this->parsedFileTag);
    if (!tag) {
        return;
    }

    auto frameListMap = tag->frameListMap();
    if (frameListMap.contains("GEOB")) {
        auto geobFrames = frameListMap["GEOB"];

        auto frameToDelete = ID3TrackFile::frameInListWithDescription(geobFrames,
                                                                      String::stringWith("Serato Markers2"));
        if (frameToDelete != geobFrames.end()) {
            geobFrames.erase(frameToDelete);
        }

        frameToDelete = ID3TrackFile::frameInListWithDescription(geobFrames,
                                                                 String::stringWith("Serato BeatGrid"));
        if (frameToDelete != geobFrames.end()) {
            geobFrames.erase(frameToDelete);
        }
    }
    else if (this->cueMarkers->length() || this->loopMarkers->length() || this->gridMarkers->length()) {
        frameListMap["GEOB"] = TagLib::ID3v2::FrameList();
    }

    if (this->cueMarkers->length() || this->loopMarkers->length()) {
        this->writeMarkersV2Frame();
    }
    
    if (this->gridMarkers->length()) {
        this->writeGridMarkersFrame();
    }
}
