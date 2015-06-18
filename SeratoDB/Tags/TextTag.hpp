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
#include <SeratoDB/Utility.hpp>

#include <Base/Blob.hpp>
#include <Base/String.hpp>

namespace NxA { namespace Serato {
    #pragma mark Class Declaration
    class TextTag : public Tag
    {
    private:
        #pragma mark Private Instance Variables
        String::ConstPointer p_value;

    public:
        #pragma mark Constructors
        explicit TextTag(const void* tagAddress);
        explicit TextTag(const uint32_t& identifier, const char* value) :
                         Tag(identifier),
                         p_value(String::stringWith(value)) { }

        #pragma mark Instance Methods
        const uint32_t& identifier(void) const;
        String::ConstPointer const& value(void) const;
        String::ConstPointer& value(void);

        void addTo(Blob::Pointer const& destination) const;
    };
} }
