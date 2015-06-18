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

#include "TrackFiles/ID3TrackFile.hpp"
#include "Base64.hpp"

#include <taglib/generalencapsulatedobjectframe.h>
#include <taglib/id3v2tag.h>
#include <taglib/attachedpictureframe.h>

using namespace NxA::Serato;
using namespace TagLib;
using namespace std;

#pragma mark Constants

static const string emptyString("");

#pragma mark Structures

typedef struct {
    unsigned char dummy;
    unsigned char mimeType[25];
    unsigned char filename;
    unsigned char description[16];
    unsigned char majorVersion;
    unsigned char minorVersion;
} SeratoGeobBodyHeaderStruct;

typedef struct {
    unsigned char majorVersion;
    unsigned char minorVersion;
    unsigned char data[0];
} SeratoGeobObjectStruct;

#pragma mark Utility Functions

static bool p_isAValidGeobFrame(const ID3v2::GeneralEncapsulatedObjectFrame* frame)
{
    String frameID(frame->frameID().data());
    if (frameID != "GEOB") {
        return false;
    }

    String mimeType = frame->mimeType();
    return mimeType == "application/octet-stream";
}

static ID3v2::FrameList::Iterator p_frameInListWithDescription(ID3v2::FrameList& list, const char* description)
{
    for (ID3v2::FrameList::Iterator it = list.begin(); it != list.end(); ++it) {
        ID3v2::GeneralEncapsulatedObjectFrame* frame = (ID3v2::GeneralEncapsulatedObjectFrame*)*it;
        if (!p_isAValidGeobFrame(frame)) {
            continue;
        }

        if (frame->description() == description) {
            return it;
        }
    }

    return list.end();
}

#pragma mark Instance Methods

void ID3TrackFile::p_readMarkers(void)
{
    ID3v2::Tag* tag = (ID3v2::Tag*)this->p_parsedFileTag;
    if (!tag) {
        return;
    }

    ID3v2::FrameListMap frameListMap = tag->frameListMap();
    if (!frameListMap.contains("GEOB")) {
        return;
    }

    ID3v2::FrameList geobFrames = frameListMap["GEOB"];
    for (auto& framePtr : geobFrames) {
        const ID3v2::GeneralEncapsulatedObjectFrame* frame = (const ID3v2::GeneralEncapsulatedObjectFrame*)framePtr;
        if (!p_isAValidGeobFrame(frame)) {
            continue;
        }

        const TagLib::String description = frame->description();
        if (description == "Serato Markers2") {
            uint32_t encodedDataSize = frame->size() - sizeof(SeratoGeobBodyHeaderStruct);
            const ByteVector frameObject = frame->object();

            SeratoGeobObjectStruct* objectData = (SeratoGeobObjectStruct*)frameObject.data();
            this->p_readMarkersV2FromBase64Data((char*)objectData->data, encodedDataSize);
        }
        else if (description == "Serato BeatGrid") {
            uint32_t dataSize = frame->size() - sizeof(SeratoGeobBodyHeaderStruct);
            const ByteVector frameObject = frame->object();

            SeratoGeobObjectStruct* headerStruct = (SeratoGeobObjectStruct*)frameObject.data();
            if ((headerStruct->majorVersion == 1) && (headerStruct->minorVersion == 0)) {
                this->p_readGridMarkersFrom((char*)headerStruct->data, dataSize);
            }
        }
    }
}

