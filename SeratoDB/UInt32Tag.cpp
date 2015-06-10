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

#include "SeratoDB/UInt32Tag.hpp"

using namespace NxA::Serato;
using namespace std;

#pragma mark Instance Methods

const uint32_t& UInt32Tag::value(void) const
{
    return this->p_value;
}

uint32_t& UInt32Tag::value(void)
{
    return const_cast<uint32_t&>(static_cast<const UInt32Tag&>(*this).value());
}

void UInt32Tag::addTo(CharVector& destination) const
{
    size_t dataSize = 4;
    size_t memoryNeededInBytes = Tag::p_memoryNeededWithDataOfSize(dataSize);
    CharVectorPtr memoryRepresentation = make_unique<CharVector>(memoryNeededInBytes, 0);

    void* tagAddress = memoryRepresentation->data();
    Tag::p_setIdentifierForTagAt(this->identifier(), tagAddress);
    Tag::p_setDataSizeForTagAt(dataSize, tagAddress);
    writeBigEndianUInt32ValueAt(this->p_value, Tag::p_dataForTagAt(tagAddress));

    destination.insert(destination.end(), memoryRepresentation->begin(), memoryRepresentation->end());
}
