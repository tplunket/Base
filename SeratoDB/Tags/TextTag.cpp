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

#include "Tags/TextTag.hpp"
#include "Tags/Internal/TextTag.hpp"

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, TextTag, Tag);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Factory Methods

TextTag::Pointer TextTag::tagWithMemoryAt(const byte* tagAddress)
{
    count size = Internal::Tag::dataSizeForTagAt(tagAddress);
    auto text = String::stringWithUTF16(Blob::blobWithMemoryAndSize(Internal::Tag::dataForTagAt(tagAddress), size));

    return TextTag::tagWithIdentifierAndValue(Tag::identifierForTagAt(tagAddress), text);
}

TextTag::Pointer TextTag::tagWithIdentifierAndValue(uinteger32 identifier, const String& value)
{
    NXA_ASSERT_EQ((identifier & 0xFF000000) >> 24, 't');

    auto newTag = TextTag::makeShared();
    newTag->internal->identifier = identifier;
    newTag->internal->value = value.pointer();

    return newTag;
}

#pragma mark Operators

bool TextTag::operator==(const TextTag& other) const
{
    if (this == &other) {
        return true;
    }

    return this->value() == other.value();
}

#pragma mark Instance Methods

const String& TextTag::value(void) const
{
    return internal->value;
}

void TextTag::setValue(const String& value)
{
    internal->value = value.pointer();
}

void TextTag::addTo(Blob& destination) const
{
    auto memoryRepresentation = Blob::blobWithCapacity(Internal::Tag::memoryNeededForTagHeader());
    count dataSize = internal->value->length() * 2;

    byte* tagAddress = memoryRepresentation->data();
    Internal::Tag::setIdentifierForTagAt(this->identifier(), tagAddress);
    Internal::Tag::setDataSizeForTagAt(dataSize, tagAddress);

    destination.append(memoryRepresentation);
    destination.append(internal->value->toUTF16());
}
