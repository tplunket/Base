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
#include "Base/Array.hpp"
#include "Base/Assert.hpp"

#include <memory>

#pragma mark Forward Declarations

namespace NxA {
    class String;
    namespace Internal {
        class Object;
    }
}

#pragma mark Macros

#define NXA_DEBUG_OBJECT_LIFECYCLE                  0

#define NXA_STR_VALUE_FOR(arg) #arg

#define NXA_GENERATED_FORWARD_DECLARATIONS_FOR(class_name) \
        namespace Internal { \
            class class_name; \
        }

#if NXA_DEBUG_OBJECT_LIFECYCLE
#define NXA_GENERATED_SHARED_DESTRUCTOR_DECLARATIONS_FOR(namespace_name, class_name) \
        public: \
            virtual ~class_name() \
            { \
                printf("Destroying " NXA_STR_VALUE_FOR(namespace_name) "::" NXA_STR_VALUE_FOR(class_name) " at 0x%08lx.\n", (long)this); \
            }
#else
#define NXA_GENERATED_SHARED_DESTRUCTOR_DECLARATIONS_FOR(namespace_name, class_name) \
        public: \
            virtual ~class_name() = default;
#endif

#define NXA_GENERATED_SHARED_DECLARATIONS_FOR(namespace_name, class_name) \
        NXA_GENERATED_SHARED_DESTRUCTOR_DECLARATIONS_FOR(namespace_name, class_name) \
        private: \
            using super = class_name; \
        public: \
            using Pointer = NxA::Pointer<namespace_name::class_name>; \
            using PointerToConst = NxA::Pointer<const namespace_name::class_name>; \

