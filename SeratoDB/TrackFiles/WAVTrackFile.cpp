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

#include "TrackFiles/WAVTrackFile.hpp"
#include "TrackFiles/Internal/WAVTrackFile.hpp"

#include <wavfile.h>

using namespace NxA;
using namespace NxA::Serato;

NXA_GENERATED_IMPLEMENTATION_FOR(NxA::Serato, WAVTrackFile);

#pragma mark Constructors & Destructors

WAVTrackFile::WAVTrackFile(NxA::Internal::Object::Pointer const& initial_internal) :
                           ID3TrackFile(initial_internal),
                           internal(initial_internal) { }

#pragma mark Factory Methods

WAVTrackFile::Pointer WAVTrackFile::fileWithFileAt(String::ConstPointer const& path)
{
    auto file = Internal::TagLibFilePointer(std::make_shared<TagLib::RIFF::WAV::File>(path->toUTF8()));
    auto internalObject = Internal::WAVTrackFile::Pointer(std::make_shared<Internal::WAVTrackFile>(path, file));
    auto newFile = WAVTrackFile::makeSharedWithInternal(internalObject);

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

#pragma mark Instance Variables

uinteger32 WAVTrackFile::lengthInMilliseconds(void) const
{
    if (internal->audioProperties) {
        auto audioProperties = reinterpret_cast<const TagLib::RIFF::WAV::Properties*>(internal->audioProperties);

        auto numberOfFrames = audioProperties->sampleFrames();
        auto sampleRate = audioProperties->sampleRate();

        uinteger32 numberOfMilliseconds = (static_cast<double>(numberOfFrames) * 1000.0) / static_cast<double>(sampleRate);
        return numberOfMilliseconds;
    }

    return 0;
}

uinteger32 WAVTrackFile::bitDepthInBitsOrZeroIfNotApplicable(void) const
{
    if (internal->audioProperties) {
        auto audioProperties = reinterpret_cast<const TagLib::RIFF::WAV::Properties*>(internal->audioProperties);
        return audioProperties->sampleWidth();
    }

    return 0;
}
