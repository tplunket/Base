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

#include "TrackFiles/Internal/InternalOGGTrackFile.hpp"
#include "Markers/GridMarker.hpp"

#include <string>
#include <vorbisfile.h>

// -- Generated internal implementation ommitted because this class does not use the default contructor.

namespace NxA { namespace Serato {
    #pragma mark Constants
    constexpr const character* oggMarkersItemName = "SERATO_MARKERS";
    constexpr const character* oggMarkersV2ItemName = "SERATO_MARKERS2";
    constexpr const character* oggBeatgridItemName = "SERATO_BEATGRID";
} }

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Constructors & Destructors

InternalOGGTrackFile::InternalOGGTrackFile(const String& path) :
    InternalTrackFile(path),
    nameOfFields(String::ArrayOfConst::array()),
    nameOfFieldsToRemove(String::ArrayOfConst::array())
{
}

#pragma mark Class Methods

String::Pointer InternalOGGTrackFile::stringValueForFieldNamedInComment(const character* name,
                                                                        const TagLib::Ogg::XiphComment& oggComment)
{
    auto& stringList = oggComment.fieldListMap()[name];
    if (stringList.size() == 0) {
        return String::string();
    }

    return String::stringWith(stringList.front().to8Bit(true).c_str());
}

void InternalOGGTrackFile::setStringValueForFieldNamedInComment(const String& value, const character* name, TagLib::Ogg::XiphComment& oggComment)
{
    oggComment.addField(name, TagLib::String(value.toUTF8()));
}

String::Pointer InternalOGGTrackFile::releaseDateInComment(const TagLib::Ogg::XiphComment& oggComment)
{
    return InternalOGGTrackFile::stringValueForFieldNamedInComment(oggDateFieldName, oggComment);
}

void InternalOGGTrackFile::setReleaseDateInComment(const String& date, TagLib::Ogg::XiphComment& oggComment)
{
    InternalOGGTrackFile::setStringValueForFieldNamedInComment(date, oggDateFieldName, oggComment);

    auto components = date.splitBySeperator('-');
    if (components->length()) {
        InternalOGGTrackFile::setStringValueForFieldNamedInComment(components->firstObject(), oggYearFieldName, oggComment);
    }
    else {
        InternalOGGTrackFile::setStringValueForFieldNamedInComment(String::stringWith(""), oggYearFieldName, oggComment);
    }
}

#pragma mark Instance Methods

void InternalOGGTrackFile::parseMarkersInComment(const TagLib::Ogg::XiphComment& oggComment)
{
    auto& fieldListMap = oggComment.fieldListMap();

    auto markersEncodedData = fieldListMap[oggMarkersV2ItemName].toString();
    auto encodedDataSize = markersEncodedData.size();
    if (encodedDataSize) {
        this->parseMarkersV2FromBase64String(reinterpret_cast<const byte*>(markersEncodedData.data(TagLib::String::UTF8).data()),
                                             encodedDataSize);
    }

    auto beatGridEncodedData = fieldListMap[oggBeatgridItemName].toString();
    auto encodedBeatGridDataSize = beatGridEncodedData.size();
    if (encodedBeatGridDataSize) {
        auto majorVersion = beatGridEncodedData.substr(0, 8).toInt();
        auto minorVersion = beatGridEncodedData.substr(8, 8).toInt();
        if ((majorVersion == 1) && (minorVersion == 0)) {
            auto numberOfGridMarkers = beatGridEncodedData.substr(16, 8).toInt();
            auto markerStrings = beatGridEncodedData.substr(25).split("(");
            if (markerStrings.size() == numberOfGridMarkers) {
                for (auto& markerString : markerStrings) {
                    auto values = markerString.split(",");
                    auto bpm = values[1].substr(0, values[1].length() - 1);

                    this->gridMarkers->append(GridMarker::markerWithPositionAndBeatsPerMinute(decimal3(values[0].to8Bit(true).c_str()),
                                                                                              decimal2(bpm.to8Bit(true).c_str())));
                }
            }
        }
    }
}

void InternalOGGTrackFile::replaceMarkersV2FieldInComment(TagLib::Ogg::XiphComment& oggComment) const
{
    oggComment.removeField(oggMarkersV2ItemName);

    auto base64String = this->base64StringFromMarkersV2();
    if (!base64String->length()) {
        return;
    }

    auto decodedData = Blob::blob();

    oggComment.addField(oggMarkersV2ItemName, TagLib::String(base64String->toUTF8()));
}

void InternalOGGTrackFile::replaceGridMarkersFieldInComment(TagLib::Ogg::XiphComment& oggComment) const
{
    oggComment.removeField(oggBeatgridItemName);

    if (!this->gridMarkers->length()) {
        return;
    }

    TagLib::String propertyString;
    char buffer[32];

    propertyString.append("00000001");
    propertyString.append("00000000");
    ::snprintf(buffer, sizeof(buffer), "%08ld", this->gridMarkers->length());
    propertyString.append(buffer);

    for (auto& marker : *(this->gridMarkers)) {
        ::snprintf(buffer, sizeof(buffer), "(%s000f,%s0000)", marker->positionInSecondsAsString()->toUTF8(), marker->beatsPerMinuteAsString()->toUTF8());
        propertyString.append(buffer);
    }

    oggComment.addField(oggBeatgridItemName, propertyString);
}

