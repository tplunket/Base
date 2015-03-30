//
//  NxASeratoBooleanTag.cpp
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

#include "SeratoDB/NxASeratoBooleanTag.h"

using namespace NxA;
using namespace std;

#pragma mark Constructors

SeratoBooleanTag::SeratoBooleanTag(const void* tagAddress) : SeratoTag(tagAddress)
{
    char* tagData = (char*)SeratoTag::p_dataForTagAt(tagAddress);
    this->p_value = (*tagData == 1) ? true : false;
}

#pragma mark Instance Methods

const uint32_t& SeratoBooleanTag::identifier(void) const
{
    return this->p_identifier;
}

const bool& SeratoBooleanTag::value(void) const
{
    return this->p_value;
}

bool& SeratoBooleanTag::value(void)
{
    return const_cast<bool&>(static_cast<const SeratoBooleanTag&>(*this).value());
}

void SeratoBooleanTag::addTo(CharVector& destination) const
{
    size_t dataSize = 1;
    size_t memoryNeededInBytes = SeratoTag::p_memoryNeededWithDataOfSize(dataSize);
    CharVectorPtr memoryRepresentation = make_unique<CharVector>(memoryNeededInBytes, 0);

    void* tagAddress = memoryRepresentation->data();
    SeratoTag::p_setIdentifierForTagAt(this->identifier(), tagAddress);
    SeratoTag::p_setDataSizeForTagAt(dataSize, tagAddress);
    char* tagData = (char*)SeratoTag::p_dataForTagAt(tagAddress);
    *tagData = this->value() ? 1 : 0;

    destination.insert(destination.end(), memoryRepresentation->begin(), memoryRepresentation->end());
}
