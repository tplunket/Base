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

#include "SeratoDB/LoopMarker.hpp"

#include <string>

using namespace NxA::Serato;
using namespace std;

#pragma mark Structures

typedef struct {
    unsigned char tag[5];
    unsigned char size[4];
} SeratoLoopTagHeaderStruct;

typedef struct {
    unsigned char tag[5];
    unsigned char size[4];
    unsigned char index[2];
    unsigned char position[4];
    unsigned char loopPosition[4];
    unsigned char loopIterations[4];
    unsigned char color[4];
    unsigned char loop_enabled;
    unsigned char loop_locked;
    unsigned char label[0];
} SeratoLoopTagStruct;

#pragma mark Constructors

LoopMarker::LoopMarker(const char* id3TagStart)
{
    const SeratoLoopTagStruct* tagStruct = (const SeratoLoopTagStruct* )id3TagStart;

    this->p_startPositionInMilliSeconds = bigEndianUInt32ValueAt(tagStruct->position);
    this->p_endPositionInMilliSeconds = bigEndianUInt32ValueAt(tagStruct->loopPosition);
    this->p_index = bigEndianUInt16ValueAt(tagStruct->index);
    this->p_label = (char*)tagStruct->label;
}

#pragma mark Instance Methods

uint32_t LoopMarker::startPositionInMilliseconds(void) const
{
    return this->p_startPositionInMilliSeconds;
}

uint32_t LoopMarker::endPositionInMilliseconds(void) const
{
    return this->p_endPositionInMilliSeconds;
}

uint16_t LoopMarker::index(void) const
{
    return this->p_index;
}

const string& LoopMarker::label(void) const
{
    return this->p_label;
}

void LoopMarker::addId3TagTo(CharVector& data) const
{
    SeratoLoopTagStruct header;

    memcpy(header.tag, "LOOP", 5);
    size_t size = sizeof(SeratoLoopTagStruct) + this->p_label.length() + 1 - sizeof(SeratoLoopTagHeaderStruct);
    writeBigEndianUInt32ValueAt((uint32_t)size, &header.size);
    writeBigEndianUInt16ValueAt(this->p_index, &header.index);
    writeBigEndianUInt32ValueAt(this->p_startPositionInMilliSeconds, &header.position);
    writeBigEndianUInt32ValueAt(this->p_endPositionInMilliSeconds, &header.loopPosition);
    writeBigEndianUInt32ValueAt(0, &header.loopIterations);
    writeBigEndianUInt32ValueAt(0, &header.color);
    header.loop_enabled = 0;
    header.loop_locked = 0;

    CharVector headerData((char*)&header, (char*)&header.label);
    data.insert(data.end(), headerData.begin(), headerData.end());

    data.insert(data.end(), this->p_label.begin(), this->p_label.end());
}
