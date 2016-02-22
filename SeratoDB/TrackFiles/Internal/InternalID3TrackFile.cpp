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

#include "TrackFiles/Internal/InternalID3TrackFile.hpp"

#include <mpegfile.h>
#include <attachedpictureframe.h>
#include <textidentificationframe.h>
#include <popularimeterframe.h>
#include <privateframe.h>

// -- Generated internal implementation ommitted because this class does not use the default contructor.

namespace NxA { namespace Serato {
    #pragma mark Structures
    typedef struct {
        byte dummy;
        byte mimeType[25];
        byte filename;
        byte description[16];
        byte majorVersion;
        byte minorVersion;
    } GeobBodyHeaderStruct;

    typedef struct {
        byte majorVersion;
        byte minorVersion;
        byte data[0];
    } GeobObjectStruct;

    #pragma mark Constants
    constexpr const character* id3MarkersV1FrameDescription = "Serato Markers_";
    constexpr const character* id3MarkersV2FrameDescription = "Serato Markers2";
    constexpr const character* id3BeatgridFrameDescription = "Serato BeatGrid";
} }

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Constructors & Destructors

InternalID3TrackFile::InternalID3TrackFile(const String& path) :
    InternalTrackFile(path),
    ownersOfPrivateFrames(String::ArrayOfConst::array()),
    ownersOfPrivateFramesToRemove(String::ArrayOfConst::array())
{
}

#pragma mark Class Methods

boolean InternalID3TrackFile::isAValidGeobFrame(const TagLib::ID3v2::GeneralEncapsulatedObjectFrame& frame)
{
    auto frameID = frame.frameID();
    if (frameID.size() != 4) {
        return false;
    }

    auto frameIDData = frameID.data();
    if ((frameIDData[0] != 'G') || (frameIDData[1] != 'E') || (frameIDData[2] != 'O') || (frameIDData[3] != 'B')) {
        return false;
    }

    TagLib::String mimeType = frame.mimeType();
    return mimeType == "application/octet-stream";
}

TagLib::ID3v2::FrameList::Iterator InternalID3TrackFile::frameInListWithDescription(TagLib::ID3v2::FrameList& list,
                                                                                    const String& description)
{
    for (auto it = list.begin(); it != list.end(); ++it) {
        auto frame = (TagLib::ID3v2::GeneralEncapsulatedObjectFrame*)*it;
        if (frame->description() == description.toUTF8()) {
            return it;
        }
    }

    return list.end();
}

String::Pointer InternalID3TrackFile::stringValueForFrameNamedInTag(const character* name, const TagLib::ID3v2::Tag& tag)
{
    auto frameList = tag.frameList(name);
    for (auto& frame : frameList) {
        if (frame) {
            auto stringValue = frame->toString();
            if (stringValue.length()) {
                return String::stringWith(stringValue.to8Bit(true).c_str());
            }
        }
    }

    return String::string();
}

integer InternalID3TrackFile::integerValueForFrameNamedInTag(const character* name, const TagLib::ID3v2::Tag& tag)
{
    return InternalID3TrackFile::stringValueForFrameNamedInTag(name, tag)->integerValue();
}

integer InternalID3TrackFile::ratingValueForRatingFrameInTag(const TagLib::ID3v2::Tag& id3v2Tag)
{
    integer rating = 0;

    auto frameList = id3v2Tag.frameList(id3RatingFrameName);
    auto frame = frameList.front();
    if (frame) {
        TagLib::ID3v2::PopularimeterFrame* popFrame = dynamic_cast<TagLib::ID3v2::PopularimeterFrame*>(frameList.front());
        if (popFrame) {
            rating = popFrame->rating();
        }
    }

    return rating;
}

void InternalID3TrackFile::setStringValueForFrameNamedInTag(const String& value, const character* name, TagLib::ID3v2::Tag& tag)
{
    tag.removeFrames(name);
    auto frame = new TagLib::ID3v2::TextIdentificationFrame(name, TagLib::String::Latin1);
    frame->setText(TagLib::StringList(TagLib::String(value.toUTF8())));
    tag.addFrame(frame);
}

void InternalID3TrackFile::setIntegerValueForFrameNamedInTag(integer value, const character* name, TagLib::ID3v2::Tag& tag)
{
    InternalID3TrackFile::setStringValueForFrameNamedInTag(String::stringWithFormat("%ld", value), name, tag);
}

