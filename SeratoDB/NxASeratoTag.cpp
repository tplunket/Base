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

#include "SeratoDB/NxASeratoTag.h"

using namespace NxA;
using namespace std;

#pragma mark Structures

typedef struct {
    unsigned char identifier[4];
    unsigned char length[4];
    unsigned char data[0];
} SeratoTagStruct;

#pragma mark Class Methods

void SeratoTag::p_setIdentifierForTagAt(const uint32_t& identifier, const void* tagAddress)
{
    SeratoTagStruct* tagStructPtr = (SeratoTagStruct*)tagAddress;
    writeBigEndianUInt32ValueAt(identifier, tagStructPtr->identifier);
}

size_t SeratoTag::p_dataSizeInBytesForTagAt(const void* tagAddress)
{
    SeratoTagStruct* tagStructPtr = (SeratoTagStruct*)tagAddress;
    unsigned long dataSizeInBytes = bigEndianUInt32ValueAt(tagStructPtr->length);
    return dataSizeInBytes;
}

void SeratoTag::p_setDataSizeForTagAt(const size_t& dataSizeInBytes, const void* tagAddress)
{
    SeratoTagStruct* tagStructPtr = (SeratoTagStruct*)tagAddress;
    writeBigEndianUInt32ValueAt((uint32_t)dataSizeInBytes, tagStructPtr->length);
}

size_t SeratoTag::p_memoryNeededWithDataOfSize(const size_t& dataSizeInBytes)
{
    return dataSizeInBytes + sizeof(SeratoTagStruct);
}

const void* SeratoTag::p_dataForTagAt(const void* tagAddress)
{
    const SeratoTagStruct* tagStructPtr = (const SeratoTagStruct*)tagAddress;
    const void* data = tagStructPtr->data;
    return data;
}

void* SeratoTag::p_dataForTagAt(void* tagAddress)
{
    return const_cast<void*>(SeratoTag::p_dataForTagAt(const_cast<const void*>(tagAddress)));
}

uint32_t SeratoTag::identifierForTagAt(const void* tagAddress)
{
    const SeratoTagStruct* tagStructPtr = (const SeratoTagStruct*)tagAddress;
    uint32_t identifier = bigEndianUInt32ValueAt(tagStructPtr->identifier);
    return identifier;
}

const void* SeratoTag::nextTagAfterBinaryRepresentationAt(const void* tagAddress)
{
    return (const unsigned char*)tagAddress + SeratoTag::p_dataSizeInBytesForTagAt(tagAddress) + sizeof(SeratoTagStruct);
}

#pragma mark Instance Methods

const uint32_t& SeratoTag::identifier(void) const
{
    return this->p_identifier;
}
