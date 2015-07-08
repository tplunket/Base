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

#include <SeratoDb/Tags/Tag.hpp>

#include <Base/Base.hpp>

namespace NxA { namespace Serato {
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR_CLASS(UInteger32Tag);

    class UInteger32Tag : public Tag {
        NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_CLASS(NxA::Serato, UInteger32Tag);

    public:
        #pragma mark Factory Methods
        static UInteger32Tag::Pointer tagWithMemoryAt(const byte* tagAddress);
        static UInteger32Tag::Pointer tagWithIdentifierAndValue(uinteger32 identifier, uinteger32 value);

        #pragma mark Operators
        bool operator==(const UInteger32Tag& other) const;

        #pragma mark Instance Methods
        uinteger32 value(void) const;
        void setValue(uinteger32 value);

        void addTo(Blob& destination) const;
    };
} }
