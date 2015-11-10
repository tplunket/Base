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
#include "Tags/VersionTag.hpp"
#include "Tags/TagFactory.hpp"

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, Crate, Object);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Constants

static const character* crateFileCurrentVersionString = "1.0/Serato ScratchLive Crate";

#pragma mark Factory Methods

Crate::Pointer Crate::crateWithName(const String& crateFullName)
{
    auto internalObject = Internal::Crate::Pointer(std::make_shared<Internal::Crate>(crateFullName));
    auto newCrate = Crate::makeSharedWithInternal(NxA::Internal::Object::Pointer::dynamicCastFrom(internalObject));

    auto lastSeperatorIndex = crateFullName.indexOfLastOccurenceOf("%%");
    if (lastSeperatorIndex != crateFullName.length()) {
        newCrate->internal->crateName = crateFullName.subString(lastSeperatorIndex + 2);
    }
    else {
        newCrate->internal->crateName = crateFullName.pointer();
    }

    return newCrate;
}

#pragma mark Class Methods

String::Pointer Crate::subCratesDirectoryPathInSeratoFolder(const String& seratoFolderPath)
{
    auto joinedPath = File::joinPaths(seratoFolderPath, String::stringWith("Subcrates"));
    return joinedPath;
}

String::Pointer Crate::smartCratesDirectoryPathInSeratoFolder(const String& seratoFolderPath)
{
    auto joinedPath = File::joinPaths(seratoFolderPath, String::stringWith("SmartCrates"));
    return joinedPath;
}

String::Pointer Crate::crateFilePathForCrateNameInSeratoFolder(const String& crateName,
                                                               const String& seratoFolderPath)
{
    auto cratesFolderPath = subCratesDirectoryPathInSeratoFolder(seratoFolderPath);
    auto crateFilePartialPath = File::joinPaths(cratesFolderPath, crateName);
    crateFilePartialPath->append(".crate");

    return crateFilePartialPath;
}

String::Pointer Crate::crateFilePathForSmartCrateNameInSeratoFolder(const String& crateName,
                                                                    const String& seratoFolderPath)
{
    auto cratesFolderPath = subCratesDirectoryPathInSeratoFolder(seratoFolderPath);
    auto crateFilePartialPath = File::joinPaths(cratesFolderPath, crateName);
    crateFilePartialPath->append(".scrate");

    return crateFilePartialPath;
}

boolean Crate::isAValidCrateName(const String& crateFullName, const String& seratoFolderPath)
{
    auto crateFilePath = crateFilePathForCrateNameInSeratoFolder(crateFullName, seratoFolderPath);
    return File::fileExistsAt(crateFilePath);
}

boolean Crate::isASmartCrateName(const String& crateFullName, const String& seratoFolderPath)
{
    auto crateFilePath = crateFilePathForSmartCrateNameInSeratoFolder(crateFullName, seratoFolderPath);
    return File::fileExistsAt(crateFilePath);
}

#pragma mark Instance Methods

const String& Crate::crateName(void) const
{
    return internal->crateName;
}

const String& Crate::crateFullName(void) const
{
    return internal->crateFullName;
}

void Crate::addFullCrateNameWithPrefixAndRecurseToChildren(String& destination, const char* prefix) const
{
    if (internal->crateFullName->length()) {
        destination.append(prefix);
        destination.append(internal->crateFullName);
        destination.append("\n");
    }

    for (auto& crate : *internal->childrenCrates) {
        crate->addFullCrateNameWithPrefixAndRecurseToChildren(destination, prefix);
    }
}

TrackEntry::Array& Crate::trackEntries(void)
{
    return *(internal->trackEntries);
}

Crate::Array& Crate::crates(void)
{
    return *(internal->childrenCrates);
}

void Crate::addCrate(Crate& crate)
{
    NXA_ASSERT_FALSE(crate.hasParentCrate());

    crate.internal->parentCrate = Crate::WeakPointer(this->pointer());
    internal->childrenCrates->append(crate);

    internal->markCratesAsModified();
}

void Crate::removeCrate(Crate& crate)
{
    NXA_ASSERT_TRUE(crate.hasParentCrate() && &(crate.parentCrate()) == this);

    crate.internal->parentCrate.release();
    internal->childrenCrates->remove(crate);

    internal->markCratesAsModified();
}