void InternalOGGTrackFile::updateMarkersInComment(TagLib::Ogg::XiphComment& oggComment) const
{
    oggComment.removeField(oggMarkersItemName);

    TagLib::String propertyString;
    oggComment.addField(oggMarkersItemName, propertyString);

    this->replaceMarkersV2FieldInComment(oggComment);
    this->replaceGridMarkersFieldInComment(oggComment);
}

void InternalOGGTrackFile::parseComment(TagLib::Ogg::XiphComment& oggComment)
{
    this->InternalTrackFile::parseTag(oggComment);

    for (auto pair : oggComment.fieldListMap()) {
        this->nameOfFields->append(String::stringWith(pair.first.to8Bit(true).c_str()));
    }

    this->releaseDate = InternalOGGTrackFile::releaseDateInComment(oggComment);

    this->composer = InternalOGGTrackFile::stringValueForFieldNamedInComment(oggComposerFieldName, oggComment);
    this->grouping = InternalOGGTrackFile::stringValueForFieldNamedInComment(oggGroupingFieldName, oggComment);
    this->bpm = InternalOGGTrackFile::stringValueForFieldNamedInComment(oggBpmFieldName, oggComment);
    this->recordLabel = InternalOGGTrackFile::stringValueForFieldNamedInComment(oggRecordLabelFieldName, oggComment);
    this->remixer = InternalOGGTrackFile::stringValueForFieldNamedInComment(oggRemixerFieldName, oggComment);
    // -- TODO: Rating to be implemented.
    // -- TODO: Artwork to be implemented.
}

void InternalOGGTrackFile::updateComment(TagLib::Ogg::XiphComment& oggComment) const
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

        InternalOGGTrackFile::setStringValueForFieldNamedInComment(this->composer, oggComposerFieldName, oggComment);
        InternalOGGTrackFile::setStringValueForFieldNamedInComment(this->grouping, oggGroupingFieldName, oggComment);
        InternalOGGTrackFile::setStringValueForFieldNamedInComment(this->bpm, oggBpmFieldName, oggComment);
        InternalOGGTrackFile::setStringValueForFieldNamedInComment(this->recordLabel, oggRecordLabelFieldName, oggComment);
        InternalOGGTrackFile::setStringValueForFieldNamedInComment(this->remixer, oggRemixerFieldName, oggComment);
        // -- TODO: Rating to be implemented.
        // -- TODO: Artwork to be implemented.
    }
}

#pragma mark Overridden TrackFile Instance Methods

void InternalOGGTrackFile::loadAndParseFile(void)
{
    TagLib::Vorbis::File file(this->filePath->toUTF8(),
                              true,
                              TagLib::AudioProperties::ReadStyle::Fast);
    if (!file.isValid()) {
        throw TrackFileError::exceptionWith("Error loading track file '%s'.", this->filePath->toUTF8());
    }

    auto oggComment = file.tag();
    if (!oggComment) {
        throw TrackFileError::exceptionWith("Error reading tags from track file '%s'.", this->filePath->toUTF8());
    }
    this->parseComment(*oggComment);

    auto audioProperties = file.audioProperties();
    if (!audioProperties) {
        throw TrackFileError::exceptionWith("Error reading audio properties from track file '%s'.", this->filePath->toUTF8());
    }
    this->parseAudioProperties(*audioProperties);
    
    if (!this->markersWereIgnored) {
        try {
            this->parseMarkersInComment(*oggComment);
        }
        catch (MarkerError& exception) {
            throw TrackFileError::exceptionWith("Error reading markers '%s'.", exception.what());
        }
    }
}

void InternalOGGTrackFile::updateAndSaveFile(void) const
{
    TagLib::Vorbis::File file(this->filePath->toUTF8(),
                              true,
                              TagLib::AudioProperties::ReadStyle::Fast);
    if (!file.isValid()) {
        throw TrackFileError::exceptionWith("Error loading track file '%s'.", this->filePath->toUTF8());
    }

    auto oggComment = file.tag();
    if (!oggComment) {
        throw TrackFileError::exceptionWith("Error reading tags from track file '%s'.", this->filePath->toUTF8());
    }

    this->updateComment(*oggComment);

    if (this->markersWereModified) {
        NXA_ASSERT_FALSE(this->markersWereIgnored);

        this->updateMarkersInComment(*oggComment);
    }

    // -- This is misleading. It doesn't actually save anything to disk.
    // -- Instead, real saving takes place in the file's desctructor. #ugh
    file.save();
}
