//
//  Copyright (c) 2015-2016 Next Audio Labs, LLC. All rights reserved.
//
//  This file contains confidential and proprietary information of Serato
//  Inc. LLP ("Serato"). No use is permitted without express written
//  permission of Serato. If you are not a party to a Confidentiality/
//  Non-Disclosure Agreement with Serato, please immediately delete this
//  file as well as all copies in your possession. For further information,
//  please refer to the modified MIT license provided with this library,
//  or email licensing@serato.com.
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
//  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
//  PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
//  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include "SeratoDB/TrackFiles/Internal/InternalFLACTrackFile.hpp"
#include "SeratoDB/TrackFiles/Internal/InternalID3TrackFile.hpp"
#include "SeratoDB/TrackFiles/Internal/InternalOGGTrackFile.hpp"
#include "SeratoDB/Exceptions.hpp"

#include "flacfile.h"
#include "privateframe.h"

// -- Generated internal implementation ommitted because this class does not use the default contructor.

NXA_ENTER_NAMESPACE(NxA);
NXA_ENTER_NAMESPACE(Serato);

#pragma mark Structures
typedef struct {
    unsigned char mimeType[25];
    unsigned char filename[1];
    unsigned char description[16];
    unsigned char majorVersion;
    unsigned char minorVersion;
    unsigned char data[0];
} FLACMarkersHeaderStruct;

#pragma mark Constants
constexpr const character* flacMarkersV1ItemName = "SERATO_MARKERS";
constexpr const character* flacMarkersV2ItemName = "SERATO_MARKERS_V2";
constexpr const character* flacBeatgridItemName = "SERATO_BEATGRID";

NXA_EXIT_NAMESPACE;
NXA_EXIT_NAMESPACE;

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Constructors & Destructors

InternalFLACTrackFile::InternalFLACTrackFile(const String& path) :
    InternalTrackFile(path),
    nameOfFields(String::ArrayOfConst::array()),
    nameOfFieldsToRemove(String::ArrayOfConst::array()),
    ownersOfPrivateFrames(String::ArrayOfConst::array()),
    ownersOfPrivateFramesToRemove(String::ArrayOfConst::array()),
    hasRating(false)
{
}

#pragma mark Instance Methods

void InternalFLACTrackFile::parseAudioProperties(const TagLib::FLAC::Properties& properties)
{
    this->InternalTrackFile::parseAudioProperties(properties);

    this->bitDepthInBits = properties.sampleWidth();
}

void InternalFLACTrackFile::parseTag(const TagLib::ID3v2::Tag& tag)
{
    this->InternalTrackFile::parseTag(tag);

    auto privateFrames = tag.frameList("PRIV");
    for (auto frame : privateFrames) {
        auto *privateFrame = dynamic_cast<TagLib::ID3v2::PrivateFrame *>(frame);
        if (privateFrame) {
            this->ownersOfPrivateFrames->append(String::stringWith(privateFrame->owner().to8Bit(true).c_str()));
        }
    }

    this->releaseDate = InternalID3TrackFile::releaseDateFromTag(tag);

    this->key = InternalID3TrackFile::stringValueForFrameNamedInTag(id3KeyFrameName, tag);
    this->composer = InternalID3TrackFile::stringValueForFrameNamedInTag(id3ComposerFrameName, tag);
    this->grouping = InternalID3TrackFile::stringValueForFrameNamedInTag(id3GroupingFrameName, tag);
    this->bpm = InternalID3TrackFile::stringValueForFrameNamedInTag(id3BpmFrameName, tag);
    this->rating = InternalID3TrackFile::ratingValueForRatingFrameInTag(tag);
    this->hasRating = true;
}

void InternalFLACTrackFile::parseComment(const TagLib::Ogg::XiphComment& oggComment)
{
    this->InternalTrackFile::parseTag(oggComment);

    for (auto pair : oggComment.fieldListMap()) {
        this->nameOfFields->append(String::stringWith(pair.first.to8Bit(true).c_str()));
    }

    this->releaseDate = InternalOGGTrackFile::releaseDateInComment(oggComment);

    this->key = InternalOGGTrackFile::stringValueForFieldNamedInComment(oggKeyFieldName, oggComment);
    this->composer = InternalOGGTrackFile::stringValueForFieldNamedInComment(oggComposerFieldName, oggComment);
    this->grouping = InternalOGGTrackFile::stringValueForFieldNamedInComment(oggGroupingFieldName, oggComment);
    this->bpm = InternalOGGTrackFile::stringValueForFieldNamedInComment(oggBpmFieldName, oggComment);
    // -- TODO: Rating to be implemented.
    this->hasRating = false;
}

