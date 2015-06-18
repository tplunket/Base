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
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR(GridMarker);

    class GridMarker : public NxA::Object {
        NXA_GENERATED_DECLARATIONS_FOR(NxA::Serato, GridMarker);

    public:
        #pragma mark Factory Methods
        static GridMarker::Pointer gridMarkerWith(float positionInSeconds,
                                                  float bpm);
        static GridMarker::Pointer gridMarkerWith(const byte* markerData);
        static GridMarker::Pointer gridMarkerWith(GridMarker::ConstPointer const& other);

        #pragma mark Class Methods
        static const byte* nextGridMarkerAfter(const byte* markerData);

        #pragma mark Instance Methods
        float positionInSeconds(void) const;
        float bpm(void) const;

        void addDataTo(Blob::Pointer const& data) const;
    };
} }
