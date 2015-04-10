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

#include "SeratoDB/NxASeratoUInt16Tag.h"

using namespace NxA;
using namespace std;

#pragma mark Instance Methods

const uint16_t& SeratoUInt16Tag::value(void) const
{
    return this->p_value;
}

uint16_t& SeratoUInt16Tag::value(void)
{
    return const_cast<uint16_t&>(static_cast<const SeratoUInt16Tag&>(*this).value());
}

void SeratoUInt16Tag::addTo(CharVector& destination) const
{
    size_t dataSize = 2;
    size_t memoryNeededInBytes = SeratoTag::p_memoryNeededWithDataOfSize(dataSize);
    CharVectorPtr memoryRepresentation = make_unique<CharVector>(memoryNeededInBytes, 0);

    void* tagAddress = memoryRepresentation->data();
    SeratoTag::p_setIdentifierForTagAt(this->identifier(), tagAddress);
    SeratoTag::p_setDataSizeForTagAt(dataSize, tagAddress);
    writeBigEndianUInt16ValueAt(this->p_value, SeratoTag::p_dataForTagAt(tagAddress));

    destination.insert(destination.end(), memoryRepresentation->begin(), memoryRepresentation->end());
}
