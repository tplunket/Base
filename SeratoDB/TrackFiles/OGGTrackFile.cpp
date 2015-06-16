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

#include "TrackFiles/OGGTrackFile.hpp"
#include "Base64.hpp"

#include <taglib/vorbisfile.h>

using namespace NxA::Serato;
using namespace TagLib;
using namespace std;

#pragma mark Constants

static const string emptyString("");

#pragma mark Constructors

OGGTrackFile::OGGTrackFile(const char* trackFilePath) : ID3TrackFile(trackFilePath)
{
    TaglibFilePtr file = make_unique<Vorbis::File>(trackFilePath);
    if (!file->isValid()) {
        this->p_file = TaglibFilePtr();
        this->p_parsedFileTag = NULL;
        this->p_audioProperties = NULL;
        return;
    }

    this->p_parsedFileTag = file->tag();
    this->p_audioProperties = file->audioProperties();
    this->p_properties = file->properties();
    this->p_file = move(file);

    this->p_readMarkers();
}

#pragma mark Instance Methods

void OGGTrackFile::p_readMarkers(void)
{
    const TagLib::String markersEncodedData = this->p_properties["SERATO_MARKERS2"].toString();
    uint32_t encodedDataSize = markersEncodedData.size();
    if (encodedDataSize) {
        this->p_readMarkersV2FromBase64Data((char*)markersEncodedData.data(TagLib::String::UTF8).data(), encodedDataSize);
    }

    const TagLib::String beatGridEncodedData = this->p_properties["SERATO_BEATGRID"].toString();
    uint32_t encodedBeatGridDataSize = beatGridEncodedData.size();
    if (encodedBeatGridDataSize) {
        uint32_t majorVersion = beatGridEncodedData.substr(0, 8).toInt();
        uint32_t minorVersion = beatGridEncodedData.substr(8, 8).toInt();
        if ((majorVersion == 1) && (minorVersion == 0)) {
            uint32_t numberOfGridMarkers = beatGridEncodedData.substr(16, 8).toInt();
            StringList markerStrings(beatGridEncodedData.substr(25).split("("));
            if (markerStrings.size() == numberOfGridMarkers) {
                for (auto& markerString : markerStrings) {
                    StringList values = markerString.split(",");
                    double position = stod(values[0].toCString());
                    double bpm = stod(values[1].substr(0, values[1].length() - 1).toCString());

                    this->p_addGridMarker(make_unique<GridMarker>(position, bpm));
                }
            }
        }
    }
}

void OGGTrackFile::p_writeMarkers(void)
{
    if (this->cueMarkers().size() || this->loopMarkers().size()) {
        CharVector decodedData;

        CharVectorPtr base64Data(this->p_base64DataFromMarkersV2());
        base64Data->push_back('\0');

        StringList newList;
        newList.append(TagLib::String(base64Data->data()));
        this->p_properties["SERATO_MARKERS2"] = newList;
    }
    else {
        this->p_properties.erase("SERATO_MARKERS2");
    }

    if (this->gridMarkers().size()) {
        TagLib::String propertyString;
        char buffer[32];

        propertyString.append("00000001");
        propertyString.append("00000000");
        ::snprintf(buffer, sizeof(buffer), "%08ld", this->gridMarkers().size());
        propertyString.append(buffer);

        for (auto& marker : this->gridMarkers()) {
            ::snprintf(buffer, sizeof(buffer), "(%0.6f,%0.6f)", marker->positionInSeconds(), marker->bpm());
            propertyString.append(buffer);
        }

        StringList newList;
        newList.append(propertyString);
        this->p_properties["SERATO_BEATGRID"] = newList;
    }
    else {
        this->p_properties.erase("SERATO_BEATGRID");
    }
}

bool OGGTrackFile::hasKey(void) const
{
    return false;
}

string OGGTrackFile::key(void) const
{
    return emptyString;
}

string OGGTrackFile::grouping(void) const
{
    TagLib::String text = this->p_properties["GROUPING"].toString();
    if (text != TagLib::String::null) {
        return text.to8Bit();
    }

    return emptyString;
}

bool OGGTrackFile::hasRecordLabel(void) const
{
    return true;
}

string OGGTrackFile::recordLabel(void) const
{
    TagLib::String text = this->p_properties["LABEL"].toString();
    if (text != TagLib::String::null) {
        return text.to8Bit();
    }

    return emptyString;
}

bool OGGTrackFile::hasRemixer(void) const
{
    return true;
}

string OGGTrackFile::remixer(void) const
{
    TagLib::String text = this->p_properties["REMIXER"].toString();
    if (text != TagLib::String::null) {
        return text.to8Bit();
    }

    return emptyString;
}

string OGGTrackFile::yearReleased(void) const
{
    TagLib::String text = this->p_properties["YEAR"].toString();
    if (text != TagLib::String::null) {
        return text.to8Bit();
    }

    return emptyString;
}

CharVectorPtr OGGTrackFile::artwork(void) const
{
    // -- TODO: To be implemented.
    return CharVectorPtr();
}

void OGGTrackFile::setKey(const char* key)
{
    // -- This is not supported by OGG files.
}

void OGGTrackFile::setGrouping(const char* grouping)
{
    this->p_properties["GROUPING"] = TagLib::String(grouping);
}

void OGGTrackFile::setRecordLabel(const char* recordLabel)
{
    this->p_properties["LABEL"] = TagLib::String(recordLabel);
}

void OGGTrackFile::setRemixer(const char* remixer)
{
    this->p_properties["REMIXER"] = TagLib::String(remixer);
}

void OGGTrackFile::setYearReleased(const char* year)
{
    this->p_properties["YEAR"] = TagLib::String(year);
}

void OGGTrackFile::setArtwork(CharVectorPtr artwork)
{
    // -- TODO: To be implemented.
}
