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
    unsigned char tag[4];
    unsigned char size[4];
} SeratoCueTagHeaderStruct;

typedef struct {
    unsigned char tag[4];
    unsigned char size[4];
    unsigned char index[2];
    unsigned char position[4];
    unsigned char color[4];
    unsigned char loop_enabled;
    unsigned char loop_locked;
    unsigned char label[0];
} SeratoCueTagStruct;

#pragma mark Constructors & Destructors

CueMarker::CueMarker(NxA::Internal::Object::Pointer const& initial_internal) :
                     Object(initial_internal),
                     internal(initial_internal),
                     p_label(String::string()) { }

#pragma mark Factory Methods

CueMarker::Pointer CueMarker::cueMarkerWith(const char* id3TagStart)
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
    newMarker->p_positionInMilliSeconds = positionInMilliseconds;
    newMarker->p_index = index;
    newMarker->p_label = label;

    return newMarker;
}

CueMarker::Pointer CueMarker::cueMarkerWith(CueMarker::ConstPointer const& other)
{
    return CueMarker::cueMarkerWith(other->label(), other->positionInMilliseconds(), other->index());
}

#pragma mark Instance Methods

uint32_t CueMarker::positionInMilliseconds(void) const
{
    return this->p_positionInMilliSeconds;
}

uint16_t CueMarker::index(void) const
{
    return this->p_index;
}

const String::ConstPointer& CueMarker::label(void) const
{
    return this->p_label;
}

void CueMarker::addId3TagTo(Blob::Pointer const& data) const
{
    SeratoCueTagStruct header;

    memcpy(header.tag, "CUE", 4);
    size_t size = sizeof(SeratoCueTagStruct) + this->p_label->length() + 1 - sizeof(SeratoCueTagHeaderStruct);
    Platform::writeBigEndianUInt32ValueAt((uint32_t)size, &header.size);
    Platform::writeBigEndianUInt16ValueAt(this->p_index, &header.index);
    Platform::writeBigEndianUInt32ValueAt(this->p_positionInMilliSeconds, &header.position);
    Platform::writeBigEndianUInt32ValueAt(0, &header.color);
    header.loop_enabled = 0;
    header.loop_locked = 0;

    auto headerData = Blob::blobWithCharPointer(reinterpret_cast<const character*>(&header), sizeof(SeratoCueTagStruct));
    data->append(headerData);
    data->append(this->p_label->toUTF8());
}
