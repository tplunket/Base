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

using namespace NxA::Serato;
using namespace std;

#pragma mark Constructors

TextTag::TextTag(const void* tagAddress) : Tag(tagAddress)
{
    int numberOfCharacters = (int)Tag::p_dataSizeInBytesForTagAt(tagAddress) / 2;
    const char16_t* textToRead = (const char16_t*)p_dataForTagAt(tagAddress);

    this->p_value = convertUTF16ToStdString(textToRead, numberOfCharacters);
}

#pragma mark Instance Methods

const uint32_t& TextTag::identifier(void) const
{
    return this->p_identifier;
}

const string& TextTag::value(void) const
{
    return *(this->p_value);
}

string& TextTag::value(void)
{
    return const_cast<string&>(static_cast<const TextTag&>(*this).value());
}

void TextTag::addTo(CharVector& destination) const
{
    size_t dataSize = this->p_value->length() * 2;
    size_t memoryNeededInBytes = Tag::p_memoryNeededWithDataOfSize(dataSize);
    CharVectorPtr memoryRepresentation = make_unique<CharVector>(memoryNeededInBytes, 0);

    void* tagAddress = memoryRepresentation->data();
    Tag::p_setIdentifierForTagAt(this->identifier(), tagAddress);
    Tag::p_setDataSizeForTagAt(dataSize, tagAddress);
    writeStringAsUTF16At(this->p_value->c_str(), Tag::p_dataForTagAt(tagAddress));

    destination.insert(destination.end(), memoryRepresentation->begin(), memoryRepresentation->end());
}
