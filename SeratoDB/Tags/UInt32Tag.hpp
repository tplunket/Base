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

namespace NxA { namespace Serato {
    #pragma mark Class Declaration
    class UInt32Tag : public Tag
    {
    private:
        #pragma mark Private Instance Variables
        uint32_t p_value;

    public:
        #pragma mark Constructors
        explicit UInt32Tag(const void* tagAddress) :
                            Tag(tagAddress),
                            p_value(bigEndianUInt32ValueAt(Tag::p_dataForTagAt(tagAddress))) { }
        explicit UInt32Tag(const uint32_t& identifier, const uint32_t& value) :
                            Tag(identifier),
                            p_value(value) { }

        #pragma mark Instance Methods
        const uint32_t& value(void) const;
        uint32_t& value(void);

        void addTo(CharVector& destination) const;
    };
} }