void InternalID3TrackFile::setRatingValueForRatingFrameInTag(integer value, TagLib::ID3v2::Tag& tag)
{
    count counter = 0;
    auto frameList = tag.frameList(id3RatingFrameName);
    auto frame = frameList.front();
    if (frame) {
        TagLib::ID3v2::PopularimeterFrame* popFrame = dynamic_cast<TagLib::ID3v2::PopularimeterFrame*>(frameList.front());
        if (popFrame) {
            counter = popFrame->counter();
        }
    }

    tag.removeFrames(id3RatingFrameName);

    auto popFrame = new TagLib::ID3v2::PopularimeterFrame();
    popFrame->setRating(value);
    popFrame->setCounter(counter);

    tag.addFrame(popFrame);
}

void InternalID3TrackFile::removePrivateFramesNamedInTag(const String& name, TagLib::ID3v2::Tag& tag)
{
    auto framesList = tag.frameList("PRIV");
    if (!framesList.size()) {
        return;
    }

    TagLib::ID3v2::FrameList framesToRemove;

    for (auto frame : framesList) {
        auto *privateFrame = dynamic_cast<TagLib::ID3v2::PrivateFrame *>(frame);
        if (privateFrame) {
            if (privateFrame->owner() == name.toUTF8()) {
                framesToRemove.append(frame);
            }
        }
    }

    if (!framesToRemove.size()) {
        return;
    }

    for (auto frame : framesToRemove) {
        tag.removeFrame(frame);
    }
}

void InternalID3TrackFile::removeGEOBFrameNamedInTag(const String& name, TagLib::ID3v2::Tag& tag)
{
    auto framesList = tag.frameList("GEOB");
    if (!framesList.size()) {
        return;
    }

    auto frameToDelete = InternalID3TrackFile::frameInListWithDescription(framesList, name);
    if (frameToDelete != framesList.end()) {
        tag.removeFrame(*frameToDelete);
    }
}

void InternalID3TrackFile::parseMarkersInTagToTrackFile(const TagLib::ID3v2::Tag& tag, InternalTrackFile& trackFile)
{
    auto framesList = tag.frameList("GEOB");
    if (!framesList.size()) {
        return;
    }

    auto framePos = InternalID3TrackFile::frameInListWithDescription(framesList, String::stringWith(id3MarkersV2FrameDescription));
    if (framePos != framesList.end()) {
        auto frame = dynamic_cast<const TagLib::ID3v2::GeneralEncapsulatedObjectFrame*>(*framePos);
        if (isAValidGeobFrame(*frame)) {
            auto frameObject = frame->object();
            if (frameObject.size()) {
                auto headerStruct = reinterpret_cast<GeobObjectStruct*>(frameObject.data());
                if ((headerStruct->majorVersion == 1) && (headerStruct->minorVersion == 1)) {
                    count size = frameObject.size() - sizeof(GeobObjectStruct);
                    if (size) {
                        trackFile.parseMarkersV2FromBase64String(headerStruct->data, size);
                    }
                }
            }
        }
    }
    else {
        framePos = InternalID3TrackFile::frameInListWithDescription(framesList, String::stringWith(id3MarkersV1FrameDescription));
        if (framePos != framesList.end()) {
            auto frame = dynamic_cast<const TagLib::ID3v2::GeneralEncapsulatedObjectFrame*>(*framePos);
            if (isAValidGeobFrame(*frame)) {
                auto frameObject = frame->object();
                if (frameObject.size()) {
                    auto headerStruct = reinterpret_cast<GeobObjectStruct*>(frameObject.data());
                    if ((headerStruct->majorVersion == 2) && (headerStruct->minorVersion == 5)) {
                        count size = frameObject.size() - sizeof(GeobObjectStruct);
                        if (size) {
                            trackFile.parseMarkersV1FromEncodedByteArray(headerStruct->data, size);
                        }
                    }
                }
            }
        }
    }

    framePos = InternalID3TrackFile::frameInListWithDescription(framesList, String::stringWith(id3BeatgridFrameDescription));
    if (framePos != framesList.end()) {
        auto frame = dynamic_cast<const TagLib::ID3v2::GeneralEncapsulatedObjectFrame*>(*framePos);
        if (isAValidGeobFrame(*frame)) {
            auto frameObject = frame->object();
            if (frameObject.size()) {
                auto headerStruct = reinterpret_cast<GeobObjectStruct*>(frameObject.data());
                if ((headerStruct->majorVersion == 1) && (headerStruct->minorVersion == 0)) {
                    count size = frame->size() - sizeof(GeobObjectStruct);
                    if (size) {
                        trackFile.parseGridMarkersFrom(headerStruct->data);
                    }
                }
            }
        }
    }
}

