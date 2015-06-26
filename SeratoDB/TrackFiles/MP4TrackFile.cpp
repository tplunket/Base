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

#include "TrackFiles/MP4TrackFile.hpp"
#include "TrackFiles/Internal/MP4TrackFile.hpp"

#include <mp4tag.h>

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, MP4TrackFile, TrackFile);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Factory Methods

MP4TrackFile::Pointer MP4TrackFile::fileWithFileAt(const String& path)
{
    auto file = Internal::TagLibFilePointer(std::make_shared<TagLib::MP4::File>(path.toUTF8()));
    auto internalObject = Internal::MP4TrackFile::Pointer(std::make_shared<Internal::MP4TrackFile>(path, file));
    auto newFile = MP4TrackFile::makeSharedWithInternal(NxA::Internal::Object::Pointer::dynamicCastFrom(internalObject));

    if (!file->isValid()) {
        newFile->internal->itemListMap = nullptr;
        newFile->internal->parsedFileTag = nullptr;
        newFile->internal->audioProperties = nullptr;
        return newFile;
    }

    auto tag = reinterpret_cast<TagLib::MP4::Tag*>(file->tag());
    newFile->internal->parsedFileTag = tag;
    newFile->internal->itemListMap = &(tag->itemListMap());
    newFile->internal->audioProperties = file->audioProperties();
    newFile->internal->properties = file->properties();

    newFile->internal->readMarkers();

    return newFile;
}

#pragma mark Instance Methods

bool MP4TrackFile::hasKey(void) const
{
    return true;
}

String::Pointer MP4TrackFile::key(void) const
{
    auto item = (*internal->itemListMap)["----:com.apple.iTunes:initialkey"];
    if (item.isValid()) {
        auto text = item.toStringList().front();
        if (text != TagLib::String::null) {
            return String::stringWith(text.toCString());
        }
    }

    return String::string();
}

String::Pointer MP4TrackFile::grouping(void) const
{
    auto text = internal->properties["GROUPING"].toString();
    if (text != TagLib::String::null) {
        return String::stringWith(text.toCString());
    }

    return String::string();
}

bool MP4TrackFile::hasRecordLabel(void) const
{
    return false;
}

String::Pointer MP4TrackFile::recordLabel(void) const
{
    return String::string();
}

bool MP4TrackFile::hasRemixer(void) const
{
    return false;
}

String::Pointer MP4TrackFile::remixer(void) const
{
    return String::string();
}

String::Pointer MP4TrackFile::yearReleased(void) const
{
    auto text = internal->properties["DATE"].toString();
    if (text != TagLib::String::null) {
        return String::stringWith(text.toCString());
    }

    return String::string();
}

Blob::Pointer MP4TrackFile::artwork(void) const
{
    TagLib::MP4::Item item = (*internal->itemListMap)["covr"];
    if (item.isValid()) {
        auto coverArtList = item.toCoverArtList();
        auto coverArt = coverArtList.front();
        auto coverArtData = coverArt.data();
        auto size = coverArtData.size();
        if (size) {
            const byte* data = reinterpret_cast<const byte*>(coverArtData.data());
            return Blob::blobWithMemoryAndSize(data, size);
        }
    }

    return Blob::blob();
}

void MP4TrackFile::setKey(const String& key)
{
    TagLib::StringList newList;
    newList.append(TagLib::String(key.toUTF8()));
    TagLib::MP4::Item newItem(newList);
    (*internal->itemListMap)["----:com.apple.iTunes:initialkey"] = newItem;
}

void MP4TrackFile::setGrouping(const String& grouping)
{
    internal->properties["GROUPING"] = TagLib::String(grouping.toUTF8());
}

void MP4TrackFile::setRecordLabel(const String& recordLabel)
{
    // -- This is not supported by MP4 files.
}

void MP4TrackFile::setRemixer(const String& remixer)
{
    // -- This is not supported by MP4 files.
}

void MP4TrackFile::setYearReleased(const String& year)
{
    internal->properties["DATE"] = TagLib::String(year.toUTF8());
}

void MP4TrackFile::setArtwork(Blob::Pointer const& artwork)
{
    TagLib::ByteVector data(*artwork->data(), artwork->size());
    TagLib::MP4::CoverArt newCoverArt(TagLib::MP4::CoverArt::Unknown, data);
    TagLib::MP4::CoverArtList newCoverArtList;
    newCoverArtList.append(newCoverArt);

    TagLib::MP4::Item newItem(newCoverArtList);
    (*internal->itemListMap)["covr"] = newItem;
}
