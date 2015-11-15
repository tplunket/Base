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

#include "TrackFiles/ID3TrackFile.hpp"
#include "TrackFiles/Internal/ID3TrackFile.hpp"

#include <attachedpictureframe.h>

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, ID3TrackFile, TrackFile);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Instance Methods

String::Pointer ID3TrackFile::releaseDate(void) const
{
    auto date = Internal::ID3TrackFile::releaseDateFromTag(internal->id3v2Tag);
    if (!date->length()) {
        date = this->TrackFile::releaseDate();
    }

    return date;
}

boolean ID3TrackFile::hasKey(void) const
{
    return true;
}

String::Pointer ID3TrackFile::key(void) const
{
    return internal->stringValueForFrameNamed(Internal::id3KeyFrameName);
}

String::Pointer ID3TrackFile::composer(void) const
{
    return internal->stringValueForFrameNamed(Internal::id3ComposerFrameName);
}

String::Pointer ID3TrackFile::grouping(void) const
{
    return internal->stringValueForFrameNamed(Internal::id3GroupingFrameName);
}

String::Pointer ID3TrackFile::bpm(void) const
{
    return internal->stringValueForFrameNamed(Internal::id3BpmFrameName);
}

boolean ID3TrackFile::hasRecordLabel(void) const
{
    return true;
}

String::Pointer ID3TrackFile::recordLabel(void) const
{
    return internal->stringValueForFrameNamed(Internal::id3RecordLabelFrameName);
}

boolean ID3TrackFile::hasRemixer(void) const
{
    return true;
}

String::Pointer ID3TrackFile::remixer(void) const
{
    return internal->stringValueForFrameNamed(Internal::id3RemixerFrameName);
}

boolean ID3TrackFile::hasRating(void) const
{
    return true;
}

integer ID3TrackFile::rating(void) const
{
    return internal->integerValueForFrameNamed(Internal::id3RatingFrameName);
}

Blob::Pointer ID3TrackFile::artwork(void) const
{
    auto frameList = internal->id3v2Tag->frameList(Internal::id3ArtworkFrameName);
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
                auto data = picture.data();
                return Blob::blobWithMemoryAndSize(reinterpret_cast<byte *>(data), size);
            }
        }
    }

    return Blob::blob();
}

void ID3TrackFile::setReleaseDate(const String& date)
{
    if (date != this->releaseDate()) {
        Internal::ID3TrackFile::setReleaseDateForTag(date, internal->id3v2Tag);
        internal->metadataWasModified = true;

        TrackFile::setReleaseDate(date);
    }
}

void ID3TrackFile::setKey(const String& key)
{
    if (key != this->key()) {
        internal->setStringValueForFrameNamed(key, Internal::id3KeyFrameName);
        internal->metadataWasModified = true;
    }
}

void ID3TrackFile::setComposer(const String& composer)
{
    if (composer != this->composer()) {
        internal->setStringValueForFrameNamed(composer, Internal::id3ComposerFrameName);
        internal->metadataWasModified = true;
    }
}

void ID3TrackFile::setGrouping(const String& grouping)
{
    if (grouping != this->grouping()) {
        internal->setStringValueForFrameNamed(grouping, Internal::id3GroupingFrameName);
        internal->metadataWasModified = true;
    }
}

void ID3TrackFile::setBpm(const String& bpm)
{
    if (bpm != this->bpm()) {
        internal->setStringValueForFrameNamed(bpm, Internal::id3BpmFrameName);
        internal->metadataWasModified = true;
    }
}

void ID3TrackFile::setRecordLabel(const String& recordLabel)
{
    if (recordLabel != this->recordLabel()) {
        internal->setStringValueForFrameNamed(recordLabel, Internal::id3RecordLabelFrameName);
        internal->metadataWasModified = true;
    }
}

void ID3TrackFile::setRemixer(const String& remixer)
{
    if (remixer != this->remixer()) {
        internal->setStringValueForFrameNamed(remixer, Internal::id3RemixerFrameName);
        internal->metadataWasModified = true;
    }
}

void ID3TrackFile::setRating(integer rating)
{
    if (rating != this->rating()) {
        internal->setIntegerValueForFrameNamed(rating, Internal::id3RatingFrameName);
        internal->metadataWasModified = true;
    }
}

void ID3TrackFile::setArtwork(const Blob& artwork)
{
    if (artwork != this->artwork()) {
        internal->removeArtwork();

        TagLib::ByteVector data(*artwork.data(), artwork.size());

        auto* newFrame = new TagLib::ID3v2::AttachedPictureFrame;
        newFrame->setData(data);
        newFrame->setType(TagLib::ID3v2::AttachedPictureFrame::FrontCover);
        newFrame->setDescription("");
        newFrame->setTextEncoding(TagLib::String::Latin1);

        internal->id3v2Tag->addFrame(newFrame);
        internal->metadataWasModified = true;
    }
}
