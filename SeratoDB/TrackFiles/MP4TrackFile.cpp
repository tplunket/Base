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

#include "TrackFiles/MP4TrackFile.hpp"
#include "Base64.hpp"

#include <taglib/mp4tag.h>

using namespace NxA::Serato;
using namespace TagLib;
using namespace std;

#pragma mark Constants

static const string emptyString("");

#pragma mark Structures

typedef struct {
    unsigned char mimeType[25];
    unsigned char filename[1];
    unsigned char description[16];
    unsigned char majorVersion;
    unsigned char minorVersion;
    unsigned char data[0];
} SeratoMP4MarkersHeaderStruct;

#pragma mark Constructors

MP4TrackFile::MP4TrackFile(const char* trackFilePath) : TrackFile(trackFilePath)
{
    TaglibFilePtr file = make_unique<MP4::File>(trackFilePath);
    if (!file->isValid()) {
        this->p_file = TaglibFilePtr();
        this->p_itemListMap = NULL;
        this->p_audioProperties = NULL;
        this->p_parsedFileTag = NULL;
        return;
    }

    MP4::Tag* tag = (MP4::Tag*)file->tag();
    this->p_parsedFileTag = tag;
    this->p_itemListMap = &(tag->itemListMap());
    this->p_audioProperties = file->audioProperties();
    this->p_properties = file->properties();
    this->p_file = move(file);

    this->p_readMarkers();
}

#pragma mark Instance Methods

void MP4TrackFile::p_readMarkers(void)
{
    if (!this->p_itemListMap) {
        return;
    }

    MP4::Item markersItem = (*this->p_itemListMap)["----:com.serato.dj:markersv2"];
    if (markersItem.isValid()) {
        const TagLib::String encodedData = markersItem.toStringList().toString();
        uint32_t encodedDataSize = encodedData.size();
        if (!encodedDataSize) {
            return;
        }

        auto decodedData = Base64::decodeBlock((const char*)encodedData.data(TagLib::String::UTF8).data(),
                                               encodedDataSize);
        const SeratoMP4MarkersHeaderStruct* headerStruct = (const SeratoMP4MarkersHeaderStruct*)decodedData->data();
        this->p_readMarkersV2FromBase64Data((const char*)headerStruct->data, decodedData->size() - sizeof(SeratoMP4MarkersHeaderStruct));
    }

    MP4::Item beatgridItem = (*this->p_itemListMap)["----:com.serato.dj:beatgrid"];
    if (beatgridItem.isValid()) {
        const TagLib::String encodedData = beatgridItem.toStringList().toString();
        uint32_t encodedDataSize = encodedData.size();
        if (!encodedDataSize) {
            return;
        }

        auto decodedData = Base64::decodeBlock((const char*)encodedData.data(TagLib::String::UTF8).data(),
                                               encodedDataSize);

        const SeratoMP4MarkersHeaderStruct* headerStruct = (const SeratoMP4MarkersHeaderStruct*)decodedData->data();
        if ((headerStruct->majorVersion == 1) && (headerStruct->minorVersion == 0)) {
            this->p_readGridMarkersFrom((const char*)headerStruct->data, decodedData->size() - sizeof(SeratoMP4MarkersHeaderStruct));
        }
    }
}

