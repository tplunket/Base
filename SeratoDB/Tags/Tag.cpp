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

using namespace NxA::Serato;
using namespace std;

#pragma mark Structures

typedef struct {
    unsigned char identifier[4];
    unsigned char length[4];
    unsigned char data[0];
} TagStruct;

#pragma mark Class Methods

void Tag::p_setIdentifierForTagAt(const uint32_t& identifier, const void* tagAddress)
{
    TagStruct* tagStructPtr = (TagStruct*)tagAddress;
    writeBigEndianUInt32ValueAt(identifier, tagStructPtr->identifier);
}

size_t Tag::p_dataSizeInBytesForTagAt(const void* tagAddress)
{
    TagStruct* tagStructPtr = (TagStruct*)tagAddress;
    unsigned long dataSizeInBytes = bigEndianUInt32ValueAt(tagStructPtr->length);
    return dataSizeInBytes;
}

void Tag::p_setDataSizeForTagAt(const size_t& dataSizeInBytes, const void* tagAddress)
{
    TagStruct* tagStructPtr = (TagStruct*)tagAddress;
    writeBigEndianUInt32ValueAt((uint32_t)dataSizeInBytes, tagStructPtr->length);
}

size_t Tag::p_memoryNeededWithDataOfSize(const size_t& dataSizeInBytes)
{
    return dataSizeInBytes + sizeof(TagStruct);
}

const void* Tag::p_dataForTagAt(const void* tagAddress)
{
    const TagStruct* tagStructPtr = (const TagStruct*)tagAddress;
    const void* data = tagStructPtr->data;
    return data;
}

void* Tag::p_dataForTagAt(void* tagAddress)
{
    return const_cast<void*>(Tag::p_dataForTagAt(const_cast<const void*>(tagAddress)));
}

uint32_t Tag::identifierForTagAt(const void* tagAddress)
{
    const TagStruct* tagStructPtr = (const TagStruct*)tagAddress;
    uint32_t identifier = bigEndianUInt32ValueAt(tagStructPtr->identifier);
    return identifier;
}

const void* Tag::nextTagAfterBinaryRepresentationAt(const void* tagAddress)
{
    return (const unsigned char*)tagAddress + Tag::p_dataSizeInBytesForTagAt(tagAddress) + sizeof(TagStruct);
}

#pragma mark Instance Methods

const uint32_t& Tag::identifier(void) const
{
    return this->p_identifier;
}
