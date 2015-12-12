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

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, LoopMarker, Marker);

namespace NxA { namespace Serato {
    #pragma mark Structures
    typedef struct {
        byte tag[5];
        byte size[4];
    } SeratoLoopTagV2HeaderStruct;

    typedef struct {
        byte tag[5];
        byte size[4];
        byte index[2];
        byte position[4];
        byte loopPosition[4];
        byte loopIterations[4];
        byte color[4];
        byte loop_enabled;
        byte loop_locked;
        byte label[0];
    } SeratoLoopTagV2Struct;
} }

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Factory Methods

LoopMarker::Pointer LoopMarker::markerWithMemoryAt(const byte* id3TagStart)
{
    auto tagStruct = reinterpret_cast<const SeratoLoopTagV2Struct*>(id3TagStart);

    NXA_ASSERT_TRUE(*String::stringWith(reinterpret_cast<const character*>(tagStruct->tag)) == "LOOP");

    return LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(String::stringWith(reinterpret_cast<const character*>(tagStruct->label)),
                                                                Platform::bigEndianUInteger32ValueAt(tagStruct->position),
                                                                Platform::bigEndianUInteger32ValueAt(tagStruct->loopPosition),
                                                                Platform::bigEndianUInteger16ValueAt(tagStruct->index),
                                                                     tagStruct->color[1],
                                                                     tagStruct->color[2],
                                                                     tagStruct->color[3]);
}

LoopMarker::Pointer LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(const String& label,
                                                                              uinteger32 startPositionInMilliseconds,
                                                                              uinteger32 endPositionInMilliseconds,
                                                                              uinteger16 index,
                                                                              byte colorRedComponent,
                                                                              byte colorGreenComponent,
                                                                              byte colorBlueComponent)

{
    if (startPositionInMilliseconds >= endPositionInMilliseconds) {
        throw LoopMarkerError::exceptionWith("Invalid loop maker start/end positions (%d and %d ms).", startPositionInMilliseconds, endPositionInMilliseconds);
    }

    auto newMarker = LoopMarker::makeShared();
    newMarker->internal->startPositionInMilliseconds = startPositionInMilliseconds;
    newMarker->internal->endPositionInMilliseconds = endPositionInMilliseconds;
    newMarker->internal->index = index;
    newMarker->internal->label = label.pointer();
    newMarker->internal->colorRedComponent = colorRedComponent;
    newMarker->internal->colorGreenComponent = colorGreenComponent;
    newMarker->internal->colorBlueComponent = colorBlueComponent;

    return newMarker;
}

LoopMarker::Pointer LoopMarker::markerWith(const LoopMarker& other)
{
    return LoopMarker::markerWithLabelStartEndPositionsIndexAndColor(other.label(),
                                                                     other.startPositionInMilliseconds(),
                                                                     other.endPositionInMilliseconds(),
                                                                     other.index(),
                                                                     other.colorRedComponent(),
                                                                     other.colorGreenComponent(),
                                                                     other.colorBlueComponent());
}

#pragma mark Operators

bool LoopMarker::operator==(const LoopMarker& other) const
{
    if (this == &other) {
        return true;
    }

    return (this->label() == other.label()) &&
           (this->startPositionInMilliseconds() == other.startPositionInMilliseconds()) &&
           (this->endPositionInMilliseconds() == other.endPositionInMilliseconds()) &&
           (this->index() == other.index() &&
           (this->colorRedComponent() == other.colorRedComponent()) &&
           (this->colorGreenComponent() == other.colorGreenComponent()) &&
           (this->colorBlueComponent() == other.colorBlueComponent()));
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

const String& LoopMarker::label(void) const
{
    return internal->label;
}

byte LoopMarker::colorRedComponent(void) const
{
    return internal->colorRedComponent;
}

byte LoopMarker::colorGreenComponent(void) const
{
    return internal->colorGreenComponent;
}

byte LoopMarker::colorBlueComponent(void) const
{
    return internal->colorBlueComponent;
}

void LoopMarker::addMarkerV2TagTo(Blob& data) const
{
    SeratoLoopTagV2Struct header;

    memcpy(header.tag, "LOOP", 5);
    size_t size = sizeof(SeratoLoopTagV2Struct) + this->label().length() + 1 - sizeof(SeratoLoopTagV2HeaderStruct);
    Platform::writeBigEndianUInteger32ValueAt((uint32_t)size, header.size);
    Platform::writeBigEndianUInteger16ValueAt(this->index(), header.index);
    Platform::writeBigEndianUInteger32ValueAt(this->startPositionInMilliseconds(), header.position);
    Platform::writeBigEndianUInteger32ValueAt(this->endPositionInMilliseconds(), header.loopPosition);
    Platform::writeBigEndianUInteger32ValueAt(0xFFFFFFFF, header.loopIterations);
    header.color[0] = 0;
    header.color[1] = this->colorRedComponent();
    header.color[2] = this->colorGreenComponent();
    header.color[3] = this->colorBlueComponent();
    header.loop_enabled = 0;
    header.loop_locked = 0;

    auto headerData = Blob::blobWithMemoryAndSize(reinterpret_cast<const byte*>(&header), sizeof(SeratoLoopTagV2Struct));
    data.append(headerData);
    data.appendWithStringTermination(this->label().toUTF8());
}


void LoopMarker::addRawMarkerV1TagTo(Blob& data) const
{
    internal->addRawMarkerV1TagWithFieldsTo(Internal::Marker::eLoopMarker,
                                            this->startPositionInMilliseconds(),
                                            this->endPositionInMilliseconds(),
                                            0xFFFFFFFF,
                                            this->colorRedComponent(),
                                            this->colorGreenComponent(),
                                            this->colorBlueComponent(),
                                            data);
}

void LoopMarker::addEncodedMarkerV1TagTo(Blob& data) const
{
    internal->addEncodedMarkerV1TagWithFieldsTo(Internal::Marker::eLoopMarker,
                                                this->startPositionInMilliseconds(),
                                                this->endPositionInMilliseconds(),
                                                0xFFFFFFFF,
                                                this->colorRedComponent(),
                                                this->colorGreenComponent(),
                                                this->colorBlueComponent(),
                                                data);
}

void LoopMarker::addEmptyRawMarkerV1TagTo(Blob& data)
{
    // Note: serato saves empty V1 loops with the 'loop' type (3) rather than empty (0)
    Internal::Marker::addRawMarkerV1TagWithFieldsTo(Internal::Marker::eLoopMarker,
                                                    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                                                    0, 0, 0, data);
}

void LoopMarker::addEmptyEncodedMarkerV1TagTo(Blob& data)
{
    // Note: serato saves empty V1 loops with the 'loop' type (3) rather than empty (0)
    Internal::Marker::addEncodedMarkerV1TagWithFieldsTo(Internal::Marker::eLoopMarker,
                                                        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                                                        0, 0, 0, data);
}
