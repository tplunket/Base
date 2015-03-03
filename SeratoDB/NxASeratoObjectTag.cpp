//
//  NxASeratoObjectTag.cpp
//  SeratoDB
//
//  Created by Didier Malenfant on 2/26/15.
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

#include "SeratoDB/NxASeratoObjectTag.h"
#include "SeratoDB/NxASeratoTagFactory.h"

using namespace NxA;
using namespace std;

#pragma mark Constructors

SeratoObjectTag::SeratoObjectTag(const void* tagAddress) : SeratoTag(tagAddress)
{
    size_t dataSizeInBytes = SeratoTag::p_dataSizeInBytesForTagAt(tagAddress);
    const void* dataAddress = SeratoTag::p_dataForTagAt(tagAddress);

    SeratoTagVectorPtr subTags = SeratoTagFactory::parseTagsAt(dataAddress, dataSizeInBytes);
    for(SeratoTagVector::iterator it = subTags->begin(); it != subTags->end(); ++it) {
        SeratoTagPtr& subTag = *it;
        this->p_subTagForIdentifier[subTag->identifier()] = move(subTag);
    }
}

SeratoObjectTag::SeratoObjectTag(const uint32_t& identifier, SeratoTagVectorPtr content) : SeratoTag(identifier)
{
    for(SeratoTagVector::iterator it = content->begin(); it != content->end(); ++it) {
        SeratoTagPtr& subTag = *it;
        this->p_subTagForIdentifier[subTag->identifier()] = move(subTag);
    }
}

#pragma mark Instance Methods

bool SeratoObjectTag::hasSubTagForIdentifier(const uint32_t& identifier) const
{
    SeratoIdentifierToTagMap::const_iterator it = this->p_subTagForIdentifier.find(identifier);
    return it != this->p_subTagForIdentifier.end();
}

const SeratoTag& SeratoObjectTag::subTagForIdentifier(const uint32_t& identifier) const
{
    SeratoIdentifierToTagMap::const_iterator it = this->p_subTagForIdentifier.find(identifier);
    return *(it->second);
}

SeratoTag& SeratoObjectTag::subTagForIdentifier(const uint32_t& identifier)
{
    return const_cast<SeratoTag&>(static_cast<const SeratoObjectTag&>(*this).subTagForIdentifier(identifier));
}

void SeratoObjectTag::addSubTag(ConstSeratoTagPtr tag)
{
    this->p_subTagForIdentifier[tag->identifier()] = move(tag);
}

void SeratoObjectTag::addTo(CharVector& destination) const
{
    for (SeratoIdentifierToTagMap::const_iterator it = this->p_subTagForIdentifier.begin(); it != this->p_subTagForIdentifier.end(); ++it) {
        it->second->addTo(destination);
    }
}
