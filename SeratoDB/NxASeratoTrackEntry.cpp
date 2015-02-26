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
#include "SeratoDB/NxASeratoCrateV1Tags.h"

using namespace NxA;
using namespace std;

#pragma mark Constructors

SeratoTrackEntry::SeratoTrackEntry(const char* trackPath, const char* locatedOnVolumePath) :
                                   p_rootVolumePath(new std::string(locatedOnVolumePath))
{
    ConstSeratoTagVectorAutoPtr tags(new ConstSeratoTagVector);

    StringAutoPtr entryPath = removePrefixFromPath(locatedOnVolumePath, trackPath);
    tags->push_back(SeratoTagAutoPtr(new SeratoTag(NxASeratoTrackEntryPathTag, entryPath->c_str())));

    this->p_trackTag = SeratoTagAutoPtr(new SeratoTag(NxASeratoTrackEntryTag, tags));
}

#pragma mark Instance Methods

bool SeratoTrackEntry::p_containsAValidTag(void) const
{
    return this->p_trackTag.get() != NULL;
}

StringAutoPtr SeratoTrackEntry::trackFilePath(void) const
{
    if (this->p_containsAValidTag()) {
        const SeratoTag* tag = this->p_trackTag->subTagWithIdentifierOrNilIfDoesNotExist(NxASeratoTrackEntryPathTag);
        if (tag != NULL) {
            StringAutoPtr pathFromRootFolder = tag->dataAsPath();
            StringAutoPtr trackFilePath = joinPaths(this->p_rootVolumePath->c_str(), pathFromRootFolder->c_str());
            return trackFilePath;
        }
    }

    return StringAutoPtr(new string(""));
}

const SeratoTag& SeratoTrackEntry::tagForEntry(void) const
{
    return *(this->p_trackTag.get());
}
