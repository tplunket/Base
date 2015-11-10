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

#include "TrackFiles/MPEGTrackFile.hpp"
#include "TrackFiles/Internal/MPEGTrackFile.hpp"

#include <mpegfile.h>
#include <mpegproperties.h>

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, MPEGTrackFile, ID3TrackFile);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Factory Methods

MPEGTrackFile::Pointer MPEGTrackFile::fileWithFileAt(const String& path, TrackFile::Flags flags)
{
    auto file = Internal::TagLibFilePointer(std::make_shared<TagLib::MPEG::File>(path.toUTF8(),
                                                                                 true,
                                                                                 TagLib::AudioProperties::ReadStyle::Fast));
    if (!file->isValid()) {
        throw TrackFileError::exceptionWith("Error loading track file '%s'.", path.toUTF8());
    }

    auto mpegFile = dynamic_cast<TagLib::MPEG::File*>(&(*file));

    auto internalObject = Internal::MPEGTrackFile::Pointer(std::make_shared<Internal::MPEGTrackFile>(path, file));
    auto newFile = MPEGTrackFile::makeSharedWithInternal(NxA::Internal::Object::Pointer::dynamicCastFrom(internalObject));
    newFile->internal->tag = newFile->internal->id3v2Tag = mpegFile->ID3v2Tag();
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
