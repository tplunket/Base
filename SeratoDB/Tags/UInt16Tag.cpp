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

#include "Tags/UInt16Tag.hpp"
#include "Tags/Internal/UInt16Tag.hpp"

NXA_GENERATED_IMPLEMENTATION_FOR(NxA::Serato, UInteger16Tag);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Constructors & Destructors

UInteger16Tag::UInteger16Tag(NxA::Internal::Object::Pointer const& initial_internal) :
                             Tag(initial_internal),
                             internal(initial_internal) { }

#pragma mark Factory Methods

UInteger16Tag::Pointer UInteger16Tag::tagWithMemoryAt(const byte* tagAddress)
{
    const byte* tagData = Internal::Tag::dataForTagAt(tagAddress);

    return UInteger16Tag::tagWithIdentifierAndValue(Tag::identifierForTagAt(tagAddress),
                                                    Platform::bigEndianUInt16ValueAt(tagData));
}

UInteger16Tag::Pointer UInteger16Tag::tagWithIdentifierAndValue(uinteger32 identifier, uinteger16 value)
{
    auto newTag = UInteger16Tag::makeShared();
    newTag->internal->identifier = identifier;
    newTag->internal->value = value;

    return newTag;
}

#pragma mark Instance Methods

uinteger16 UInteger16Tag::value(void) const
{
    return internal->value;
}

void UInteger16Tag::setValue(uinteger16 value)
{
    this->internal->value = value;
}

void UInteger16Tag::addTo(Blob::Pointer const& destination) const
{
    size_t dataSize = 2;
    auto memoryRepresentation = Blob::blobWithCapacity(Internal::Tag::memoryNeededForTagHeader() + dataSize);

    byte* tagAddress = memoryRepresentation->data();
    Internal::Tag::setIdentifierForTagAt(this->identifier(), tagAddress);
    Internal::Tag::setDataSizeForTagAt(dataSize, tagAddress);
    Platform::writeBigEndianUInt16ValueAt(this->value(), Internal::Tag::dataForTagAt(tagAddress));

    destination->append(memoryRepresentation);
}
