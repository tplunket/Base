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

#include <map>

namespace NxA { namespace Serato {
    #pragma mark Containers
    typedef std::map<uint32_t, ConstTagPtr> SeratoIdentifierToTagMap;

    #pragma mark Class Declaration
    class ObjectTag : public Tag
    {
    private:
        #pragma mark Private Instance Variables
        SeratoIdentifierToTagMap p_subTagForIdentifier;

    public:
        #pragma mark Constructors
        explicit ObjectTag(const void* tagAddress);
        explicit ObjectTag(const uint32_t &identifier, TagVectorPtr content);

        #pragma mark Instance Methods
        bool hasSubTagForIdentifier(const uint32_t& identifier) const;

        const Tag& subTagForIdentifier(const uint32_t& identifier) const;
        Tag& subTagForIdentifier(const uint32_t& identifier);

        void addSubTag(ConstTagPtr tag);

        void addTo(CharVector& destination) const;
    };
} }