void MP4TrackFile::p_writeMarkers(void)
{
    if (this->cueMarkers()->length() || this->loopMarkers()->length()) {
        auto decodedData = Blob::blob();

        SeratoMP4MarkersHeaderStruct header;
        memcpy(header.mimeType, "application/octet-stream", 25);
        header.filename[0] = 0;
        memcpy(header.description, "Serato Markers2", 16);
        header.majorVersion = 1;
        header.minorVersion = 1;

        auto headerData = Blob::blobWithCharPointer(reinterpret_cast<character*>(&header), sizeof(header));
        decodedData->append(headerData);
        decodedData->append(this->p_base64DataFromMarkersV2());

        auto encodedData = Base64::encodeBlock(decodedData->data(), decodedData->size());
        encodedData->append(static_cast<character>(0));

        StringList newList;
        newList.append(TagLib::String(reinterpret_cast<character*>(encodedData->data())));

        MP4::Item newItem(newList);
        (*this->p_itemListMap)["----:com.serato.dj:markersv2"] = newItem;
    }
    else {
        (*this->p_itemListMap).erase("----:com.serato.dj:markersv2");
    }

    if (this->gridMarkers()->length()) {
        auto decodedData = Blob::blob();

        SeratoMP4MarkersHeaderStruct header;
        memcpy(header.mimeType, "application/octet-stream", 25);
        header.filename[0] = 0;
        memcpy(header.description, "Serato Beatgrid", 16);
        header.majorVersion = 1;
        header.minorVersion = 0;

        auto headerData = Blob::blobWithCharPointer(reinterpret_cast<character*>(&header), sizeof(header));
        decodedData->append(headerData);
        decodedData->append(this->p_gridMarkerDataFromGridMarkers());

        auto encodedData = Base64::encodeBlock(decodedData->data(), decodedData->size());
        encodedData->append(static_cast<character>(0));

        StringList newList;
        newList.append(TagLib::String(reinterpret_cast<character*>(encodedData->data())));

        MP4::Item newItem(newList);
        (*this->p_itemListMap)["----:com.serato.dj:beatgrid"] = newItem;
    }
    else {
        (*this->p_itemListMap).erase("----:com.serato.dj:beatgrid");
    }
}

bool MP4TrackFile::hasKey(void) const
{
    return true;
}

string MP4TrackFile::key(void) const
{
    MP4::Item item = (*this->p_itemListMap)["----:com.apple.iTunes:initialkey"];
    if (item.isValid()) {
        TagLib::String text = item.toStringList().front();
        if (text != TagLib::String::null) {
            return text.to8Bit();
        }
    }

    return emptyString;
}

string MP4TrackFile::grouping(void) const
{
    TagLib::String text = this->p_properties["GROUPING"].toString();
    if (text != TagLib::String::null) {
        return text.to8Bit();
    }

    return emptyString;
}

bool MP4TrackFile::hasRecordLabel(void) const
{
    return false;
}

string MP4TrackFile::recordLabel(void) const
{
    return emptyString;
}

bool MP4TrackFile::hasRemixer(void) const
{
    return false;
}

string MP4TrackFile::remixer(void) const
{
    return emptyString;
}

string MP4TrackFile::yearReleased(void) const
{
    TagLib::String text = this->p_properties["DATE"].toString();
    if (text != TagLib::String::null) {
        return text.to8Bit();
    }

    return emptyString;
}

CharVectorPtr MP4TrackFile::artwork(void) const
{
    CharVectorPtr result;

    MP4::Item item = (*this->p_itemListMap)["covr"];
    if (item.isValid()) {
        MP4::CoverArtList coverArtList = item.toCoverArtList();
        MP4::CoverArt coverArt = coverArtList.front();
        ByteVector coverArtData(coverArt.data());
        size_t size = coverArtData.size();
        if (size) {
            char* data = coverArtData.data();
            result = make_unique<CharVector>(data, data + size);
        }
    }

    return move(result);
}

void MP4TrackFile::setKey(const char* key)
{
    TagLib::StringList newList;
    newList.append(TagLib::String(key));
    MP4::Item newItem(newList);
    (*this->p_itemListMap)["----:com.apple.iTunes:initialkey"] = newItem;
}

void MP4TrackFile::setGrouping(const char* grouping)
{
    this->p_properties["GROUPING"] = TagLib::String(grouping);
}

void MP4TrackFile::setRecordLabel(const char* recordLabel)
{
    // -- This is not supported by MP4 files.
}

void MP4TrackFile::setRemixer(const char* remixer)
{
    // -- This is not supported by MP4 files.
}

void MP4TrackFile::setYearReleased(const char* year)
{
    this->p_properties["DATE"] = TagLib::String(year);
}

void MP4TrackFile::setArtwork(CharVectorPtr artwork)
{
    ByteVector data(artwork->data(), artwork->size());
    MP4::CoverArt newCoverArt(MP4::CoverArt::Unknown, data);
    MP4::CoverArtList newCoverArtList;
    newCoverArtList.append(newCoverArt);

    MP4::Item newItem(newCoverArtList);
    (*this->p_itemListMap)["covr"] = newItem;
}
