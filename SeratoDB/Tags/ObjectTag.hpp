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
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR_CLASS(ObjectTag);

    class ObjectTag : public Tag {
        NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_CLASS(NxA::Serato, ObjectTag);
        
    public:
        #pragma mark Factory Methods
        static ObjectTag::Pointer tagWithMemoryAt(const byte* tagAddress);
        static ObjectTag::Pointer tagWithIdentifierAndValue(uinteger32 identifier, const Tag::Array& content);

        #pragma mark Operators
        bool operator==(const ObjectTag& other) const;

        #pragma mark Instance Methods
        boolean hasSubTagForIdentifier(uinteger32 identifier) const;

        const Tag& subTagForIdentifier(uinteger32 identifier) const;
        Tag& subTagForIdentifier(uinteger32 identifier);

        void setSubTag(Tag& tag);

        count numberOfSubTags(void);

        void addTo(Blob& destination) const;
    };
} }
