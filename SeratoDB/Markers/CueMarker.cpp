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

#include "Markers/CueMarker.hpp"
#include "Markers/Internal/CueMarker.hpp"

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, CueMarker, Object);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Structures

typedef struct {
    byte tag[4];
    byte size[4];
} SeratoCueTagHeaderStruct;

typedef struct {
    byte tag[4];
    byte size[4];
    byte index[2];
    byte position[4];
    byte color[4];
    byte loop_enabled;
    byte loop_locked;
    byte label[0];
} SeratoCueTagStruct;

#pragma mark Factory Methods

CueMarker::Pointer CueMarker::markerWithMemoryAt(const byte* id3TagStart)
{
    auto tagStruct = reinterpret_cast<const SeratoCueTagStruct*>(id3TagStart);

    return CueMarker::markerWithLabelPositionAndIndex(String::stringWith(reinterpret_cast<const character*>(tagStruct->label)),
                                                      Platform::bigEndianUInteger32ValueAt(tagStruct->position),
                                                      Platform::bigEndianUInteger16ValueAt(tagStruct->index));
}

CueMarker::Pointer CueMarker::markerWithLabelPositionAndIndex(const String& label,
                                                              uinteger32 positionInMilliseconds,
                                                              uinteger16 index)
{
    auto newMarker = CueMarker::makeShared();
    newMarker->internal->positionInMilliseconds = positionInMilliseconds;
    newMarker->internal->index = index;
    newMarker->internal->label = label.pointer();

    return newMarker;
}

CueMarker::Pointer CueMarker::markerWith(const CueMarker&other)
{
    return CueMarker::markerWithLabelPositionAndIndex(other.label(), other.positionInMilliseconds(), other.index());
}

#pragma mark Operators

bool CueMarker::operator==(const CueMarker& other) const
{
    if (this == &other) {
        return true;
    }

    return (this->label() == other.label()) &&
           (this->positionInMilliseconds() == other.positionInMilliseconds()) &&
           (this->index() == other.index());
}

#pragma mark Instance Methods

uinteger32 CueMarker::positionInMilliseconds(void) const
{
    return internal->positionInMilliseconds;
}

uinteger16 CueMarker::index(void) const
{
    return internal->index;
}

const String& CueMarker::label(void) const
{
    return internal->label;
}

void CueMarker::addId3TagTo(Blob& data) const
{
    SeratoCueTagStruct header;

    memcpy(header.tag, "CUE", 4);
    count size = sizeof(SeratoCueTagStruct) + this->label().length() + 1 - sizeof(SeratoCueTagHeaderStruct);
    Platform::writeBigEndianUInteger32ValueAt(static_cast<uinteger32>(size), header.size);
    Platform::writeBigEndianUInteger16ValueAt(this->index(), header.index);
    Platform::writeBigEndianUInteger32ValueAt(this->positionInMilliseconds(), header.position);
    Platform::writeBigEndianUInteger32ValueAt(0, header.color);
    header.loop_enabled = 0;
    header.loop_locked = 0;

    auto headerData = Blob::blobWithMemoryAndSize(reinterpret_cast<const byte*>(&header), sizeof(SeratoCueTagStruct));
    data.append(headerData);
    data.append(this->label());
}