void InternalID3TrackFile::replaceFrameNamedInTagWithDataAndVersion(const String& frameName, TagLib::ID3v2::Tag& id3v2Tag,
                                                                    const Blob& frameData, int majorVersion, int minorVersion)
{
    InternalID3TrackFile::removeGEOBFrameNamedInTag(frameName, id3v2Tag);

    if (!frameData.size()) {
        return;
    }

    auto objectData = Blob::blob();

    GeobObjectStruct header;
    header.majorVersion = majorVersion;
    header.minorVersion = minorVersion;
    auto headerData = Blob::blobWithMemoryAndSize(reinterpret_cast<const byte*>(&header), sizeof(header));
    objectData->append(headerData);
    objectData->append(frameData);

    TagLib::ByteVector newData((char*)objectData->data(), objectData->size());

    auto newFrame = new TagLib::ID3v2::GeneralEncapsulatedObjectFrame();
    newFrame->setObject(newData);
    newFrame->setTextEncoding(TagLib::String::Latin1);
    newFrame->setMimeType("application/octet-stream");
    newFrame->setFileName("");
    newFrame->setDescription(frameName.toUTF8());

    id3v2Tag.addFrame(newFrame);
}

void InternalID3TrackFile::replaceMarkersV1FrameInTagWith(TagLib::ID3v2::Tag& id3v2Tag, const Blob& markersV1Data)
{
    auto frameName = String::stringWith(id3MarkersV1FrameDescription);
    InternalID3TrackFile::replaceFrameNamedInTagWithDataAndVersion(frameName, id3v2Tag, markersV1Data, 2, 5);
}

void InternalID3TrackFile::replaceMarkersV2FrameInTagWith(TagLib::ID3v2::Tag& tag, const String& base64MarkersData)
{
    InternalID3TrackFile::removeGEOBFrameNamedInTag(String::stringWith(id3MarkersV2FrameDescription), tag);

    if (!base64MarkersData.length()) {
        return;
    }

    auto objectData = Blob::blob();

    GeobObjectStruct header;
    header.majorVersion = 1;
    header.minorVersion = 1;
    auto headerData = Blob::blobWithMemoryAndSize(reinterpret_cast<const byte*>(&header), sizeof(header));
    objectData->append(headerData);
    objectData->appendWithoutStringTermination(base64MarkersData.toUTF8());

    TagLib::ByteVector newData((char*)objectData->data(), objectData->size());

    auto newFrame = new TagLib::ID3v2::GeneralEncapsulatedObjectFrame();
    newFrame->setObject(newData);
    newFrame->setTextEncoding(TagLib::String::Latin1);
    newFrame->setMimeType("application/octet-stream");
    newFrame->setFileName("");
    newFrame->setDescription(id3MarkersV2FrameDescription);

    tag.addFrame(newFrame);
}

void InternalID3TrackFile::replaceGridMarkersFrameInTagWith(TagLib::ID3v2::Tag& tag, const Blob& gridMarkersData)
{
    InternalID3TrackFile::removeGEOBFrameNamedInTag(String::stringWith(id3BeatgridFrameDescription), tag);

    if (!gridMarkersData.size()) {
        return;
    }

    auto data = Blob::blob();

    GeobObjectStruct header;
    header.majorVersion = 1;
    header.minorVersion = 0;
    auto headerData = Blob::blobWithMemoryAndSize(reinterpret_cast<const byte*>(&header), sizeof(header));
    data->append(headerData);
    data->append(gridMarkersData);

    TagLib::ByteVector newData((char*)data->data(), data->size());

    auto newFrame = new TagLib::ID3v2::GeneralEncapsulatedObjectFrame();
    newFrame->setObject(newData);
    newFrame->setTextEncoding(TagLib::String::Latin1);
    newFrame->setMimeType("application/octet-stream");
    newFrame->setFileName("");
    newFrame->setDescription(id3BeatgridFrameDescription);
    
    tag.addFrame(newFrame);
}

void InternalID3TrackFile::updateMarkersInTagFromTrackFile(TagLib::ID3v2::Tag& tag, const InternalTrackFile& trackFile)
{
    InternalID3TrackFile::replaceMarkersV1FrameInTagWith(tag, trackFile.id3EncodedBlobFromMarkersV1());
    InternalID3TrackFile::replaceMarkersV2FrameInTagWith(tag, trackFile.base64StringFromMarkersV2());
    InternalID3TrackFile::replaceGridMarkersFrameInTagWith(tag, trackFile.gridMarkerDataFromGridMarkers());
}