void ID3TrackFile::p_writeMarkersV2Frame(void)
{
    auto decodedData = Blob::blob();

    SeratoGeobObjectStruct header;
    header.majorVersion = 1;
    header.minorVersion = 1;
    auto headerData = Blob::blobWithMemoryAndSize((char*)&header, sizeof(header));
    decodedData->append(headerData);
    decodedData->append(this->p_base64DataFromMarkersV2());

    auto encodedData = Base64::encodeBlock(decodedData->data(), decodedData->size());

    ByteVector newData((char*)encodedData->data(), encodedData->size());

    ID3v2::GeneralEncapsulatedObjectFrame* newFrame = new ID3v2::GeneralEncapsulatedObjectFrame(newData);
    newFrame->setTextEncoding(TagLib::String::Latin1);
    newFrame->setMimeType("application/octet-stream");
    newFrame->setFileName("");
    newFrame->setDescription("Serato Markers2");

    ID3v2::Tag* tag = (ID3v2::Tag*)this->p_parsedFileTag;
    if (!tag) {
        // -- TODO: This should be asserted against.
        return;
    }

    ID3v2::FrameListMap frameListMap = tag->frameListMap();
    ID3v2::FrameList geobFrames = frameListMap["GEOB"];
    geobFrames.append(newFrame);
}

void ID3TrackFile::p_writeGridMarkersFrame(void)
{
    auto data = Blob::blob();

    SeratoGeobObjectStruct header;
    header.majorVersion = 1;
    header.minorVersion = 0;
    auto headerData = Blob::blobWithMemoryAndSize((char*)&header, sizeof(header));
    data->append(headerData);
    data->append(this->p_gridMarkerDataFromGridMarkers());

    ByteVector newData((char*)data->data(), data->size());

    ID3v2::GeneralEncapsulatedObjectFrame* newFrame = new ID3v2::GeneralEncapsulatedObjectFrame(newData);
    newFrame->setTextEncoding(TagLib::String::Latin1);
    newFrame->setMimeType("application/octet-stream");
    newFrame->setFileName("");
    newFrame->setDescription("Serato BeatGrid");

    ID3v2::Tag* tag = (ID3v2::Tag*)this->p_parsedFileTag;
    if (!tag) {
        // -- TODO: This should be asserted against.
        return;
    }

    ID3v2::FrameListMap frameListMap = tag->frameListMap();
    ID3v2::FrameList geobFrames = frameListMap["GEOB"];
    geobFrames.append(newFrame);
}

void ID3TrackFile::p_writeMarkers(void)
{
    ID3v2::Tag* tag = (ID3v2::Tag*)this->p_parsedFileTag;
    if (!tag) {
        return;
    }

    ID3v2::FrameListMap frameListMap = tag->frameListMap();
    if (frameListMap.contains("GEOB")) {
        ID3v2::FrameList geobFrames = frameListMap["GEOB"];

        ID3v2::FrameList::Iterator frameToDelete = p_frameInListWithDescription(geobFrames, "Serato Markers2");
        if (frameToDelete != geobFrames.end()) {
            geobFrames.erase(frameToDelete);
        }

        frameToDelete = p_frameInListWithDescription(geobFrames, "Serato BeatGrid");
        if (frameToDelete != geobFrames.end()) {
            geobFrames.erase(frameToDelete);
        }
    }
    else if (this->cueMarkers()->length() || this->loopMarkers()->length() || this->gridMarkers()->length()) {
        frameListMap["GEOB"] = ID3v2::FrameList();
    }

    if (this->cueMarkers()->length() || this->loopMarkers()->length()) {
        this->p_writeMarkersV2Frame();
    }

    if (this->gridMarkers()->length()) {
        this->p_writeGridMarkersFrame();
    }
}

bool ID3TrackFile::hasKey(void) const
{
    return true;
}

string ID3TrackFile::key(void) const
{
    TagLib::String text = this->p_properties["INITIALKEY"].toString();
    if (text != TagLib::String::null) {
        return text.to8Bit();
    }

    return emptyString;
}

string ID3TrackFile::grouping(void) const
{
    TagLib::String text = this->p_properties["CONTENTGROUP"].toString();
    if (text != TagLib::String::null) {
        return text.to8Bit();
    }

    return emptyString;
}

bool ID3TrackFile::hasRecordLabel(void) const
{
    return true;
}

