//
//  NxASeratoTagFactory.cpp
//  SeratoDB
//
//  Created by Didier Malenfant on 2/27/15.
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

#include "SeratoDB/NxASeratoTagFactory.h"
#include "SeratoDB/NxASeratoBooleanTag.h"
#include "SeratoDB/NxASeratoPathTag.h"
#include "SeratoDB/NxASeratoTextTag.h"
#include "SeratoDB/NxASeratoBlobTag.h"
#include "SeratoDB/NxASeratoObjectTag.h"
#include "SeratoDB/NxASeratoUInt16Tag.h"
#include "SeratoDB/NxASeratoUInt32Tag.h"

using namespace NxA;
using namespace std;

#pragma mark Class Methods

SeratoTagPtr SeratoTagFactory::tagForBinaryRepresentationAt(const void* tagAddress)
{
    uint32_t identifier = SeratoTag::identifierForTagAt(tagAddress);
    char typeIdentifier = (identifier >> 24) & 0xff;

    SeratoTagPtr newTag;

    switch (typeIdentifier) {
        case 'b': {
            newTag = make_unique<SeratoBooleanTag>(tagAddress);
            break;
        }
        case 's': {
            newTag = make_unique<SeratoUInt16Tag>(tagAddress);
            break;
        }
        case 'u': {
            newTag = make_unique<SeratoUInt32Tag>(tagAddress);
            break;
        }
        case 'v':
        case 't': {
            newTag = make_unique<SeratoTextTag>(tagAddress);
            break;
        }
        case 'p': {
            newTag = make_unique<SeratoPathTag>(tagAddress);
            break;
        }
        case 'a': {
            newTag = make_unique<SeratoBlobTag>(tagAddress);
            break;
        }
        case 'o': {
            newTag = make_unique<SeratoObjectTag>(tagAddress);
            break;
        }
    }

    return move(newTag);
}

SeratoTagVectorPtr SeratoTagFactory::parseTagsAt(const void* firstTagAddress, size_t sizeFromFirstTagInBytes)
{
    const void* tagAddress = firstTagAddress;
    const void* endOfTagsAddress = (unsigned char*)firstTagAddress + sizeFromFirstTagInBytes;

    SeratoTagVectorPtr newTags = make_unique<SeratoTagVector>();

    while (tagAddress < endOfTagsAddress) {
        newTags->push_back(SeratoTagFactory::tagForBinaryRepresentationAt(tagAddress));
        tagAddress = SeratoTag::nextTagAfterBinaryRepresentationAt(tagAddress);
    }

    return move(newTags);
}
