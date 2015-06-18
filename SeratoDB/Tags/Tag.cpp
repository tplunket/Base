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

#include "Base/Platform.hpp"

using namespace NxA;
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
    Platform::writeBigEndianUInt32ValueAt(identifier, tagStructPtr->identifier);
}

size_t Tag::p_dataSizeForTagAt(const void* tagAddress)
{
    TagStruct* tagStructPtr = (TagStruct*)tagAddress;
    unsigned long dataSize = Platform::bigEndianUInt32ValueAt(tagStructPtr->length);
    return dataSize;
}

void Tag::p_setDataSizeForTagAt(const size_t& dataSize, const void* tagAddress)
{
    TagStruct* tagStructPtr = (TagStruct*)tagAddress;
    Platform::writeBigEndianUInt32ValueAt((uint32_t)dataSize, tagStructPtr->length);
}

size_t Tag::p_memoryNeededForTagHeader(void)
{
    return sizeof(TagStruct);
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
    uint32_t identifier = Platform::bigEndianUInt32ValueAt(tagStructPtr->identifier);
    return identifier;
}

const void* Tag::nextTagAfterBinaryRepresentationAt(const void* tagAddress)
{
    return (const unsigned char*)tagAddress + Tag::p_dataSizeForTagAt(tagAddress) + sizeof(TagStruct);
}

#pragma mark Instance Methods

const uint32_t& Tag::identifier(void) const
{
    return this->p_identifier;
}
