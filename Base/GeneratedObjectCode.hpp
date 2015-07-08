//
//  Copyright (c) 2015 Next Audio Labs, LLC. All rights reserved.
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

#include "Base/Pointer.hpp"
#include "Base/WeakPointer.hpp"
#include "Base/Assert.hpp"

#include <memory>

#pragma mark Forward Declarations

namespace NxA {
    class String;
    template <class T> class ArrayContainer;
    namespace Internal {
        class Object;
    }
}

#pragma mark Macros

#define NXA_STR_VALUE_FOR(arg...) #arg

// -- Forward declarations used by objects with a internal implementation.

#define NXA_GENERATED_FORWARD_DECLARATIONS_FOR_CLASS(class_name...) \
        namespace Internal { \
            class class_name; \
        }

// -- Generated declarations and implementations for internal objects.
// -- Internal objects contain the private variables and implementation of an Object.
// -- They are derived from NxA::Internal::Object

#define NXA_GENERATED_SHARED_INTERNAL_CONSTRUCTORS_DECLARATION_FOR(namespace_name, class_name...) \
        protected: \
            struct constructor_access { };

#define NXA_GENERATED_INTERNAL_DECLARATIONS_WITHOUT_CONSTRUCTORS_FOR(namespace_name, class_name...) \
        public: \
            virtual ~class_name() = default;

#define NXA_GENERATED_INTERNAL_DECLARATIONS_FOR(namespace_name, class_name...) \
        NXA_GENERATED_SHARED_INTERNAL_CONSTRUCTORS_DECLARATION_FOR(namespace_name, class_name) \
        NXA_GENERATED_INTERNAL_DECLARATIONS_WITHOUT_CONSTRUCTORS_FOR(namespace_name, class_name) \
        protected: \
            class_name(); \
        public: \
            explicit class_name(const namespace_name::Internal::class_name::constructor_access&) : class_name() { } \
        public: \
            using Pointer = NxA::Pointer<namespace_name::Internal::class_name>; \
            friend class namespace_name::class_name; \
            static NxA::Pointer<namespace_name::Internal::class_name> makeShared(void) \
            { \
                return NxA::Pointer<namespace_name::Internal::class_name>(std::make_shared<namespace_name::Internal::class_name>(namespace_name::Internal::class_name::constructor_access())); \
            }

#define NXA_GENERATED_SHARED_INTERNAL_DECLARATIONS_FOR(namespace_name, class_name...) \
        NXA_GENERATED_SHARED_INTERNAL_CONSTRUCTORS_DECLARATION_FOR(namespace_name, class_name) \
        NXA_GENERATED_INTERNAL_DECLARATIONS_WITHOUT_CONSTRUCTORS_FOR(namespace_name, class_name) \

// -- Generated declarations and implementations for base objects.
// -- Base objects are classes like String, Map, Array which do not contain an internal object
// -- but are derived from NxA::Object and externally behave like a regular object.

#define NXA_GENERATED_SHARED_BASE_OPERATOR_EQUAL_WITH_OBJECT_DECLARATIONS_FOR(namespace_name, class_name...) \
        public: \
            virtual bool operator==(const NxA::Object& other) const \
            { \
                NXA_ASSERT_EQ(this->className(), other.className()); \
                return this->operator==(dynamic_cast<const namespace_name::class_name&>(other)); \
            } \
            bool operator!=(const NxA::Object& other) const \
            { \
                return !this->operator==(other); \
            }

