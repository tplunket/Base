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
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR_CLASS(Tag);

    class Tag : public Object {
        NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_PURE_VIRTUAL_CLASS(NxA::Serato, Tag);

    public:
        #pragma mark Class Methods
        static uinteger32 identifierForTagAt(const byte* tagAddress);
        static const byte* nextTagAfterTagAt(const byte* tagAddress);

        #pragma mark Instance Methods
        uinteger32 identifier(void) const;

        virtual void addTo(Blob& destination) const = 0;
    };
} }
