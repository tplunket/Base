//
//  Copyright (c) 2015 Next Audio Labs, LLC. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy of
//  this software and associated documentation files (the "Software"), to deal in the
//  Software without restriction, including without limitation the rights to use, copy,
//  modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
//  and to permit persons to whom the Software is furnished to do so, subject to the
//  following conditions:
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

using namespace NxA;
using namespace NxA::Serato::Internal;

#pragma mark Constructors & Destructors

OGGTrackFile::OGGTrackFile(String::Pointer const& path, TagLibFilePointer const& newFile) :
                           ID3TrackFile(path, newFile) { }

#pragma mark Instance Methods

void OGGTrackFile::readMarkers(void)
{
    auto markersEncodedData = this->properties["SERATO_MARKERS2"].toString();
    auto encodedDataSize = markersEncodedData.size();
    if (encodedDataSize) {
        this->readMarkersV2FromBase64Data(reinterpret_cast<const byte*>(markersEncodedData.data(TagLib::String::UTF8).data()),
                                          encodedDataSize);
    }

    auto beatGridEncodedData = this->properties["SERATO_BEATGRID"].toString();
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
                    double position = std::stod(values[0].toCString());
                    double bpm = std::stod(values[1].substr(0, values[1].length() - 1).toCString());

                    this->addGridMarker(Serato::GridMarker::markerWithPositionAndBeatsPerMinute(position, bpm));
                }
            }
        }
    }
}

void OGGTrackFile::writeMarkers(void)
{
    if (this->cueMarkers->length() || this->loopMarkers->length()) {
        auto decodedData = Blob::blob();

        auto base64Data = this->base64DataFromMarkersV2();
        base64Data->append(static_cast<character>(0));

        TagLib::StringList newList;
        newList.append(TagLib::String((char*)base64Data->data()));
        this->properties["SERATO_MARKERS2"] = newList;
    }
    else {
        this->properties.erase("SERATO_MARKERS2");
    }

    if (this->gridMarkers->length()) {
        TagLib::String propertyString;
        char buffer[32];

        propertyString.append("00000001");
        propertyString.append("00000000");
        ::snprintf(buffer, sizeof(buffer), "%08ld", this->gridMarkers->length());
        propertyString.append(buffer);

        for (auto& marker : *(this->gridMarkers)) {
            ::snprintf(buffer, sizeof(buffer), "(%0.6f,%0.6f)", marker->positionInSeconds(), marker->beatsPerMinute());
            propertyString.append(buffer);
        }

        TagLib::StringList newList;
        newList.append(propertyString);
        this->properties["SERATO_BEATGRID"] = newList;
    }
    else {
        this->properties.erase("SERATO_BEATGRID");
    }
}