#define NXA_GENERATED_SHARED_BASE_DECLARATIONS_FOR_PURE_VIRTUAL_CLASS(namespace_name, class_name...) \
        NXA_GENERATED_SHARED_INTERNAL_DECLARATIONS_FOR(namespace_name, class_name) \
        public: \
            using Pointer = NxA::Pointer<namespace_name::class_name>; \
            using PointerToConst = NxA::Pointer<const namespace_name::class_name>; \
            using WeakPointer = NxA::WeakPointer<namespace_name::class_name>; \
            using WeakPointerToConst = NxA::WeakPointer<namespace_name::class_name>; \
            using Array = NxA::ArrayContainer<namespace_name::class_name>; \
            using ArrayOfConst = NxA::ArrayContainer<const namespace_name::class_name>; \
            NxA::Pointer<namespace_name::class_name> pointer(void) \
            { \
                std::shared_ptr<namespace_name::class_name> result = std::dynamic_pointer_cast<namespace_name::class_name>(this->shared_from_this()); \
                NXA_ASSERT_NOT_NULL(result.get()); \
                return NxA::Pointer<namespace_name::class_name>(result); \
            } \
            NxA::Pointer<const namespace_name::class_name> pointer(void) const \
            { \
                std::shared_ptr<const namespace_name::class_name> result = std::dynamic_pointer_cast<const namespace_name::class_name>(this->shared_from_this()); \
                NXA_ASSERT_NOT_NULL(result.get()); \
                return NxA::Pointer<const namespace_name::class_name>(result); \
            } \
            virtual const character* className(void) const \
            { \
                return NXA_STR_VALUE_FOR(namespace_name) "::" NXA_STR_VALUE_FOR(class_name); \
            } \
            bool operator!=(const namespace_name::class_name& other) const \
            { \
                return !this->operator==(other); \
            }

#define NXA_GENERATED_SHARED_BASE_DECLARATIONS_FOR(namespace_name, class_name...) \
        NXA_GENERATED_SHARED_BASE_DECLARATIONS_FOR_PURE_VIRTUAL_CLASS(namespace_name, class_name) \
        protected: \
            static NxA::Pointer<class_name> makeShared(void) \
            { \
                return NxA::Pointer<namespace_name::class_name>(std::make_shared<namespace_name::class_name>(namespace_name::class_name::constructor_access())); \
            }

#define NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_BASE_CLASS(namespace_name, class_name...) \
        NXA_GENERATED_SHARED_BASE_DECLARATIONS_FOR(namespace_name, class_name) \
        NXA_GENERATED_SHARED_BASE_OPERATOR_EQUAL_WITH_OBJECT_DECLARATIONS_FOR(namespace_name, class_name) \
        public: \
            explicit class_name(const namespace_name::class_name::constructor_access&) : Object(NxA::Object::constructor_access()) { }

// -- Generated declarations and implementations for the Object class.
// -- This should only be directly used by the NxA::Object class.

#define NXA_GENERATED_SHARED_OBJECT_CONSTRUCTORS_DECLARATION_FOR(namespace_name, class_name...) \
        protected: \
            explicit class_name() : class_name(namespace_name::class_name::makeInternal()) { }

#define NXA_ENABLE_SHARED_FROM_THIS(class_name) std::enable_shared_from_this<class_name>

#define NXA_GENERATED_SHARED_OBJECT_DECLARATION_FOR_PURE_VIRTUAL_CLASS(namespace_name, class_name...) \
        NXA_GENERATED_SHARED_BASE_DECLARATIONS_FOR_PURE_VIRTUAL_CLASS(namespace_name, class_name) \
        NXA_GENERATED_SHARED_OBJECT_CONSTRUCTORS_DECLARATION_FOR(namespace_name, class_name) \
        public: \
            friend namespace_name::Internal::class_name; \
        private: \
            namespace_name::Internal::class_name* internal;

#define NXA_GENERATED_SHARED_OBJECT_DECLARATION_FOR(namespace_name, class_name...) \
        NXA_GENERATED_SHARED_BASE_DECLARATIONS_FOR(namespace_name, class_name) \
        NXA_GENERATED_SHARED_OBJECT_CONSTRUCTORS_DECLARATION_FOR(namespace_name, class_name) \
        public: \
            friend namespace_name::Internal::class_name; \
        protected: \
            static NxA::Pointer<NxA::Internal::Object> makeInternal(void); \
        private: \
            namespace_name::Internal::class_name* internal;

