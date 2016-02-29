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

#include "TrackFiles/Internal/InternalMP4TrackFile.hpp"

#include <mp4tag.h>
#include <mp4file.h>

// -- Generated internal implementation ommitted because this class does not use the default contructor.

NXA_ENTER_NAMESPACE(NxA);
NXA_ENTER_NAMESPACE(Serato);

#pragma mark Structures
typedef struct {
    byte mimeType[25];
    byte filename[1];
    byte description[16];
    byte majorVersion;
    byte minorVersion;
    byte data[0];
} MP4MarkersHeaderStruct;

#pragma mark Constants
constexpr const character* mp4MarkersV1ItemName = "----:com.serato.dj:markers";
constexpr const character* mp4MarkersV2ItemName = "----:com.serato.dj:markersv2";
constexpr const character* mp4BeatgridItemName = "----:com.serato.dj:beatgrid";
constexpr const character* mp4KeyItemName = "----:com.apple.iTunes:initialkey";
constexpr const character* mp4PublisherItemName = "----:com.apple.iTunes:publisher";
constexpr const character* mp4LabelItemName = "----:com.apple.iTunes:LABEL";
constexpr const character* mp4ComposerItemName = "\251wrt";
constexpr const character* mp4GroupingItemName = "\251grp";
constexpr const character* mp4BpmItemName = "tmpo";
constexpr const character* mp4ArtworkItemName = "covr";

constexpr const character* mp4MarkersV1FrameDescription = "Serato Markers_";
constexpr const character* mp4MarkersV2FrameDescription = "Serato Markers2";
constexpr const character* mp4BeatgridFrameDescription = "Serato Beatgrid";

NXA_EXIT_NAMESPACE;
NXA_EXIT_NAMESPACE;

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Constructors & Destructors

InternalMP4TrackFile::InternalMP4TrackFile(const String& path) :
    InternalTrackFile(path),
    nameOfItems(String::ArrayOfConst::array()),
    nameOfItemsToRemove(String::ArrayOfConst::array())
{
}

#pragma mark Class Methods

integer InternalMP4TrackFile::integerValueForItemNamedInTag(const character* name, const TagLib::MP4::Tag& tag)
{
    auto item = tag.item(name);
    if (!item.isValid()) {
        return 0;
    }
    return item.toInt();
}

String::Pointer InternalMP4TrackFile::stringValueForItemNamedInTag(const character* name, const TagLib::MP4::Tag& tag)
{
    auto item = tag.item(name);
    if (!item.isValid()) {
        return String::string();
    }
    auto stringList = item.toStringList();
    if (stringList.size() == 0) {
        return String::string();
    }

    NXA_ASSERT_EQ(stringList.size(), 1);
    return String::stringWith(stringList.front().to8Bit(true).c_str());
}

void InternalMP4TrackFile::setIntegerValueForItemNamedInTag(integer value, const character* name, TagLib::MP4::Tag& tag)
{
    auto newItem = std::make_unique<TagLib::MP4::Item>(value);
    tag.setItem(name, *newItem);
}

void InternalMP4TrackFile::setStringValueForItemNamedInTag(const String& value, const character* name, TagLib::MP4::Tag& tag)
{
    auto newItem = std::make_unique<TagLib::MP4::Item>(TagLib::StringList(TagLib::String(value.toUTF8())));
    tag.setItem(name, *newItem);
}

