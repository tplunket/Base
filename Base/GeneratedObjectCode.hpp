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
#include "Base/ConstArray.hpp"

#include <memory>

#pragma mark Forward Declaration

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
#define NXA_GENERATED_SHARED_DECLARATIONS_FOR(namespace_name, class_name) \
        protected: \
            static NxA::Pointer<class_name> makeShared(void); \
        public: \
            using Pointer = NxA::Pointer<namespace_name::class_name>; \
            using ConstPointer = NxA::ConstPointer<namespace_name::class_name>; \
            virtual ~class_name() \
            { \
                printf("Destroying " NXA_STR_VALUE_FOR(namespace_name) "::" NXA_STR_VALUE_FOR(class_name) " at 0x%08lx.\n", (long)this); \
            }
#else
#define NXA_GENERATED_SHARED_DECLARATIONS_FOR(namespace_name, class_name) \
        protected: \
            static NxA::Pointer<class_name> makeShared(void); \
        public: \
            using Pointer = NxA::Pointer<namespace_name::class_name>; \
            using ConstPointer = NxA::ConstPointer<namespace_name::class_name>;
#endif

#define NXA_GENERATED_DECLARATIONS_FOR(namespace_name, class_name) \
        protected: \
            struct class_name ## _constructor_access { }; \
            class_name() : class_name(namespace_name::class_name::makeInternal()) { } \
            class_name(NxA::Pointer<NxA::Internal::Object> const& initial_internal); \
        NXA_GENERATED_SHARED_DECLARATIONS_FOR(namespace_name, class_name) \
        public: \
            explicit class_name(const class_name ## _constructor_access&) : class_name() { } \
            using WeakPointer = NxA::WeakPointer<namespace_name::class_name>; \
            using Array = NxA::Array<namespace_name::class_name>; \
            using ConstArray = NxA::ConstArray<namespace_name::class_name>; \
            virtual const NxA::ConstPointer<NxA::String> className(void) const; \
        protected: \
            NxA::Pointer<namespace_name::Internal::class_name> internal; \
            static NxA::Pointer<NxA::Internal::Object> makeInternal(void);

#define NXA_GENERATED_INTERNAL_DECLARATIONS_FOR(namespace_name, class_name) \
        friend class namespace_name::class_name; \
        protected: \
            class_name(); \
            struct Internal_ ## class_name ## _constructor_access { }; \
        NXA_GENERATED_SHARED_DECLARATIONS_FOR(namespace_name::Internal, class_name) \
        public: \
            explicit class_name(Internal_ ## class_name ## _constructor_access const&) : class_name() { }

#if NXA_DEBUG_OBJECT_LIFECYCLE
#define NXA_GENERATED_INTERNAL_IMPLEMENTATION_FOR(namespace_name, class_name) \
        NxA::Pointer<namespace_name::Internal::class_name> namespace_name::Internal::class_name::makeShared(void) \
        { \
            printf("Calling makeShared for " NXA_STR_VALUE_FOR(namespace_name) "::Internal::" NXA_STR_VALUE_FOR(class_name) "\n"); \
            return NxA::Pointer<namespace_name::Internal::class_name>(std::make_shared<namespace_name::Internal::class_name>(namespace_name::Internal::class_name::Internal_ ## class_name ## _constructor_access())); \
        }

#define NXA_GENERATED_IMPLEMENTATION_FOR(namespace_name, class_name) \
        NxA::Pointer<namespace_name::class_name> namespace_name::class_name::makeShared(void) \
        { \
            printf("Calling makeShared for " NXA_STR_VALUE_FOR(namespace_name) "::" NXA_STR_VALUE_FOR(class_name) "\n"); \
            return NxA::Pointer<namespace_name::class_name>(std::make_shared<namespace_name::class_name>(namespace_name::class_name::class_name ## _constructor_access())); \
        } \
        const NxA::ConstPointer<NxA::String> namespace_name::class_name::className(void) const \
        { \
            return NxA::String::stringWith(NXA_STR_VALUE_FOR(namespace_name) "::" NXA_STR_VALUE_FOR(class_name)); \
        } \
        NxA::Pointer<NxA::Internal::Object> namespace_name::class_name::makeInternal(void) \
        { \
            printf("Calling makeInternal for " NXA_STR_VALUE_FOR(namespace_name) "::" NXA_STR_VALUE_FOR(class_name) "\n"); \
            return NxA::Pointer<NxA::Internal::Object>(namespace_name::Internal::class_name::makeShared()); \
        }
#else
#define NXA_GENERATED_INTERNAL_IMPLEMENTATION_FOR(namespace_name, class_name) \
        NxA::Pointer<namespace_name::Internal::class_name> namespace_name::Internal::class_name::makeShared(void) \
        { \
            return NxA::Pointer<namespace_name::Internal::class_name>(std::make_shared<namespace_name::Internal::class_name>(namespace_name::Internal::class_name::Internal_ ## class_name ## _constructor_access())); \
        }

#define NXA_GENERATED_IMPLEMENTATION_FOR(namespace_name, class_name) \
        NxA::Pointer<namespace_name::class_name> namespace_name::class_name::makeShared(void) \
        { \
            return NxA::Pointer<namespace_name::class_name>(std::make_shared<namespace_name::class_name>(namespace_name::class_name::class_name ## _constructor_access())); \
        } \
        const NxA::ConstPointer<NxA::String> namespace_name::class_name::className(void) const \
        { \
            return NxA::String::stringWith(NXA_STR_VALUE_FOR(namespace_name) "::" NXA_STR_VALUE_FOR(class_name)); \
        } \
        NxA::Pointer<NxA::Internal::Object> namespace_name::class_name::makeInternal(void) \
        { \
            return NxA::Pointer<NxA::Internal::Object>(namespace_name::Internal::class_name::makeShared()); \
        }
#endif
