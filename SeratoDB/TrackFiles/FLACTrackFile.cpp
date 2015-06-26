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

#include "TrackFiles/FLACTrackFile.hpp"
#include "TrackFiles/Internal/FLACTrackFile.hpp"

#include <flacfile.h>

NXA_GENERATED_IMPLEMENTATION_FOR(NxA::Serato, FLACTrackFile, TrackFile);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Factory Methods

FLACTrackFile::Pointer FLACTrackFile::fileWithFileAt(String::Pointer const& path)
{
    auto file = Internal::TagLibFilePointer(std::make_shared<TagLib::FLAC::File>(path->toUTF8()));
    auto internalObject = Internal::FLACTrackFile::Pointer(std::make_shared<Internal::FLACTrackFile>(path, file));
    auto newFile = FLACTrackFile::makeSharedWithInternal(NxA::Internal::Object::Pointer::dynamicCastFrom(internalObject));

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

bool FLACTrackFile::hasKey(void) const
{
    return true;
}

String::Pointer FLACTrackFile::key(void) const
{
    auto text = internal->properties["INITIALKEY"].toString();
    if (text != TagLib::String::null) {
        return String::stringWith(text.toCString());
    }

    return String::string();
}

String::Pointer FLACTrackFile::grouping(void) const
{
    auto text = internal->properties["GROUPING"].toString();
    if (text != TagLib::String::null) {
        return String::stringWith(text.toCString());
    }

    return String::string();
}

uinteger32 FLACTrackFile::lengthInMilliseconds(void) const
{
    if (internal->audioProperties) {
        auto audioProperties = reinterpret_cast<const TagLib::FLAC::Properties*>(internal->audioProperties);

        auto numberOfFrames = audioProperties->sampleFrames();
        auto sampleRate = audioProperties->sampleRate();

        uinteger32 numberOfMilliseconds = (static_cast<double>(numberOfFrames) * 1000.0) / static_cast<double>(sampleRate);
        return numberOfMilliseconds;
    }

    return 0;
}

uinteger32 FLACTrackFile::bitDepthInBitsOrZeroIfNotApplicable(void) const
{
    if (internal->audioProperties) {
        auto audioProperties = reinterpret_cast<const TagLib::FLAC::Properties*>(internal->audioProperties);
        return audioProperties->sampleWidth();
    }

    return 0;
}

bool FLACTrackFile::hasRecordLabel(void) const
{
    return false;
}

String::Pointer FLACTrackFile::recordLabel(void) const
{
    return String::string();
}

bool FLACTrackFile::hasRemixer(void) const
{
    return false;
}

String::Pointer FLACTrackFile::remixer(void) const
{
    return String::string();
}

String::Pointer FLACTrackFile::yearReleased(void) const
{
    auto text = internal->properties["DATE"].toString();
    if (text != TagLib::String::null) {
        return String::stringWith(text.toCString());
    }

    return String::string();
}

Blob::Pointer FLACTrackFile::artwork(void) const
{
    // -- TODO: To be implemented.
    return Blob::blob();
}

void FLACTrackFile::setKey(String::Pointer const& key)
{
    internal->properties["INITIALKEY"] = TagLib::String(key->toUTF8());
}

void FLACTrackFile::setGrouping(String::Pointer const& grouping)
{
    internal->properties["GROUPING"] = TagLib::String(grouping->toUTF8());
}

void FLACTrackFile::setRecordLabel(String::Pointer const& recordLabel)
{
    // -- This is not supported by FLAC files.
}

void FLACTrackFile::setRemixer(String::Pointer const& remixer)
{
    // -- This is not supported by FLAC files.
}

void FLACTrackFile::setYearReleased(String::Pointer const& year)
{
    internal->properties["DATE"] = TagLib::String(year->toUTF8());
}

void FLACTrackFile::setArtwork(Blob::Pointer const& artwork)
{
    // -- TODO: To be implemented.
}
