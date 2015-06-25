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

#include "Tags/TagFactory.hpp"
#include "Tags/BooleanTag.hpp"
#include "Tags/PathTag.hpp"
#include "Tags/TextTag.hpp"
#include "Tags/BlobTag.hpp"
#include "Tags/ObjectTag.hpp"
#include "Tags/UInt16Tag.hpp"
#include "Tags/UInt32Tag.hpp"

using namespace NxA::Serato;
using namespace std;

#pragma mark Class Methods

Tag::Pointer TagFactory::tagForTagAt(const byte* tagAddress)
{
    uinteger32 identifier = Tag::identifierForTagAt(tagAddress);
    char typeIdentifier = (identifier >> 24) & 0xff;

    switch (typeIdentifier) {
        case 'b': {
            return BooleanTag::tagWithMemoryAt(tagAddress);
        }
        case 's': {
            return UInteger16Tag::tagWithMemoryAt(tagAddress);
        }
        case 'u': {
            return UInteger32Tag::tagWithMemoryAt(tagAddress);
        }
        case 'v':
        case 't': {
            return TextTag::tagWithMemoryAt(tagAddress);
        }
        case 'p': {
            return PathTag::tagWithMemoryAt(tagAddress);
        }
        case 'a': {
            return BlobTag::tagWithMemoryAt(tagAddress);
        }
        case 'o': {
            return ObjectTag::tagWithMemoryAt(tagAddress);
        }
    }

    NXA_ALOG("Illegal Serato tag type.");
}

Tag::Array::Pointer TagFactory::parseTagsAt(const byte* firstTagAddress, count sizeFromFirstTag)
{
    const byte* tagAddress = firstTagAddress;
    const byte* endOfTagsAddress = firstTagAddress + sizeFromFirstTag;

    auto newTags = Tag::Array::array();

    while (tagAddress < endOfTagsAddress) {
        auto tag = TagFactory::tagForTagAt(tagAddress);
        newTags->append(tag);

        tagAddress = Tag::nextTagAfterTagAt(tagAddress);
    }

    return newTags;
}
