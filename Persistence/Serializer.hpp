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

#define NXA_SERIALIZED_MEMBER(p) \
    NxA::Serializer::createSerializedMember(#p, p)

namespace NxA {
    class Serializer {
    public:
        #pragma mark Nested Structs
        template<typename T>
        struct SerializedMember {
        public:
            const character* name;
            T& value;
            
            SerializedMember(const character* _name, T& _value) : name(_name), value(_value) {
            }
        };

        #pragma mark Static Methods
        template<typename T>
        static SerializedMember<T> createSerializedMember(const character* name, T value)
        {
            return SerializedMember<T>(name, value);
        }

        #pragma mark Instance Methods
        template<typename T> void serialize(SerializedMember<T> m)
        {
            serializeField(m.name, m.value);
        }

        virtual void serializeField(const character* name, integer32& value) = 0;
        virtual void serializeField(const character* name, String::Pointer& value) = 0;
    };
}