Blob::Pointer InternalMP4TrackFile::artworkInTag(const TagLib::MP4::Tag& tag)
{
    auto item = tag.item(mp4ArtworkItemName);
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

#pragma mark Instance Methods

void InternalMP4TrackFile::parseMarkersInTag(const TagLib::MP4::Tag& tag)
{
    auto markersItem = tag.item(mp4MarkersV2ItemName);
    if (markersItem.isValid() && (markersItem.atomDataType() == TagLib::MP4::AtomDataType::TypeUTF8)) {
        auto encodedData = markersItem.toStringList().toString();

        auto encodedDataSize = encodedData.size();
        if (encodedDataSize) {
            auto decodedData = Blob::blobWithBase64String(String::stringWith(encodedData.data(TagLib::String::UTF8).data(),
                                                                             encodedDataSize));
            auto headerStruct = reinterpret_cast<const MP4MarkersHeaderStruct*>(decodedData->data());
            if ((headerStruct->majorVersion == 1) && (headerStruct->minorVersion == 1)) {
                this->parseMarkersV2FromBase64String(headerStruct->data, decodedData->size() - sizeof(MP4MarkersHeaderStruct));
            }
        }
    }
    else {
        auto markersV1Item = tag.item(mp4MarkersV1ItemName);
        if (markersV1Item.isValid() && (markersV1Item.atomDataType() == TagLib::MP4::AtomDataType::TypeUTF8)) {
            auto encodedData = markersV1Item.toStringList().toString();
        
            auto encodedDataSize = encodedData.size();
            if (encodedDataSize) {
                
                auto decodedData = Blob::blobWithBase64String(String::stringWith(encodedData.data(TagLib::String::UTF8).data(),
                                                                                 encodedDataSize));
                auto headerStruct = reinterpret_cast<const MP4MarkersHeaderStruct*>(decodedData->data());
                if ((headerStruct->majorVersion == 2) && (headerStruct->minorVersion == 5)) {
                    this->parseMarkersV1FromRawByteArray(headerStruct->data, decodedData->size() - sizeof(MP4MarkersHeaderStruct));
                }
            }
        }
    }

    auto beatgridItem = tag.item(mp4BeatgridItemName);
    if (beatgridItem.isValid() && (beatgridItem.atomDataType() == TagLib::MP4::AtomDataType::TypeUTF8)) {
        auto encodedData = beatgridItem.toStringList().toString();

        auto encodedDataSize = encodedData.size();
        if (encodedDataSize) {
            auto decodedData = Blob::blobWithBase64String(String::stringWith(encodedData.data(TagLib::String::UTF8).data(),
                                                                             encodedDataSize));
            auto headerStruct = reinterpret_cast<const MP4MarkersHeaderStruct*>(decodedData->data());
            if ((headerStruct->majorVersion == 1) && (headerStruct->minorVersion == 0)) {
                if ((decodedData->size() - sizeof(MP4MarkersHeaderStruct)) > 0) {
                    this->parseGridMarkersFrom(headerStruct->data);
                }
            }
        }
    }
}

void InternalMP4TrackFile::replaceFrameNamedWithDataAndVersionInTag(const character* frameName,
                                                                    const character* frameDescription,
                                                                    const Blob& frameData,
                                                                    integer majorVersion,
                                                                    integer minorVersion,
                                                                    TagLib::MP4::Tag& tag) const
{
    tag.removeItem(frameName);

    if (!frameData.size()) {
        return;
    }

    auto decodedData = Blob::blob();

    MP4MarkersHeaderStruct header;
    memcpy(header.mimeType, "application/octet-stream", 25);
    header.filename[0] = 0;
    memcpy(header.description, frameDescription, 16);
    header.majorVersion = majorVersion;
    header.minorVersion = minorVersion;

    auto headerData = Blob::blobWithMemoryAndSize(reinterpret_cast<byte*>(&header), sizeof(header));
    decodedData->append(headerData);
    decodedData->append(frameData);

    auto encodedData = Blob::base64StringFor(decodedData->data(), decodedData->size());
    TagLib::String newString(encodedData->toUTF8(), TagLib::String::UTF8);
    TagLib::StringList newList(newString);

    TagLib::MP4::Item newItem(newList);
    newItem.setAtomDataType(TagLib::MP4::AtomDataType::TypeUTF8);
    NXA_ASSERT_TRUE(newItem.isValid());

    tag.setItem(frameName, newItem);
}

void InternalMP4TrackFile::replaceMarkersV1ItemInTag(TagLib::MP4::Tag& tag) const
{
    auto markerData = this->rawBlobFromMarkersV1();
    this->replaceFrameNamedWithDataAndVersionInTag(mp4MarkersV1ItemName, mp4MarkersV1FrameDescription, markerData, 2, 5, tag);
}

void InternalMP4TrackFile::replaceMarkersV2ItemInTag(TagLib::MP4::Tag& tag) const
{
    tag.removeItem(mp4MarkersV2ItemName);

    auto base64String = this->base64StringFromMarkersV2();
    if (!base64String->length()) {
        return;
    }

    auto decodedData = Blob::blob();

    MP4MarkersHeaderStruct header;
    memcpy(header.mimeType, "application/octet-stream", 25);
    header.filename[0] = 0;
    memcpy(header.description, mp4MarkersV2FrameDescription, 16);
    header.majorVersion = 1;
    header.minorVersion = 1;

    auto headerData = Blob::blobWithMemoryAndSize(reinterpret_cast<byte*>(&header), sizeof(header));
    decodedData->append(headerData);

    decodedData->appendWithoutStringTermination(base64String->toUTF8());

    constexpr integer paddingMultipleOf = 256;
    count paddingSize = (((base64String->length() + paddingMultipleOf - 1) / paddingMultipleOf) * paddingMultipleOf) - base64String->length();
    if (paddingSize) {
        decodedData->append(Blob::blobWithCapacity(paddingSize));
    }

    auto encodedData = Blob::base64StringFor(decodedData->data(), decodedData->size());
    TagLib::String newString(encodedData->toUTF8(), TagLib::String::UTF8);
    TagLib::StringList newList(newString);

    TagLib::MP4::Item newItem(newList);
    newItem.setAtomDataType(TagLib::MP4::AtomDataType::TypeUTF8);
    NXA_ASSERT_TRUE(newItem.isValid());

    tag.setItem(mp4MarkersV2ItemName, newItem);
}

void InternalMP4TrackFile::replaceGridMarkersItemInTag(TagLib::MP4::Tag& tag) const
{
    tag.removeItem(mp4BeatgridItemName);

    auto gridMarkerData = this->gridMarkerDataFromGridMarkers();
    if (!gridMarkerData->size()) {
        return;
    }

    auto decodedData = Blob::blob();

    MP4MarkersHeaderStruct header;
    memcpy(header.mimeType, "application/octet-stream", 25);
    header.filename[0] = 0;
    memcpy(header.description, mp4BeatgridFrameDescription, 16);
    header.majorVersion = 1;
    header.minorVersion = 0;

    auto headerData = Blob::blobWithMemoryAndSize(reinterpret_cast<byte*>(&header), sizeof(header));
    decodedData->append(headerData);
    decodedData->append(gridMarkerData);

    auto encodedData = Blob::base64StringFor(decodedData->data(), decodedData->size());
    TagLib::String newString(encodedData->toUTF8(), TagLib::String::UTF8);
    TagLib::StringList newList(newString);

    TagLib::MP4::Item newItem(newList);
    newItem.setAtomDataType(TagLib::MP4::AtomDataType::TypeUTF8);
    NXA_ASSERT_TRUE(newItem.isValid());

    tag.setItem(mp4BeatgridItemName, newItem);
}

void InternalMP4TrackFile::updateMarkersInTag(TagLib::MP4::Tag& tag) const
{
    this->replaceMarkersV1ItemInTag(tag);
    this->replaceMarkersV2ItemInTag(tag);
    this->replaceGridMarkersItemInTag(tag);
}

void InternalMP4TrackFile::parseTag(const TagLib::MP4::Tag& tag)
{
    this->InternalTrackFile::parseTag(tag);

    for (auto pair : tag.itemMap()) {
        this->nameOfItems->append(String::stringWith(pair.first.to8Bit(true).c_str()));
    }

    this->key = InternalMP4TrackFile::stringValueForItemNamedInTag(mp4KeyItemName, tag);
    this->composer = InternalMP4TrackFile::stringValueForItemNamedInTag(mp4ComposerItemName, tag);
    this->grouping = InternalMP4TrackFile::stringValueForItemNamedInTag(mp4GroupingItemName, tag);
    this->bpm = String::stringWithFormat("%d", InternalMP4TrackFile::integerValueForItemNamedInTag(mp4BpmItemName, tag));
    this->recordLabel = InternalMP4TrackFile::stringValueForItemNamedInTag(mp4LabelItemName, tag);
    this->artwork = InternalMP4TrackFile::artworkInTag(tag);
}

void InternalMP4TrackFile::updateArtworkInTag(TagLib::MP4::Tag& tag) const
{
    if (this->artwork->size()) {
        TagLib::ByteVector data(*this->artwork->data(), this->artwork->size());
        TagLib::MP4::CoverArt newCoverArt(TagLib::MP4::CoverArt::Unknown, data);
        TagLib::MP4::CoverArtList newCoverArtList;
        newCoverArtList.append(newCoverArt);

        TagLib::MP4::Item newItem(newCoverArtList);
        // -- TODO: This needs to be set to the correct type.
        newItem.setAtomDataType(TagLib::MP4::AtomDataType::TypePNG);

        tag.setItem(mp4ArtworkItemName, newItem);
    }
    else {
        tag.removeItem(mp4ArtworkItemName);
    }
}

void InternalMP4TrackFile::updateTag(TagLib::MP4::Tag& tag) const
{
    if (this->nameOfItemsToRemove->length()) {
        for (auto name : *this->nameOfItemsToRemove) {
            tag.removeItem(name->toUTF8());
            this->nameOfItems->remove(name);
        }

        this->nameOfItemsToRemove->emptyAll();
    }

    if (this->metadataWasModified) {
        this->InternalTrackFile::updateTag(tag);

        InternalMP4TrackFile::setStringValueForItemNamedInTag(this->key, mp4KeyItemName, tag);
        InternalMP4TrackFile::setStringValueForItemNamedInTag(this->composer, mp4ComposerItemName, tag);
        InternalMP4TrackFile::setStringValueForItemNamedInTag(this->grouping, mp4GroupingItemName, tag);
        InternalMP4TrackFile::setIntegerValueForItemNamedInTag(this->bpm->integerValue(), mp4BpmItemName, tag);
        InternalMP4TrackFile::setStringValueForItemNamedInTag(this->recordLabel, mp4LabelItemName, tag);
        InternalMP4TrackFile::setStringValueForItemNamedInTag(this->recordLabel, mp4PublisherItemName, tag);
        
        // -- TODO: Re-enable arwork outputting.
    }
}

#pragma mark Overridden TrackFile Instance Methods

void InternalMP4TrackFile::loadAndParseFile(void)
{
    NXA_ASSERT_TRUE(this->filePath->length() > 0);

    TagLib::MP4::File file(this->filePath->toUTF8(),
                           true,
                           TagLib::AudioProperties::ReadStyle::Fast);
    if (!file.isValid()) {
        throw TrackFileError::exceptionWith("Error loading track file.");
    }

    auto tag = file.tag();
    if (!tag) {
        throw TrackFileError::exceptionWith("Error reading tags from track file.");
    }
    this->parseTag(*tag);

    auto audioProperties = file.audioProperties();
    if (!audioProperties) {
        throw TrackFileError::exceptionWith("Error reading audio properties from track file.");
    }
    this->parseAudioProperties(*audioProperties);

    if (!this->markersWereIgnored) {
        try {
            this->parseMarkersInTag(*tag);
        }
        catch (MarkerError& exception) {
            throw TrackFileError::exceptionWith("Error reading markers '%s'.", exception.what());
        }
    }
}

void InternalMP4TrackFile::updateAndSaveFile(void) const
{
    TagLib::MP4::File file(this->filePath->toUTF8(),
                           true,
                           TagLib::AudioProperties::ReadStyle::Fast);
    if (!file.isValid()) {
        throw TrackFileError::exceptionWith("Error loading track file.");
    }

    auto tag = file.tag();
    if (!tag) {
        throw TrackFileError::exceptionWith("Error reading tags from track file.");
    }

    this->updateTag(*tag);

    if (this->markersWereModified) {
        NXA_ASSERT_FALSE(this->markersWereIgnored);

        this->updateMarkersInTag(*tag);
    }

    // -- This is misleading. It doesn't actually save anything to disk.
    // -- Instead, real saving takes place in the file's desctructor. #ugh
    file.save();
}
