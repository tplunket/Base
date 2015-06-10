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

#include <memory>

#pragma mark Macros

#define NXA_GENERATED_FORWARD_DECLARATIONS_FOR(class_name) \
        class String; \
        namespace Internal { \
            class class_name; \
        }

#define NXA_SHARED_GENERATED_DECLARATIONS_FOR(class_name) \
        protected: \
            struct constructor_access { }; \
            class_name(); \
        public: \
            using Pointer = NxA::Serato::Pointer<class_name>; \
            explicit class_name(const constructor_access&) : class_name() { } \
        private: \
            static class_name::Pointer makeShared(void);

#define NXA_GENERATED_DECLARATIONS_FOR(class_name) \
        NXA_SHARED_GENERATED_DECLARATIONS_FOR(class_name) \
        public: \
            using WeakPointer = NxA::Serato::WeakPointer<class_name>; \
            using Array = NxA::Serato::Array<class_name>; \
            virtual const NxA::Serato::Pointer<NxA::Serato::String> className(void) const; \
        private: \
            NxA::Serato::Pointer<Internal::class_name> internal;

#define NXA_GENERATED_INTERNAL_DECLARATIONS_FOR(class_name) \
        friend class NxA::Serato::class_name; \
        NXA_SHARED_GENERATED_DECLARATIONS_FOR(class_name)

#define NXA_STR_VALUE_FOR(arg) #arg

#define NXA_GENERATED_INTERNAL_IMPLEMENTATION_FOR(class_name) \
        NxA::Serato::Pointer<class_name> class_name::makeShared(void) \
        { \
            return NxA::Serato::Pointer<class_name>(std::make_shared<class_name>(class_name::constructor_access())); \
        }

#define NXA_GENERATED_IMPLEMENTATION_FOR(class_name) \
        NXA_GENERATED_INTERNAL_IMPLEMENTATION_FOR(class_name) \
        const NxA::Serato::Pointer<NxA::Serato::String> class_name::className(void) const \
        { \
            return NxA::Serato::String::stringWithCharPointer(NXA_STR_VALUE_FOR(class_name)); \
        }
