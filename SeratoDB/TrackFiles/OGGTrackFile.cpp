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

#include "TrackFiles/OGGTrackFile.hpp"
#include "TrackFiles/Internal/OGGTrackFile.hpp"

#include <taglib/vorbisfile.h>

using namespace NxA;
using namespace NxA::Serato;

NXA_GENERATED_IMPLEMENTATION_FOR(NxA::Serato, OGGTrackFile);

#pragma mark Constructors & Destructors

OGGTrackFile::OGGTrackFile(NxA::Internal::Object::Pointer const& initial_internal) :
                           ID3TrackFile(initial_internal),
                           internal(initial_internal) { }

#pragma mark Factory Methods

OGGTrackFile::Pointer OGGTrackFile::fileWithFileAt(String::ConstPointer const& path)
{
    auto file = Internal::TagLibFilePointer(std::make_shared<TagLib::Vorbis::File>(path->toUTF8()));
    auto internalObject = Internal::OGGTrackFile::Pointer(std::make_shared<Internal::OGGTrackFile>(path, file));
    auto newFile = OGGTrackFile::makeSharedWithInternal(internalObject);

    if (!file->isValid()) {
        newFile->internal->parsedFileTag = nullptr;
        newFile->internal->audioProperties = nullptr;
        return newFile;
    }

    newFile->internal->parsedFileTag = file->tag();
    newFile->internal->audioProperties = file->audioProperties();
    newFile->internal->properties = file->properties();

    newFile->internal->readMarkers();

    return newFile;
}

#pragma mark Instance Methods

bool OGGTrackFile::hasKey(void) const
{
    return false;
}

String::Pointer OGGTrackFile::key(void) const
{
    return String::string();
}

String::Pointer OGGTrackFile::grouping(void) const
{
    auto text = internal->properties["GROUPING"].toString();
    if (text != TagLib::String::null) {
        return String::stringWith(text.toCString());
    }

    return String::string();
}

bool OGGTrackFile::hasRecordLabel(void) const
{
    return true;
}

String::Pointer OGGTrackFile::recordLabel(void) const
{
    auto text = internal->properties["LABEL"].toString();
    if (text != TagLib::String::null) {
        return String::stringWith(text.toCString());
    }

    return String::string();
}

bool OGGTrackFile::hasRemixer(void) const
{
    return true;
}

String::Pointer OGGTrackFile::remixer(void) const
{
    auto text = internal->properties["REMIXER"].toString();
    if (text != TagLib::String::null) {
        return String::stringWith(text.toCString());
    }

    return String::string();
}

String::Pointer OGGTrackFile::yearReleased(void) const
{
    auto text = internal->properties["YEAR"].toString();
    if (text != TagLib::String::null) {
        return String::stringWith(text.toCString());
    }

    return String::string();
}

Blob::Pointer OGGTrackFile::artwork(void) const
{
    // -- TODO: To be implemented.
    return Blob::blob();
}

void OGGTrackFile::setKey(String::ConstPointer const& key)
{
    // -- This is not supported by OGG files.
}

void OGGTrackFile::setGrouping(String::ConstPointer const& grouping)
{
    internal->properties["GROUPING"] = TagLib::String(grouping->toUTF8());
}

void OGGTrackFile::setRecordLabel(String::ConstPointer const& recordLabel)
{
    internal->properties["LABEL"] = TagLib::String(recordLabel->toUTF8());
}

void OGGTrackFile::setRemixer(String::ConstPointer const& remixer)
{
    internal->properties["REMIXER"] = TagLib::String(remixer->toUTF8());
}

void OGGTrackFile::setYearReleased(String::ConstPointer const& year)
{
    internal->properties["YEAR"] = TagLib::String(year->toUTF8());
}

void OGGTrackFile::setArtwork(Blob::ConstPointer const& artwork)
{
    // -- TODO: To be implemented.
}
