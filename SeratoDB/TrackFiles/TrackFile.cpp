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

#include "TrackFiles/TrackFile.hpp"
#include "TrackFiles/Internal/TrackFile.hpp"

#include <tpropertymap.h>
#include <generalencapsulatedobjectframe.h>
#include <audioproperties.h>

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_PURE_VIRTUAL_CLASS_WITH_PARENT(NxA::Serato, TrackFile, Object);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Instance Methods

String::PointerToConst TrackFile::filePath(void) const
{
    return internal->trackFilePath;
}

String::Pointer TrackFile::title(void) const
{
    return String::stringWith(internal->tag->title().toCString());
}

String::Pointer TrackFile::artist(void) const
{
    return String::stringWith(internal->tag->artist().toCString());
}

String::Pointer TrackFile::genre(void) const
{
    return String::stringWith(internal->tag->genre().toCString());
}

String::Pointer TrackFile::comments(void) const
{
    return String::stringWith(internal->tag->comment().toCString());
}

String::Pointer TrackFile::album(void) const
{
    return String::stringWith(internal->tag->album().toCString());
}

count TrackFile::trackNumber(void) const
{
    return internal->tag->track();
}

count TrackFile::audioDataSizeInBytes(void) const
{
    return ((uinteger64)this->lengthInMilliseconds() * (uinteger64)this->bitRateInKiloBitsPerSecond() * 1024) / 8 / 1000;
}

uinteger32 TrackFile::lengthInMilliseconds(void) const
{
    auto audioProperties = internal->file->audioProperties();
    NXA_ASSERT_NOT_NULL(audioProperties);

    return audioProperties->lengthInMilliseconds();
}

uinteger32 TrackFile::bitRateInKiloBitsPerSecond(void) const
{
    auto audioProperties = internal->file->audioProperties();
    NXA_ASSERT_NOT_NULL(audioProperties);

    return audioProperties->bitrate();
}

boolean TrackFile::hasBitDepth(void) const
{
    return false;
}

uinteger32 TrackFile::bitDepthInBits(void) const
{
    return 0;
}

uinteger32 TrackFile::sampleRateInSamplesPerSecond(void) const
{
    TagLib::AudioProperties* audioProperties = internal->file->audioProperties();
    NXA_ASSERT_NOT_NULL(audioProperties);

    return audioProperties->sampleRate();
}

String::Pointer TrackFile::releaseDate(void) const
{
    integer year = internal->file->tag()->year();
    return year ? String::stringWithFormat("%04d-01-01", year) : String::string();
}

const CueMarker::Array& TrackFile::cueMarkers(void) const
{
    return internal->cueMarkers;
}

const LoopMarker::Array& TrackFile::loopMarkers(void) const
{
    return internal->loopMarkers;
}

const GridMarker::Array& TrackFile::gridMarkers(void) const
{
    return internal->gridMarkers;
}

void TrackFile::setTitle(const String& title)
{
    if (title != this->title()) {
        internal->tag->setTitle(TagLib::String(title.toUTF8()));
        internal->metadataWasModified = true;
    }
}

void TrackFile::setArtist(const String& artist)
{
    if (artist != this->artist()) {
        internal->tag->setArtist(TagLib::String(artist.toUTF8()));
        internal->metadataWasModified = true;
    }
}

void TrackFile::setGenre(const String& genre)
{
    if (genre != this->genre()) {
        internal->tag->setGenre(TagLib::String(genre.toUTF8()));
        internal->metadataWasModified = true;
    }
}

void TrackFile::setComments(const String& comments)
{
    if (comments != this->comments()) {
        internal->tag->setComment(TagLib::String(comments.toUTF8()));
        internal->metadataWasModified = true;
    }
}

void TrackFile::setAlbum(const String& album)
{
    if (album != this->album()) {
        internal->tag->setAlbum(TagLib::String(album.toUTF8()));
        internal->metadataWasModified = true;
    }
}

void TrackFile::setTrackNumber(count trackNumber)
{
    if (trackNumber != this->trackNumber()) {
        internal->tag->setTrack(trackNumber);
        internal->metadataWasModified = true;
    }
}

void TrackFile::setReleaseDate(const String& date)
{
    count year = 0;

    if (date.length()) {
        auto components = date.splitBySeperator('-');
        year = ::atoi(components->firstObject().toUTF8());
    }

    if (year != internal->file->tag()->year()) {
        internal->tag->setYear(year);
        internal->metadataWasModified = true;
    }
}

void TrackFile::setCueMarkers(CueMarker::Array& markers)
{
    NXA_ASSERT_FALSE(internal->markersWereIgnored);

    if (markers != this->cueMarkers()) {
        internal->cueMarkers = markers.pointer();
        internal->markersWereModified = true;
    }
}

void TrackFile::setLoopMarkers(LoopMarker::Array& markers)
{
    NXA_ASSERT_FALSE(internal->markersWereIgnored);

    if (markers != this->loopMarkers()) {
        internal->loopMarkers = markers.pointer();
        internal->markersWereModified = true;
    }
}

void TrackFile::setGridMarkers(GridMarker::Array& markers, String::ArrayOfConst& warningLog)
{
    NXA_ASSERT_FALSE(internal->markersWereIgnored);

    // -- Serato doesn't support very flexible grid markers, the ones we end up writing
    // -- might not be exactly the ones we wanted to write so we fix them
    // -- before setting them to make sure our internal ones match what was written.
    auto fixedMarkers = GridMarker::Array::array();
    count numberOfMarkers = markers.length();
    count lastMarkerIndex = numberOfMarkers - 1;

    for (count index = 0; index < numberOfMarkers; ++index) {
        auto& marker = markers[index];

        if (index == lastMarkerIndex) {
            fixedMarkers->append(marker);
        }
        else {
            auto& nextMarker = markers[index + 1];
            decimal numberOfBeats = (marker.beatsPerMinute() * (nextMarker.positionInSeconds() - marker.positionInSeconds())) / 60.f;

            if (!GridMarker::numberOfBeatsValueSupportedBySerato(numberOfBeats)) {
                warningLog.append(String::stringWith("with grid markers unsupported by Serato. Those have not been written to Serato."));
                printf("Invalid grid markers for '%s':\n%s\n", this->filePath()->toUTF8(), markers.description()->toUTF8());
                printf("Invalid number of beats %f for marker index %ld.\n", numberOfBeats, index);
                return;
            }
            else {
                fixedMarkers->append(marker);
            }
        }
    }

    if (fixedMarkers != this->gridMarkers()) {
        internal->gridMarkers = fixedMarkers;
        internal->markersWereModified = true;
    }
}

void TrackFile::saveChangesIfAny(void)
{
    if (internal->markersWereModified) {
#if NXA_PRINT_DEBUG_INFO
        printf("Writing markers for track file '%s'.\n", this->filePath()->toUTF8());
        printf("Cue Markers:\n%s\n", this->cueMarkers().description()->toUTF8());
        printf("Loop Markers:\n%s\n", this->loopMarkers().description()->toUTF8());
        printf("GridMarkers:\n%s\n", this->gridMarkers().description()->toUTF8());
#endif

        internal->writeMarkers();

        internal->markersWereModified = false;
        internal->metadataWasModified = true;
    }

    if (internal->metadataWasModified) {
#if NXA_PRINT_DEBUG_INFO
        printf("Saving track file '%s'.\n", this->filePath()->toUTF8());
#endif

        internal->metadataWasModified = false;

        if (!internal->file->save()) {
            throw TrackFileError::exceptionWith("Couldn't not save file at '%s'.", this->filePath()->toUTF8());
        }
    }
}
