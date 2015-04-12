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

#include <SeratoDB/NxASeratoTag.h>
#include <SeratoDB/NxASeratoDbUtility.h>

#include <map>

namespace NxA {
    #pragma mark Containers
    typedef std::map<uint32_t, ConstSeratoTagPtr> SeratoIdentifierToTagMap;

    #pragma mark Class Declaration
    class SeratoObjectTag : public SeratoTag
    {
    private:
        #pragma mark Private Instance Variables
        SeratoIdentifierToTagMap p_subTagForIdentifier;

    public:
        #pragma mark Constructors
        explicit SeratoObjectTag(const void* tagAddress);
        explicit SeratoObjectTag(const uint32_t &identifier, SeratoTagVectorPtr content);

        #pragma mark Instance Methods
        bool hasSubTagForIdentifier(const uint32_t& identifier) const;

        const SeratoTag& subTagForIdentifier(const uint32_t& identifier) const;
        SeratoTag& subTagForIdentifier(const uint32_t& identifier);

        void addSubTag(ConstSeratoTagPtr tag);

        void addTo(CharVector& destination) const;
    };
}
