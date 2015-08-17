//
//  Copyright (c) 2015 Next Audio Labs, LLC. All rights reserved.
//
//  This file contains confidential and proprietary information of Next
//  Audio Labs, LLC ("Next"). No use is permitted without express written
//  permission of Next. If you are not a party to a Confidentiality/
//  Non-Disclosure Agreement with Next, please immediately delete this
//  file as well as all copies in your possession. For further information,
//  please email info@nextaudiolabs.com.
//

#pragma once

#include "Base/Base.hpp"
#include "Persistence/Serializer.hpp"

namespace NxA {
    struct ObjectID {
    public:
        #pragma mark Instance Variables
        uinteger32 type;
        integer32 index;

        #pragma mark Constructors
        ObjectID() : type(0xFFFFFFFF), index(0xFFFFFFFF)
        {
        }
        ObjectID(uinteger32 type_, integer32 index_) : type(type_), index(index_)
        {
        }

        #pragma mark Instance Methods
        String::Pointer description(void) const;

        #pragma mark Comparison Operators
        integer32 compareTo(const ObjectID& other) const;
        boolean operator<(const ObjectID& other) const;
        boolean operator>(const ObjectID& other) const;
        boolean operator<=(const ObjectID& other) const;
        boolean operator>=(const ObjectID& other) const;
        boolean operator==(const ObjectID& other) const;
        boolean operator!=(const ObjectID& other) const;
    };
}
