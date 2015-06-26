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
#include "Tags/Internal/UInt32Tag.hpp"

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, UInteger32Tag, Tag);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Factory Methods

UInteger32Tag::Pointer UInteger32Tag::tagWithMemoryAt(const byte* tagAddress)
{
    const byte* tagData = Internal::Tag::dataForTagAt(tagAddress);

    return UInteger32Tag::tagWithIdentifierAndValue(Tag::identifierForTagAt(tagAddress),
                                                    Platform::bigEndianUInt32ValueAt(tagData));
}

UInteger32Tag::Pointer UInteger32Tag::tagWithIdentifierAndValue(uinteger32 identifier, uinteger32 value)
{
    auto newTag = UInteger32Tag::makeShared();
    newTag->internal->identifier = identifier;
    newTag->internal->value = value;

    return newTag;
}

#pragma mark Instance Methods

uinteger32 UInteger32Tag::value(void) const
{
    return internal->value;
}

void UInteger32Tag::setValue(uinteger32 value)
{
    this->internal->value = value;
}

void UInteger32Tag::addTo(Blob::Pointer& destination) const
{
    size_t dataSize = 4;
    auto memoryRepresentation = Blob::blobWithCapacity(Internal::Tag::memoryNeededForTagHeader() + dataSize);

    byte* tagAddress = memoryRepresentation->data();
    Internal::Tag::setIdentifierForTagAt(this->identifier(), tagAddress);
    Internal::Tag::setDataSizeForTagAt(dataSize, tagAddress);
    Platform::writeBigEndianUInt32ValueAt(this->value(), Internal::Tag::dataForTagAt(tagAddress));

    destination->append(memoryRepresentation);
}
