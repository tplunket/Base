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

#include "TrackFiles/Internal/FLACTrackFile.hpp"
#include "TrackFiles/Internal/ID3TrackFile.hpp"

// -- Generated internal implementation ommitted because this class does not use the default contructor.

namespace NxA { namespace Serato { namespace Internal {
    #pragma mark Structures
    typedef struct {
        unsigned char mimeType[25];
        unsigned char filename[1];
        unsigned char description[16];
        unsigned char majorVersion;
        unsigned char minorVersion;
        unsigned char data[0];
    } FLACMarkersHeaderStruct;

    #pragma mark Constants
    constexpr const character* flacMarkersItemName = "SERATO_MARKERS";
    constexpr const character* flacMarkersV2ItemName = "SERATO_MARKERS_V2";
    constexpr const character* flacBeatgridItemName = "SERATO_BEATGRID";
} } }

using namespace NxA;
using namespace NxA::Serato::Internal;

#pragma mark Constructors & Destructors

FLACTrackFile::FLACTrackFile(const String& path, const TagLibFilePointer& newFile) : TrackFile(path, newFile) { }

#pragma mark Instance Methods

void FLACTrackFile::readMarkers(void)
{
    auto& fieldListMap = this->oggComment->fieldListMap();

    auto markersEncodedData = fieldListMap[flacMarkersV2ItemName].toString();
    auto encodedDataSize = markersEncodedData.size();
    if (encodedDataSize) {
        auto decodedMarkersData = Blob::blobWithBase64String(String::stringWith(markersEncodedData.data(TagLib::String::UTF8).data(),
                                                                                encodedDataSize));

        auto headerStruct = reinterpret_cast<const FLACMarkersHeaderStruct*>(decodedMarkersData->data());
        if ((headerStruct->majorVersion == 1) && (headerStruct->minorVersion == 1)) {
            this->readMarkersV2FromBase64String(headerStruct->data, decodedMarkersData->size() - sizeof(FLACMarkersHeaderStruct));
        }
    }

    auto beatGridEncodedData = fieldListMap[flacBeatgridItemName].toString();
    auto encodedBeatGridDataSize = beatGridEncodedData.size();
    if (encodedBeatGridDataSize) {
        auto decodedGridMarkersData = Blob::blobWithBase64String(String::stringWith(beatGridEncodedData.data(TagLib::String::UTF8).data(),
                                                                                    encodedBeatGridDataSize));
        auto headerStruct = reinterpret_cast<const FLACMarkersHeaderStruct*>(decodedGridMarkersData->data());
        if ((headerStruct->majorVersion == 1) && (headerStruct->minorVersion == 0)) {
            if ((decodedGridMarkersData->size() - sizeof(FLACMarkersHeaderStruct)) > 0) {
                this->readGridMarkersFrom(headerStruct->data);
            }
        }
    }
}

void FLACTrackFile::writeMarkersV2Item(void)
{
    this->oggComment->removeField(flacMarkersV2ItemName);

    auto base64String = this->base64StringFromMarkersV2();
    if (!base64String->length()) {
        return;
    }

    auto decodedData = Blob::blob();

    FLACMarkersHeaderStruct header;
    memcpy(header.mimeType, "application/octet-stream", 25);
    header.filename[0] = 0;
    memcpy(header.description, flacMarkersV2ItemName, 16);
    header.majorVersion = 1;
    header.minorVersion = 1;

    auto headerData = Blob::blobWithMemoryAndSize(reinterpret_cast<const byte*>(&header), sizeof(header));
    decodedData->append(headerData);

    decodedData->appendWithoutStringTermination(base64String->toUTF8());

    auto encodedData = Blob::base64StringFor(decodedData->data(), decodedData->size());
    this->oggComment->addField(flacMarkersV2ItemName, TagLib::String(encodedData->toUTF8()));
}

void FLACTrackFile::writeGridMarkersItem(void)
{
    this->oggComment->removeField(flacBeatgridItemName);

    auto gridMarkerData = this->gridMarkerDataFromGridMarkers();
    if (!gridMarkerData->size()) {
        return;
    }

    auto decodedData = Blob::blob();

    FLACMarkersHeaderStruct header;
    memcpy(header.mimeType, "application/octet-stream", 25);
    header.filename[0] = 0;
    memcpy(header.description, flacBeatgridItemName, 16);
    header.majorVersion = 1;
    header.minorVersion = 0;

    auto headerData = Blob::blobWithMemoryAndSize(reinterpret_cast<const byte*>(&header), sizeof(header));
    decodedData->append(headerData);
    decodedData->append(gridMarkerData);

    auto encodedData = Blob::base64StringFor(decodedData->data(), decodedData->size());
    this->oggComment->addField(flacBeatgridItemName, TagLib::String(encodedData->toUTF8()));
}

void FLACTrackFile::writeMarkers(void)
{
    if (this->id3v2Tag) {
        ID3TrackFile::replaceMarkersV1FrameInTagWith(this->id3v2Tag, this->blobFromMarkersV1());
        ID3TrackFile::replaceMarkersV2FrameInTagWith(this->id3v2Tag, this->base64StringFromMarkersV2());
        ID3TrackFile::replaceGridMarkersFrameInTagWith(this->id3v2Tag, this->gridMarkerDataFromGridMarkers());
    }
    else {
        this->oggComment->removeField(flacMarkersItemName);
        this->oggComment->addField(flacMarkersItemName, TagLib::String());

        this->writeMarkersV2Item();
        this->writeGridMarkersItem();
    }
}
