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
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR(LoopMarker);

    class LoopMarker : public NxA::Object {
        NXA_GENERATED_DECLARATIONS_FOR(NxA::Serato, LoopMarker);
        
    public:
        #pragma mark Factory Methods
        static LoopMarker::Pointer loopMarkerWith(const char* id3TagStart);
        static LoopMarker::Pointer loopMarkerWith(String::ConstPointer const& label,
                                                  uinteger32 startPositionInMilliseconds,
                                                  uinteger32 endPositionInMilliseconds,
                                                  uinteger16 index);
        static LoopMarker::Pointer loopMarkerWith(LoopMarker::ConstPointer const& other);

        #pragma mark Instance Methods
        uinteger32 startPositionInMilliseconds(void) const;
        uinteger32 endPositionInMilliseconds(void) const;
        uinteger16 index(void) const;
        String::ConstPointer const& label(void) const;

        void addId3TagTo(Blob::Pointer const& data) const;
    };
} }
