//
//  NxASeratoCueMarker.cpp
//  SeratoDB
//
//  Created by Didier Malenfant on 2/11/15.
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

#include "SeratoDB/NxASeratoCueMarker.h"

using namespace NxA;
using namespace std;

#pragma mark Structures

typedef struct {
    unsigned char tag[4];
    unsigned char size[4];
    unsigned char index[2];
    unsigned char position[4];
    unsigned char color[4];
    unsigned char loop_enabled[1];
    unsigned char loop_locked[1];
    unsigned char label[0];
} SeratoCueTagStruct;

#pragma mark Constructors

SeratoCueMarker::SeratoCueMarker(const char* id3TagStart)
{
    const SeratoCueTagStruct* tagStruct = (const SeratoCueTagStruct* )id3TagStart;

    this->p_positionInMilliSeconds = bigEndianUInt32ValueAt(tagStruct->position);
    this->p_index = bigEndianUInt16ValueAt(tagStruct->index);
    this->p_label = StringAutoPtr(new string((char*)tagStruct->label));
}

#pragma mark Instance Methods

uint32_t SeratoCueMarker::positionInMilliseconds(void) const
{
    return this->p_positionInMilliSeconds;
}

uint16_t SeratoCueMarker::index(void) const
{
    return this->p_index;
}

const string& SeratoCueMarker::label(void) const
{
    return *this->p_label;
}
