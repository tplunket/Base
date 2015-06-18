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

NXA_GENERATED_IMPLEMENTATION_FOR(NxA::Serato, GridMarker);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Structures

typedef struct {
    unsigned char positionInSeconds[4];
    unsigned char bpm[4];
    unsigned char endOfStruct[0];
} GridMarkerStruct;

#pragma mark Constructors & Destructors

GridMarker::GridMarker(NxA::Internal::Object::Pointer const& initial_internal) :
                       Object(initial_internal),
                       internal(initial_internal) { }

#pragma mark Factory Methods

GridMarker::Pointer GridMarker::gridMarkerWith(const char* markerData)
{
    const GridMarkerStruct* gridMarker = (const GridMarkerStruct* )markerData;

    return GridMarker::gridMarkerWith(Platform::bigEndianFloatValueAt(gridMarker->positionInSeconds),
                                      Platform::bigEndianFloatValueAt(gridMarker->bpm));
}

GridMarker::Pointer GridMarker::gridMarkerWith(float positionInSeconds, float bpm)
{
    auto newMarker = GridMarker::makeShared();
    newMarker->p_positionInSeconds = positionInSeconds;
    newMarker->p_bpm = bpm;

    return newMarker;
}

GridMarker::Pointer GridMarker::gridMarkerWith(GridMarker::ConstPointer const& other)
{
    return GridMarker::gridMarkerWith(other->positionInSeconds(), other->bpm());
}

#pragma mark Class Methods

const char* GridMarker::nextGridMarkerAfter(const char* markerData)
{
    return markerData + sizeof(GridMarkerStruct);
}

#pragma mark Instance Methods

float GridMarker::positionInSeconds(void) const
{
    return this->p_positionInSeconds;
}

float GridMarker::bpm(void) const
{
    return this->p_bpm;
}

void GridMarker::addDataTo(Blob::Pointer const& data) const
{
    GridMarkerStruct marker;

    Platform::writeBigEndianFloatValueAt(this->p_positionInSeconds, marker.positionInSeconds);
    Platform::writeBigEndianFloatValueAt(this->p_bpm, marker.bpm);

    auto headerData = Blob::blobWithCharPointer(reinterpret_cast<const character*>(&marker), sizeof(GridMarkerStruct));
    data->append(headerData);
}
