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

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Constructors

TextTag::TextTag(const void* tagAddress) : Tag(tagAddress), p_value(String::string())
{
    count size = Tag::p_dataSizeForTagAt(tagAddress);
    const character* textToRead = static_cast<const character*>(p_dataForTagAt(tagAddress));
    auto utf16Text = Blob::blobWithMemoryAndSize(textToRead, size);

    this->p_value = String::stringWithUTF16(utf16Text);
}

#pragma mark Instance Methods

const uint32_t& TextTag::identifier(void) const
{
    return this->p_identifier;
}

String::ConstPointer const& TextTag::value(void) const
{
    return this->p_value;
}

String::ConstPointer& TextTag::value(void)
{
    return const_cast<String::ConstPointer&>(static_cast<const TextTag&>(*this).value());
}

void TextTag::addTo(Blob::Pointer const& destination) const
{
    auto memoryRepresentation = Blob::blobWithCapacity(Tag::p_memoryNeededForTagHeader());
    size_t dataSize = this->p_value->length() * 2;

    void* tagAddress = memoryRepresentation->data();
    Tag::p_setIdentifierForTagAt(this->identifier(), tagAddress);
    Tag::p_setDataSizeForTagAt(dataSize, tagAddress);

    destination->append(memoryRepresentation);
    destination->append(this->p_value->toUTF16());
}
