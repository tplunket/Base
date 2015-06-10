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

#include "SeratoDB/TrackEntry.hpp"
#include "SeratoDB/PathTag.hpp"
#include "SeratoDB/CrateV1Tags.hpp"

using namespace NxA::Serato;
using namespace std;

#pragma mark Constructors

TrackEntry::TrackEntry(const char* trackPath, const char* locatedOnVolumePath) :
                                   p_rootVolumePath(make_unique<string>(locatedOnVolumePath))
{
    ConstStringPtr entryPath = removePrefixFromPath(locatedOnVolumePath, trackPath);

    TagVectorPtr tags(make_unique<TagVector>());
    tags->push_back(make_unique<PathTag>(NxASeratoTrackEntryPathTag, entryPath->c_str()));

    this->p_trackEntryTag = make_unique<ObjectTag>(NxASeratoTrackEntryTag, move(tags));
}

#pragma mark Instance Methods

bool TrackEntry::p_containsAValidTrackEntryTag(void) const
{
    return this->p_trackEntryTag.get() != NULL;
}

ConstStringPtr TrackEntry::trackFilePath(void) const
{
    if (this->p_containsAValidTrackEntryTag()) {
        const ObjectTag* trackObjectTag = dynamic_cast<const ObjectTag*>(this->p_trackEntryTag.get());
        if (trackObjectTag->hasSubTagForIdentifier(NxASeratoTrackEntryPathTag)) {
            const PathTag& pathTag = dynamic_cast<const PathTag&>(trackObjectTag->subTagForIdentifier(NxASeratoTrackEntryPathTag));
            const string& pathFromRootFolder = pathTag.value();
            ConstStringPtr trackFilePath = joinPaths(this->p_rootVolumePath->c_str(), pathFromRootFolder.c_str());
            return trackFilePath;
        }
    }

    return make_unique<string>("");
}

const Tag& TrackEntry::tagForEntry(void) const
{
    return *(this->p_trackEntryTag.get());
}

void TrackEntry::destroy(void)
{
    delete this;
}
