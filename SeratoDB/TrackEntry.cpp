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
#include "SeratoDB/Internal/TrackEntry.hpp"
#include "Tags/PathTag.hpp"
#include "Tags/ObjectTag.hpp"
#include "Tags/CrateV1Tags.hpp"

NXA_GENERATED_IMPLEMENTATION_FOR(NxA::Serato, TrackEntry);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Constructors & Destructors

TrackEntry::TrackEntry(NxA::Internal::Object::Pointer const& initial_internal) :
                       Object(initial_internal),
                       internal(initial_internal) { }

#pragma mark Factory Methods

TrackEntry::Pointer TrackEntry::entryWithTagOnVolume(NxA::ConstPointer<Tag> const& tag,
                                                     String::ConstPointer const& volumePath)
{
    auto internalObject = Internal::TrackEntry::Pointer(std::make_shared<Internal::TrackEntry>(tag, volumePath));
    auto newTrackEntry = TrackEntry::makeSharedWithInternal(internalObject);
    return newTrackEntry;
}

TrackEntry::Pointer TrackEntry::entryWithTrackFileAtOnVolume(String::ConstPointer const& path,
                                                             String::ConstPointer const& volumePath)
{
    auto entryPath = File::removePrefixFromPath(volumePath, path);

    auto tags = Tag::Array::array();
    tags->append(PathTag::tagWithIdentifierAndValue(trackEntryPathTagIdentifier, entryPath));

    auto trackEntryTag = ObjectTag::tagWithIdentifierAndValue(trackEntryTagIdentifier, tags);

    return TrackEntry::entryWithTagOnVolume(trackEntryTag, volumePath);
}

#pragma mark Instance Methods

String::ConstPointer TrackEntry::trackFilePath(void) const
{
    auto trackObjectTag = ObjectTag::ConstPointer(internal->trackEntryTag);
    if (trackObjectTag->hasSubTagForIdentifier(trackEntryPathTagIdentifier)) {
        auto pathTag = PathTag::ConstPointer(trackObjectTag->subTagForIdentifier(trackEntryPathTagIdentifier));
        auto& pathFromRootFolder = pathTag->value();

        auto trackFilePath = File::joinPaths(internal->rootVolumePath, pathFromRootFolder);
        return trackFilePath;
    }

    return String::string();
}

Tag::ConstPointer const& TrackEntry::tagForEntry(void) const
{
    return internal->trackEntryTag;
}

void TrackEntry::destroy(void)
{
    delete this;
}
