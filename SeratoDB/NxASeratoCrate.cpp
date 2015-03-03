//
//  NxASeratoCrate.cpp
//  SeratoDB
//
//  Created by Didier Malenfant on 2/4/15.
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

using namespace NxA;
using namespace std;

#pragma mark Constants

static const char* NxASeratoCrateFileCurrentVersion = "1.0/Serato ScratchLive Crate";

#pragma mark Constructors

SeratoCrate::SeratoCrate(const char* crateFullName,
                         const char* inSeratoFolderPath,
                         const char* locatedOnVolumePath) :
        p_crateFullName(ConstStringPtr(make_unique<string>(crateFullName))),
        p_rootVolumePath(ConstStringPtr(make_unique<string>(locatedOnVolumePath))),
        p_crateFilePath(crateFilePathForCrateNameInSeratoFolder(crateFullName, inSeratoFolderPath)),
        p_parentCrate(NULL),
        p_tracksWereModified(false),
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

    for (SeratoCrateVector::iterator it = this->p_childrenCrates->begin(); it != this->p_childrenCrates->end(); ++it) {
        const SeratoCrate* crate = it->get();
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
    for(SeratoTagVector::iterator it = tags->begin(); it != tags->end(); ++it) {
        SeratoTag* tag = it->get();

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
                this->p_storeTrackTag(move(*it));
                break;
            }
            default: {
                this->p_storeOtherTag(move(*it));
                break;
            }
        }
    }
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
    if (this->p_rootVolumePath.get() &&
        this->p_tracksWereModified) {
        CharVectorPtr outputData = make_unique<CharVector>();

        SeratoTagPtr versionTag(make_unique<SeratoTextTag>(NxASeratoCrateVersionTag, NxASeratoCrateFileCurrentVersion));
        versionTag->addTo(*outputData);

        for (SeratoTrackEntryVector::const_iterator it = this->p_trackEntries->begin(); it != this->p_trackEntries->end(); ++it) {
            it->get()->tagForEntry().addTo(*(outputData.get()));
        }

        for (ConstSeratoTagVector::const_iterator it = this->p_otherTags.begin(); it != this->p_otherTags.end(); ++it) {
            it->get()->addTo(*outputData);
        }

        writeToFile(this->p_crateFilePath->c_str(), *outputData);
    }

    for (SeratoCrateVector::iterator it = this->p_childrenCrates->begin(); it != this->p_childrenCrates->end(); ++it) {
        const SeratoCrate* crate = it->get();
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

    for (SeratoCrateVector::iterator it = childrenCrates->begin(); it != childrenCrates->end(); ++it) {
        SeratoCrate* crate = it->get();

        crate->p_parentCrate = NULL;
    }

    return childrenCrates;
}
