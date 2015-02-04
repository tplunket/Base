//
//  NxASeratoDatabaseParser.cpp
//  SeratoDB
//
//  Created by Didier Malenfant on 1/30/15.
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

#include "SeratoDB/NxASeratoDatabaseParser.h"
#include "SeratoDB/NxASeratoDatabaseV2Tags.h"

using namespace NxA;

#pragma mark Constructors

SeratoDatabaseParser::SeratoDatabaseParser(const void* startOfFile, unsigned long lengthInBytes)
{
    SeratoDatabaseTagVector* tags = SeratoDatabaseTag::parseTagsIn(startOfFile, lengthInBytes);
    this->p_tags = tags;
}

#pragma mark Destructor

SeratoDatabaseParser::~SeratoDatabaseParser()
{
    for(SeratoDatabaseTagVector::iterator it = this->p_tags->begin(); it != this->p_tags->end(); ++it) {
        delete *it;
    }

    delete this->p_tags;
}

#pragma mark Instance Methods

const std::string* SeratoDatabaseParser::versionAsString(void) const
{
    SeratoDatabaseTag* firstTag = (*this->p_tags)[0];
    if (firstTag->identifier() == NxASeratoDatabaseVersionTag) {
        return firstTag->dataAsString();
    }

    return NULL;
}
