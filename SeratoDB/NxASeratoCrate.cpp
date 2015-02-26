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

using namespace NxA;
using namespace std;

#pragma mark Constructors

SeratoCrate::SeratoCrate(const char* crateFullName,
                         const char* inSeratoFolderPath,
                         const char* locatedOnVolumePath) :
        p_crateFullName(StringAutoPtr(new string(crateFullName))),
        p_rootVolumePath(StringAutoPtr(new string(locatedOnVolumePath))),
        p_crateFilePath(crateFilePathForCrateNameInSeratoFolder(crateFullName, inSeratoFolderPath)),
        p_parentCrate(NULL),
        p_tracksWereModified(false),
        p_cratesWereModified(false),
        p_trackEntries(SeratoTrackEntryVectorAutoPtr(new SeratoTrackEntryVector)),
        p_childrenCrates(SeratoCrateVectorAutoPtr(new SeratoCrateVector))
{
    string crateName(crateFullName);
    size_t lastSeperator = crateName.rfind("%%");
    if (lastSeperator != string::npos) {
        this->p_crateName = StringAutoPtr(new string(crateName.substr(lastSeperator + 2)));
    }
    else {
        this->p_crateName = StringAutoPtr(new string(crateFullName));
    }
}

#pragma mark Class Methods

bool SeratoCrate::isAValidCrateName(const char* crateFullName, const char* seratoFolderPath)
{
    StringAutoPtr crateFilePath = crateFilePathForCrateNameInSeratoFolder(crateFullName, seratoFolderPath);
    return fileExistsAt(crateFilePath->c_str());
}

#pragma mark Instance Methods

void SeratoCrate::p_storeVersionTag(const SeratoTag* tag)
{
    if (this->p_versionTag.get()) {
        return;
    }

    this->p_versionTag = ConstSeratoTagAutoPtr(tag);
}

void SeratoCrate::p_storeTrackTag(const SeratoTag* tag)
{
    SeratoTrackEntry* newTrack = new SeratoTrackEntry(tag, this->p_rootVolumePath->c_str());
    this->addTrackEntry(SeratoTrackEntryAutoPtr(newTrack));
}

void SeratoCrate::p_storeOtherTag(const SeratoTag* tag)
{
    this->p_otherTags.push_back(ConstSeratoTagAutoPtr(tag));
}

void SeratoCrate::p_markCratesAsModified()
{
    this->p_cratesWereModified = true;

    if (this->p_parentCrate) {
        this->p_parentCrate->markCratesAsModified();
    }
}

StringAutoPtr SeratoCrate::versionAsString(void) const
{
    if (this->p_versionTag.get()) {
        return this->p_versionTag->dataAsString();
    }

    return StringAutoPtr();
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
    CharVectorAutoPtr crateFileData = readFileAt(this->p_crateFilePath->c_str());

    ConstSeratoTagVectorAutoPtr tags(SeratoTag::parseTagsIn(crateFileData));
    for(ConstSeratoTagVector::iterator it = tags->begin(); it != tags->end(); ++it) {
        const SeratoTag* tag = it->release();

        switch (tag->identifier()) {
            case NxASeratoCrateVersionTag: {
                this->p_storeVersionTag(tag);
                break;
            }
            case NxASeratoTrackEntryTag: {
                this->p_storeTrackTag(tag);
                break;
            }
            default: {
                this->p_storeOtherTag(tag);
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
        CharVectorAutoPtr outputData = CharVectorAutoPtr(new CharVector);

        SeratoTagAutoPtr versionTag(new SeratoTag(NxASeratoCrateVersionTag, "1.0/Serato ScratchLive Crate"));
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

void SeratoCrate::addTrackEntry(SeratoTrackEntryAutoPtr trackEntry)
{
    this->p_tracksWereModified = true;
    this->p_trackEntries->push_back(SeratoTrackEntryAutoPtr(trackEntry));
}

void SeratoCrate::addChildCrate(SeratoCrateAutoPtr crate)
{
    crate->p_parentCrate = this;

    this->p_markCratesAsModified();

    this->p_childrenCrates->push_back(SeratoCrateAutoPtr(crate));
}

SeratoTrackEntryVectorAutoPtr SeratoCrate::removeAndReturnTrackEntries(void)
{
    SeratoTrackEntryVectorAutoPtr trackEntries = this->p_trackEntries;
    this->p_trackEntries = SeratoTrackEntryVectorAutoPtr(new SeratoTrackEntryVector);

    this->p_tracksWereModified = true;

    return trackEntries;
}

SeratoCrateVectorAutoPtr SeratoCrate::removeAndReturnChildrenCrates(void)
{
    SeratoCrateVectorAutoPtr childrenCrates = this->p_childrenCrates;
    this->p_childrenCrates = SeratoCrateVectorAutoPtr(new SeratoCrateVector);

    this->p_markCratesAsModified();

    for (SeratoCrateVector::iterator it = childrenCrates->begin(); it != childrenCrates->end(); ++it) {
        SeratoCrate* crate = it->get();

        crate->p_parentCrate = NULL;
    }

    return childrenCrates;
}
