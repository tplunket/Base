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

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, TrackEntry, Object);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Factory Methods

TrackEntry::Pointer TrackEntry::entryWithTagOnVolume(NxA::Pointer<Tag> const& tag,
                                                     String::Pointer const& volumePath)
{
    auto internalObject = Internal::TrackEntry::Pointer(std::make_shared<Internal::TrackEntry>(tag, volumePath));
    auto newTrackEntry = TrackEntry::makeSharedWithInternal(NxA::Internal::Object::Pointer::dynamicCastFrom(internalObject));
    return newTrackEntry;
}

TrackEntry::Pointer TrackEntry::entryWithTrackFileAtOnVolume(String::Pointer const& path,
                                                             String::Pointer const& volumePath)
{
    auto entryPath = File::removePrefixFromPath(volumePath, path);

    auto tags = Tag::Array::array();
    tags->append(Serato::Tag::Pointer::dynamicCastFrom(PathTag::tagWithIdentifierAndValue(trackEntryPathTagIdentifier, entryPath)));

    auto trackEntryTag = ObjectTag::tagWithIdentifierAndValue(trackEntryTagIdentifier, tags);

    return TrackEntry::entryWithTagOnVolume(Serato::Tag::Pointer::dynamicCastFrom(trackEntryTag), volumePath);
}

#pragma mark Instance Methods

String::Pointer TrackEntry::trackFilePath(void) const
{
    auto trackObjectTag = ObjectTag::Pointer::dynamicCastFrom(internal->trackEntryTag);
    if (trackObjectTag->hasSubTagForIdentifier(trackEntryPathTagIdentifier)) {
        auto pathTag = PathTag::Pointer::dynamicCastFrom(trackObjectTag->subTagForIdentifier(trackEntryPathTagIdentifier));
        auto& pathFromRootFolder = pathTag->value();

        auto trackFilePath = File::joinPaths(internal->rootVolumePath, pathFromRootFolder);
        return trackFilePath;
    }

    return String::string();
}

Tag::Pointer const& TrackEntry::tagForEntry(void) const
{
    return internal->trackEntryTag;
}

void TrackEntry::destroy(void)
{
    delete this;
}