string ID3TrackFile::recordLabel(void) const
{
    TagLib::String text = this->p_properties["LABEL"].toString();
    if (text != TagLib::String::null) {
        return text.to8Bit();
    }

    return emptyString;
}

bool ID3TrackFile::hasRemixer(void) const
{
    return true;
}

string ID3TrackFile::remixer(void) const
{
    TagLib::String text = this->p_properties["REMIXER"].toString();
    if (text != TagLib::String::null) {
        return text.to8Bit();
    }

    return emptyString;
}

string ID3TrackFile::yearReleased(void) const
{
    TagLib::String text = this->p_properties["DATE"].toString();
    if (text != TagLib::String::null) {
        return text.to8Bit();
    }

    return emptyString;
}

CharVectorPtr ID3TrackFile::artwork(void) const
{
    CharVectorPtr result;

    ID3v2::Tag* tag = (ID3v2::Tag*)this->p_parsedFileTag;
    if(tag) {
        ID3v2::FrameListMap frameListMap = tag->frameListMap();
        ID3v2::FrameList frameList = frameListMap["APIC"];

        const ID3v2::AttachedPictureFrame* artworkFrame = nullptr;

        for (const ID3v2::Frame* frame : frameList) {
            const ID3v2::AttachedPictureFrame* pic = dynamic_cast<const ID3v2::AttachedPictureFrame*>(frame);
            if (pic->type() == ID3v2::AttachedPictureFrame::FrontCover) {
                artworkFrame = pic;
                break;
            }
            else if (pic->type() == ID3v2::AttachedPictureFrame::Other) {
                artworkFrame = pic;
            }
        }

        if (artworkFrame) {
            ByteVector picture(artworkFrame->picture());
            size_t size = picture.size();
            if (size) {
                char* data = picture.data();
                result = make_unique<CharVector>(data, data + size);
            }
        }
    }

    return move(result);
}

void ID3TrackFile::setKey(const char* key)
{
    this->p_properties["INITIALKEY"] = TagLib::String(key);
}

void ID3TrackFile::setGrouping(const char* grouping)
{
    this->p_properties["CONTENTGROUP"] = TagLib::String(grouping);
}

void ID3TrackFile::setRecordLabel(const char* recordLabel)
{
    this->p_properties["RECORD"] = TagLib::String(recordLabel);
}

void ID3TrackFile::setRemixer(const char* remixer)
{
    this->p_properties["REMIXER"] = TagLib::String(remixer);
}

void ID3TrackFile::setYearReleased(const char* year)
{
    this->p_properties["DATE"] = TagLib::String(year);
}

void ID3TrackFile::setArtwork(CharVectorPtr artwork)
{
    ID3v2::Tag* tag = (ID3v2::Tag*)this->p_parsedFileTag;
    if(tag) {
        ID3v2::FrameListMap frameListMap = tag->frameListMap();
        ID3v2::FrameList frameList = frameListMap["APIC"];

        ID3v2::FrameList::Iterator frameToRemove = frameList.end();
        for (ID3v2::FrameList::Iterator it = frameList.begin(); it != frameList.end(); ++it) {
            ID3v2::AttachedPictureFrame* pic = dynamic_cast<ID3v2::AttachedPictureFrame*>(*it);

            if (pic->type() == ID3v2::AttachedPictureFrame::FrontCover) {
                frameToRemove = it;
                break;
            }
            else if (pic->type() == ID3v2::AttachedPictureFrame::Other) {
                frameToRemove = it;
            }
        }

        frameList.erase(frameToRemove);

        ByteVector data(artwork->data(), artwork->size());

        ID3v2::AttachedPictureFrame* newFrame = new ID3v2::AttachedPictureFrame;
        newFrame->setData(data);
        newFrame->setType(ID3v2::AttachedPictureFrame::FrontCover);
        newFrame->setDescription("");
        newFrame->setTextEncoding(TagLib::String::Latin1);
        frameList.append(newFrame);
    }
}
