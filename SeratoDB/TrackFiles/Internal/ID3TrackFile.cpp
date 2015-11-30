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

#include <mpegfile.h>
#include <attachedpictureframe.h>
#include <textidentificationframe.h>

// -- Generated internal implementation ommitted because this class does not use the default contructor.

namespace NxA { namespace Serato { namespace Internal {
    #pragma mark Structures
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

    #pragma mark Constants
    constexpr const character* id3MarkersV1FrameDescription = "Serato Markers_";
    constexpr const character* id3MarkersV2FrameDescription = "Serato Markers2";
    constexpr const character* id3BeatgridFrameDescription = "Serato BeatGrid";
} } }

using namespace NxA;
using namespace NxA::Serato::Internal;

#pragma mark Constructors & Destructors

ID3TrackFile::ID3TrackFile(const String& path, const TagLibFilePointer& newFile) : TrackFile(path, newFile) { }

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
        if (frame->description() == description.toUTF8()) {
            return it;
        }
    }

    return list.end();
}

String::Pointer ID3TrackFile::stringValueForFrameNamedInTag(const character* name, const TagLib::ID3v2::Tag* id3v2Tag)
{
    auto frameList = id3v2Tag->frameList(name);
    for (auto& frame : frameList) {
        auto stringValue = frame->toString();
        if (stringValue.length()) {
            return String::stringWith(stringValue.toCString());
        }
    }

    return String::string();
}

integer ID3TrackFile::integerValueForFrameNamedInTag(const character* name, const TagLib::ID3v2::Tag* id3v2Tag)
{
    return ID3TrackFile::stringValueForFrameNamedInTag(name, id3v2Tag)->integerValue();
}

void ID3TrackFile::setStringValueForFrameNamedInTag(const String& value, const character* name, TagLib::ID3v2::Tag* id3v2Tag)
{
    id3v2Tag->removeFrames(name);

    auto frame = new TagLib::ID3v2::TextIdentificationFrame(name, TagLib::String::Latin1);
    frame->setText(TagLib::StringList(TagLib::String(value.toUTF8())));
    id3v2Tag->addFrame(frame);
}

void ID3TrackFile::setIntegerValueForFrameNamedInTag(integer value, const character* name, TagLib::ID3v2::Tag* id3v2Tag)
{
    ID3TrackFile::setStringValueForFrameNamedInTag(String::stringWithFormat("%ld", value), name, id3v2Tag);
}


void ID3TrackFile::removeGEOBFrameNamedInTag(const String& name, TagLib::ID3v2::Tag* id3v2Tag)
{
    auto framesList = id3v2Tag->frameList("GEOB");
    if (!framesList.size()) {
        return;
    }

    auto frameToDelete = ID3TrackFile::frameInListWithDescription(framesList, name);
    if (frameToDelete != framesList.end()) {
        id3v2Tag->removeFrame(*frameToDelete);
    }
}

void ID3TrackFile::replaceMarkersFrameInTagWithEmptyFrame(TagLib::ID3v2::Tag* id3v2Tag)
{
    ID3TrackFile::removeGEOBFrameNamedInTag(String::stringWith(id3MarkersFrameDescription), id3v2Tag);

    auto newFrame = new TagLib::ID3v2::GeneralEncapsulatedObjectFrame();
    TagLib::ByteVector newData;
    newFrame->setObject(newData);
    newFrame->setTextEncoding(TagLib::String::Latin1);
    newFrame->setMimeType("application/octet-stream");
    newFrame->setFileName("");
    newFrame->setDescription(id3MarkersFrameDescription);

    id3v2Tag->addFrame(newFrame);
}

void ID3TrackFile::replaceFrameNamedInTagWithDataAndVersion(const String& frameName, TagLib::ID3v2::Tag* id3v2Tag,
                                                            const Blob& frameData, int majorVersion, int minorVersion)
{
    ID3TrackFile::removeGEOBFrameNamedInTag(frameName, id3v2Tag);

    if (!frameData.size()) {
        return;
    }

    auto objectData = Blob::blob();

    GeobObjectStruct header;
    header.majorVersion = majorVersion;
    header.minorVersion = minorVersion;
    auto headerData = Blob::blobWithMemoryAndSize(reinterpret_cast<const byte*>(&header), sizeof(header));
    objectData->append(headerData);
    objectData->append(frameData);

    TagLib::ByteVector newData((char*)objectData->data(), objectData->size());

    auto newFrame = new TagLib::ID3v2::GeneralEncapsulatedObjectFrame();
    newFrame->setObject(newData);
    newFrame->setTextEncoding(TagLib::String::Latin1);
    newFrame->setMimeType("application/octet-stream");
    newFrame->setFileName("");
    newFrame->setDescription(frameName.toUTF8());

    id3v2Tag->addFrame(newFrame);
}

void ID3TrackFile::replaceMarkersV1FrameInTagWith(TagLib::ID3v2::Tag* id3v2Tag, const Blob& markersV1Data)
{
    auto frameName = String::stringWith(id3MarkersV1FrameDescription);
    ID3TrackFile::replaceFrameNamedInTagWithDataAndVersion(frameName, id3v2Tag, markersV1Data, 2, 5);
}

void ID3TrackFile::replaceMarkersV2FrameInTagWith(TagLib::ID3v2::Tag* id3v2Tag, const String& base64MarkersData)
{
    auto frameName = String::stringWith(id3MarkersV2FrameDescription);
    auto frameData = Blob::blobWithStringWithoutTerminator(base64MarkersData);
    ID3TrackFile::replaceFrameNamedInTagWithDataAndVersion(frameName, id3v2Tag, frameData, 1, 1);
}

