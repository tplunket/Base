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
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR(TextTag);

    class TextTag : public Tag, NXA_INHERITANCE_FOR(TextTag) {
        NXA_GENERATED_DECLARATIONS_FOR(NxA::Serato, TextTag);

    public:
        #pragma mark Factory Methods
        static TextTag::Pointer tagWithMemoryAt(const byte* tagAddress);
        static TextTag::Pointer tagWithIdentifierAndValue(uinteger32 identifier, const String& value);

        #pragma mark Instance Methods
        const String& value(void) const;
        void setValue(const String& value);

        void addTo(Blob::Pointer& destination) const;
    };
} }
