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
#include "Tags/Internal/BlobTag.hpp"

NXA_GENERATED_IMPLEMENTATION_FOR(NxA::Serato, BlobTag, Tag);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Factory Methods

BlobTag::Pointer BlobTag::tagWithMemoryAt(const byte* tagAddress)
{
    return BlobTag::tagWithIdentifierAndValue(Tag::identifierForTagAt(tagAddress),
                                              Blob::blobWithMemoryAndSize(Internal::Tag::dataForTagAt(tagAddress),
                                                                          Internal::Tag::dataSizeForTagAt(tagAddress)));
}

BlobTag::Pointer BlobTag::tagWithIdentifierAndValue(uinteger32 identifier, Blob::Pointer const& value)
{
    auto newTag = BlobTag::makeShared();
    newTag->internal->identifier = identifier;
    newTag->internal->value = value;

    return newTag;
}

#pragma mark Instance Methods

Blob::Pointer const& BlobTag::value(void) const
{
    return internal->value;
}

void BlobTag::setValue(Blob::Pointer const& newValue)
{
    internal->value = newValue;
}

void BlobTag::addTo(Blob::Pointer& destination) const
{
    count dataSize = internal->value->size();
    count totalSizeNeeded = Internal::Tag::memoryNeededForTagHeader() + dataSize;
    auto memoryRepresentation = Blob::blobWithCapacity(totalSizeNeeded);

    byte* tagAddress = memoryRepresentation->data();
    Internal::Tag::setIdentifierForTagAt(this->identifier(), tagAddress);
    Internal::Tag::setDataSizeForTagAt(dataSize, tagAddress);
    memcpy(Internal::Tag::dataForTagAt(tagAddress), this->value()->data(), this->value()->size());

    destination->append(memoryRepresentation);
}
