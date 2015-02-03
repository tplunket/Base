//
//  NxASeratoDatabaseTagParser.cpp
//  SeratoDB
//
//  Created by Didier Malenfant on 1/31/15.
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

#include "SeratoDB/NxASeratoDatabaseTagParser.h"

#include <stdlib.h>
#include <codecvt>
#include <locale>

#pragma mark Structures

typedef struct {
    unsigned char identifier[4];
    unsigned char length[4];
    unsigned char data[0];
} SeratoDatabaseTagStruct;

#pragma mark Implementation

using namespace NxA;

#pragma mark Utility Methods

const char16_t* p_convertEndiannessOfUTF16Characters(const char16_t* characters, uint numberOfCharacters)
{
    uint numberOfBytes = numberOfCharacters * 2;
    char* newCharacters = (char*)malloc(numberOfBytes);

    for (int i = 0; i < numberOfBytes; i += 2) {
        newCharacters[i] = ((char*)characters)[i+1];
        newCharacters[i+1] = ((char*)characters)[i];
    }

    return (char16_t*)newCharacters;
}

const std::string* p_convertUTF16ToStdString(const char16_t* characters, uint numberOfCharacters)
{
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
    std::string* stdString = new std::string(convert.to_bytes(characters, characters + numberOfCharacters));
    return stdString;
}

#pragma mark Instance Methods

size_t SeratoDatabaseTagParser::p_sizeOfDataInBytes(void) const
{
    if (this->hasParsedAllTags()) {
        return 0;
    }

    SeratoDatabaseTagStruct* tagStructPtr = (SeratoDatabaseTagStruct*)this->p_currentTagAddress;

    unsigned long dataLength = tagStructPtr->length[0] << 24 |
    tagStructPtr->length[1] << 16 |
    tagStructPtr->length[2] << 8 |
    tagStructPtr->length[3];
    return dataLength;
}

bool SeratoDatabaseTagParser::hasParsedAllTags(void) const
{
    return this->p_currentTagAddress >= p_endOfTagsAddress;
}

void SeratoDatabaseTagParser::goToTagWithIdentifier(uint32_t identifier)
{
    while (!this->hasParsedAllTags()) {
        if (this->identifier() == identifier) {
            return;
        }

        this->goToNextTag();
    }

    this->goToEnd();
}

void SeratoDatabaseTagParser::goToNextTag(void)
{
    if (this->hasParsedAllTags()) {
        return;
    }

    unsigned long dataLength = this->p_sizeOfDataInBytes();
    this->p_currentTagAddress = (const unsigned char*)this->p_currentTagAddress + dataLength + sizeof(SeratoDatabaseTagStruct);
}

void SeratoDatabaseTagParser::goToDataAsTag(void)
{
    if (this->hasParsedAllTags()) {
        return;
    }

    SeratoDatabaseTagStruct* tagStructPtr = (SeratoDatabaseTagStruct*)this->p_currentTagAddress;
    unsigned long dataLength = this->p_sizeOfDataInBytes();

    this->p_currentTagAddress = (const unsigned char*)tagStructPtr->data;
    this->p_endOfTagsAddress = (const unsigned char*)this->p_currentTagAddress + dataLength;
}

void SeratoDatabaseTagParser::goToEnd(void)
{
    this->p_currentTagAddress = p_endOfTagsAddress;
}

uint32_t SeratoDatabaseTagParser::identifier(void) const
{
    if (this->hasParsedAllTags()) {
        return 0;
    }

    SeratoDatabaseTagStruct* tagStructPtr = (SeratoDatabaseTagStruct*)this->p_currentTagAddress;

    return tagStructPtr->identifier[0] << 24 |
    tagStructPtr->identifier[1] << 16 |
    tagStructPtr->identifier[2] << 8 |
    tagStructPtr->identifier[3];
}

const std::string* SeratoDatabaseTagParser::dataAsString(void) const
{
    if (this->hasParsedAllTags()) {
        return NULL;
    }

    SeratoDatabaseTagStruct* tagStructPtr = (SeratoDatabaseTagStruct*)this->p_currentTagAddress;

    uint numberOfCharacters = uint(this->p_sizeOfDataInBytes() / 2);
    const char16_t* textToRead = (const char16_t*)tagStructPtr->data;

#ifdef __LITTLE_ENDIAN__
    textToRead = p_convertEndiannessOfUTF16Characters(textToRead, numberOfCharacters);
#endif

    const std::string *dataAsAString = p_convertUTF16ToStdString(textToRead, numberOfCharacters);

#ifdef __LITTLE_ENDIAN__
    free((void*)textToRead);
#endif

    return dataAsAString;
}
