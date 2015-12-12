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

#include "SeratoDB/Tags/ObjectTag.hpp"

#include "TrackFiles/TrackFile.hpp"
#include "TrackFiles/Internal/TrackFile.hpp"

#include <tpropertymap.h>
#include <generalencapsulatedobjectframe.h>

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_PURE_VIRTUAL_CLASS_WITH_PARENT(NxA::Serato, TrackFile, Object);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Instance Methods

const String& TrackFile::filePath(void) const
{
    return internal->filePath;
}

const String& TrackFile::title(void) const
{
    return internal->title;
}

void TrackFile::setTitle(const String& title)
{
    if (title != *internal->title) {
        internal->title = String::stringWith(title);
        internal->metadataWasModified = true;
    }
}

const String& TrackFile::artist(void) const
{
    return internal->artist;
}

void TrackFile::setArtist(const String& artist)
{
    if (artist != internal->artist) {
        internal->artist = String::stringWith(artist);
        internal->metadataWasModified = true;
    }
}

const String& TrackFile::genre(void) const
{
    return internal->genre;
}

void TrackFile::setGenre(const String& genre)
{
    if (genre != internal->genre) {
        internal->genre = String::stringWith(genre);
        internal->metadataWasModified = true;
    }
}

const String& TrackFile::comments(void) const
{
    return internal->comments;
}

void TrackFile::setComments(const String& comments)
{
    if (comments != internal->comments) {
        internal->comments = String::stringWith(comments);
        internal->metadataWasModified = true;
    }
}

const String& TrackFile::album(void) const
{
    return internal->album;
}

void TrackFile::setAlbum(const String& album)
{
    if (album != internal->album) {
        internal->album = String::stringWith(album);
        internal->metadataWasModified = true;
    }
}

count TrackFile::trackNumber(void) const
{
    return internal->trackNumber;
}

void TrackFile::setTrackNumber(count trackNumber)
{
    if (trackNumber != internal->trackNumber) {
        internal->trackNumber = trackNumber;
        internal->metadataWasModified = true;
    }
}

const String& TrackFile::releaseDate(void) const
{
    return internal->releaseDate;
}

void TrackFile::setReleaseDate(const String& date)
{
    if (date != internal->releaseDate) {
        internal->releaseDate = String::stringWith(date);
        internal->metadataWasModified = true;
    }
}

boolean TrackFile::hasKey(void) const
{
    return false;
}

const String& TrackFile::key(void) const
{
    NXA_ASSERT_TRUE(this->hasKey());
    return internal->key;
}

void TrackFile::setKey(const String& key)
{
    NXA_ASSERT_TRUE(this->hasKey());
    if (key != internal->key) {
        internal->key = String::stringWith(key);
        internal->metadataWasModified = true;
    }
}

const String& TrackFile::composer(void) const
{
    return internal->composer;
}

void TrackFile::setComposer(const String& composer)
{
    if (composer != internal->composer) {
        internal->composer = String::stringWith(composer);
        internal->metadataWasModified = true;
    }
}

const String& TrackFile::grouping(void) const
{
    return internal->grouping;
}

void TrackFile::setGrouping(const String& grouping)
{
    if (grouping != internal->grouping) {
        internal->grouping = String::stringWith(grouping);
        internal->metadataWasModified = true;
    }
}

const String& TrackFile::bpm(void) const
{
    return internal->bpm;
}

void TrackFile::setBpm(const String& bpm)
{
    if (bpm != internal->bpm) {
        internal->bpm = String::stringWith(bpm);
        internal->metadataWasModified = true;
    }
}

boolean TrackFile::hasRecordLabel(void) const
{
    return false;
}

const String& TrackFile::recordLabel(void) const
{
    NXA_ASSERT_TRUE(this->hasRecordLabel());
    return internal->recordLabel;
}

