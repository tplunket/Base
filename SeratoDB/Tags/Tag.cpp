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

#include "Tags/Tag.hpp"
#include "Tags/Internal/BlobTag.hpp"

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_PURE_VIRTUAL_CLASS_WITH_PARENT(NxA::Serato, Tag, Object);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Class Methods

uint32_t Tag::identifierForTagAt(const byte* tagAddress)
{
    const TagStruct* tagStructPtr = reinterpret_cast<const TagStruct*>(tagAddress);
    uint32_t identifier = Platform::bigEndianUInteger32ValueAt(tagStructPtr->identifier);
    return identifier;
}

const byte* Tag::nextTagAfterTagAt(const byte* tagAddress)
{
    return tagAddress + Tag::dataSizeForTagAt(tagAddress) + sizeof(TagStruct);
}

count Tag::dataSizeForTagAt(const byte* tagAddress)
{
    const TagStruct* tagStructPtr = reinterpret_cast<const TagStruct*>(tagAddress);
    unsigned long dataSize = Platform::bigEndianUInteger32ValueAt(tagStructPtr->length);
    return dataSize;
}

#pragma mark Instance Methods

uinteger32 Tag::identifier(void) const
{
    return internal->identifier;
}