void InternalFLACTrackFile::parseMarkersInComment(const TagLib::Ogg::XiphComment& oggComment)
{
    auto& fieldListMap = oggComment.fieldListMap();

    auto markersEncodedData = fieldListMap[flacMarkersV2ItemName].toString();
    auto encodedDataSize = markersEncodedData.size();
    if (encodedDataSize) {
        auto decodedMarkersData = Blob::blobWithBase64String(String::stringWith(markersEncodedData.data(TagLib::String::UTF8).data(),
                                                                                encodedDataSize));

        auto headerStruct = reinterpret_cast<const FLACMarkersHeaderStruct*>(decodedMarkersData->data());
        if ((headerStruct->majorVersion == 1) && (headerStruct->minorVersion == 1)) {
            this->parseMarkersV2FromBase64String(headerStruct->data, decodedMarkersData->size() - sizeof(FLACMarkersHeaderStruct));
        }
    }

    auto beatGridEncodedData = fieldListMap[flacBeatgridItemName].toString();
    auto encodedBeatGridDataSize = beatGridEncodedData.size();
    if (encodedBeatGridDataSize) {
        auto decodedGridMarkersData = Blob::blobWithBase64String(String::stringWith(beatGridEncodedData.data(TagLib::String::UTF8).data(),
                                                                                    encodedBeatGridDataSize));
        auto headerStruct = reinterpret_cast<const FLACMarkersHeaderStruct*>(decodedGridMarkersData->data());
        if ((headerStruct->majorVersion == 1) && (headerStruct->minorVersion == 0)) {
            if ((decodedGridMarkersData->size() - sizeof(FLACMarkersHeaderStruct)) > 0) {
                this->parseGridMarkersFrom(headerStruct->data);
            }
        }
    }
}

void InternalFLACTrackFile::updateMarkersInComment(TagLib::Ogg::XiphComment& oggComment) const
{
    this->updateMarkersV1ItemInComment(oggComment);
    this->updateMarkersV2ItemInComment(oggComment);
    this->updateGridMarkersItemInComment(oggComment);
}

void InternalFLACTrackFile::updateMarkersV1ItemInComment(TagLib::Ogg::XiphComment& oggComment) const
{
    oggComment.removeField(flacMarkersV1ItemName);
    oggComment.addField(flacMarkersV1ItemName, TagLib::String());
}

void InternalFLACTrackFile::updateMarkersV2ItemInComment(TagLib::Ogg::XiphComment& oggComment) const
{
    oggComment.removeField(flacMarkersV2ItemName);

    auto base64String = this->base64StringFromMarkersV2();
    if (!base64String->length()) {
        return;
    }

    auto decodedData = Blob::blob();

    FLACMarkersHeaderStruct header;
    memcpy(header.mimeType, "application/octet-stream", 25);
    header.filename[0] = 0;
    memcpy(header.description, flacMarkersV2ItemName, 16);
    header.majorVersion = 1;
    header.minorVersion = 1;

    auto headerData = Blob::blobWithMemoryAndSize(reinterpret_cast<const byte*>(&header), sizeof(header));
    decodedData->append(headerData);

    decodedData->appendWithoutStringTermination(base64String->toUTF8());

    auto encodedData = Blob::base64StringFor(decodedData->data(), decodedData->size());
    oggComment.addField(flacMarkersV2ItemName, TagLib::String(encodedData->toUTF8()));
}

void InternalFLACTrackFile::updateGridMarkersItemInComment(TagLib::Ogg::XiphComment& oggComment) const
{
    oggComment.removeField(flacBeatgridItemName);

    auto gridMarkerData = this->gridMarkerDataFromGridMarkers();
    if (!gridMarkerData->size()) {
        return;
    }

    auto decodedData = Blob::blob();

    FLACMarkersHeaderStruct header;
    memcpy(header.mimeType, "application/octet-stream", 25);
    header.filename[0] = 0;
    memcpy(header.description, flacBeatgridItemName, 16);
    header.majorVersion = 1;
    header.minorVersion = 0;

    auto headerData = Blob::blobWithMemoryAndSize(reinterpret_cast<const byte*>(&header), sizeof(header));
    decodedData->append(headerData);
    decodedData->append(gridMarkerData);

    auto encodedData = Blob::base64StringFor(decodedData->data(), decodedData->size());
    oggComment.addField(flacBeatgridItemName, TagLib::String(encodedData->toUTF8()));
}

