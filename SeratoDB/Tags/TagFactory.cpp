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
#include "Tags/VersionTag.hpp"
#include "Tags/BlobTag.hpp"
#include "Tags/ObjectTag.hpp"
#include "Tags/UInteger16Tag.hpp"
#include "Tags/UInteger32Tag.hpp"

using namespace NxA::Serato;
using namespace std;

#pragma mark Class Methods

Tag::Pointer TagFactory::tagForTagAt(const byte* tagAddress)
{
    uinteger32 identifier = Tag::identifierForTagAt(tagAddress);
    char typeIdentifier = (identifier >> 24) & 0xff;

    switch (typeIdentifier) {
        case 'b': {
            return Tag::Pointer::dynamicCastFrom(BooleanTag::tagWithMemoryAt(tagAddress));
        }
        case 's': {
            return Tag::Pointer::dynamicCastFrom(UInteger16Tag::tagWithMemoryAt(tagAddress));
        }
        case 'u': {
            return Tag::Pointer::dynamicCastFrom(UInteger32Tag::tagWithMemoryAt(tagAddress));
        }
        case 'v': {
            return Tag::Pointer::dynamicCastFrom(VersionTag::tagWithMemoryAt(tagAddress));
        }
        case 't': {
            return Tag::Pointer::dynamicCastFrom(TextTag::tagWithMemoryAt(tagAddress));
        }
        case 'p': {
            return Tag::Pointer::dynamicCastFrom(PathTag::tagWithMemoryAt(tagAddress));
        }
        case 'a': {
            return Tag::Pointer::dynamicCastFrom(BlobTag::tagWithMemoryAt(tagAddress));
        }
        case 'o': {
            return Tag::Pointer::dynamicCastFrom(ObjectTag::tagWithMemoryAt(tagAddress));
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
        if (Tag::dataSizeForTagAt(tagAddress) > 0) {
            auto tag = TagFactory::tagForTagAt(tagAddress);
            newTags->append(tag);
        }

        tagAddress = Tag::nextTagAfterTagAt(tagAddress);
    }

    return newTags;
}
