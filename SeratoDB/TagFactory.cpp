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

#include "SeratoDB/TagFactory.hpp"
#include "SeratoDB/BooleanTag.hpp"
#include "SeratoDB/PathTag.hpp"
#include "SeratoDB/TextTag.hpp"
#include "SeratoDB/BlobTag.hpp"
#include "SeratoDB/ObjectTag.hpp"
#include "SeratoDB/UInt16Tag.hpp"
#include "SeratoDB/UInt32Tag.hpp"

using namespace NxA::Serato;
using namespace std;

#pragma mark Class Methods

TagPtr TagFactory::tagForBinaryRepresentationAt(const void* tagAddress)
{
    uint32_t identifier = Tag::identifierForTagAt(tagAddress);
    char typeIdentifier = (identifier >> 24) & 0xff;

    TagPtr newTag;

    switch (typeIdentifier) {
        case 'b': {
            newTag = make_unique<BooleanTag>(tagAddress);
            break;
        }
        case 's': {
            newTag = make_unique<UInt16Tag>(tagAddress);
            break;
        }
        case 'u': {
            newTag = make_unique<UInt32Tag>(tagAddress);
            break;
        }
        case 'v':
        case 't': {
            newTag = make_unique<TextTag>(tagAddress);
            break;
        }
        case 'p': {
            newTag = make_unique<PathTag>(tagAddress);
            break;
        }
        case 'a': {
            newTag = make_unique<BlobTag>(tagAddress);
            break;
        }
        case 'o': {
            newTag = make_unique<ObjectTag>(tagAddress);
            break;
        }
    }

    return move(newTag);
}

TagVectorPtr TagFactory::parseTagsAt(const void* firstTagAddress, size_t sizeFromFirstTagInBytes)
{
    const void* tagAddress = firstTagAddress;
    const void* endOfTagsAddress = (unsigned char*)firstTagAddress + sizeFromFirstTagInBytes;

    TagVectorPtr newTags = make_unique<TagVector>();

    while (tagAddress < endOfTagsAddress) {
        TagPtr tag(TagFactory::tagForBinaryRepresentationAt(tagAddress));
        if (tag.get()) {
            newTags->push_back(move(tag));
        }

        tagAddress = Tag::nextTagAfterBinaryRepresentationAt(tagAddress);
    }

    return move(newTags);
}