String::Pointer InternalID3TrackFile::releaseDateFromTag(const TagLib::ID3v2::Tag& tag)
{
    auto date = InternalID3TrackFile::stringValueForFrameNamedInTag(id3ReleaseTimeFrameName, tag);
    if (!date->length()) {
        date = InternalID3TrackFile::stringValueForFrameNamedInTag(id3OriginalReleaseTimeFrameName, tag);
        if (!date->length()) {
            return InternalID3TrackFile::stringValueForFrameNamedInTag(id3RecordingTimeFrameName, tag);
        }
    }

    if (date->length() == 4) {
        date = String::stringWithFormat("%s-01-01", date->toUTF8());
    }

    return date;
}

void InternalID3TrackFile::setReleaseDateInTag(const String& date, TagLib::ID3v2::Tag& tag)
{
    InternalID3TrackFile::setStringValueForFrameNamedInTag(date, id3OriginalReleaseTimeFrameName, tag);
    InternalID3TrackFile::setStringValueForFrameNamedInTag(date, id3RecordingTimeFrameName, tag);
    InternalID3TrackFile::setStringValueForFrameNamedInTag(date, id3ReleaseTimeFrameName, tag);
}

Blob::Pointer InternalID3TrackFile::artworkInTag(const TagLib::ID3v2::Tag& tag)
{
    auto frameList = tag.frameList(id3ArtworkFrameName);
    if (frameList.size()) {
        const TagLib::ID3v2::AttachedPictureFrame* artworkFrame = nullptr;

        for (const TagLib::ID3v2::Frame* frame : frameList) {
            auto pic = dynamic_cast<const TagLib::ID3v2::AttachedPictureFrame*>(frame);
            if (pic->type() == TagLib::ID3v2::AttachedPictureFrame::FrontCover) {
                artworkFrame = pic;
                break;
            }
            else if (pic->type() == TagLib::ID3v2::AttachedPictureFrame::Other) {
                artworkFrame = pic;
            }
        }

        if (artworkFrame) {
            auto picture = artworkFrame->picture();
            auto size = picture.size();
            if (size) {
                char *artworkData = picture.data();
                return Blob::blobWithMemoryAndSize(reinterpret_cast<byte *>(artworkData), size);
            }
        }
    }

    return Blob::blob();
}

void InternalID3TrackFile::removeArtworkInTag(TagLib::ID3v2::Tag& tag)
{
    auto frameList = tag.frameList("APIC");
    auto frameToRemove = frameList.end();
    if (frameList.size()) {
        for (auto it = frameList.begin(); it != frameList.end(); ++it) {
            auto* pic = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame*>(*it);

            if (pic->type() == TagLib::ID3v2::AttachedPictureFrame::FrontCover) {
                frameToRemove = it;
                break;
            }
            else if (pic->type() == TagLib::ID3v2::AttachedPictureFrame::Other) {
                frameToRemove = it;
            }
        }
    }

    if (frameToRemove != frameList.end()) {
        tag.removeFrame(*frameToRemove);
    }
}

#pragma mark Instance Methods

void InternalID3TrackFile::parseTag(const TagLib::ID3v2::Tag& tag)
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
    this->recordLabel = InternalID3TrackFile::stringValueForFrameNamedInTag(id3RecordLabelFrameName, tag);
    this->remixer = InternalID3TrackFile::stringValueForFrameNamedInTag(id3RemixerFrameName, tag);
    this->rating = InternalID3TrackFile::ratingValueForRatingFrameInTag(tag);

    this->artwork = InternalID3TrackFile::artworkInTag(tag);
}

void InternalID3TrackFile::updateArtworkInTag(TagLib::ID3v2::Tag& tag) const
{
    InternalID3TrackFile::removeArtworkInTag(tag);

    if (this->artwork->size()) {
        TagLib::ByteVector data(*this->artwork->data(), this->artwork->size());

        auto* newFrame = new TagLib::ID3v2::AttachedPictureFrame;
        newFrame->setData(data);
        newFrame->setType(TagLib::ID3v2::AttachedPictureFrame::FrontCover);
        newFrame->setDescription("");
        newFrame->setTextEncoding(TagLib::String::Latin1);

        tag.addFrame(newFrame);
    }
}

void InternalID3TrackFile::updateTag(TagLib::ID3v2::Tag& tag) const
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
        InternalID3TrackFile::setStringValueForFrameNamedInTag(this->recordLabel, id3RecordLabelFrameName, tag);
        InternalID3TrackFile::setStringValueForFrameNamedInTag(this->remixer, id3RemixerFrameName, tag);
        InternalID3TrackFile::setRatingValueForRatingFrameInTag(this->rating, tag);

        // -- TODO: Re-enable arwork outputting.
    }
}
