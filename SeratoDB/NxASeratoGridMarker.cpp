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

#include "SeratoDB/NxASeratoGridMarker.h"

using namespace NxA;
using namespace std;

#pragma mark Structures

typedef struct {
    unsigned char positionInSeconds[4];
    unsigned char bpm[4];
    unsigned char endOfStruct[0];
} SeratoGridMarkerStruct;

#pragma mark Constructors

SeratoGridMarker::SeratoGridMarker(const char* markerData)
{
    const SeratoGridMarkerStruct* gridMarker = (const SeratoGridMarkerStruct* )markerData;

    this->p_positionInSeconds = bigEndianFloatValueAt(gridMarker->positionInSeconds);
    this->p_bpm = bigEndianFloatValueAt(gridMarker->bpm);
}

#pragma mark Class Methods

const char* SeratoGridMarker::nextGridMarkerAfter(const char* markerData)
{
    return markerData + sizeof(SeratoGridMarkerStruct);
}

#pragma mark Instance Methods

float SeratoGridMarker::positionInSeconds(void) const
{
    return this->p_positionInSeconds;
}

float SeratoGridMarker::bpm(void) const
{
    return this->p_bpm;
}

void SeratoGridMarker::addDataTo(CharVector& data) const
{
    SeratoGridMarkerStruct marker;

    writeBigEndianFloatValueAt(this->p_positionInSeconds, marker.positionInSeconds);
    writeBigEndianFloatValueAt(this->p_bpm, marker.bpm);

    CharVector markerData((char*)&marker, (char*)&marker.endOfStruct);
    data.insert(data.end(), markerData.begin(), markerData.end());
}