void TrackFile::setRecordLabel(const String& recordLabel)
{
    NXA_ASSERT_TRUE(this->hasRecordLabel());
    if (recordLabel != internal->recordLabel) {
        internal->recordLabel = String::stringWith(recordLabel);
        internal->metadataWasModified = true;
    }
}

boolean TrackFile::hasRemixer(void) const
{
    return false;
}

const String& TrackFile::remixer(void) const
{
    NXA_ASSERT_TRUE(this->hasRemixer());
    return internal->remixer;
}


void TrackFile::setRemixer(const String& remixer)
{
    NXA_ASSERT_TRUE(this->hasRemixer());
    if (remixer != internal->remixer) {
        internal->remixer = String::stringWith(remixer);
        internal->metadataWasModified = true;
    }
}

boolean TrackFile::hasRating(void) const
{
    return false;
}

integer TrackFile::rating(void) const
{
    NXA_ASSERT_TRUE(this->hasRating());
    return internal->rating;
}

void TrackFile::setRating(integer rating)
{
    NXA_ASSERT_TRUE(this->hasRating());
    if (rating != internal->rating) {
        internal->rating = rating;
        internal->metadataWasModified = true;
    }
}

const Blob& TrackFile::artwork(void) const
{
    return internal->artwork;
}

void TrackFile::setArtwork(const Blob& artwork)
{
    if (artwork != internal->artwork) {
        internal->artwork = Blob::blobWith(artwork);
        internal->metadataWasModified = true;
    }
}

count TrackFile::audioDataSizeInBytes(void) const
{
    return internal->audioDataSizeInBytes;
}

uinteger32 TrackFile::lengthInMilliseconds(void) const
{
    return internal->lengthInMilliseconds;
}

uinteger32 TrackFile::bitRateInKiloBitsPerSecond(void) const
{
    return internal->bitRateInKiloBitsPerSecond;
}

boolean TrackFile::hasBitDepth(void) const
{
    return false;
}

uinteger32 TrackFile::bitDepthInBits(void) const
{
    NXA_ASSERT_TRUE(this->hasBitDepth());
    return internal->bitDepthInBits;
}

uinteger32 TrackFile::sampleRateInSamplesPerSecond(void) const
{
    return internal->sampleRateInSamplesPerSecond;
}

const CueMarker::Array& TrackFile::cueMarkers(void) const
{
    return internal->cueMarkers;
}

void TrackFile::setCueMarkers(const CueMarker::Array& markers)
{
    NXA_ASSERT_FALSE(internal->markersWereIgnored);

    if (markers != internal->cueMarkers) {
        internal->cueMarkers = CueMarker::Array::arrayWith(markers);
        internal->markersWereModified = true;
    }
}

const LoopMarker::Array& TrackFile::loopMarkers(void) const
{
    return internal->loopMarkers;
}

void TrackFile::setLoopMarkers(const LoopMarker::Array& markers)
{
    NXA_ASSERT_FALSE(internal->markersWereIgnored);

    if (markers != internal->loopMarkers) {
        internal->loopMarkers = LoopMarker::Array::arrayWith(markers);
        internal->markersWereModified = true;
    }
}

const GridMarker::Array& TrackFile::gridMarkers(void) const
{
    return internal->gridMarkers;
}

void TrackFile::setGridMarkers(const GridMarker::Array& markers)
{
    NXA_ASSERT_FALSE(internal->markersWereIgnored);
    NXA_ASSERT_TRUE(GridMarker::gridMarkersAreValid(markers));

    if (markers != this->gridMarkers()) {
        internal->gridMarkers = GridMarker::Array::arrayWith(markers);
        internal->markersWereModified = true;
    }
}

boolean TrackFile::saveIfModified(void)
{
    if (!internal->metadataWasModified && !internal->markersWereModified) {
        return false;
    }

    internal->updateAndSaveFile();

    internal->metadataWasModified = false;
    internal->markersWereModified = false;

    return true;
}
