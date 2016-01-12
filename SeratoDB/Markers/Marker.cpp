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

#include "Markers/Marker.hpp"
#include "Markers/Internal/Marker.hpp"

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, Marker, Object);

using namespace NxA;
using namespace NxA::Serato;

#pragma mark Class Methods

bool Marker::isValidV1RawMarker(const byte* tagStart)
{
    auto tagStruct = reinterpret_cast<const Internal::Marker::SeratoRawTagV1Struct*>(tagStart);
    return (Platform::bigEndianUInteger32ValueAt(tagStruct->position) != 0xffffffff);
}

bool Marker::isValidV1EncodedMarker(const byte* tagStart)
{
    auto encodedStruct = reinterpret_cast<const Internal::Marker::SeratoEncodedTagV1Struct*>(tagStart);

    Internal::Marker::SeratoRawTagV1Struct rawStruct;
    Internal::Marker::rawV1TagFromEncodedV1TagStruct(rawStruct, *encodedStruct);

    return (Platform::bigEndianUInteger32ValueAt(rawStruct.position) != 0xffffffff);
}

integer32 Marker::sizeOfV1RawMarker(void)
{
    return sizeof(Internal::Marker::SeratoRawTagV1Struct);
}

integer32 Marker::sizeOfV1EncodedMarker(void)
{
    return sizeof(Internal::Marker::SeratoEncodedTagV1Struct);
}