#define NXA_GENERATED_SHARED_DECLARATIONS_FOR_OBJECT(namespace_name, class_name) \
        NXA_GENERATED_SHARED_DECLARATIONS_FOR(namespace_name, class_name) \
        friend namespace_name::Internal::class_name; \
        private: \
            namespace_name::Internal::class_name* internal; \
        protected: \
            struct class_name ## _constructor_access { }; \
            explicit class_name() : class_name(namespace_name::class_name::makeInternal()) { } \
            explicit class_name(const NxA::Pointer<NxA::Internal::Object>& initial_internal); \
            static NxA::Pointer<NxA::Internal::Object> makeInternal(void); \
            static NxA::Pointer<class_name> makeShared(void); \
            static NxA::Pointer<class_name> makeSharedWithInternal(const NxA::Pointer<NxA::Internal::Object>& initial_internal); \
        public: \
            explicit class_name(const class_name ## _constructor_access&) : class_name() { } \
            explicit class_name(const class_name ## _constructor_access&, const NxA::Pointer<NxA::Internal::Object>& initial_internal) : class_name(initial_internal) { } \
            using WeakPointer = NxA::WeakPointer<namespace_name::class_name>; \
            using WeakPointerToConst = NxA::WeakPointer<namespace_name::class_name>; \
            using Array = NxA::Array<namespace_name::class_name>; \
            using ArrayOfConst = NxA::Array<const namespace_name::class_name>; \
            NxA::Pointer<namespace_name::class_name> pointer(void); \
            NxA::Pointer<const namespace_name::class_name> pointer(void) const; \
            virtual const NxA::String& className(void) const; \
            virtual bool operator!=(const namespace_name::class_name& other) const \
            { \
                return !this->operator==(other); \
            }

#define NXA_GENERATED_DECLARATIONS_FOR_OBJECT(namespace_name, class_name) \
        NXA_GENERATED_SHARED_DECLARATIONS_FOR_OBJECT(namespace_name, class_name) \
        private: \
            NxA::Pointer<NxA::Internal::Object> internalImplementation; \

#define NXA_GENERATED_DECLARATIONS_FOR(namespace_name, class_name) \
        NXA_GENERATED_SHARED_DECLARATIONS_FOR_OBJECT(namespace_name, class_name) \
        public: \
            virtual bool operator==(const NxA::Object& other) const \
            { \
                NXA_ASSERT_TRUE(this->className() == other.className()); \
                return this->operator==(dynamic_cast<const namespace_name::class_name&>(other)); \
            }

#define NXA_GENERATED_INTERNAL_DECLARATIONS_FOR(namespace_name, class_name) \
        friend class namespace_name::class_name; \
        protected: \
            class_name(); \
            struct Internal_ ## class_name ## _constructor_access { }; \
            static NxA::Pointer<class_name> makeShared(void); \
            NXA_GENERATED_SHARED_DECLARATIONS_FOR(namespace_name::Internal, class_name) \
        public: \
            explicit class_name(Internal_ ## class_name ## _constructor_access const&) : class_name() { }

#define NXA_GENERATED_INTERNAL_DECLARATIONS_WITHOUT_CONSTRUCTOR_FOR(namespace_name, class_name) \
        friend class namespace_name::class_name; \
        protected: \
            NXA_GENERATED_SHARED_DECLARATIONS_FOR(namespace_name::Internal, class_name)

#define NXA_GENERATED_PURE_VIRTUAL_IMPLEMENTATION_FOR(namespace_name, class_name) \
        const NxA::String& namespace_name::class_name::className(void) const \
        { \
            static auto result = NxA::String::stringWith(NXA_STR_VALUE_FOR(namespace_name) "::" NXA_STR_VALUE_FOR(class_name)); \
            return *result; \
        } \
        NxA::Pointer<namespace_name::class_name> namespace_name::class_name::pointer(void) { \
            std::shared_ptr<namespace_name::class_name> result = std::dynamic_pointer_cast<namespace_name::class_name>(this->shared_from_this()); \
            NXA_ASSERT_NOT_NULL(result.get()); \
            return NxA::Pointer<namespace_name::class_name>(result); \
        } \
        NxA::Pointer<const namespace_name::class_name> namespace_name::class_name::pointer(void) const { \
            std::shared_ptr<const namespace_name::class_name> result = std::dynamic_pointer_cast<const namespace_name::class_name>(this->shared_from_this()); \
            NXA_ASSERT_NOT_NULL(result.get()); \
            return NxA::Pointer<const namespace_name::class_name>(result); \
        }

#define NXA_GENERATED_INTERNAL_IMPLEMENTATION_FOR(namespace_name, class_name) \
        NxA::Pointer<namespace_name::Internal::class_name> namespace_name::Internal::class_name::makeShared(void) \
        { \
            return NxA::Pointer<namespace_name::Internal::class_name>(std::make_shared<namespace_name::Internal::class_name>(namespace_name::Internal::class_name::Internal_ ## class_name ## _constructor_access())); \
        }

#define NXA_GENERATED_FACTORY_METHODS_FOR(namespace_name, class_name) \
        NXA_GENERATED_PURE_VIRTUAL_IMPLEMENTATION_FOR(namespace_name, class_name) \
        NxA::Pointer<namespace_name::class_name> namespace_name::class_name::makeShared(void) \
        { \
            return NxA::Pointer<namespace_name::class_name>(std::make_shared<namespace_name::class_name>(namespace_name::class_name::class_name ## _constructor_access())); \
        } \
        NxA::Pointer<namespace_name::class_name> namespace_name::class_name::makeSharedWithInternal(const NxA::Pointer<NxA::Internal::Object>& initial_internal) \
        { \
            return NxA::Pointer<namespace_name::class_name>(std::make_shared<namespace_name::class_name>(namespace_name::class_name::class_name ## _constructor_access(), initial_internal)); \
        } \
        NxA::Pointer<NxA::Internal::Object> namespace_name::class_name::makeInternal(void) \
        { \
            return NxA::Pointer<NxA::Internal::Object>::dynamicCastFrom(namespace_name::Internal::class_name::makeShared()); \
        }

#if NXA_DEBUG_OBJECT_LIFECYCLE
#define NXA_GENERATED_CONSTRUCTORS_FOR(namespace_name, class_name, parent_class) \
        namespace_name::class_name::class_name(const NxA::Pointer<NxA::Internal::Object>& initial_internal) : \
                                               parent_class(initial_internal), \
                                               internal(dynamic_cast<namespace_name::Internal::class_name*>(&(*initial_internal))) \
        { \
            printf("Constructing " NXA_STR_VALUE_FOR(namespace_name) "::" NXA_STR_VALUE_FOR(class_name) " at 0x%08lx.\n", (long)this); \
        }
#else
#define NXA_GENERATED_CONSTRUCTORS_FOR(namespace_name, class_name, parent_class) \
        namespace_name::class_name::class_name(const NxA::Pointer<NxA::Internal::Object>& initial_internal) : \
                                               parent_class(initial_internal), \
                                               internal(dynamic_cast<namespace_name::Internal::class_name*>(&(*initial_internal))) { };
#endif

#define NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(namespace_name, class_name, parent_class) \
        NXA_GENERATED_FACTORY_METHODS_FOR(namespace_name, class_name) \
        NXA_GENERATED_CONSTRUCTORS_FOR(namespace_name, class_name, parent_class)

#define NXA_ENABLE_SHARED_FROM_THIS(class_name) std::enable_shared_from_this<class_name>
