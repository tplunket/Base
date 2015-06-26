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

#include "SeratoDB/Crate.hpp"
#include "Internal/Crate.hpp"
#include "SeratoDB/Database.hpp"
#include "Tags/CrateV1Tags.hpp"
#include "Tags/TextTag.hpp"
#include "Tags/TagFactory.hpp"
#include "Utility.hpp"

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, Crate, Object);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Constants

static const character* crateFileCurrentVersionString = "1.0/Serato ScratchLive Crate";

#pragma mark Factory Methods

Crate::Pointer Crate::crateWithNameInFolderOnVolume(String::Pointer const& crateFullName,
                                                    String::Pointer const& seratoFolderPath,
                                                    String::Pointer const& volumePath)
{
    auto internalObject = Internal::Crate::Pointer(std::make_shared<Internal::Crate>(crateFullName,
                                                                                     volumePath,
                                                                                     crateFilePathForCrateNameInSeratoFolder(crateFullName,
                                                                                                                             seratoFolderPath)));
    auto newCrate = Crate::makeSharedWithInternal(NxA::Internal::Object::Pointer::dynamicCastFrom(internalObject));

    auto lastSeperatorIndex = crateFullName->indexOfLastOccurenceOf("%%");
    if (lastSeperatorIndex != crateFullName->length()) {
        newCrate->internal->crateName = crateFullName->subString(lastSeperatorIndex + 2);
    }
    else {
        newCrate->internal->crateName = crateFullName;
    }

    return newCrate;
}

#pragma mark Class Methods

bool Crate::isAValidCrateName(String::Pointer const& crateFullName,
                              String::Pointer const& seratoFolderPath)
{
    auto crateFilePath = crateFilePathForCrateNameInSeratoFolder(crateFullName, seratoFolderPath);
    return File::fileExistsAt(crateFilePath);
}

bool Crate::isASmartCrateName(String::Pointer const& crateFullName,
                              String::Pointer const& seratoFolderPath)
{
    auto crateFilePath = crateFilePathForSmartCrateNameInSeratoFolder(crateFullName, seratoFolderPath);
    return File::fileExistsAt(crateFilePath);
}

void Crate::addCrateAsChildOfCrate(Crate::Pointer& crate, Crate::Pointer& parentCrate)
{
    crate->internal->parentCrate = Crate::WeakPointer(parentCrate);

    parentCrate->internal->markCratesAsModified();

    parentCrate->internal->childrenCrates->append(crate);
}

void Crate::destroy(Crate::Pointer const& crate)
{
    for (auto &childrenCrate : *(crate->internal->childrenCrates)) {
        Crate::destroy(childrenCrate);
    }

    if (crate->hasParentCrate()) {
        auto parentCrate = crate->internal->parentCrate.toPointer();
        parentCrate->removeChildrenCrate(crate);
    }
}

#pragma mark Instance Methods

String::Pointer const& Crate::crateName(void) const
{
    return internal->crateName;
}

String::Pointer const& Crate::crateFullName(void) const
{
    return internal->crateFullName;
}

void Crate::addFullCrateNameWithPrefixAndRecurseToChildren(String::Pointer& destination, const char* prefix) const
{
    if (internal->crateFullName->length()) {
        destination->append(prefix);
        destination->append(internal->crateFullName);
        destination->append("\n");
    }

    for (auto& crate : *internal->childrenCrates) {
        crate->addFullCrateNameWithPrefixAndRecurseToChildren(destination, prefix);
    }
}

TrackEntry::Array::Pointer const& Crate::trackEntries(void) const
{
    return internal->trackEntries;
}

Crate::Array::Pointer const& Crate::crates(void) const
{
    return internal->childrenCrates;
}

void Crate::addTrackEntry(Serato::TrackEntry::Pointer const& trackEntry)
{
    internal->addTrackEntry(trackEntry);
}

void Crate::loadFromFile(void)
{
    auto crateFileData = File::readFileAt(internal->crateFilePath);
    auto tags = TagFactory::parseTagsAt(crateFileData->data(), crateFileData->size());
    if (!tags->length()) {
        return;
    }

    for (auto& tag : *(tags)) {
        switch (tag->identifier()) {
            case crateVersionTagIdentifier: {
                auto versionTag = TextTag::Pointer::dynamicCastFrom(tag);
                if (!versionTag->value()->isEqualTo(crateFileCurrentVersionString)) {
                    internal->otherTags->emptyAll();
                    internal->trackEntries->emptyAll();
                    return;
                }
                break;
            }
            case trackEntryTagIdentifier: {
                internal->storeTrackTag(tag);
                break;
            }
            default: {
                internal->storeOtherTag(tag);
                break;
            }
        }
    }
}

bool Crate::hasParentCrate(void) const
{
    return internal->parentCrate.isValid();
}

Crate::Pointer Crate::parentCrate(void) const
{
    return internal->parentCrate.toPointer();
}

String::Pointer const& Crate::crateFilePath(void) const
{
    return internal->crateFilePath;
}

void Crate::resetModificationFlags()
{
    internal->cratesWereModified = false;
    internal->tracksWereModified = false;
}

bool Crate::childrenCratesWereModified(void) const
{
    return internal->cratesWereModified;
}

void Crate::saveIfModifiedAndRecurseToChildren(void) const
{
    if (internal->rootVolumePath->length() && internal->tracksWereModified) {
        auto outputData = Blob::blob();

        auto versionTag = TextTag::tagWithIdentifierAndValue(crateVersionTagIdentifier,
                                                             String::stringWith(crateFileCurrentVersionString));
        versionTag->addTo(outputData);

        for (auto& trackEntry : *internal->trackEntries) {
            trackEntry->tagForEntry()->addTo(outputData);
        }

        for (auto& tag : *internal->otherTags) {
            tag->addTo(outputData);
        }

        File::writeBlobToFileAt(outputData, internal->crateFilePath);
    }

    for (auto& crate : *internal->childrenCrates) {
        crate->saveIfModifiedAndRecurseToChildren();
    }
}

void Crate::removeChildrenCrate(Crate::Pointer const& crate)
{
    auto position = internal->childrenCrates->find(crate);
    if (position == internal->childrenCrates->end()) {
        return;
    }

    crate->internal->parentCrate.release();
    internal->childrenCrates->remove(position);
    internal->markCratesAsModified();
}

TrackEntry::Array::Pointer Crate::removeAndReturnTrackEntries(void)
{
    auto currentEntries = internal->trackEntries;

    internal->trackEntries = TrackEntry::Array::array();
    internal->tracksWereModified = true;

    return currentEntries;
}

Crate::Array::Pointer Crate::removeAndReturnChildrenCrates(void)
{
    auto currentChildrenCrates = internal->childrenCrates;

    internal->childrenCrates = Crate::Array::array();
    internal->markCratesAsModified();

    for (auto& crate : *currentChildrenCrates) {
        crate->internal->parentCrate.release();
    }

    return currentChildrenCrates;
}