void InternalFLACTrackFile::updateTag(TagLib::ID3v2::Tag& tag) const
{
    if (this->ownersOfPrivateFramesToRemove->length()) {
        for (auto name : *this->ownersOfPrivateFramesToRemove) {
            InternalID3TrackFile::removePrivateFramesNamedInTag(name, tag);
            this->ownersOfPrivateFrames->remove(name);
        }

        this->ownersOfPrivateFramesToRemove->emptyAll();
    }

    if (this->metadataWasModified) {
        this->InternalTrackFile::updateTag(tag);

        InternalID3TrackFile::setReleaseDateInTag(this->releaseDate, tag);

        InternalID3TrackFile::setStringValueForFrameNamedInTag(this->key, id3KeyFrameName, tag);
        InternalID3TrackFile::setStringValueForFrameNamedInTag(this->composer, id3ComposerFrameName, tag);
        InternalID3TrackFile::setStringValueForFrameNamedInTag(this->grouping, id3GroupingFrameName, tag);
        InternalID3TrackFile::setStringValueForFrameNamedInTag(this->bpm, id3BpmFrameName, tag);
        InternalID3TrackFile::setRatingValueForRatingFrameInTag(this->rating, tag);
        // -- TODO: Artwork to be implemented.
    }
}

void InternalFLACTrackFile::updateComment(TagLib::Ogg::XiphComment& oggComment) const
{
    if (this->nameOfFieldsToRemove->length()) {
        for (auto name : *this->nameOfFieldsToRemove) {
            oggComment.removeField(name->toUTF8());
            this->nameOfFields->remove(name);
        }

        this->nameOfFieldsToRemove->emptyAll();
    }

    if (this->metadataWasModified) {
        this->InternalTrackFile::updateTag(oggComment);

        InternalOGGTrackFile::setReleaseDateInComment(this->releaseDate, oggComment);

        InternalOGGTrackFile::setStringValueForFieldNamedInComment(this->key, oggKeyFieldName, oggComment);
        InternalOGGTrackFile::setStringValueForFieldNamedInComment(this->composer, oggComposerFieldName, oggComment);
        InternalOGGTrackFile::setStringValueForFieldNamedInComment(this->grouping, oggGroupingFieldName, oggComment);
        InternalOGGTrackFile::setStringValueForFieldNamedInComment(this->bpm, oggBpmFieldName, oggComment);
        // -- TODO: Artwork to be implemented.
    }
}

#pragma mark Overridden TrackFile Instance Methods

void InternalFLACTrackFile::loadAndParseFile(void)
{
    NXA_ASSERT_TRUE(this->filePath->length() > 0);

    TagLib::FLAC::File file(this->filePath->toUTF8(),
                            true,
                            TagLib::AudioProperties::ReadStyle::Fast);

    if (!file.isValid()) {
        throw TrackFileError::exceptionWith("Error loading track file.");
    }

    auto id3v2Tag = file.hasID3v2Tag() ? file.ID3v2Tag() : nullptr;
    auto oggComment = file.hasXiphComment() ? file.xiphComment() : nullptr;
    if (!id3v2Tag && !oggComment) {
        throw TrackFileError::exceptionWith("Error reading tags from track file.");
    }
    if (oggComment) {
        this->parseComment(*oggComment);
    }
    else {
        this->parseTag(*id3v2Tag);
    }

    auto audioProperties = file.audioProperties();
    if (!audioProperties) {
        throw TrackFileError::exceptionWith("Error reading audio properties from track file.");
    }
    this->parseAudioProperties(*audioProperties);

    if (!this->markersWereIgnored) {
        try {
            if (oggComment) {
                this->parseMarkersInComment(*oggComment);
            }
            else {
                InternalID3TrackFile::parseMarkersInTagToTrackFile(*id3v2Tag, *this);
            }
        }
        catch (MarkerError& exception) {
            throw TrackFileError::exceptionWith("Error reading markers '%s'.", exception.what());
        }
    }
}

void InternalFLACTrackFile::updateAndSaveFile(void) const
{
    TagLib::FLAC::File file(this->filePath->toUTF8(),
                            true,
                            TagLib::AudioProperties::ReadStyle::Fast);

    if (!file.isValid()) {
        throw TrackFileError::exceptionWith("Error loading track file.");
    }

    auto id3v2Tag = file.hasID3v2Tag() ? file.ID3v2Tag() : nullptr;
    auto oggComment = file.hasXiphComment() ? file.xiphComment() : nullptr;
    if (!id3v2Tag && !oggComment) {
        throw TrackFileError::exceptionWith("Error reading tags from track file.");
    }

    if (oggComment) {
        this->updateComment(*oggComment);
    }
    else {
        this->updateTag(*id3v2Tag);
    }

    if (this->markersWereModified) {
        NXA_ASSERT_FALSE(this->markersWereIgnored);

        if (oggComment) {
            this->updateMarkersInComment(*oggComment);
        }
        else {
            InternalID3TrackFile::updateMarkersInTagFromTrackFile(*id3v2Tag, *this);
        }
    }

    // -- This is misleading. It doesn't actually save anything to disk.
    // -- Instead, real saving takes place in the file's desctructor. #ugh
    file.save();
}
