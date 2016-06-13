//
//  Copyright (c) 2015-2016 Next Audio Labs, LLC. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy of
//  this software and associated documentation files (the "Software"), to deal in the
//  Software without restriction, including without limitation the rights to use, copy,
//  modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
//  and to permit persons to whom the Software is furnished to do so, subject to the
//  following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
//  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
//  PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
//  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#pragma once

#include <Base/Types.hpp>

#include <memory>

#pragma mark Forward Declarations

namespace NxA {

template <class T> class WeakReference;
class String;

}

#pragma mark Macros

#define NXA_STR_VALUE_FOR(arg...) #arg

#define NXA_GENERATED_INTERNAL_OBJECT_FORWARD_DECLARATION() \
        protected: \
            struct Internal; \
        private:

#define NXA_GENERATED_INTERNAL_OBJECT_FORWARD_DECLARATION_USING(class_name...) \
        private:\
            using Internal = class_name; \

#define NXA_GENERATED_NAME_AND_HASH_INTERNAL_OBJECT_METHODS_DECLARATIONS_FOR(class_name...) \
        static const character* staticClassName() \
        { \
            return NXA_STR_VALUE_FOR(class_name); \
        } \
        static uinteger32 staticClassHash() \
        { \
            static uinteger32 result = String::hashFor(class_name::staticClassName()); \
            return result; \
        } \
        virtual uinteger32 classHash() const override \
        { \
            return class_name::staticClassHash(); \
        } \
        virtual const character* className() const override \
        { \
            return class_name::staticClassName(); \
        }

#define NXA_GENERATED_COMMON_OBJECT_METHODS_DECLARATIONS_WITHOUT_INTERNAL_OBJECT_FOR(class_name...) \
        protected: \
            class_name(std::shared_ptr<Internal>&&); \
            friend WeakReference<class_name>; \
        public: \
            class_name(class_name&&); \
            class_name(class_name&); \
            ~class_name(); \
            class_name& operator=(class_name&&); \
            class_name& operator=(const class_name&); \
            bool operator==(const class_name&) const; \
            bool operator!=(const class_name& other) const \
            { \
                return !this->operator==(other); \
            } \
            static const character* staticClassName() \
            { \
                return NXA_STR_VALUE_FOR(class_name); \
            } \
            static uinteger32 staticClassHash() \
            { \
                static uinteger32 result = String::hashFor(class_name::staticClassName()); \
                return result; \
            } \
            uinteger32 classHash() const \
            { \
                return class_name::staticClassHash(); \
            } \
            const character* className() const \
            { \
                return class_name::staticClassName(); \
            } \
            String description() const; \
        private:

#define NXA_GENERATED_UNIQUE_OBJECT_METHODS_DECLARATIONS_WITHOUT_INTERNAL_OBJECT_FOR(class_name...) \
        public: \
            class_name(const class_name&) = delete; \
        NXA_GENERATED_COMMON_OBJECT_METHODS_DECLARATIONS_WITHOUT_INTERNAL_OBJECT_FOR(class_name) \

#define NXA_GENERATED_UNIQUE_OBJECT_METHODS_DECLARATIONS_FOR(class_name...) \
        protected: \
            std::shared_ptr<Internal> internal; \
        public: \
            class_name(const class_name&) = delete; \
        NXA_GENERATED_COMMON_OBJECT_METHODS_DECLARATIONS_WITHOUT_INTERNAL_OBJECT_FOR(class_name)

#define NXA_GENERATED_OBJECT_METHODS_DECLARATIONS_WITHOUT_INTERNAL_OBJECT_FOR(class_name...) \
        public: \
            class_name(const class_name&); \
        NXA_GENERATED_COMMON_OBJECT_METHODS_DECLARATIONS_WITHOUT_INTERNAL_OBJECT_FOR(class_name) \

#define NXA_GENERATED_OBJECT_METHODS_DECLARATIONS_FOR(class_name...) \
        protected: \
            std::shared_ptr<Internal> internal; \
        public: \
            class_name(const class_name&); \
        NXA_GENERATED_COMMON_OBJECT_METHODS_DECLARATIONS_WITHOUT_INTERNAL_OBJECT_FOR(class_name)

/*uinteger32 class_name::staticClassHash() \
{ \
    static uinteger32 result = String::hashFor(class_name::staticClassName()); \
    return result; \
} \*/

#define NXA_GENERATED_OBJECT_METHODS_DEFINITIONS_WITH_INTERNAL_STORED_IN_FOR(internal_location, class_name...) \
        class_name::class_name(class_name&&) = default; \
        class_name::class_name(class_name&) = default; \
        class_name::class_name(std::shared_ptr<Internal>&& other) : internal_location{ std::move(other) } { } \
        class_name::~class_name() = default; \
        class_name& class_name::operator=(class_name&&) = default; \
        class_name& class_name::operator=(const class_name&) = default; \
        boolean class_name::operator==(const class_name& other) const \
        { \
            if (internal == other.internal) { \
                return true; \
            } \
            return *internal == *(other.internal); \
        }

#define NXA_GENERATED_MUTABLE_OBJECT_METHODS_DEFINITIONS_WITH_INTERNAL_STORED_IN_FOR(internal_location, class_name...) \
        class_name::class_name(const class_name& other) : internal_location{ std::make_shared<Internal>(*other.internal) } { } \
        NXA_GENERATED_OBJECT_METHODS_DEFINITIONS_WITH_INTERNAL_STORED_IN_FOR(internal_location, class_name)

#define NXA_GENERATED_IMMUTABLE_OBJECT_METHODS_DEFINITIONS_WITH_INTERNAL_STORED_IN_FOR(internal_location, class_name...) \
        class_name::class_name(const class_name&) = default; \
        NXA_GENERATED_OBJECT_METHODS_DEFINITIONS_WITH_INTERNAL_STORED_IN_FOR(internal_location, class_name)

#define NXA_GENERATED_UNIQUE_OBJECT_METHODS_DEFINITIONS_WITH_INTERNAL_STORED_IN_FOR(internal_location, class_name...) \
        NXA_GENERATED_OBJECT_METHODS_DEFINITIONS_WITH_INTERNAL_STORED_IN_FOR(internal_location, class_name)

#define NXA_GENERATED_MUTABLE_OBJECT_METHODS_DEFINITIONS_FOR(class_name...) \
        class_name::class_name(const class_name& other) : internal{ std::make_shared<Internal>(*other.internal) } { } \
        NXA_GENERATED_OBJECT_METHODS_DEFINITIONS_WITH_INTERNAL_STORED_IN_FOR(internal, class_name)

#define NXA_GENERATED_IMMUTABLE_OBJECT_METHODS_DEFINITIONS_FOR(class_name...) \
        class_name::class_name(const class_name&) = default; \
        NXA_GENERATED_OBJECT_METHODS_DEFINITIONS_WITH_INTERNAL_STORED_IN_FOR(internal, class_name)

#define NXA_GENERATED_UNIQUE_OBJECT_METHODS_DEFINITIONS_FOR(class_name...) \
        NXA_GENERATED_OBJECT_METHODS_DEFINITIONS_WITH_INTERNAL_STORED_IN_FOR(internal, class_name)
