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

#include "TrackFiles/Internal/FLACTrackFile.hpp"
#include "TrackFiles/Internal/ID3TrackFile.hpp"
#include "TrackFiles/Internal/OGGTrackFile.hpp"

#include <flacfile.h>

// -- Generated internal implementation ommitted because this class does not use the default contructor.

namespace NxA { namespace Serato { namespace Internal {
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
    constexpr const character* flacMarkersItemName = "SERATO_MARKERS";
    constexpr const character* flacMarkersV2ItemName = "SERATO_MARKERS_V2";
    constexpr const character* flacBeatgridItemName = "SERATO_BEATGRID";
} } }

using namespace NxA;
using namespace NxA::Serato::Internal;

#pragma mark Constructors & Destructors

FLACTrackFile::FLACTrackFile(const String& path) : TrackFile(path),
                                                   hasRating(false) { }

#pragma mark Instance Methods

void FLACTrackFile::parseAudioProperties(const TagLib::FLAC::Properties& properties)
{
    this->TrackFile::parseAudioProperties(properties);

    this->bitDepthInBits = properties.sampleWidth();
}

void FLACTrackFile::parseTag(const TagLib::ID3v2::Tag& tag)
{
    this->TrackFile::parseTag(tag);

    this->releaseDate = ID3TrackFile::releaseDateFromTag(tag);

    this->key = ID3TrackFile::stringValueForFrameNamedInTag(Internal::id3KeyFrameName, tag);
    this->composer = ID3TrackFile::stringValueForFrameNamedInTag(Internal::id3ComposerFrameName, tag);
    this->grouping = ID3TrackFile::stringValueForFrameNamedInTag(Internal::id3GroupingFrameName, tag);
    this->bpm = ID3TrackFile::stringValueForFrameNamedInTag(Internal::id3BpmFrameName, tag);
    this->rating = ID3TrackFile::ratingValueForRatingFrameInTag(tag);
    this->hasRating = true;
}

void FLACTrackFile::parseComment(const TagLib::Ogg::XiphComment& oggComment)
{
    this->TrackFile::parseTag(oggComment);

    this->releaseDate = OGGTrackFile::releaseDateInComment(oggComment);

    this->key = OGGTrackFile::stringValueForFieldNamedInComment(Internal::oggKeyFieldName, oggComment);
    this->composer = OGGTrackFile::stringValueForFieldNamedInComment(Internal::oggComposerFieldName, oggComment);
    this->grouping = OGGTrackFile::stringValueForFieldNamedInComment(Internal::oggGroupingFieldName, oggComment);
    this->bpm = OGGTrackFile::stringValueForFieldNamedInComment(Internal::oggBpmFieldName, oggComment);
    // -- TODO: Rating to be implemented.
    this->hasRating = false;
}

void FLACTrackFile::parseMarkersInComment(const TagLib::Ogg::XiphComment& oggComment)
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

void FLACTrackFile::updateMarkersV2ItemInComment(TagLib::Ogg::XiphComment& oggComment) const
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

void FLACTrackFile::updateGridMarkersItemInComment(TagLib::Ogg::XiphComment& oggComment) const
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

void FLACTrackFile::updateMarkersInComment(TagLib::Ogg::XiphComment& oggComment) const
{
    oggComment.removeField(flacMarkersItemName);
    oggComment.addField(flacMarkersItemName, TagLib::String());

    this->updateMarkersV2ItemInComment(oggComment);
    this->updateGridMarkersItemInComment(oggComment);
}

void FLACTrackFile::updateTag(TagLib::ID3v2::Tag& tag) const
{
    this->TrackFile::updateTag(tag);

    ID3TrackFile::setReleaseDateInTag(this->releaseDate, tag);

    ID3TrackFile::setStringValueForFrameNamedInTag(this->key, Internal::id3KeyFrameName, tag);
    ID3TrackFile::setStringValueForFrameNamedInTag(this->composer, Internal::id3ComposerFrameName, tag);
    ID3TrackFile::setStringValueForFrameNamedInTag(this->grouping, Internal::id3GroupingFrameName, tag);
    ID3TrackFile::setStringValueForFrameNamedInTag(this->bpm, Internal::id3BpmFrameName, tag);
    ID3TrackFile::setRatingValueForRatingFrameInTag(this->rating, tag);
    // -- TODO: Artwork to be implemented.
}

