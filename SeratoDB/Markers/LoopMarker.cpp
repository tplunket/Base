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

#include "Markers/LoopMarker.hpp"
#include "Markers/Internal/LoopMarker.hpp"

NXA_GENERATED_IMPLEMENTATION_FOR(NxA::Serato, LoopMarker);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Structures

typedef struct {
    unsigned char tag[5];
    unsigned char size[4];
} SeratoLoopTagHeaderStruct;

typedef struct {
    unsigned char tag[5];
    unsigned char size[4];
    unsigned char index[2];
    unsigned char position[4];
    unsigned char loopPosition[4];
    unsigned char loopIterations[4];
    unsigned char color[4];
    unsigned char loop_enabled;
    unsigned char loop_locked;
    unsigned char label[0];
} SeratoLoopTagStruct;

#pragma mark Constructors & Destructors

LoopMarker::LoopMarker(NxA::Internal::Object::Pointer const& initial_internal) :
                       Object(initial_internal),
                       internal(initial_internal) { }

#pragma mark Factory Methods

LoopMarker::Pointer LoopMarker::loopMarkerWith(const char* id3TagStart)
{
    const SeratoLoopTagStruct* tagStruct = (const SeratoLoopTagStruct* )id3TagStart;

    return LoopMarker::loopMarkerWith(String::stringWith(reinterpret_cast<const character*>(tagStruct->label)),
                                      Platform::bigEndianUInt32ValueAt(tagStruct->position),
                                      Platform::bigEndianUInt32ValueAt(tagStruct->loopPosition),
                                      Platform::bigEndianUInt16ValueAt(tagStruct->index));
}

LoopMarker::Pointer LoopMarker::loopMarkerWith(String::ConstPointer const& label,
                                               uinteger32 startPositionInMilliseconds,
                                               uinteger32 endPositionInMilliseconds,
                                               uinteger16 index)
{
    auto newMarker = LoopMarker::makeShared();
    newMarker->internal->startPositionInMilliseconds = startPositionInMilliseconds;
    newMarker->internal->endPositionInMilliseconds = endPositionInMilliseconds;
    newMarker->internal->index = index;
    newMarker->internal->label = label;

    return newMarker;
}

LoopMarker::Pointer LoopMarker::loopMarkerWith(LoopMarker::ConstPointer const& other)
{
    return LoopMarker::loopMarkerWith(other->label(),
                                      other->startPositionInMilliseconds(),
                                      other->endPositionInMilliseconds(),
                                      other->index());
}

#pragma mark Instance Methods

uinteger32 LoopMarker::startPositionInMilliseconds(void) const
{
    return internal->startPositionInMilliseconds;
}

uinteger32 LoopMarker::endPositionInMilliseconds(void) const
{
    return internal->endPositionInMilliseconds;
}

uinteger16 LoopMarker::index(void) const
{
    return internal->index;
}

String::ConstPointer const& LoopMarker::label(void) const
{
    return internal->label;
}

void LoopMarker::addId3TagTo(Blob::Pointer const& data) const
{
    SeratoLoopTagStruct header;

    memcpy(header.tag, "LOOP", 5);
    size_t size = sizeof(SeratoLoopTagStruct) + this->label()->length() + 1 - sizeof(SeratoLoopTagHeaderStruct);
    Platform::writeBigEndianUInt32ValueAt((uint32_t)size, &header.size);
    Platform::writeBigEndianUInt16ValueAt(this->index(), &header.index);
    Platform::writeBigEndianUInt32ValueAt(this->startPositionInMilliseconds(), &header.position);
    Platform::writeBigEndianUInt32ValueAt(this->endPositionInMilliseconds(), &header.loopPosition);
    Platform::writeBigEndianUInt32ValueAt(0, &header.loopIterations);
    Platform::writeBigEndianUInt32ValueAt(0, &header.color);
    header.loop_enabled = 0;
    header.loop_locked = 0;

    auto headerData = Blob::blobWithMemoryAndSize(reinterpret_cast<const character*>(&header), sizeof(SeratoLoopTagStruct));
    data->append(headerData);
    data->append(this->label()->toUTF8());
}
