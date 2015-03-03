//
//  NxASeratoBlobTag.cpp
//  SeratoDB
//
//  Created by Didier Malenfant on 2/27/15.
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

#include "SeratoDB/NxASeratoBlobTag.h"

using namespace NxA;
using namespace std;

#pragma mark Constructors

SeratoBlobTag::SeratoBlobTag(const void* tagAddress) : SeratoTag(tagAddress)
{
    size_t dataSizeInBytes = SeratoTag::p_dataSizeInBytesForTagAt(tagAddress);
    const char* dataStart = static_cast<const char*>(SeratoTag::p_dataForTagAt(tagAddress));

    this->p_value = CharVectorPtr(new CharVector(dataStart, dataStart + dataSizeInBytes));
}

#pragma mark Instance Methods

const CharVector& SeratoBlobTag::value(void) const
{
    return *(this->p_value);
}

CharVector& SeratoBlobTag::value(void)
{
    return const_cast<CharVector&>(static_cast<const SeratoBlobTag&>(*this).value());
}

void SeratoBlobTag::addTo(CharVector& destination) const
{
    size_t memoryNeededInBytes = SeratoTag::p_memoryNeededWithDataOfSize(this->p_value->size());
    CharVectorPtr memoryRepresentation = CharVectorPtr(new CharVector(memoryNeededInBytes, 0));

    void* tagAddress = memoryRepresentation->data();
    SeratoTag::p_setIdentifierForTagAt(this->identifier(), tagAddress);
    memcpy(SeratoTag::p_dataForTagAt(tagAddress), this->p_value->data(), this->p_value->size());

    destination.insert(destination.end(), memoryRepresentation->begin(), memoryRepresentation->end());
}
