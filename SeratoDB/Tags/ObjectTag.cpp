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

#include "Tags/ObjectTag.hpp"
#include "Tags/Internal/ObjectTag.hpp"

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, ObjectTag, Tag);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Factory Methods

ObjectTag::Pointer ObjectTag::tagWithMemoryAt(const byte* tagAddress)
{
    auto dataSize = Internal::Tag::dataSizeForTagAt(tagAddress);
    auto tagData = Internal::Tag::dataForTagAt(tagAddress);
    auto subTags = TagFactory::parseTagsAt(tagData, dataSize);

    return ObjectTag::tagWithIdentifierAndValue(Tag::identifierForTagAt(tagAddress),
                                                subTags);
}

ObjectTag::Pointer ObjectTag::tagWithIdentifierAndValue(uinteger32 identifier, const Tag::Array& content)
{
    NXA_ASSERT_NOT_EQ(content.length(), 0);
    NXA_ASSERT_EQ((identifier & 0xFF000000) >> 24, 'o');

    auto newTag = ObjectTag::makeShared();
    newTag->internal->identifier = identifier;

    for (auto& subTag : content) {
        newTag->setSubTag(*subTag);
    }

    return newTag;
}

#pragma mark Operators

bool ObjectTag::operator==(const ObjectTag& other) const
{
    if (this == &other) {
        return true;
    }

    if (internal->subTagForIdentifier->length() != other.internal->subTagForIdentifier->length()) {
        return false;
    }

    for (auto& pair : *internal->subTagForIdentifier) {
        auto& key = pair.first;

        if (!other.hasSubTagForIdentifier(key)) {
            return false;
        }

        auto& value = *(pair.second);
        if (value != other.subTagForIdentifier(key)) {
            return false;
        }
    }

    return true;
}

#pragma mark Instance Methods

boolean ObjectTag::hasSubTagForIdentifier(uinteger32 identifier) const
{
    return internal->subTagForIdentifier->containsValueForKey(identifier);
}

const Tag& ObjectTag::subTagForIdentifier(uinteger32 identifier) const
{
    return internal->subTagForIdentifier->valueForKey(identifier);
}

Tag& ObjectTag::subTagForIdentifier(uinteger32 identifier)
{
    return internal->subTagForIdentifier->valueForKey(identifier);
}

void ObjectTag::setSubTag(Tag& tag)
{
    internal->subTagForIdentifier->setValueForKey(tag, tag.identifier());
}

count ObjectTag::numberOfSubTags(void) const
{
    return internal->subTagForIdentifier->length();
}

void ObjectTag::addInSeratoTrackOrderTo(Blob& destination) const
{
    std::vector<uinteger32> identifierOrder = {
        'ttyp', 'pfil', 'tsng', 'tart', 'talb', 'tgen', 'tlen', 'tsiz', 'tbit', 'tsmp', 'tbpm', 'tcom', 'tgrp',
        'trmx', 'tlbl', 'tcmp', 'ttyr', 'tadd', 'tkey', 'uadd', 'utkn', 'ulbl', 'utme', 'ufsb', 'udsc', 'sbav',
        'bhrt', 'bmis', 'bply', 'blop', 'bitu', 'bovc', 'bcrt', 'biro', 'bwlb', 'bwll', 'buns', 'bbgl', 'bkrk'
    };

    std::vector<uinteger32> identifiersToOutput;
    for (auto& identifierAndTag : *(internal->subTagForIdentifier)) {
        identifiersToOutput.push_back(identifierAndTag.first);
    }

    auto subTagsRepresentation = Blob::blob();
    for (auto& identifier : identifierOrder) {
        if (this->hasSubTagForIdentifier(identifier)) {
            auto& tag = this->subTagForIdentifier(identifier);
            tag.addTo(subTagsRepresentation);
            identifiersToOutput.erase(std::find(identifiersToOutput.begin(), identifiersToOutput.end(), identifier));
        }
    }

    for (auto& identifier : identifiersToOutput) {
        NXA_ASSERT_TRUE(this->hasSubTagForIdentifier(identifier));
        auto& tag = this->subTagForIdentifier(identifier);
        tag.addTo(subTagsRepresentation);
    }

    auto sizeNeededForHeader = Internal::Tag::memoryNeededForTagHeader();
    auto headerRepresentation = Blob::blobWithCapacity(sizeNeededForHeader);
    auto tagAddress = headerRepresentation->data();
    Internal::Tag::setIdentifierForTagAt(this->identifier(), tagAddress);
    Internal::Tag::setDataSizeForTagAt(subTagsRepresentation->size(), tagAddress);

    destination.append(headerRepresentation);
    destination.append(subTagsRepresentation);
}

void ObjectTag::addTo(Blob& destination) const
{
    auto subTagsRepresentation = Blob::blob();
    for (auto& identifierAndTag : *(internal->subTagForIdentifier)) {
        identifierAndTag.second->addTo(subTagsRepresentation);
    }

    auto sizeNeededForHeader = Internal::Tag::memoryNeededForTagHeader();
    auto headerRepresentation = Blob::blobWithCapacity(sizeNeededForHeader);
    auto tagAddress = headerRepresentation->data();
    Internal::Tag::setIdentifierForTagAt(this->identifier(), tagAddress);
    Internal::Tag::setDataSizeForTagAt(subTagsRepresentation->size(), tagAddress);

    destination.append(headerRepresentation);
    destination.append(subTagsRepresentation);
}
