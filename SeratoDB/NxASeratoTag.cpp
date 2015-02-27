//
//  NxASeratoTag.cpp
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

#include "SeratoDB/NxASeratoTag.h"

using namespace NxA;
using namespace std;

#pragma mark Structures

typedef struct {
    unsigned char identifier[4];
    unsigned char length[4];
    unsigned char data[0];
} SeratoTagStruct;

#pragma mark Utility Functions

namespace NxA {
    static uint32_t p_identifierForTagAt(const void* tagAddress)
    {
        const SeratoTagStruct* tagStructPtr = (const SeratoTagStruct*)tagAddress;
        uint32_t identifier = bigEndianUInt32ValueAt(tagStructPtr->identifier);
        return identifier;
    }

    static const void* p_dataForTagAt(const void* tagAddress)
    {
        const SeratoTagStruct* tagStructPtr = (const SeratoTagStruct*)tagAddress;
        const void* data = tagStructPtr->data;
        return data;
    }

    static size_t p_dataSizeInBytesForTagAt(const void* tagAddress)
    {
        SeratoTagStruct* tagStructPtr = (SeratoTagStruct*)tagAddress;
        unsigned long dataSizeInBytes = bigEndianUInt32ValueAt(tagStructPtr->length);
        return dataSizeInBytes;
    }
}

#pragma mark Constructors

SeratoTag::SeratoTag(const void* tagAddress)
{
    this->p_identifier = p_identifierForTagAt(tagAddress);
    this->p_dataSizeInBytes = p_dataSizeInBytesForTagAt(tagAddress);
    this->p_memoryRepresentation = CharVectorAutoPtr(new CharVector((char*)tagAddress,
                                                                    (char*)tagAddress + this->p_dataSizeInBytes + sizeof(SeratoTagStruct)));

    this->p_parentTag = NULL;

    if ((char)(this->p_identifier >> 24) == 'o') {
        const void* dataAddress = p_dataForTagAt(tagAddress);
        
        this->p_subTags = SeratoTag::p_parseTagsInForParentTag(dataAddress, this->p_dataSizeInBytes, this);

        for(ConstSeratoTagVector::iterator it = this->p_subTags->begin(); it != this->p_subTags->end(); ++it) {
            const SeratoTag* subTag = it->get();
            this->p_subTagForIdentifier[subTag->p_identifier] = subTag;
        }
    }
}

SeratoTag::SeratoTag(uint32_t identifier, const char* content)
{
    this->p_parentTag = NULL;

    this->p_identifier = identifier;
    this->p_dataSizeInBytes = strlen(content) * 2;
    this->p_memoryRepresentation = CharVectorAutoPtr(new CharVector(this->p_dataSizeInBytes + sizeof(SeratoTagStruct), 0));

    SeratoTagStruct* tagStructPtr = (SeratoTagStruct*)p_memoryRepresentation->data();
    writeBigEndianUInt32ValueAt(identifier, tagStructPtr->identifier);
    writeBigEndianUInt32ValueAt((uint32_t)this->p_dataSizeInBytes, tagStructPtr->length);
    writeStringAsUTF16At(content, tagStructPtr->data);
}

SeratoTag::SeratoTag(uint32_t identifier, ConstSeratoTagVectorAutoPtr content)
{
    this->p_parentTag = NULL;

    this->p_identifier = identifier;
    this->p_dataSizeInBytes = 0;
    this->p_memoryRepresentation = CharVectorAutoPtr(new CharVector(this->p_dataSizeInBytes + sizeof(SeratoTagStruct), 0));

    for(ConstSeratoTagVector::iterator it = content->begin(); it != content->end(); ++it) {
        const SeratoTag* tag = it->get();

        this->p_subTagForIdentifier[tag->p_identifier] = tag;

        tag->addTo(*(this->p_memoryRepresentation.get()));

        this->p_dataSizeInBytes += tag->p_dataSizeInBytes + sizeof(SeratoTagStruct);
    }

    this->p_subTags = content;

    SeratoTagStruct* tagStructPtr = (SeratoTagStruct*)p_memoryRepresentation->data();
    writeBigEndianUInt32ValueAt(identifier, tagStructPtr->identifier);
    writeBigEndianUInt32ValueAt((uint32_t)this->p_dataSizeInBytes, tagStructPtr->length);
}

#pragma mark Class Methods

ConstSeratoTagVectorAutoPtr SeratoTag::p_parseTagsInForParentTag(const void* firstTagAddress,
                                                                 size_t sizeFromFirstTagInBytes,
                                                                 const SeratoTag* parentTag)
{
    const void* tagAddress = firstTagAddress;
    const void* endOfTagsAddress = (unsigned char*)firstTagAddress + sizeFromFirstTagInBytes;

    ConstSeratoTagVector* newTags = new ConstSeratoTagVector;

    while (tagAddress < endOfTagsAddress) {
        SeratoTag* newTag = parentTag ? new SeratoTag(tagAddress, parentTag) : new SeratoTag(tagAddress);
        newTags->push_back(ConstSeratoTagAutoPtr(newTag));

        tagAddress = (const unsigned char*)tagAddress + newTag->p_dataSizeInBytes + sizeof(SeratoTagStruct);
    }

    return ConstSeratoTagVectorAutoPtr(newTags);
}

ConstSeratoTagVectorAutoPtr SeratoTag::parseTagsIn(const CharVectorAutoPtr& seratoTagData)
{
    return SeratoTag::p_parseTagsInForParentTag(seratoTagData->data(), seratoTagData->size(), NULL);
}

#pragma mark Instance Methods

const SeratoTag* SeratoTag::subTagWithIdentifierOrNilIfDoesNotExist(uint32_t identifier) const
{
    SeratoIdentifierToTagMap::const_iterator it = this->p_subTagForIdentifier.find(identifier);
    if (it == this->p_subTagForIdentifier.end()) {
        return NULL;
    }

    return it->second;
}

size_t SeratoTag::dataSizeInBytes(void) const
{
    return this->p_dataSizeInBytes;
}

uint32_t SeratoTag::identifier(void) const
{
    return this->p_identifier;
}

StringAutoPtr SeratoTag::dataAsString(void) const
{
    int numberOfCharacters = (int)this->p_dataSizeInBytes / 2;
    const char16_t* textToRead = (const char16_t*)this->data();

    return convertUTF16ToStdString(textToRead, numberOfCharacters);
}

bool SeratoTag::dataAsBoolean(void) const
{
    const char* dataAsChar = (const char*)this->data();
    return *dataAsChar != 0;
}

uint16_t SeratoTag::dataAsUInt16(void) const
{
    uint16_t value = bigEndianUInt16ValueAt(this->data());
    return value;
}

uint32_t SeratoTag::dataAsUInt32(void) const
{
    uint32_t value = bigEndianUInt32ValueAt(this->data());
    return value;
}

StringAutoPtr SeratoTag::dataAsPath(void) const
{
    return this->dataAsString();
}

const void* SeratoTag::data(void) const
{
    return p_dataForTagAt(this->p_memoryRepresentation->data());
}

void SeratoTag::addTo(CharVector& destination) const
{
    destination.insert(destination.end(), this->p_memoryRepresentation->begin(), this->p_memoryRepresentation->end());
}

