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

#include "TrackFiles/Internal/OGGTrackFile.hpp"
#include "Markers/GridMarker.hpp"

#include <string>

// -- Generated internal implementation ommitted because this class does not use the default contructor.

namespace NxA { namespace Serato { namespace Internal {
    #pragma mark Constants
    constexpr const character* oggMarkersItemName = "SERATO_MARKERS";
    constexpr const character* oggMarkersV2ItemName = "SERATO_MARKERS2";
    constexpr const character* oggBeatgridItemName = "SERATO_BEATGRID";
} } }

using namespace NxA;
using namespace NxA::Serato::Internal;

#pragma mark Constructors & Destructors

OGGTrackFile::OGGTrackFile(const String& path, const TagLibFilePointer& newFile) : TrackFile(path, newFile) { }

#pragma mark Class Methods

String::Pointer OGGTrackFile::stringValueForFieldNamedInComment(const character* name, const TagLib::Ogg::XiphComment* oggComment)
{
    auto& stringList = oggComment->fieldListMap()[name];
    if (stringList.size() == 0) {
        return String::string();
    }

    NXA_ASSERT_EQ(stringList.size(), 1);
    return String::stringWith(stringList.front().toCString());
}

void OGGTrackFile::setStringValueForFieldNamedInComment(const String& value, const character* name, TagLib::Ogg::XiphComment* oggComment)
{
    oggComment->addField(name, TagLib::String(value.toUTF8()));
}

String::Pointer OGGTrackFile::releaseDateInComment(TagLib::Ogg::XiphComment* oggComment)
{
    return OGGTrackFile::stringValueForFieldNamedInComment(oggDateFieldName, oggComment);
}

void OGGTrackFile::setReleaseDateInComment(const String& date, TagLib::Ogg::XiphComment* oggComment)
{
    OGGTrackFile::setStringValueForFieldNamedInComment(date, oggDateFieldName, oggComment);

    auto components = date.splitBySeperator('-');
    OGGTrackFile::setStringValueForFieldNamedInComment(components->firstObject(), oggYearFieldName, oggComment);
}

#pragma mark Instance Methods

String::Pointer OGGTrackFile::stringValueForFieldNamed(const character* name) const
{
    return OGGTrackFile::stringValueForFieldNamedInComment(name, this->oggComment);
}

void OGGTrackFile::setStringValueForFieldNamed(const String& value, const character* name)
{
    OGGTrackFile::setStringValueForFieldNamedInComment(value, name, this->oggComment);
}

void OGGTrackFile::readMarkers(void)
{
    auto& fieldListMap = this->oggComment->fieldListMap();

    auto markersEncodedData = fieldListMap[oggMarkersV2ItemName].toString();
    auto encodedDataSize = markersEncodedData.size();
    if (encodedDataSize) {
        this->readMarkersV2FromBase64String(reinterpret_cast<const byte*>(markersEncodedData.data(TagLib::String::UTF8).data()),
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

                    this->gridMarkers->append(Serato::GridMarker::markerWithPositionAndBeatsPerMinute(decimal3(values[0].toCString()),
                                                                                                      decimal2(bpm.toCString())));
                }
            }
        }
    }
}

void OGGTrackFile::replaceMarkersV2Field(void)
{
    this->oggComment->removeField(oggMarkersV2ItemName);

    auto base64String = this->base64StringFromMarkersV2();
    if (!base64String->length()) {
        return;
    }

    auto decodedData = Blob::blob();

    this->oggComment->addField(oggMarkersV2ItemName, TagLib::String(base64String->toUTF8()));
}

void OGGTrackFile::replaceGridMarkersField(void)
{
    this->oggComment->removeField(oggBeatgridItemName);

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

    this->oggComment->addField(oggBeatgridItemName, propertyString);
}

void OGGTrackFile::writeMarkers(void)
{
    this->oggComment->removeField(oggMarkersItemName);
    TagLib::String propertyString;
    this->oggComment->addField(oggMarkersItemName, propertyString);

    this->replaceMarkersV2Field();
    this->replaceGridMarkersField();
}
