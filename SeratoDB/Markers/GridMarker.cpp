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
        byte endOfStruct[0];
    } GridMarkerStruct;
} }

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Factory Methods

GridMarker::Pointer GridMarker::markerWithMemoryAt(const byte* id3TagStart)
{
    const GridMarkerStruct* gridMarker = (const GridMarkerStruct* )id3TagStart;

    return GridMarker::markerWithPositionAndBeatsPerMinute(Platform::bigEndianFloatValueAt(gridMarker->positionInSeconds),
                                                           Platform::bigEndianFloatValueAt(gridMarker->beatsPerMinute));
}

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

const byte* GridMarker::nextGridMarkerAfter(const byte* markerData)
{
    return markerData + sizeof(GridMarkerStruct);
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

void GridMarker::addDataTo(Blob& data) const
{
    GridMarkerStruct marker;

    Platform::writeBigEndianFloatValueAt(this->positionInSeconds(), marker.positionInSeconds);
    Platform::writeBigEndianFloatValueAt(this->beatsPerMinute(), marker.beatsPerMinute);

    auto headerData = Blob::blobWithMemoryAndSize(reinterpret_cast<const byte*>(&marker), sizeof(GridMarkerStruct));
    data.append(headerData);
}