void ID3TrackFile::replaceGridMarkersFrameInTagWith(TagLib::ID3v2::Tag* id3v2Tag, const Blob& gridMarkersData)
{
    auto frameName = String::stringWith(id3BeatgridFrameDescription);
    ID3TrackFile::replaceFrameNamedInTagWithDataAndVersion(frameName, id3v2Tag, gridMarkersData, 1, 0);
}

String::Pointer ID3TrackFile::releaseDateFromTag(TagLib::ID3v2::Tag* id3v2Tag)
{
    auto date = ID3TrackFile::stringValueForFrameNamedInTag(id3ReleaseTimeFrameName, id3v2Tag);
    if (!date->length()) {
        date = ID3TrackFile::stringValueForFrameNamedInTag(id3OriginalReleaseTimeFrameName, id3v2Tag);
        if (!date->length()) {
            return ID3TrackFile::stringValueForFrameNamedInTag(id3RecordingTimeFrameName, id3v2Tag);
        }
    }

    if (date->length() == 4) {
        date = String::stringWithFormat("%s-01-01", date->toUTF8());
    }

    return date;
}

void ID3TrackFile::setReleaseDateForTag(const String& date, TagLib::ID3v2::Tag* id3v2Tag)
{
    ID3TrackFile::setStringValueForFrameNamedInTag(date, id3OriginalReleaseTimeFrameName, id3v2Tag);
    ID3TrackFile::setStringValueForFrameNamedInTag(date, id3RecordingTimeFrameName, id3v2Tag);
    ID3TrackFile::setStringValueForFrameNamedInTag(date, id3ReleaseTimeFrameName, id3v2Tag);
}

#pragma mark Instance Methods

String::Pointer ID3TrackFile::stringValueForFrameNamed(const character* name) const
{
    return ID3TrackFile::stringValueForFrameNamedInTag(name, this->id3v2Tag);
}

integer ID3TrackFile::integerValueForFrameNamed(const character* name) const
{
    return ID3TrackFile::integerValueForFrameNamedInTag(name, this->id3v2Tag);
}

void ID3TrackFile::setStringValueForFrameNamed(const String& value, const character* name)
{
    ID3TrackFile::setStringValueForFrameNamedInTag(value, name, this->id3v2Tag);
}

void ID3TrackFile::setIntegerValueForFrameNamed(integer value, const character* name)
{
    ID3TrackFile::setIntegerValueForFrameNamedInTag(value, name, this->id3v2Tag);
}

void ID3TrackFile::removeArtwork(void)
{
    auto frameList = this->id3v2Tag->frameList("APIC");
    auto frameToRemove = frameList.end();
    if (frameList.size()) {
        for (auto it = frameList.begin(); it != frameList.end(); ++it) {
            auto* pic = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame*>(*it);

            if (pic->type() == TagLib::ID3v2::AttachedPictureFrame::FrontCover) {
                frameToRemove = it;
                break;
            }
            else if (pic->type() == TagLib::ID3v2::AttachedPictureFrame::Other) {
                frameToRemove = it;
            }
        }
    }

    if (frameToRemove != frameList.end()) {
        this->id3v2Tag->removeFrame(*frameToRemove);
    }
}

void ID3TrackFile::readMarkers(void)
{
    auto framesList = this->id3v2Tag->frameList("GEOB");
    if (!framesList.size()) {
        return;
    }

    auto framePos = ID3TrackFile::frameInListWithDescription(framesList, String::stringWith(id3MarkersV2FrameDescription));
    if (framePos != framesList.end()) {
        auto frame = dynamic_cast<const TagLib::ID3v2::GeneralEncapsulatedObjectFrame*>(*framePos);
        if (isAValidGeobFrame(frame)) {
            auto frameObject = frame->object();
            auto headerStruct = reinterpret_cast<GeobObjectStruct*>(frameObject.data());
            if ((headerStruct->majorVersion == 1) && (headerStruct->minorVersion == 1)) {
                count size = frameObject.size() - sizeof(GeobObjectStruct);
                if (size) {
                    this->readMarkersV2FromBase64String(headerStruct->data, size);
                }
            }
        }
    }

    framePos = ID3TrackFile::frameInListWithDescription(framesList, String::stringWith(id3BeatgridFrameDescription));
    if (framePos != framesList.end()) {
        auto frame = dynamic_cast<const TagLib::ID3v2::GeneralEncapsulatedObjectFrame*>(*framePos);
        if (isAValidGeobFrame(frame)) {
            auto frameObject = frame->object();
            auto headerStruct = reinterpret_cast<GeobObjectStruct*>(frameObject.data());
            if ((headerStruct->majorVersion == 1) && (headerStruct->minorVersion == 0)) {
                count size = frame->size() - sizeof(GeobObjectStruct);
                if (size) {
                    this->readGridMarkersFrom(headerStruct->data);
                }
            }
        }
    }
}

void ID3TrackFile::writeMarkers(void)
{
    ID3TrackFile::replaceMarkersFrameInTagWithEmptyFrame(this->id3v2Tag);

    ID3TrackFile::replaceMarkersV2FrameInTagWith(this->id3v2Tag, this->base64StringFromMarkersV2());
    ID3TrackFile::replaceGridMarkersFrameInTagWith(this->id3v2Tag, this->gridMarkerDataFromGridMarkers());
}
