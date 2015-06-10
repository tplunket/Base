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

#include "SeratoDB/BlobTag.hpp"

using namespace NxA::Serato;
using namespace std;

#pragma mark Constructors

BlobTag::BlobTag(const void* tagAddress) : Tag(tagAddress)
{
    size_t dataSizeInBytes = Tag::p_dataSizeInBytesForTagAt(tagAddress);
    const char* dataStart = static_cast<const char*>(Tag::p_dataForTagAt(tagAddress));

    this->p_value = make_unique<CharVector>(dataStart, dataStart + dataSizeInBytes);
}

#pragma mark Instance Methods

const CharVector& BlobTag::value(void) const
{
    return *(this->p_value);
}

CharVector& BlobTag::value(void)
{
    return const_cast<CharVector&>(static_cast<const BlobTag&>(*this).value());
}

void BlobTag::addTo(CharVector& destination) const
{
    size_t dataSize = this->p_value->size();
    size_t memoryNeededInBytes = Tag::p_memoryNeededWithDataOfSize(this->p_value->size());
    CharVectorPtr memoryRepresentation = make_unique<CharVector>(memoryNeededInBytes, 0);

    void* tagAddress = memoryRepresentation->data();
    Tag::p_setIdentifierForTagAt(this->identifier(), tagAddress);
    Tag::p_setDataSizeForTagAt(dataSize, tagAddress);
    memcpy(Tag::p_dataForTagAt(tagAddress), this->p_value->data(), this->p_value->size());

    destination.insert(destination.end(), memoryRepresentation->begin(), memoryRepresentation->end());
}
