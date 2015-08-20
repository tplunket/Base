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

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, BlobTag, Tag);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Factory Methods

BlobTag::Pointer BlobTag::tagWithMemoryAt(const byte* tagAddress)
{
    return BlobTag::tagWithIdentifierAndValue(Tag::identifierForTagAt(tagAddress),
                                              Blob::blobWithMemoryAndSize(Internal::Tag::dataForTagAt(tagAddress),
                                                                          Tag::dataSizeForTagAt(tagAddress)));
}

BlobTag::Pointer BlobTag::tagWithIdentifierAndValue(uinteger32 identifier, const Blob& value)
{
    NXA_ASSERT_EQ((identifier & 0xFF000000) >> 24, 'a');

    auto newTag = BlobTag::makeShared();
    newTag->internal->identifier = identifier;
    newTag->internal->value = value.pointer();

    return newTag;
}

#pragma mark Operators

bool BlobTag::operator==(const BlobTag& other) const
{
    if (this == &other) {
        return true;
    }

    return this->value() == other.value();
}

#pragma mark Instance Methods

const Blob& BlobTag::value(void) const
{
    return internal->value;
}

void BlobTag::setValue(const Blob& newValue)
{
    internal->value = newValue.pointer();
}

void BlobTag::addTo(Blob& destination) const
{
    count dataSize = internal->value->size();
    count totalSizeNeeded = Internal::Tag::memoryNeededForTagHeader() + dataSize;
    auto memoryRepresentation = Blob::blobWithCapacity(totalSizeNeeded);

    byte* tagAddress = memoryRepresentation->data();
    Internal::Tag::setIdentifierForTagAt(this->identifier(), tagAddress);
    Internal::Tag::setDataSizeForTagAt(dataSize, tagAddress);
    memcpy(Internal::Tag::dataForTagAt(tagAddress), this->value().data(), this->value().size());

    destination.append(memoryRepresentation);
}
