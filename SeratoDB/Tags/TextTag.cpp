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

NXA_GENERATED_IMPLEMENTATION_FOR(NxA::Serato, TextTag);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Constructors & Destructors

TextTag::TextTag(NxA::Internal::Object::Pointer const& initial_internal) :
                 Tag(initial_internal),
                 internal(initial_internal) { }

#pragma mark Factory Methods

TextTag::Pointer TextTag::tagWithMemoryAt(const byte* tagAddress)
{
    count size = Internal::Tag::dataSizeForTagAt(tagAddress);
    auto text = String::stringWithUTF16(Blob::blobWithMemoryAndSize(Internal::Tag::dataForTagAt(tagAddress), size));

    return TextTag::tagWithIdentifierAndValue(Tag::identifierForTagAt(tagAddress), text);
}

TextTag::Pointer TextTag::tagWithIdentifierAndValue(uinteger32 identifier, String::ConstPointer const& value)
{
    auto newTag = TextTag::makeShared();
    newTag->internal->identifier = identifier;
    newTag->internal->value = value;

    return newTag;
}

#pragma mark Instance Methods

String::ConstPointer const& TextTag::value(void) const
{
    return internal->value;
}

void TextTag::setValue(String::ConstPointer const& value)
{
    internal->value = value;
}

void TextTag::addTo(Blob::Pointer const& destination) const
{
    auto memoryRepresentation = Blob::blobWithCapacity(Internal::Tag::memoryNeededForTagHeader());
    count dataSize = internal->value->length() * 2;

    byte* tagAddress = memoryRepresentation->data();
    Internal::Tag::setIdentifierForTagAt(this->identifier(), tagAddress);
    Internal::Tag::setDataSizeForTagAt(dataSize, tagAddress);

    destination->append(memoryRepresentation);
    destination->append(internal->value->toUTF16());
}
