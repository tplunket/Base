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

#include "Markers/GridMarker.hpp"
#include "Markers/Internal/GridMarker.hpp"

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, GridMarker, Object);

namespace NxA { namespace Serato {
    #pragma mark Structures
    typedef struct {
        byte positionInSeconds[4];
        byte beatsPerMinute[4];
    } GridMarkerStruct;
} }

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Factory Methods

GridMarker::Pointer GridMarker::markerWithPositionAndBeatsPerMinute(decimal positionInSeconds, decimal beatsPerMinute)
{
    auto newMarker = GridMarker::makeShared();
    newMarker->internal->positionInSeconds = positionInSeconds;
    newMarker->internal->beatsPerMinute = beatsPerMinute;

    return newMarker;
}

GridMarker::Pointer GridMarker::markerWith(const GridMarker& other)
{
    return GridMarker::markerWithPositionAndBeatsPerMinute(other.positionInSeconds(), other.beatsPerMinute());
}

#pragma mark Class Methods

GridMarker::Array::Pointer GridMarker::markersWithMemoryAt(const byte* id3TagStart)
{
    auto result = GridMarker::Array::array();

    auto numberOfMarkers = Platform::bigEndianUInteger32ValueAt(id3TagStart);
    if (!numberOfMarkers) {
        return result;
    }
    else if (numberOfMarkers > 256) {
        numberOfMarkers = 256;
    }

    float nextPosition;

    const GridMarkerStruct* gridMarker = (const GridMarkerStruct* )(id3TagStart + 4);
    for (count markerIndex = 0; markerIndex < numberOfMarkers; ++markerIndex) {
        decimal position = markerIndex ? nextPosition : Platform::bigEndianFloatValueAt(gridMarker->positionInSeconds);
        decimal bpm;

        const GridMarkerStruct* nextGridMarker = gridMarker + 1;
        if ((markerIndex + 1) < numberOfMarkers) {
            nextPosition = Platform::bigEndianFloatValueAt(nextGridMarker->positionInSeconds);

            bpm = 240.0f / (nextPosition - position);
        }
        else {
            bpm = Platform::bigEndianFloatValueAt(gridMarker->beatsPerMinute);
        }

        result->append(GridMarker::markerWithPositionAndBeatsPerMinute(position, bpm));

        gridMarker = nextGridMarker;
    }
    
    return result;
}

void GridMarker::addMarkersTo(GridMarker::Array& markers, NxA::Blob& data)
{
    uinteger32 numberOfMarkers = static_cast<uinteger32>(markers.length());

    uinteger32 numberOfMarkersBuffer;
    Platform::writeBigEndianUInteger32ValueAt(numberOfMarkers, reinterpret_cast<byte*>(&numberOfMarkersBuffer));
    auto numberOfMarkersData = Blob::blobWithMemoryAndSize(reinterpret_cast<byte*>(&numberOfMarkersBuffer), sizeof(numberOfMarkersBuffer));
    data.append(numberOfMarkersData);

    count lastMarkerIndex = numberOfMarkers - 1;

    for (count index = 0; index < numberOfMarkers; ++index) {
        auto marker = *(markers.begin() + index);

        GridMarkerStruct markerData;

        Platform::writeBigEndianFloatValueAt(marker->positionInSeconds(), markerData.positionInSeconds);
        if (index == lastMarkerIndex) {
            Platform::writeBigEndianFloatValueAt(marker->beatsPerMinute(), markerData.beatsPerMinute);
        }
        else {
            // -- Currently I'm not sure what this value means but it seems only the last marker stores a bpm value.
            Platform::writeBigEndianUInteger32ValueAt(4, markerData.beatsPerMinute);
        }

        auto headerData = Blob::blobWithMemoryAndSize(reinterpret_cast<const byte*>(&markerData), sizeof(GridMarkerStruct));
        data.append(headerData);
    }

    // -- This marks the end of tags.
    data.append('\0');
}

#pragma mark Operators

bool GridMarker::operator==(const GridMarker& other) const
{
    if (this == &other) {
        return true;
    }

    return (this->positionInSeconds() == other.positionInSeconds()) &&
           (this->beatsPerMinute() == other.beatsPerMinute());
}

#pragma mark Instance Methods

decimal GridMarker::positionInSeconds(void) const
{
    return internal->positionInSeconds;
}

decimal GridMarker::beatsPerMinute(void) const
{
    return internal->beatsPerMinute;
}
