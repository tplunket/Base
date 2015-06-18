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

#include "Tags/UInt32Tag.hpp"

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Instance Methods

const uint32_t& UInt32Tag::value(void) const
{
    return this->p_value;
}

uint32_t& UInt32Tag::value(void)
{
    return const_cast<uint32_t&>(static_cast<const UInt32Tag&>(*this).value());
}

void UInt32Tag::addTo(Blob::Pointer const& destination) const
{
    size_t dataSize = 4;
    Blob::Pointer memoryRepresentation = Blob::blobWithCapacity(Tag::p_memoryNeededForTagHeader() + dataSize);

    void* tagAddress = memoryRepresentation->data();
    Tag::p_setIdentifierForTagAt(this->identifier(), tagAddress);
    Tag::p_setDataSizeForTagAt(dataSize, tagAddress);
    Platform::writeBigEndianUInt32ValueAt(this->p_value, Tag::p_dataForTagAt(tagAddress));

    destination->append(memoryRepresentation);
}
