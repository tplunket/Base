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

#include <SeratoDb/Tags/TextTag.hpp>

#include <Base/Base.hpp>

namespace NxA { namespace Serato {
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR(VersionTag);

    class VersionTag : public TextTag {
        NXA_GENERATED_DECLARATIONS_FOR(NxA::Serato, VersionTag);

    public:
        #pragma mark Factory Methods
        static VersionTag::Pointer tagWithMemoryAt(const byte* tagAddress);
        static VersionTag::Pointer tagWithIdentifierAndValue(uinteger32 identifier, const String& value);
    };
} }
