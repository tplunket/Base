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

#include "Tags/VersionTag.hpp"
#include "Tags/Internal/VersionTag.hpp"

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, VersionTag, TextTag);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Factory Methods

VersionTag::Pointer VersionTag::tagWithMemoryAt(const byte* tagAddress)
{
    count size = Tag::dataSizeForTagAt(tagAddress);
    auto text = String::stringWithUTF16(Blob::blobWithMemoryAndSize(Internal::Tag::dataForTagAt(tagAddress), size));

    return VersionTag::tagWithIdentifierAndValue(Tag::identifierForTagAt(tagAddress), text);
}

VersionTag::Pointer VersionTag::tagWithIdentifierAndValue(uinteger32 identifier, const String& value)
{
    NXA_ASSERT_EQ((identifier & 0xFF000000) >> 24, 'v');

    auto newTag = VersionTag::makeShared();
    newTag->internal->identifier = identifier;
    newTag->internal->value = value.pointer();

    return newTag;
}

#pragma mark Operators

bool VersionTag::operator==(const VersionTag& other) const
{
    if (this == &other) {
        return true;
    }

    return this->value() == other.value();
}
