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

#include "TrackFiles/AIFFTrackFile.hpp"
#include "TrackFiles/Internal/AIFFTrackFile.hpp"

#include <aifffile.h>

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, AIFFTrackFile, ID3TrackFile);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Factory Methods

AIFFTrackFile::Pointer AIFFTrackFile::fileWithFileAt(const String& path)
{
    auto file = Internal::TagLibFilePointer(std::make_shared<TagLib::RIFF::AIFF::File>(path.toUTF8()));
    if (!file->isValid()) {
        throw TrackFileError::exceptionWith("Error loading track file '%s'.", path.toUTF8());
    }

    auto internalObject = Internal::AIFFTrackFile::Pointer(std::make_shared<Internal::AIFFTrackFile>(path, file));
    auto newFile = AIFFTrackFile::makeSharedWithInternal(NxA::Internal::Object::Pointer::dynamicCastFrom(internalObject));

    newFile->internal->parsedFileTag = file->tag();
    if (!newFile->internal->parsedFileTag) {
        throw TrackFileError::exceptionWith("Error reading tags from track file '%s'.", path.toUTF8());
    }

    newFile->internal->audioProperties = file->audioProperties();
    newFile->internal->properties = file->properties();

    newFile->internal->readMarkers();

    return newFile;
}

#pragma mark Instance Methods

uinteger32 AIFFTrackFile::lengthInMilliseconds(void) const
{
    auto audioProperties = reinterpret_cast<const TagLib::RIFF::AIFF::Properties*>(internal->audioProperties);

    auto numberOfFrames = audioProperties->sampleFrames();
    auto sampleRate = audioProperties->sampleRate();

    uinteger32 numberOfMilliseconds = (static_cast<double>(numberOfFrames) * 1000.0) / static_cast<double>(sampleRate);
    return numberOfMilliseconds;
}

boolean AIFFTrackFile::hasBitDepth(void) const
{
    return true;
}

uinteger32 AIFFTrackFile::bitDepthInBits(void) const
{
    auto audioProperties = reinterpret_cast<const TagLib::RIFF::AIFF::Properties*>(internal->audioProperties);
    return audioProperties->sampleWidth();
}
