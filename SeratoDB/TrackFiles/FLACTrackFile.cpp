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
#include "TrackFiles/Internal/OGGTrackFile.hpp"
#include "TrackFiles/Internal/ID3TrackFile.hpp"

#include <flacfile.h>

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, FLACTrackFile, TrackFile);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Factory Methods

FLACTrackFile::Pointer FLACTrackFile::fileWithFileAt(const String& path, TrackFile::Flags flags)
{
    auto file = Internal::TagLibFilePointer(std::make_shared<TagLib::FLAC::File>(path.toUTF8()));
    if (!file->isValid()) {
        throw TrackFileError::exceptionWith("Error loading track file '%s'.", path.toUTF8());
    }

    auto flacFile = dynamic_cast<TagLib::FLAC::File*>(&(*file));

    auto internalObject = Internal::FLACTrackFile::Pointer(std::make_shared<Internal::FLACTrackFile>(path, file));
    auto newFile = FLACTrackFile::makeSharedWithInternal(NxA::Internal::Object::Pointer::dynamicCastFrom(internalObject));
    newFile->internal->tag = nullptr;
    newFile->internal->id3v2Tag = flacFile->hasID3v2Tag() ? flacFile->ID3v2Tag() : nullptr;
    newFile->internal->oggComment = flacFile->hasXiphComment() ? flacFile->xiphComment() : nullptr;
    if (newFile->internal->oggComment) {
        newFile->internal->tag = newFile->internal->oggComment;
    }
    else if (newFile->internal->id3v2Tag) {
        newFile->internal->tag = newFile->internal->id3v2Tag;
    }
    else {
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

boolean FLACTrackFile::hasKey(void) const
{
    return internal->id3v2Tag != nullptr;
}

String::Pointer FLACTrackFile::key(void) const
{
    if (internal->id3v2Tag) {
        return Internal::ID3TrackFile::stringValueForFrameNamedInTag(Internal::id3KeyFrameName, internal->id3v2Tag);
    }

    return String::string();
}

String::Pointer FLACTrackFile::composer(void) const
{
    if (internal->oggComment) {
        return Internal::OGGTrackFile::stringValueForFieldNamedInComment(Internal::oggComposerFieldName, internal->oggComment);
    }
    else {
        return Internal::ID3TrackFile::stringValueForFrameNamedInTag(Internal::id3ComposerFrameName, internal->id3v2Tag);
    }
}

String::Pointer FLACTrackFile::grouping(void) const
{
    if (internal->oggComment) {
        return Internal::OGGTrackFile::stringValueForFieldNamedInComment(Internal::oggGroupingFieldName, internal->oggComment);
    }
    else {
        return Internal::ID3TrackFile::stringValueForFrameNamedInTag(Internal::id3GroupingFrameName, internal->id3v2Tag);
    }
}

String::Pointer FLACTrackFile::bpm(void) const
{
    if (internal->oggComment) {
        return Internal::OGGTrackFile::stringValueForFieldNamedInComment(Internal::oggBpmFieldName, internal->oggComment);
    }
    else {
        return Internal::ID3TrackFile::stringValueForFrameNamedInTag(Internal::id3BpmFrameName, internal->id3v2Tag);
    }
}

uinteger32 FLACTrackFile::lengthInMilliseconds(void) const
{
    auto audioProperties = dynamic_cast<const TagLib::FLAC::Properties*>(internal->file->audioProperties());
    NXA_ASSERT_NOT_NULL(audioProperties);

    auto numberOfFrames = audioProperties->sampleFrames();
    auto sampleRate = audioProperties->sampleRate();

    uinteger32 numberOfMilliseconds = (static_cast<double>(numberOfFrames) * 1000.0) / static_cast<double>(sampleRate);
    return numberOfMilliseconds;
}

boolean FLACTrackFile::hasBitDepth(void) const
{
    return true;
}

uinteger32 FLACTrackFile::bitDepthInBits(void) const
{
    auto audioProperties = dynamic_cast<const TagLib::FLAC::Properties*>(internal->file->audioProperties());
    NXA_ASSERT_NOT_NULL(audioProperties);
    
    return audioProperties->sampleWidth();
}

boolean FLACTrackFile::hasRecordLabel(void) const
{
    return false;
}

String::Pointer FLACTrackFile::recordLabel(void) const
{
    return String::string();
}

boolean FLACTrackFile::hasRemixer(void) const
{
    return false;
}

String::Pointer FLACTrackFile::remixer(void) const
{
    return String::string();
}

boolean FLACTrackFile::hasRating(void) const
{
    if (internal->oggComment) {
        return false;
    }
    else {
        return true;
    }
}

integer FLACTrackFile::rating(void) const
{
    if (internal->oggComment) {
        // -- TODO: To be implemented.
        return 0;
    }
    else {
        return Internal::ID3TrackFile::integerValueForFrameNamedInTag(Internal::id3RatingFrameName, internal->id3v2Tag);
    }
}

Blob::Pointer FLACTrackFile::artwork(void) const
{
    // -- TODO: To be implemented.
    return Blob::blob();
}

void FLACTrackFile::setKey(const String& key)
{
    if (internal->id3v2Tag) {
        Internal::ID3TrackFile::setStringValueForFrameNamedInTag(key, Internal::id3KeyFrameName, internal->id3v2Tag);
    }
}

void FLACTrackFile::setComposer(const String& composer)
{
    if (internal->oggComment) {
        Internal::OGGTrackFile::setStringValueForFieldNamedInComment(composer, Internal::oggComposerFieldName, internal->oggComment);
    }
    else {
        Internal::ID3TrackFile::setStringValueForFrameNamedInTag(composer, Internal::id3ComposerFrameName, internal->id3v2Tag);
    }
}

void FLACTrackFile::setGrouping(const String& grouping)
{
    if (internal->oggComment) {
        Internal::OGGTrackFile::setStringValueForFieldNamedInComment(grouping, Internal::oggGroupingFieldName, internal->oggComment);
    }
    else {
        Internal::ID3TrackFile::setStringValueForFrameNamedInTag(grouping, Internal::id3GroupingFrameName, internal->id3v2Tag);
    }
}

void FLACTrackFile::setBpm(const String& bpm)
{
    if (internal->oggComment) {
        Internal::OGGTrackFile::setStringValueForFieldNamedInComment(bpm, Internal::oggBpmFieldName, internal->oggComment);
    }
    else {
        Internal::ID3TrackFile::setStringValueForFrameNamedInTag(bpm, Internal::id3BpmFrameName, internal->id3v2Tag);
    }
}

void FLACTrackFile::setRecordLabel(const String& recordLabel)
{
    // -- This is not supported by FLAC files.
}

void FLACTrackFile::setRemixer(const String& remixer)
{
    // -- This is not supported by FLAC files.
}

void FLACTrackFile::setRating(integer rating)
{
    if (internal->oggComment) {
        // -- TODO: To be implemented.
    }
    else {
        Internal::ID3TrackFile::setIntegerValueForFrameNamedInTag(rating, Internal::id3RatingFrameName, internal->id3v2Tag);
    }
}

void FLACTrackFile::setArtwork(const Blob& artwork)
{
    // -- TODO: To be implemented.
}
