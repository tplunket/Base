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

// -- Generated internal implementation ommitted because this class does not use the default contructor.

namespace NxA { namespace Serato { namespace Internal {
    #pragma mark Structures
    typedef struct {
        byte mimeType[25];
        byte filename[1];
        byte description[16];
        byte majorVersion;
        byte minorVersion;
        byte data[0];
    } MP4MarkersHeaderStruct;

    #pragma mark Constants
    constexpr const character* mp4MarkersV1ItemName = "----:com.serato.dj:markers";
    constexpr const character* mp4MarkersV2ItemName = "----:com.serato.dj:markersv2";
    constexpr const character* mp4BeatgridItemName = "----:com.serato.dj:beatgrid";

    //constexpr const character* mp4MarkersV1FrameDescription = "Serato Markers";
    constexpr const character* mp4MarkersV2FrameDescription = "Serato Markers2";
    constexpr const character* mp4BeatgridFrameDescription = "Serato Beatgrid";
} } }

using namespace NxA;
using namespace NxA::Serato::Internal;

#pragma mark Constructors & Destructors

MP4TrackFile::MP4TrackFile(const String& path, const TagLibFilePointer& newFile) : TrackFile(path, newFile) { }

#pragma mark Instance Methods

integer MP4TrackFile::integerValueForItemNamed(const character* name) const
{
    auto item = this->mp4Tag->item(name);
    if (!item.isValid()) {
        return 0;
    }
    return item.toInt();
}

String::Pointer MP4TrackFile::stringValueForItemNamed(const character* name) const
{
    auto item = this->mp4Tag->item(name);
    if (!item.isValid()) {
        return String::string();
    }
    auto stringList = item.toStringList();
    if (stringList.size() == 0) {
        return String::string();
    }

    NXA_ASSERT_EQ(stringList.size(), 1);
    return String::stringWith(stringList.front().toCString());
}

void MP4TrackFile::setIntegerValueForItemNamed(integer value, const character* name)
{
    auto newItem = std::make_unique<TagLib::MP4::Item>(value);
    this->mp4Tag->setItem(name, *newItem);
}

void MP4TrackFile::setStringValueForItemNamed(const String& value, const character* name)
{
    auto newItem = std::make_unique<TagLib::MP4::Item>(TagLib::StringList(TagLib::String(value.toUTF8())));
    this->mp4Tag->setItem(name, *newItem);
}

void MP4TrackFile::readMarkers(void)
{
    auto markersItem = this->mp4Tag->item(mp4MarkersV2ItemName);
    if (markersItem.isValid() && (markersItem.atomDataType() == TagLib::MP4::AtomDataType::TypeUTF8)) {
        auto encodedData = markersItem.toStringList().toString();

        auto encodedDataSize = encodedData.size();
        if (encodedDataSize) {
            auto decodedData = Blob::blobWithBase64String(String::stringWith(encodedData.data(TagLib::String::UTF8).data(),
                                                                             encodedDataSize));
            auto headerStruct = reinterpret_cast<const MP4MarkersHeaderStruct*>(decodedData->data());
            if ((headerStruct->majorVersion == 1) && (headerStruct->minorVersion == 1)) {
                this->readMarkersV2FromBase64String(headerStruct->data, decodedData->size() - sizeof(MP4MarkersHeaderStruct));
            }
        }
    }

    auto beatgridItem = this->mp4Tag->item(mp4BeatgridItemName);
    if (beatgridItem.isValid() && (beatgridItem.atomDataType() == TagLib::MP4::AtomDataType::TypeUTF8)) {
        auto encodedData = beatgridItem.toStringList().toString();

        auto encodedDataSize = encodedData.size();
        if (encodedDataSize) {
            auto decodedData = Blob::blobWithBase64String(String::stringWith(encodedData.data(TagLib::String::UTF8).data(),
                                                                             encodedDataSize));
            auto headerStruct = reinterpret_cast<const MP4MarkersHeaderStruct*>(decodedData->data());
            if ((headerStruct->majorVersion == 1) && (headerStruct->minorVersion == 0)) {
                if ((decodedData->size() - sizeof(MP4MarkersHeaderStruct)) > 0) {
                    this->readGridMarkersFrom(headerStruct->data);
                }
            }
        }
    }
}

void MP4TrackFile::replaceFrameNamedWithDataAndVersion(const character* frameName, const character* frameDescription,
                                                       const Blob& frameData, int majorVersion, int minorVersion)
{
    this->mp4Tag->removeItem(frameName);

    if (!frameData.size()) {
        return;
    }

    auto decodedData = Blob::blob();

    MP4MarkersHeaderStruct header;
    memcpy(header.mimeType, "application/octet-stream", 25);
    header.filename[0] = 0;
    memcpy(header.description, frameDescription, 16);
    header.majorVersion = majorVersion;
    header.minorVersion = minorVersion;

    auto headerData = Blob::blobWithMemoryAndSize(reinterpret_cast<byte*>(&header), sizeof(header));
    decodedData->append(headerData);
    decodedData->append(frameData);

    auto encodedData = Blob::base64StringFor(decodedData->data(), decodedData->size());
    TagLib::String newString(encodedData->toUTF8(), TagLib::String::UTF8);
    TagLib::StringList newList(newString);

    TagLib::MP4::Item newItem(newList);
    newItem.setAtomDataType(TagLib::MP4::AtomDataType::TypeUTF8);
    NXA_ASSERT_TRUE(newItem.isValid());

    this->mp4Tag->setItem(frameName, newItem);
}

void MP4TrackFile::replaceMarkersV1Item(void)
{
    this->mp4Tag->removeItem(mp4MarkersV1ItemName);
    TagLib::String newString;
    TagLib::StringList newList(newString);

    TagLib::MP4::Item newItem(newList);
    newItem.setAtomDataType(TagLib::MP4::AtomDataType::TypeUTF8);
    NXA_ASSERT_TRUE(newItem.isValid());
    this->mp4Tag->setItem(mp4MarkersV1ItemName, newItem);
}

void MP4TrackFile::replaceMarkersV2Item(void)
{
    auto base64String = this->base64StringFromMarkersV2();
    auto frameData = Blob::blobWithStringWithoutTerminator(base64String);
    if (base64String->length() > 0)
    {
        constexpr integer paddingMulptipleOf = 256;
        count paddingSize = (((base64String->length() + paddingMulptipleOf - 1) / paddingMulptipleOf) * paddingMulptipleOf) - base64String->length();
        frameData->append(Blob::blobWithCapacity(paddingSize));
    }

    replaceFrameNamedWithDataAndVersion(mp4MarkersV2ItemName, mp4MarkersV2FrameDescription, frameData, 1, 1);
}

void MP4TrackFile::replaceGridMarkersItem(void)
{
    auto gridMarkerData = this->gridMarkerDataFromGridMarkers();
    replaceFrameNamedWithDataAndVersion(mp4BeatgridItemName, mp4BeatgridFrameDescription, gridMarkerData, 1, 0);
}

void MP4TrackFile::writeMarkers(void)
{
    this->replaceMarkersV1Item();
    this->replaceMarkersV2Item();
    this->replaceGridMarkersItem();
}