void FLACTrackFile::updateComment(TagLib::Ogg::XiphComment& oggComment) const
{
    this->TrackFile::updateTag(oggComment);

    OGGTrackFile::setReleaseDateInComment(this->releaseDate, oggComment);

    OGGTrackFile::setStringValueForFieldNamedInComment(this->key, Internal::oggKeyFieldName, oggComment);
    OGGTrackFile::setStringValueForFieldNamedInComment(this->composer, Internal::oggComposerFieldName, oggComment);
    OGGTrackFile::setStringValueForFieldNamedInComment(this->grouping, Internal::oggGroupingFieldName, oggComment);
    OGGTrackFile::setStringValueForFieldNamedInComment(this->bpm, Internal::oggBpmFieldName, oggComment);
    // -- TODO: Artwork to be implemented.
}

#pragma mark Overridden TrackFile Instance Methods

void FLACTrackFile::loadAndParseFile(void)
{
    TagLib::FLAC::File file(this->filePath->toUTF8(),
                            true,
                            TagLib::AudioProperties::ReadStyle::Fast);

    if (!file.isValid()) {
        throw TrackFileError::exceptionWith("Error loading track file '%s'.", this->filePath->toUTF8());
    }

    auto id3v2Tag = file.hasID3v2Tag() ? file.ID3v2Tag() : nullptr;
    auto oggComment = file.hasXiphComment() ? file.xiphComment() : nullptr;
    if (!id3v2Tag && !oggComment) {
        throw TrackFileError::exceptionWith("Error reading tags from track file '%s'.", this->filePath->toUTF8());
    }
    if (oggComment) {
        this->parseComment(*oggComment);
    }
    else {
        this->parseTag(*id3v2Tag);
    }

    auto audioProperties = file.audioProperties();
    if (!audioProperties) {
        throw TrackFileError::exceptionWith("Error reading audio properties from track file '%s'.", this->filePath->toUTF8());
    }
    this->parseAudioProperties(*audioProperties);

    if (!this->markersWereIgnored) {
        if (oggComment) {
            this->parseMarkersInComment(*oggComment);
        }
        else {
            ID3TrackFile::parseMarkersInTagToTrackFile(*id3v2Tag, *this);
        }
    }
}

void FLACTrackFile::updateAndSaveFileIfModified(void) const
{
    if (!this->metadataWasModified && !this->markersWereModified) {
        return;
    }

    TagLib::FLAC::File file(this->filePath->toUTF8(),
                            true,
                            TagLib::AudioProperties::ReadStyle::Fast);

    if (!file.isValid()) {
        throw TrackFileError::exceptionWith("Error loading track file '%s'.", this->filePath->toUTF8());
    }

    auto id3v2Tag = file.hasID3v2Tag() ? file.ID3v2Tag() : nullptr;
    auto oggComment = file.hasXiphComment() ? file.xiphComment() : nullptr;
    if (!id3v2Tag && !oggComment) {
        throw TrackFileError::exceptionWith("Error reading tags from track file '%s'.", this->filePath->toUTF8());
    }

    if (this->metadataWasModified) {
        if (oggComment) {
            this->updateComment(*oggComment);
        }
        else {
            this->updateTag(*id3v2Tag);
        }
    }

    if (this->markersWereModified) {
        NXA_ASSERT_FALSE(this->markersWereIgnored);

        if (oggComment) {
            this->updateMarkersInComment(*oggComment);
        }
        else {
            ID3TrackFile::updateMarkersInTagFromTrackFile(*id3v2Tag, *this);
        }
    }

    // -- This is misleading. It doesn't actually save anything to disk.
    // -- Instead, real saving takes place in the file's desctructor. #ugh
    file.save();
}