Crate::Pointer Crate::findOrAddCrateWithName(const String& crateName) {
    for (auto& crate : crates()) {
        if (crate->crateFullName() == crateName) {
            return crate;
        }
    }

    auto crate = Serato::Crate::crateWithName(crateName);
    crates().append(crate);
    return crate;
}

void Crate::addTrackEntry(Serato::TrackEntry& trackEntry)
{
    if (trackEntry.hasParentCrate()) {
        NXA_ASSERT_TRUE(&trackEntry.parentCrate() == this);
        return;
    }

    trackEntry.setParentCrate(*this);
    internal->trackEntries->append(trackEntry);

    internal->tracksWereModified = true;
}

void Crate::removeTrackEntry(TrackEntry& trackEntry)
{
    auto position = internal->trackEntries->find(trackEntry);
    if (position != internal->trackEntries->end()) {
        internal->trackEntries->removeObjectAt(position);
        internal->tracksWereModified = true;
    }

    if (trackEntry.hasParentCrate()) {
        NXA_ASSERT_TRUE(&trackEntry.parentCrate() == this);
        trackEntry.removeFromParentCrate();
    }
}

void Crate::readFromFolderInVolume(const String& seratoFolderPath, const String& volume)
{
    auto filePath = crateFilePathForCrateNameInSeratoFolder(crateFullName(), seratoFolderPath);

    internal->crateFilePaths->append(String::stringWith(filePath));

    auto crateFileData = File::readFileAt(filePath);
    auto tags = TagFactory::parseTagsAt(crateFileData->data(), crateFileData->size());
    if (!tags->length()) {
        return;
    }

    for (auto& tag : *(tags)) {
        switch (tag->identifier()) {
            case crateVersionTagIdentifier: {
                auto& versionTag = dynamic_cast<VersionTag&>(*tag);
                if (versionTag.value() != crateFileCurrentVersionString) {
                    internal->otherTags->emptyAll();
                    internal->trackEntries->emptyAll();
                    return;
                }
                break;
            }
            case trackEntryTagIdentifier: {
                auto entry = Serato::TrackEntry::entryWithTagOnVolume(dynamic_cast<ObjectTag&>(*tag), volume);
                this->addTrackEntry(entry);
                break;
            }
            default: {
                internal->otherTags->append(tag);
                break;
            }
        }
    }
}

boolean Crate::hasParentCrate(void) const
{
    return internal->parentCrate.isValid();
}

Crate& Crate::parentCrate(void)
{
    return *(internal->parentCrate.pointer());
}

void Crate::removeFromParentCrate(void)
{
    this->parentCrate().removeCrate(*this);
}

const String::Array& Crate::crateFilePaths(void) const
{
    return internal->crateFilePaths;
}

void Crate::resetModificationFlags()
{
    internal->cratesWereModified = false;
    internal->tracksWereModified = false;
}

boolean Crate::childrenCratesWereModified(void) const
{
    return internal->cratesWereModified;
}

void Crate::saveIfModifiedAndRecurseToChildren(void) const
{
    if (internal->tracksWereModified) {
        for (auto cratePath : *internal->crateFilePaths) {
            auto outputData = Blob::blob();

            auto versionTag = VersionTag::tagWithIdentifierAndValue(crateVersionTagIdentifier,
                                                                    String::stringWith(crateFileCurrentVersionString));
            versionTag->addTo(outputData);

            for (auto& trackEntry : *internal->trackEntries) {
                trackEntry->tagForEntry().addTo(outputData);
            }

            for (auto& tag : *internal->otherTags) {
                tag->addTo(outputData);
            }

            File::writeBlobToFileAt(outputData, cratePath);
        }
    }

    for (auto& crate : *internal->childrenCrates) {
        crate->saveIfModifiedAndRecurseToChildren();
    }
}

TrackEntry::Array::Pointer Crate::removeAndReturnTrackEntries(void)
{
    auto result = TrackEntry::Array::arrayWith(this->trackEntries());

    for (auto& entry : *result) {
        entry->removeFromParentCrate();
    }

    internal->tracksWereModified = true;

    return result;
}

Crate::Array::Pointer Crate::removeAndReturnChildrenCrates(void)
{
    auto result = Crate::Array::arrayWith(this->crates());

    for (auto& crate : *result) {
        crate->removeFromParentCrate();
    }

    internal->markCratesAsModified();

    return result;
}
