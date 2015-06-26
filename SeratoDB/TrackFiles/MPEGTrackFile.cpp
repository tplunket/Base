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

NXA_GENERATED_IMPLEMENTATION_FOR(NxA::Serato, MPEGTrackFile, ID3TrackFile);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Factory Methods

MPEGTrackFile::Pointer MPEGTrackFile::fileWithFileAt(String::Pointer const& path)
{
    auto file = Internal::TagLibFilePointer(std::make_shared<TagLib::MPEG::File>(path->toUTF8()));
    auto internalObject = Internal::MPEGTrackFile::Pointer(std::make_shared<Internal::MPEGTrackFile>(path, file));
    auto newFile = MPEGTrackFile::makeSharedWithInternal(NxA::Internal::Object::Pointer::dynamicCastFrom(internalObject));

    if (!file->isValid()) {
        newFile->internal->parsedFileTag = nullptr;
        newFile->internal->audioProperties = nullptr;
        return newFile;
    }

    auto mpegFile = dynamic_cast<TagLib::MPEG::File*>(&(*file));
    newFile->internal->parsedFileTag = dynamic_cast<TagLib::Tag*>(mpegFile->ID3v2Tag());
    newFile->internal->audioProperties = file->audioProperties();
    newFile->internal->properties = file->properties();

    newFile->internal->readMarkers();

    return newFile;
}
