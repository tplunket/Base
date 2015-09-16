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
#include <mp4file.h>

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, MP4TrackFile, TrackFile);

namespace NxA { namespace Serato { namespace Internal {
    #pragma mark Constants
    constexpr const character* mp4KeyItemName = "----:com.apple.iTunes:initialkey";
    constexpr const character* mp4ComposerItemName = "\251wrt";
    constexpr const character* mp4GroupingItemName = "\251grp";
    constexpr const character* mp4BpmItemName = "tmpo";
    constexpr const character* mp4ArtworkItemName = "covr";
} } }

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Factory Methods

MP4TrackFile::Pointer MP4TrackFile::fileWithFileAt(const String& path, TrackFile::Flags flags)
{
    auto file = Internal::TagLibFilePointer(std::make_shared<TagLib::MP4::File>(path.toUTF8()));
    if (!file->isValid()) {
        throw TrackFileError::exceptionWith("Error loading track file '%s'.", path.toUTF8());
    }

    auto mp4File = dynamic_cast<TagLib::MP4::File*>(&(*file));

    auto internalObject = Internal::MP4TrackFile::Pointer(std::make_shared<Internal::MP4TrackFile>(path, file));
    auto newFile = MP4TrackFile::makeSharedWithInternal(NxA::Internal::Object::Pointer::dynamicCastFrom(internalObject));
    newFile->internal->tag = newFile->internal->mp4Tag = mp4File->tag();
    if (!newFile->internal->tag) {
        throw TrackFileError::exceptionWith("Error reading tags from track file '%s'.", path.toUTF8());
    }

    if (flags & TrackFile::Flags::IgnoreMarkers) {
        newFile->internal->markersWereIgnored = true;
    }
    else {
        newFile->internal->readMarkers();
    }

    return newFile;
}

#pragma mark Instance Methods

boolean MP4TrackFile::hasKey(void) const
{
    return true;
}

String::Pointer MP4TrackFile::key(void) const
{
    return internal->stringValueForItemNamed(Internal::mp4KeyItemName);
}

String::Pointer MP4TrackFile::composer(void) const
{
    return internal->stringValueForItemNamed(Internal::mp4ComposerItemName);
}

String::Pointer MP4TrackFile::grouping(void) const
{
    return internal->stringValueForItemNamed(Internal::mp4GroupingItemName);
}

String::Pointer MP4TrackFile::bpm(void) const
{
    return String::stringWithFormat("%d", internal->integerValueForItemNamed(Internal::mp4BpmItemName));
}

boolean MP4TrackFile::hasRecordLabel(void) const
{
    return false;
}

String::Pointer MP4TrackFile::recordLabel(void) const
{
    return String::string();
}

boolean MP4TrackFile::hasRemixer(void) const
{
    return false;
}

String::Pointer MP4TrackFile::remixer(void) const
{
    return String::string();
}

boolean MP4TrackFile::hasRating(void) const
{
    return false;
}

integer MP4TrackFile::rating(void) const
{
    return 0;
}

Blob::Pointer MP4TrackFile::artwork(void) const
{
    auto item = internal->mp4Tag->item(Internal::mp4ArtworkItemName);
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
    internal->setStringValueForItemNamed(key, Internal::mp4KeyItemName);
    internal->metadataWasModified = true;
}

void MP4TrackFile::setComposer(const String& composer)
{
    internal->setStringValueForItemNamed(composer, Internal::mp4ComposerItemName);
    internal->metadataWasModified = true;
}

void MP4TrackFile::setGrouping(const String& grouping)
{
    internal->setStringValueForItemNamed(grouping, Internal::mp4GroupingItemName);
    internal->metadataWasModified = true;
}

void MP4TrackFile::setBpm(const String& bpm)
{
    internal->setIntegerValueForItemNamed(bpm.integerValue(), Internal::mp4BpmItemName);
    internal->metadataWasModified = true;
}

void MP4TrackFile::setRecordLabel(const String& recordLabel)
{
    // -- This is not supported by MP4 files.
}

void MP4TrackFile::setRemixer(const String& remixer)
{
    // -- This is not supported by MP4 files.
}

void MP4TrackFile::setRating(integer rating)
{
    // -- This is not supported by MP4 files.
}

void MP4TrackFile::setArtwork(const Blob& artwork)
{
    TagLib::ByteVector data(*artwork.data(), artwork.size());
    TagLib::MP4::CoverArt newCoverArt(TagLib::MP4::CoverArt::Unknown, data);
    TagLib::MP4::CoverArtList newCoverArtList;
    newCoverArtList.append(newCoverArt);

    TagLib::MP4::Item newItem(newCoverArtList);
    // -- TODO: This needs to be set to the correct type.
    newItem.setAtomDataType(TagLib::MP4::AtomDataType::TypePNG);

    internal->mp4Tag->setItem(Internal::mp4ArtworkItemName, newItem);
    internal->metadataWasModified = true;
}
