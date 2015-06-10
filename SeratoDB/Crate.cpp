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
#include "SeratoDB/CrateV1Tags.hpp"
#include "SeratoDB/TextTag.hpp"
#include "SeratoDB/TagFactory.hpp"
#include "SeratoDB/Database.hpp"

using namespace NxA::Serato;
using namespace std;

#pragma mark Constants

static const char* crateFileCurrentVersionString = "1.0/Serato ScratchLive Crate";

#pragma mark Constructors

Crate::Crate(const char* crateFullName,
             const char* inSeratoFolderPath,
             const char* locatedOnVolumePath,
             Database& database) :
        p_crateFullName(ConstStringPtr(make_unique<string>(crateFullName))),
        p_rootVolumePath(ConstStringPtr(make_unique<string>(locatedOnVolumePath))),
        p_crateFilePath(crateFilePathForCrateNameInSeratoFolder(crateFullName, inSeratoFolderPath)),
        p_parentDatabase(database),
        p_parentCrate(NULL),
        p_tracksWereModified(true),
        p_cratesWereModified(false),
        p_trackEntries(make_unique<TrackEntryVector>()),
        p_childrenCrates(make_unique<CrateVector>())
{
    string crateName(crateFullName);
    size_t lastSeperator = crateName.rfind("%%");
    if (lastSeperator != string::npos) {
        this->p_crateName = make_unique<string>(crateName.substr(lastSeperator + 2));
    }
    else {
        this->p_crateName = make_unique<string>(crateFullName);
    }
}

#pragma mark Class Methods

bool Crate::isAValidCrateName(const char* crateFullName, const char* seratoFolderPath)
{
    ConstStringPtr crateFilePath = crateFilePathForCrateNameInSeratoFolder(crateFullName, seratoFolderPath);
    return fileExistsAt(crateFilePath->c_str());
}

bool Crate::isASmartCrateName(const char* crateFullName, const char* seratoFolderPath)
{
    ConstStringPtr crateFilePath = crateFilePathForSmartCrateNameInSeratoFolder(crateFullName, seratoFolderPath);
    return fileExistsAt(crateFilePath->c_str());
}

#pragma mark Instance Methods

void Crate::p_storeTrackTag(TagPtr tag)
{
    this->addTrackEntry(make_unique<TrackEntry>(move(tag), this->p_rootVolumePath->c_str()));
}

void Crate::p_storeOtherTag(TagPtr tag)
{
    this->p_otherTags.push_back(move(tag));
}

void Crate::p_markCratesAsModified()
{
    this->p_cratesWereModified = true;

    if (this->p_parentCrate) {
        this->p_parentCrate->p_markCratesAsModified();
    }
}

const std::string& Crate::crateName(void) const
{
    return *(this->p_crateName);
}

const std::string& Crate::crateFullName(void) const
{
    return *(this->p_crateFullName);
}

void Crate::addFullCrateNameWithPrefixAndRecurseToChildren(std::string& destination, const char* prefix) const
{
    const string& fullName = *(this->p_crateFullName.get());
    if (fullName.length()) {
        destination += prefix;
        destination += fullName;
        destination += '\n';
    }

    for (auto& crate : *this->p_childrenCrates) {
        crate->addFullCrateNameWithPrefixAndRecurseToChildren(destination, prefix);
    }
}

const TrackEntryVector& Crate::trackEntries(void) const
{
    return *(this->p_trackEntries.get());
}

const CrateVector& Crate::crates(void) const
{
    return *(this->p_childrenCrates.get());
}

void Crate::loadFromFile(void)
{
    CharVectorPtr crateFileData = readFileAt(this->p_crateFilePath->c_str());
    TagVectorPtr tags(TagFactory::parseTagsAt(crateFileData->data(), crateFileData->size()));
    if (!tags->size()) {
        return;
    }

    for (auto& tagPtr : *(tags)) {
        Tag* tag = tagPtr.get();
        switch (tag->identifier()) {
            case NxASeratoCrateVersionTag: {
                const TextTag* versionTag = dynamic_cast<const TextTag*>(tag);
                if (versionTag->value() != crateFileCurrentVersionString) {
                    this->p_otherTags.clear();
                    this->p_trackEntries->clear();
                    return;
                }
                break;
            }
            case NxASeratoTrackEntryTag: {
                this->p_storeTrackTag(move(tagPtr));
                break;
            }
            default: {
                this->p_storeOtherTag(move(tagPtr));
                break;
            }
        }
    }
}

