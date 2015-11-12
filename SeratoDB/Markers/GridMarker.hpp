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

#pragma once

#include <Base/Base.hpp>

namespace NxA { namespace Serato {
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR_CLASS(GridMarker);

    class GridMarker : public Object {
        NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_CLASS(NxA::Serato, GridMarker);

    public:
        #pragma mark Factory Methods
        static GridMarker::Pointer markerWithPositionAndBeatsPerMinute(decimal positionInSeconds,
                                                                       decimal beatsPerMinute);
        static GridMarker::Pointer markerWith(const GridMarker& other);

        #pragma mark Class Methods
        static GridMarker::Array::Pointer markersWithMemoryAt(const byte* id3TagStart);
        static void addMarkersTo(GridMarker::Array& markers, Blob& data);

        #pragma mark Operators
        bool operator==(const GridMarker& other) const;

        #pragma mark Instance Methods
        decimal positionInSeconds(void) const;
        decimal beatsPerMinute(void) const;
    };
} }
