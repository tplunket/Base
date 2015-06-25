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
#include "Tags/Internal/BooleanTag.hpp"

NXA_GENERATED_IMPLEMENTATION_FOR(NxA::Serato, BooleanTag);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Constructors & Destructors

BooleanTag::BooleanTag(NxA::Internal::Object::Pointer const& initial_internal) :
                       Tag(initial_internal),
                       internal(initial_internal) { }

#pragma mark Factory Methods

BooleanTag::Pointer BooleanTag::tagWithMemoryAt(const byte* tagAddress)
{
    const byte* tagData = Internal::Tag::dataForTagAt(tagAddress);

    return BooleanTag::tagWithIdentifierAndValue(Tag::identifierForTagAt(tagAddress),
                                                 (*tagData == 1) ? true : false);
}

BooleanTag::Pointer BooleanTag::tagWithIdentifierAndValue(uinteger32 identifier, bool value)
{
    auto newTag = BooleanTag::makeShared();
    newTag->internal->identifier = identifier;
    newTag->internal->value = value;

    return newTag;
}

#pragma mark Instance Methods

bool BooleanTag::value(void) const
{
    return internal->value;
}

void BooleanTag::setValue(bool newValue)
{
    internal->value = newValue;
}

void BooleanTag::addTo(Blob::Pointer const& destination) const
{
    count dataSize = 1;
    count totalSizeNeeded = Internal::Tag::memoryNeededForTagHeader() + dataSize;
    auto memoryRepresentation = Blob::blobWithCapacity(totalSizeNeeded);

    byte* tagAddress = memoryRepresentation->data();
    Internal::Tag::setIdentifierForTagAt(this->identifier(), tagAddress);
    Internal::Tag::setDataSizeForTagAt(dataSize, tagAddress);
    byte* tagData = Internal::Tag::dataForTagAt(tagAddress);
    *tagData = this->value() ? 1 : 0;

    destination->append(memoryRepresentation);
}
