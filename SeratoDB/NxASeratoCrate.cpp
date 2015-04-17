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

#include "SeratoDB/NxASeratoCrate.h"
#include "SeratoDB/NxASeratoCrateV1Tags.h"
#include "SeratoDB/NxASeratoTextTag.h"
#include "SeratoDB/NxASeratoTagFactory.h"
#include "SeratoDB/NxASeratoDatabase.h"

using namespace NxA;
using namespace std;

#pragma mark Constants

static const char* NxASeratoCrateFileCurrentVersion = "1.0/Serato ScratchLive Crate";

#pragma mark Constructors

SeratoCrate::SeratoCrate(const char* crateFullName,
                         const char* inSeratoFolderPath,
                         const char* locatedOnVolumePath,
                         SeratoDatabase& database) :
        p_crateFullName(ConstStringPtr(make_unique<string>(crateFullName))),
        p_rootVolumePath(ConstStringPtr(make_unique<string>(locatedOnVolumePath))),
        p_crateFilePath(crateFilePathForCrateNameInSeratoFolder(crateFullName, inSeratoFolderPath)),
        p_parentDatabase(database),
        p_parentCrate(NULL),
        p_tracksWereModified(true),
        p_cratesWereModified(false),
        p_trackEntries(make_unique<SeratoTrackEntryVector>()),
        p_childrenCrates(make_unique<SeratoCrateVector>())
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

bool SeratoCrate::isAValidCrateName(const char* crateFullName, const char* seratoFolderPath)
{
    ConstStringPtr crateFilePath = crateFilePathForCrateNameInSeratoFolder(crateFullName, seratoFolderPath);
    return fileExistsAt(crateFilePath->c_str());
}

bool SeratoCrate::isASmartCrateName(const char* crateFullName, const char* seratoFolderPath)
{
    ConstStringPtr crateFilePath = crateFilePathForSmartCrateNameInSeratoFolder(crateFullName, seratoFolderPath);
    return fileExistsAt(crateFilePath->c_str());
}

#pragma mark Instance Methods

void SeratoCrate::p_storeTrackTag(SeratoTagPtr tag)
{
    this->addTrackEntry(make_unique<SeratoTrackEntry>(move(tag), this->p_rootVolumePath->c_str()));
}

void SeratoCrate::p_storeOtherTag(SeratoTagPtr tag)
{
    this->p_otherTags.push_back(move(tag));
}

void SeratoCrate::p_markCratesAsModified()
{
    this->p_cratesWereModified = true;

    if (this->p_parentCrate) {
        this->p_parentCrate->p_markCratesAsModified();
    }
}

const std::string& SeratoCrate::crateName(void) const
{
    return *(this->p_crateName);
}

const std::string& SeratoCrate::crateFullName(void) const
{
    return *(this->p_crateFullName);
}

void SeratoCrate::addFullCrateNameWithPrefixAndRecurseToChildren(std::string& destination, const char* prefix) const
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

const SeratoTrackEntryVector& SeratoCrate::trackEntries(void) const
{
    return *(this->p_trackEntries.get());
}

const SeratoCrateVector& SeratoCrate::crates(void) const
{
    return *(this->p_childrenCrates.get());
}

void SeratoCrate::loadFromFile(void)
{
    CharVectorPtr crateFileData = readFileAt(this->p_crateFilePath->c_str());
    SeratoTagVectorPtr tags(SeratoTagFactory::parseTagsAt(crateFileData->data(), crateFileData->size()));
    if (!tags->size()) {
        return;
    }

    for (auto& tagPtr : *(tags)) {
        SeratoTag* tag = tagPtr.get();
        switch (tag->identifier()) {
            case NxASeratoCrateVersionTag: {
                const SeratoTextTag* versionTag = dynamic_cast<const SeratoTextTag*>(tag);
                if (versionTag->value() != NxASeratoCrateFileCurrentVersion) {
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

bool SeratoCrate::hasParentCrate(void) const
{
    return this->p_parentCrate != nullptr;
}

SeratoCrate& SeratoCrate::parentCrate(void) const
{
    return *(this->p_parentCrate);
}

void SeratoCrate::resetModificationFlags()
{
    this->p_cratesWereModified = false;
    this->p_tracksWereModified = false;
}

bool SeratoCrate::childrenCratesWereModified(void) const
{
    return this->p_cratesWereModified;
}

void SeratoCrate::saveIfModifiedAndRecurseToChildren(void) const
{
    if (this->p_rootVolumePath.get() && this->p_tracksWereModified) {
        CharVectorPtr outputData = make_unique<CharVector>();

        SeratoTagPtr versionTag(make_unique<SeratoTextTag>(NxASeratoCrateVersionTag, NxASeratoCrateFileCurrentVersion));
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

void SeratoCrate::addTrackEntry(SeratoTrackEntryPtr trackEntry)
{
    this->p_tracksWereModified = true;
    this->p_trackEntries->push_back(move(trackEntry));
}

void SeratoCrate::addChildCrate(SeratoCratePtr crate)
{
    crate->p_parentCrate = this;

    this->p_markCratesAsModified();

    this->p_childrenCrates->push_back(move(crate));
}

SeratoCratePtr SeratoCrate::removeAndReturnChildrenCrate(SeratoCrate& crate)
{
    crate.p_parentCrate = nullptr;

    uint32_t indexOfChildrenCrate = 0;
    SeratoCrateVector& childrenCrates = *(this->p_childrenCrates);
    for (auto& childrenCrate : childrenCrates) {
        if (childrenCrate.get() == &crate) {
            SeratoCratePtr crateToRemove(move(childrenCrate));

            this->p_markCratesAsModified();

            childrenCrates.erase(childrenCrates.begin() + indexOfChildrenCrate);

            return move(crateToRemove);
        }

        ++indexOfChildrenCrate;
    }

    return SeratoCratePtr(nullptr);
}

SeratoTrackEntryVectorPtr SeratoCrate::removeAndReturnTrackEntries(void)
{
    SeratoTrackEntryVectorPtr trackEntries = move(this->p_trackEntries);
    this->p_trackEntries = SeratoTrackEntryVectorPtr(make_unique<SeratoTrackEntryVector>());

    this->p_tracksWereModified = true;

    return trackEntries;
}

SeratoCrateVectorPtr SeratoCrate::removeAndReturnChildrenCrates(void)
{
    SeratoCrateVectorPtr childrenCrates = move(this->p_childrenCrates);
    this->p_childrenCrates = SeratoCrateVectorPtr(make_unique<SeratoCrateVector>());

    this->p_markCratesAsModified();

    for (auto& crate : *this->p_childrenCrates) {
        crate->p_parentCrate = NULL;
    }

    return childrenCrates;
}

void SeratoCrate::destroy(void)
{
    for (auto &childrenCrate : *(this->p_childrenCrates)) {
        childrenCrate->destroy();
    }

    this->p_parentDatabase.addCrateFileToDelete(*(this->p_crateFilePath));
    
    if (this->hasParentCrate()) {
        SeratoCrate &parentCrate = this->parentCrate();

        SeratoCratePtr self(parentCrate.removeAndReturnChildrenCrate(*this));
        delete self.release();
    }
    else {
        delete this;
    }
}