bool Crate::hasParentCrate(void) const
{
    return this->p_parentCrate != nullptr;
}

Crate& Crate::parentCrate(void) const
{
    return *(this->p_parentCrate);
}

void Crate::resetModificationFlags()
{
    this->p_cratesWereModified = false;
    this->p_tracksWereModified = false;
}

bool Crate::childrenCratesWereModified(void) const
{
    return this->p_cratesWereModified;
}

void Crate::saveIfModifiedAndRecurseToChildren(void) const
{
    if (this->p_rootVolumePath.get() && this->p_tracksWereModified) {
        CharVectorPtr outputData = make_unique<CharVector>();

        TagPtr versionTag(make_unique<TextTag>(NxASeratoCrateVersionTag, crateFileCurrentVersionString));
        versionTag->addTo(*outputData);

        for (auto& trackEntry : *this->p_trackEntries) {
            trackEntry->tagForEntry().addTo(*outputData);
        }

        for (auto& tag : this->p_otherTags) {
            tag->addTo(*outputData);
        }

        writeToFile(this->p_crateFilePath->c_str(), *outputData);
    }

    for (auto& crate : *this->p_childrenCrates) {
        crate->saveIfModifiedAndRecurseToChildren();
    }
}

void Crate::addTrackEntry(TrackEntryPtr trackEntry)
{
    this->p_tracksWereModified = true;
    this->p_trackEntries->push_back(move(trackEntry));
}

void Crate::addChildCrate(CratePtr crate)
{
    crate->p_parentCrate = this;

    this->p_markCratesAsModified();

    this->p_childrenCrates->push_back(move(crate));
}

CratePtr Crate::removeAndReturnChildrenCrate(Crate& crate)
{
    crate.p_parentCrate = nullptr;

    uint32_t indexOfChildrenCrate = 0;
    CrateVector& childrenCrates = *(this->p_childrenCrates);
    for (auto& childrenCrate : childrenCrates) {
        if (childrenCrate.get() == &crate) {
            CratePtr crateToRemove(move(childrenCrate));

            this->p_markCratesAsModified();

            childrenCrates.erase(childrenCrates.begin() + indexOfChildrenCrate);

            return move(crateToRemove);
        }

        ++indexOfChildrenCrate;
    }

    return CratePtr(nullptr);
}

TrackEntryVectorPtr Crate::removeAndReturnTrackEntries(void)
{
    TrackEntryVectorPtr trackEntries = move(this->p_trackEntries);
    this->p_trackEntries = TrackEntryVectorPtr(make_unique<TrackEntryVector>());

    this->p_tracksWereModified = true;

    return trackEntries;
}

CrateVectorPtr Crate::removeAndReturnChildrenCrates(void)
{
    CrateVectorPtr childrenCrates = move(this->p_childrenCrates);
    this->p_childrenCrates = CrateVectorPtr(make_unique<CrateVector>());

    this->p_markCratesAsModified();

    for (auto& crate : *this->p_childrenCrates) {
        crate->p_parentCrate = NULL;
    }

    return childrenCrates;
}

void Crate::destroy(void)
{
    for (auto &childrenCrate : *(this->p_childrenCrates)) {
        childrenCrate->destroy();
    }

    this->p_parentDatabase.addCrateFileToDelete(*(this->p_crateFilePath));
    
    if (this->hasParentCrate()) {
        Crate &parentCrate = this->parentCrate();

        CratePtr self(parentCrate.removeAndReturnChildrenCrate(*this));
        delete self.release();
    }
    else {
        delete this;
    }
}
