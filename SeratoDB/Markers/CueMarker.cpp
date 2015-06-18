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

NXA_GENERATED_IMPLEMENTATION_FOR(NxA::Serato, CueMarker);

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

#pragma mark Constructors & Destructors

CueMarker::CueMarker(NxA::Internal::Object::Pointer const& initial_internal) :
                     Object(initial_internal),
                     internal(initial_internal) { }

#pragma mark Factory Methods

CueMarker::Pointer CueMarker::cueMarkerWith(const byte* id3TagStart)
{
    auto tagStruct = reinterpret_cast<const SeratoCueTagStruct*>(id3TagStart);

    return CueMarker::cueMarkerWith(String::stringWith(reinterpret_cast<const character*>(tagStruct->label)),
                                    Platform::bigEndianUInt32ValueAt(tagStruct->position),
                                    Platform::bigEndianUInt16ValueAt(tagStruct->index));
}

CueMarker::Pointer CueMarker::cueMarkerWith(String::ConstPointer const& label,
                                            uinteger32 positionInMilliseconds,
                                            uinteger16 index)
{
    auto newMarker = CueMarker::makeShared();
    newMarker->internal->positionInMilliseconds = positionInMilliseconds;
    newMarker->internal->index = index;
    newMarker->internal->label = label;

    return newMarker;
}

CueMarker::Pointer CueMarker::cueMarkerWith(CueMarker::ConstPointer const& other)
{
    return CueMarker::cueMarkerWith(other->label(), other->positionInMilliseconds(), other->index());
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

const String::ConstPointer& CueMarker::label(void) const
{
    return internal->label;
}

void CueMarker::addId3TagTo(Blob::Pointer const& data) const
{
    SeratoCueTagStruct header;

    memcpy(header.tag, "CUE", 4);
    size_t size = sizeof(SeratoCueTagStruct) + this->label()->length() + 1 - sizeof(SeratoCueTagHeaderStruct);
    Platform::writeBigEndianUInt32ValueAt((uint32_t)size, &header.size);
    Platform::writeBigEndianUInt16ValueAt(this->index(), &header.index);
    Platform::writeBigEndianUInt32ValueAt(this->positionInMilliseconds(), &header.position);
    Platform::writeBigEndianUInt32ValueAt(0, &header.color);
    header.loop_enabled = 0;
    header.loop_locked = 0;

    auto headerData = Blob::blobWithMemoryAndSize(reinterpret_cast<const character*>(&header), sizeof(SeratoCueTagStruct));
    data->append(headerData);
    data->append(this->label()->toUTF8());
}
