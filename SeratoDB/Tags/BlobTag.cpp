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

#include "Tags/BlobTag.hpp"

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Constructors

BlobTag::BlobTag(const void* tagAddress) : Tag(tagAddress),
    p_value(Blob::blobWithMemoryAndSize(static_cast<const char*>(Tag::p_dataForTagAt(tagAddress)),
                                        Tag::p_dataSizeForTagAt(tagAddress))) { }

#pragma mark Instance Methods

Blob::ConstPointer const& BlobTag::value(void) const
{
    return this->p_value;
}

Blob::ConstPointer& BlobTag::value(void)
{
    return const_cast<Blob::ConstPointer&>(static_cast<const BlobTag&>(*this).value());
}

void BlobTag::addTo(Blob::Pointer const& destination) const
{
    size_t dataSize = this->p_value->size();
    size_t totalSizeNeeded = Tag::p_memoryNeededForTagHeader() + dataSize;
    auto memoryRepresentation = Blob::blobWithCapacity(totalSizeNeeded);

    void* tagAddress = memoryRepresentation->data();
    Tag::p_setIdentifierForTagAt(this->identifier(), tagAddress);
    Tag::p_setDataSizeForTagAt(dataSize, tagAddress);
    memcpy(Tag::p_dataForTagAt(tagAddress), this->p_value->data(), this->p_value->size());

    destination->append(memoryRepresentation);
}
