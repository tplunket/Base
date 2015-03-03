//
//  NxASeratoTrackEntry.cpp
//  SeratoDB
//
//  Created by Didier Malenfant on 2/5/15.
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

#include "SeratoDB/NxASeratoTrackEntry.h"
#include "SeratoDB/NxASeratoPathTag.h"
#include "SeratoDB/NxASeratoCrateV1Tags.h"

using namespace NxA;
using namespace std;

#pragma mark Constructors

SeratoTrackEntry::SeratoTrackEntry(const char* trackPath, const char* locatedOnVolumePath) :
                                   p_rootVolumePath(new std::string(locatedOnVolumePath))
{
    StringPtr entryPath = removePrefixFromPath(locatedOnVolumePath, trackPath);

    SeratoTagVectorPtr tags(new SeratoTagVector);
    tags->push_back(SeratoTagPtr(new SeratoPathTag(NxASeratoTrackEntryPathTag, entryPath->c_str())));

    this->p_trackEntryTag = SeratoTagPtr(new SeratoObjectTag(NxASeratoTrackEntryTag, tags));
}

#pragma mark Instance Methods

bool SeratoTrackEntry::p_containsAValidTrackEntryTag(void) const
{
    return this->p_trackEntryTag.get() != NULL;
}

StringPtr SeratoTrackEntry::trackFilePath(void) const
{
    if (this->p_containsAValidTrackEntryTag()) {
        const SeratoObjectTag* trackObjectTag = dynamic_cast<const SeratoObjectTag*>(this->p_trackEntryTag.get());
        if (trackObjectTag->hasSubTagForIdentifier(NxASeratoTrackEntryPathTag)) {
            const SeratoPathTag& pathTag = dynamic_cast<const SeratoPathTag&>(trackObjectTag->subTagForIdentifier(NxASeratoTrackEntryPathTag));
            const string& pathFromRootFolder = pathTag.value();
            StringPtr trackFilePath = joinPaths(this->p_rootVolumePath->c_str(), pathFromRootFolder.c_str());
            return trackFilePath;
        }
    }

    return StringPtr(new string(""));
}

const SeratoTag& SeratoTrackEntry::tagForEntry(void) const
{
    return *(this->p_trackEntryTag.get());
}
