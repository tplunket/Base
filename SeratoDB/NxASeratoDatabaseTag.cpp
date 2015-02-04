//
//  NxASeratoDatabaseTag.cpp
//  SeratoDB
//
//  Created by Didier Malenfant on 2/3/15.
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

#include "SeratoDB/NxASeratoDatabaseTag.h"

#include <stdlib.h>
#include <codecvt>
#include <locale>

using namespace NxA;

#pragma mark Structures

typedef struct {
    unsigned char identifier[4];
    unsigned char length[4];
    unsigned char data[0];
} SeratoDatabaseTagStruct;

#pragma mark Utility Functions

namespace NxA {
    const char16_t* p_convertEndiannessOfUTF16Characters(const char16_t* characters, int numberOfCharacters)
    {
        int numberOfBytes = numberOfCharacters * 2;
        char* newCharacters = (char*)malloc(numberOfBytes);

        for (int i = 0; i < numberOfBytes; i += 2) {
            newCharacters[i] = ((char*)characters)[i+1];
            newCharacters[i+1] = ((char*)characters)[i];
        }

        return (char16_t*)newCharacters;
    }

    const std::string* p_convertUTF16ToStdString(const char16_t* characters, int numberOfCharacters)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
        std::string* stdString = new std::string(convert.to_bytes(characters, characters + numberOfCharacters));
        return stdString;
    }

    const uint32_t p_bigEndianUInt32ValueAt(const void* ptr)
    {
        const char* charsPtr = (const char*)ptr;
        return ((charsPtr[0] << 24) & 0xff000000) |
        ((charsPtr[1] << 16) & 0xff0000) |
        ((charsPtr[2] << 8) & 0xff00) |
        (charsPtr[3] & 0xff);
    }

    const uint16_t p_bigEndianUInt16ValueAt(const void* ptr)
    {
        const char* charsPtr = (const char*)ptr;
        return ((charsPtr[0] << 8) & 0xff00) |
        (charsPtr[1] & 0xff);
    }

    uint32_t p_identifierForTagAt(const void* tagAddress)
    {
        const SeratoDatabaseTagStruct* tagStructPtr = (const SeratoDatabaseTagStruct*)tagAddress;
        uint32_t identifier = p_bigEndianUInt32ValueAt(tagStructPtr->identifier);
        return identifier;
    }

    const void* p_dataForTagAt(const void* tagAddress)
    {
        const SeratoDatabaseTagStruct* tagStructPtr = (const SeratoDatabaseTagStruct*)tagAddress;
        const void* data = tagStructPtr->data;
        return data;
    }

    const size_t p_dataSizeInBytesForTagAt(const void* tagAddress)
    {
        SeratoDatabaseTagStruct* tagStructPtr = (SeratoDatabaseTagStruct*)tagAddress;
        unsigned long dataSizeInBytes = p_bigEndianUInt32ValueAt(tagStructPtr->length);
        return dataSizeInBytes;
    }
}

#pragma mark Constructors

SeratoDatabaseTag::SeratoDatabaseTag(const void* tagAddress)
{
    this->p_identifier = p_identifierForTagAt(tagAddress);
    this->p_dataSizeInBytes = p_dataSizeInBytesForTagAt(tagAddress);
    const void* dataAddress = p_dataForTagAt(tagAddress);

    this->p_data = new SeratoDatabaseTagData((unsigned char*)dataAddress,
                                             (unsigned char*)dataAddress + this->p_dataSizeInBytes);

    this->p_childrenTagsByIdentifier = new SeratoIdentifierToTagMap;
    this->p_parentTag = NULL;

    if ((char)(this->p_identifier >> 24) == 'o') {
        SeratoDatabaseTagVector* subTags = SeratoDatabaseTag::parseTagsIn(dataAddress, this->p_dataSizeInBytes);

        for(SeratoDatabaseTagVector::iterator it = subTags->begin(); it != subTags->end(); ++it) {
            SeratoDatabaseTag* subTag = *it;
            (*this->p_childrenTagsByIdentifier)[subTag->p_identifier] = subTag;

            subTag->p_parentTag = this;
        }

        delete subTags;
    }
}

#pragma mark Destructor

SeratoDatabaseTag::~SeratoDatabaseTag()
{
    SeratoIdentifierToTagMap* children = this->p_childrenTagsByIdentifier;
    this->p_childrenTagsByIdentifier = NULL;

    for(SeratoIdentifierToTagMap::iterator it = children->begin(); it != children->end(); ++it) {
        SeratoDatabaseTag* subTag = it->second;
        delete subTag;
    }

    delete children;

    delete this->p_data;
    this->p_data = NULL;
}

#pragma mark Class Methods

SeratoDatabaseTagVector* SeratoDatabaseTag::parseTagsIn(const void* firstTagAddress, size_t sizeFromFirstTagInBytes)
{
    const void* tagAddress = firstTagAddress;
    const void* endOfTagsAddress = (unsigned char*)firstTagAddress + sizeFromFirstTagInBytes;

    SeratoDatabaseTagVector* newTags = new SeratoDatabaseTagVector;

    while (tagAddress < endOfTagsAddress) {
        SeratoDatabaseTag* newTag = new SeratoDatabaseTag(tagAddress);
        newTags->push_back(newTag);

        tagAddress = (const unsigned char*)tagAddress + newTag->p_dataSizeInBytes + sizeof(SeratoDatabaseTagStruct);
    }

    return newTags;
}

void SeratoDatabaseTag::deleteTagsIn(SeratoDatabaseTagVector* tags)
{
    for(SeratoDatabaseTagVector::iterator it = tags->begin(); it != tags->end(); ++it) {
        delete *it;
    }
}

#pragma mark Instance Methods

SeratoDatabaseTag* SeratoDatabaseTag::subTagWithIdentifier(uint32_t identifier) const
{
    if (!this->p_childrenTagsByIdentifier) {
        return NULL;
    }
    
    SeratoIdentifierToTagMap::iterator it = this->p_childrenTagsByIdentifier->find(identifier);
    if (it == this->p_childrenTagsByIdentifier->end()) {
        return NULL;
    }

    return it->second;
}

uint32_t SeratoDatabaseTag::identifier(void) const
{
    return this->p_identifier;
}

const std::string* SeratoDatabaseTag::dataAsString(void) const
{
    int numberOfCharacters = (int)this->p_dataSizeInBytes / 2;
    const char16_t* textToRead = (const char16_t*)this->data();

#ifdef __LITTLE_ENDIAN__
    textToRead = p_convertEndiannessOfUTF16Characters(textToRead, numberOfCharacters);
#endif

    const std::string* dataAsAString = p_convertUTF16ToStdString(textToRead, numberOfCharacters);

#ifdef __LITTLE_ENDIAN__
    free((void*)textToRead);
#endif

    return dataAsAString;
}

bool SeratoDatabaseTag::dataAsBoolean(void) const
{
    const char* dataAsChar = (const char*)this->data();
    return *dataAsChar != 0;
}

uint16_t SeratoDatabaseTag::dataAsUInt16(void) const
{
    uint16_t value = p_bigEndianUInt16ValueAt(this->data());
    return value;
}

uint32_t SeratoDatabaseTag::dataAsUInt32(void) const
{
    uint32_t value = p_bigEndianUInt32ValueAt(this->data());
    return value;
}

const std::string* SeratoDatabaseTag::dataAsPath(void) const
{
    return this->dataAsString();
}

const void* SeratoDatabaseTag::data(void) const
{
    return this->p_data->data();
}

size_t SeratoDatabaseTag::dataSizeInBytes(void) const
{
    return this->p_dataSizeInBytes;
}
