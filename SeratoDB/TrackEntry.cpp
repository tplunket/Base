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
#include "SeratoDB/Crate.hpp"
#include "Tags/PathTag.hpp"
#include "Tags/ObjectTag.hpp"
#include "Tags/CrateV1Tags.hpp"

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, TrackEntry, Object);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Factory Methods

TrackEntry::Pointer TrackEntry::entryWithTagOnVolume(const ObjectTag& tag, const String& volumePath)
{
    auto internalObject = Internal::TrackEntry::Pointer(std::make_shared<Internal::TrackEntry>(tag, volumePath));
    auto newTrackEntry = TrackEntry::makeSharedWithInternal(NxA::Internal::Object::Pointer::dynamicCastFrom(internalObject));
    return newTrackEntry;
}

TrackEntry::Pointer TrackEntry::entryWithTrackFileAtOnVolume(const String& path, const String& volumePath)
{
    auto entryPath = File::removePrefixFromPath(volumePath, path);

    auto tags = Tag::Array::array();
    tags->append(Serato::Tag::Pointer::dynamicCastFrom(PathTag::tagWithIdentifierAndValue(trackEntryPathTagIdentifier, entryPath)));

    auto trackEntryTag = ObjectTag::tagWithIdentifierAndValue(trackEntryTagIdentifier, tags);
    return TrackEntry::entryWithTagOnVolume(trackEntryTag, volumePath);
}

#pragma mark Instance Methods

String::Pointer TrackEntry::trackFilePath(void) const
{
    auto& trackObjectTag = dynamic_cast<const ObjectTag&>(*internal->trackEntryTag);
    if (trackObjectTag.hasSubTagForIdentifier(trackEntryPathTagIdentifier)) {
        auto& pathTag = dynamic_cast<const PathTag&>(trackObjectTag.subTagForIdentifier(trackEntryPathTagIdentifier));
        auto& pathFromRootFolder = pathTag.value();

        auto trackFilePath = File::joinPaths(internal->rootVolumePath, pathFromRootFolder);
        return trackFilePath;
    }

    return String::string();
}

boolean TrackEntry::hasParentCrate(void) const
{
    return internal->parentCrate.isValid();
}

NxA::Serato::Crate& TrackEntry::parentCrate(void)
{
    return *(internal->parentCrate.pointer());
}

void TrackEntry::setParentCrate(Crate& crate)
{
    NXA_ASSERT_FALSE(this->hasParentCrate());

    internal->parentCrate = Crate::WeakPointer(crate.pointer());

    crate.addTrackEntry(*this);
}

void TrackEntry::removeFromParentCrate(void)
{
    Crate::Pointer parentCrate = this->parentCrate().pointer();

    internal->parentCrate.release();

    parentCrate->removeTrackEntry(*this);
}

const Tag& TrackEntry::tagForEntry(void) const
{
    return internal->trackEntryTag;
}
