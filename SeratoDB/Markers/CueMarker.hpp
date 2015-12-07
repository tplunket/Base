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
#include "Markers/Marker.hpp"

namespace NxA { namespace Serato {
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR_CLASS(CueMarker);

    class CueMarker : public Marker {
        NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_CLASS(NxA::Serato, CueMarker);

    public:
        #pragma mark Factory Methods
        static CueMarker::Pointer markerWithMemoryAt(const byte* id3TagStart);
        static CueMarker::Pointer markerWithLabelPositionIndexAndColor(const String& label,
                                                                       uinteger32 positionInMilliseconds,
                                                                       uinteger16 index,
                                                                       byte colorRedComponent,
                                                                       byte colorGreenComponent,
                                                                       byte colorBlueComponent);
        static CueMarker::Pointer markerWith(const CueMarker&other);

        #pragma mark Operators
        bool operator==(const CueMarker& other) const;

        #pragma mark Instance Methods
        uinteger32 positionInMilliseconds(void) const;
        uinteger16 index(void) const;
        const String& label(void) const;
        byte colorRedComponent(void) const;
        byte colorGreenComponent(void) const;
        byte colorBlueComponent(void) const;

        void addMarkerV1TagTo(Blob& data) const;
        void addMarkerV2TagTo(Blob& data) const;
        
        static void addEmptyMarkerV1TagTo(Blob& data);
    };
} }
