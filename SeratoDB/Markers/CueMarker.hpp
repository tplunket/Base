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
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR(CueMarker);

    class CueMarker : public NxA::Object {
        NXA_GENERATED_DECLARATIONS_FOR(NxA::Serato, CueMarker);

    private:
        uinteger32 p_positionInMilliSeconds;
        uinteger16 p_index;
        String::ConstPointer p_label;

    public:
        #pragma mark Factory Methods
        static CueMarker::Pointer cueMarkerWith(const char* id3TagStart);
        static CueMarker::Pointer cueMarkerWith(String::ConstPointer const& label,
                                                uinteger32 positionInMilliseconds,
                                                uinteger16 index);
        static CueMarker::Pointer cueMarkerWith(CueMarker::ConstPointer const& other);

        #pragma mark Instance Methods
        uinteger32 positionInMilliseconds(void) const;
        uinteger16 index(void) const;
        const String::ConstPointer& label(void) const;

        void addId3TagTo(Blob::Pointer const& data) const;
    };
} }