#define NXA_GENERATED_OBJECT_DECLARATIONS_FOR(namespace_name, class_name...) \
        NXA_GENERATED_SHARED_BASE_DECLARATIONS_FOR(namespace_name, class_name) \
        public: \
            explicit class_name(const namespace_name::class_name::constructor_access&); \
            friend namespace_name::Internal::class_name; \
        protected: \
            explicit class_name(const NxA::Pointer<NxA::Internal::Object>& initial_internal) : internalImplementation(initial_internal), \
                                                                                               internal(&(*initial_internal)) { } \
            static NxA::Pointer<NxA::Internal::Object> makeInternal(void); \
        private: \
            NxA::Pointer<NxA::Internal::Object> internalImplementation; \
            namespace_name::Internal::class_name* internal;

#define NXA_GENERATED_SHARED_OBJECT_IMPLEMENTATION_FOR(namespace_name, class_name...) \
        NxA::Pointer<NxA::Internal::Object> namespace_name::class_name::makeInternal(void) \
        { \
            return NxA::Pointer<NxA::Internal::Object>::dynamicCastFrom(namespace_name::Internal::class_name::makeShared()); \
        }

#define NXA_GENERATED_OBJECT_IMPLEMENTATION_FOR(namespace_name, class_name...) \
        NXA_GENERATED_SHARED_OBJECT_IMPLEMENTATION_FOR(namespace_name, class_name) \
        namespace_name::class_name::class_name(const namespace_name::class_name::constructor_access&) : Object(NxA::Internal::Object::makeShared()) { }

// -- Generated declarations and implementations for regular objects.
// -- Regular objects are derived from NxA::Object and contain an internal implementation object
// -- derived from NxA::Internal::Object;

#define NXA_GENERATED_COMMON_DECLARATIONS_IN_NAMESPACE_FOR_CLASS(namespace_name, class_name...) \
        NXA_GENERATED_SHARED_BASE_OPERATOR_EQUAL_WITH_OBJECT_DECLARATIONS_FOR(namespace_name, class_name) \
        private: \
            using super = class_name; \
        protected: \
            explicit class_name(const NxA::Pointer<NxA::Internal::Object>& initial_internal); \
        public: \
            explicit class_name(const namespace_name::class_name::constructor_access&) : class_name(namespace_name::class_name::makeInternal()) { } \
            explicit class_name(const namespace_name::class_name::constructor_access&, const NxA::Pointer<NxA::Internal::Object>& initial_internal) : class_name(initial_internal) { } \

#define NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_PURE_VIRTUAL_CLASS(namespace_name, class_name...) \
        NXA_GENERATED_SHARED_OBJECT_DECLARATION_FOR_PURE_VIRTUAL_CLASS(namespace_name, class_name) \
        NXA_GENERATED_COMMON_DECLARATIONS_IN_NAMESPACE_FOR_CLASS(namespace_name, class_name)

#define NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_CLASS(namespace_name, class_name...) \
        NXA_GENERATED_SHARED_OBJECT_DECLARATION_FOR(namespace_name, class_name) \
        NXA_GENERATED_COMMON_DECLARATIONS_IN_NAMESPACE_FOR_CLASS(namespace_name, class_name) \
        protected: \
            static NxA::Pointer<class_name> makeSharedWithInternal(const NxA::Pointer<NxA::Internal::Object>& initial_internal);

#define NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_PURE_VIRTUAL_CLASS_WITH_PARENT(namespace_name, class_name, parent_class) \
        namespace_name::class_name::class_name(const NxA::Pointer<NxA::Internal::Object>& initial_internal) : \
                        parent_class(initial_internal), \
                        internal(dynamic_cast<namespace_name::Internal::class_name*>(&(*initial_internal))) { };

#define NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(namespace_name, class_name, parent_class) \
        NXA_GENERATED_SHARED_OBJECT_IMPLEMENTATION_FOR(namespace_name, class_name) \
        NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_PURE_VIRTUAL_CLASS_WITH_PARENT(namespace_name, class_name, parent_class) \
        NxA::Pointer<namespace_name::class_name> namespace_name::class_name::makeSharedWithInternal(const NxA::Pointer<NxA::Internal::Object>& initial_internal) \
        { \
            return NxA::Pointer<namespace_name::class_name>(std::make_shared<namespace_name::class_name>(namespace_name::class_name::constructor_access(), initial_internal)); \
        }
