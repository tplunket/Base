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

#include "TrackEntry.hpp"
#include "Tags/PathTag.hpp"
#include "Tags/CrateV1Tags.hpp"

#include <Base/File.hpp>

using namespace NxA;
using namespace NxA::Serato;
using namespace std;

#pragma mark Constructors

TrackEntry::TrackEntry(const char* trackPath, const char* locatedOnVolumePath) :
                                   p_rootVolumePath(make_unique<string>(locatedOnVolumePath))
{
    auto entryPath = File::removePrefixFromPath(String::stringWith(locatedOnVolumePath),
                                                String::stringWith(trackPath));

    TagVectorPtr tags(make_unique<TagVector>());
    tags->push_back(make_unique<PathTag>(NxASeratoTrackEntryPathTag, entryPath->toUTF8()));

    this->p_trackEntryTag = make_unique<ObjectTag>(NxASeratoTrackEntryTag, move(tags));
}

#pragma mark Instance Methods

bool TrackEntry::p_containsAValidTrackEntryTag(void) const
{
    return this->p_trackEntryTag.get() != NULL;
}

String::ConstPointer TrackEntry::trackFilePath(void) const
{
    if (this->p_containsAValidTrackEntryTag()) {
        const ObjectTag* trackObjectTag = dynamic_cast<const ObjectTag*>(this->p_trackEntryTag.get());
        if (trackObjectTag->hasSubTagForIdentifier(NxASeratoTrackEntryPathTag)) {
            const PathTag& pathTag = dynamic_cast<const PathTag&>(trackObjectTag->subTagForIdentifier(NxASeratoTrackEntryPathTag));
            auto& pathFromRootFolder = pathTag.value();
            auto trackFilePath = File::joinPaths(String::stringWith(this->p_rootVolumePath->c_str()),
                                                 String::stringWith(pathFromRootFolder->toUTF8()));
            return trackFilePath;
        }
    }

    return String::string();
}

const Tag& TrackEntry::tagForEntry(void) const
{
    return *(this->p_trackEntryTag.get());
}

void TrackEntry::destroy(void)
{
    delete this;
}
