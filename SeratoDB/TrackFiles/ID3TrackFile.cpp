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
#include "TrackFiles/Internal/ID3TrackFile.hpp"

#include <attachedpictureframe.h>

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, ID3TrackFile, TrackFile);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Instance Methods

bool ID3TrackFile::hasKey(void) const
{
    return true;
}

String::Pointer ID3TrackFile::key(void) const
{
    auto text = internal->properties["INITIALKEY"].toString();
    if (text != TagLib::String::null) {
        return String::stringWith(text.toCString());
    }

    return String::string();
}

String::Pointer ID3TrackFile::grouping(void) const
{
    auto text = internal->properties["CONTENTGROUP"].toString();
    if (text != TagLib::String::null) {
        return String::stringWith(text.toCString());
    }

    return String::string();
}

bool ID3TrackFile::hasRecordLabel(void) const
{
    return true;
}

String::Pointer ID3TrackFile::recordLabel(void) const
{
    auto text = internal->properties["LABEL"].toString();
    if (text != TagLib::String::null) {
        return String::stringWith(text.toCString());
    }

    return String::string();
}

bool ID3TrackFile::hasRemixer(void) const
{
    return true;
}

String::Pointer ID3TrackFile::remixer(void) const
{
    auto text = internal->properties["REMIXER"].toString();
    if (text != TagLib::String::null) {
        return String::stringWith(text.toCString());
    }

    return String::string();
}

String::Pointer ID3TrackFile::yearReleased(void) const
{
    auto text = internal->properties["DATE"].toString();
    if (text != TagLib::String::null) {
        return String::stringWith(text.toCString());
    }

    return String::string();
}

Blob::Pointer ID3TrackFile::artwork(void) const
{
    auto tag = dynamic_cast<TagLib::ID3v2::Tag*>(internal->parsedFileTag);
    if(!tag) {
        return Blob::blob();
    }

    auto frameListMap = tag->frameListMap();
    auto frameList = frameListMap["APIC"];

    const TagLib::ID3v2::AttachedPictureFrame* artworkFrame = nullptr;

    for (const TagLib::ID3v2::Frame* frame : frameList) {
        auto pic = dynamic_cast<const TagLib::ID3v2::AttachedPictureFrame*>(frame);
        if (pic->type() == TagLib::ID3v2::AttachedPictureFrame::FrontCover) {
            artworkFrame = pic;
            break;
        }
        else if (pic->type() == TagLib::ID3v2::AttachedPictureFrame::Other) {
            artworkFrame = pic;
        }
    }

    if (artworkFrame) {
        auto picture = artworkFrame->picture();
        auto size = picture.size();
        if (size) {
            auto data = picture.data();
            return Blob::blobWithMemoryAndSize(reinterpret_cast<byte *>(data), size);
        }
    }

    return Blob::blob();
}

void ID3TrackFile::setKey(const String& key)
{
    internal->properties["INITIALKEY"] = TagLib::String(key.toUTF8());
}

void ID3TrackFile::setGrouping(const String& grouping)
{
    internal->properties["CONTENTGROUP"] = TagLib::String(grouping.toUTF8());
}

void ID3TrackFile::setRecordLabel(const String& recordLabel)
{
    internal->properties["RECORD"] = TagLib::String(recordLabel.toUTF8());
}

void ID3TrackFile::setRemixer(const String& remixer)
{
    internal->properties["REMIXER"] = TagLib::String(remixer.toUTF8());
}

void ID3TrackFile::setYearReleased(const String& year)
{
    internal->properties["DATE"] = TagLib::String(year.toUTF8());
}

void ID3TrackFile::setArtwork(Blob::Pointer const& artwork)
{
    auto tag = reinterpret_cast<TagLib::ID3v2::Tag*>(internal->parsedFileTag);
    if(tag) {
        auto frameListMap = tag->frameListMap();
        auto frameList = frameListMap["APIC"];

        auto frameToRemove = frameList.end();
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

        frameList.erase(frameToRemove);

        TagLib::ByteVector data(*artwork->data(), artwork->size());

        auto* newFrame = new TagLib::ID3v2::AttachedPictureFrame;
        newFrame->setData(data);
        newFrame->setType(TagLib::ID3v2::AttachedPictureFrame::FrontCover);
        newFrame->setDescription("");
        newFrame->setTextEncoding(TagLib::String::Latin1);
        frameList.append(newFrame);
    }
}
