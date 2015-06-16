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

#include <Base/Platform.hpp>

using namespace NxA;
using namespace NxA::Serato;
using namespace std;

#pragma mark Structures

typedef struct {
    unsigned char positionInSeconds[4];
    unsigned char bpm[4];
    unsigned char endOfStruct[0];
} GridMarkerStruct;

#pragma mark Constructors

GridMarker::GridMarker(const char* markerData)
{
    const GridMarkerStruct* gridMarker = (const GridMarkerStruct* )markerData;

    this->p_positionInSeconds = Platform::bigEndianFloatValueAt(gridMarker->positionInSeconds);
    this->p_bpm = Platform::bigEndianFloatValueAt(gridMarker->bpm);
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

void GridMarker::addDataTo(CharVector& data) const
{
    GridMarkerStruct marker;

    Platform::writeBigEndianFloatValueAt(this->p_positionInSeconds, marker.positionInSeconds);
    Platform::writeBigEndianFloatValueAt(this->p_bpm, marker.bpm);

    CharVector markerData((char*)&marker, (char*)&marker.endOfStruct);
    data.insert(data.end(), markerData.begin(), markerData.end());
}
