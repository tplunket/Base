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

#include "Tags/BooleanTag.hpp"

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Constructors

BooleanTag::BooleanTag(const void* tagAddress) : Tag(tagAddress)
{
    char* tagData = (char*)Tag::p_dataForTagAt(tagAddress);
    this->p_value = (*tagData == 1) ? true : false;
}

#pragma mark Instance Methods

const uint32_t& BooleanTag::identifier(void) const
{
    return this->p_identifier;
}

const bool& BooleanTag::value(void) const
{
    return this->p_value;
}

bool& BooleanTag::value(void)
{
    return const_cast<bool&>(static_cast<const BooleanTag&>(*this).value());
}

void BooleanTag::addTo(Blob::Pointer const& destination) const
{
    size_t dataSize = 1;
    size_t memoryNeededInBytes = Tag::p_memoryNeededForTagHeader() + dataSize;
    auto memoryRepresentation = Blob::blobWithCapacity(memoryNeededInBytes);

    void* tagAddress = memoryRepresentation->data();
    Tag::p_setIdentifierForTagAt(this->identifier(), tagAddress);
    Tag::p_setDataSizeForTagAt(dataSize, tagAddress);
    char* tagData = (char*)Tag::p_dataForTagAt(tagAddress);
    *tagData = this->value() ? 1 : 0;

    destination->append(memoryRepresentation);
}
