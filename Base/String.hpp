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

#include "Base/Object.hpp"
#include "Base/Types.hpp"

NXA_GENERATED_FORWARD_DECLARATIONS_FOR(NxA, String);

namespace NxA {
    class Blob;

    class String : public Object {
        NXA_GENERATED_DECLARATIONS_FOR(NxA, String);

    public:
        #pragma mark Factory Methods
        static String::Pointer string(void);
        static String::Pointer stringWithCharPointer(const char* other);
        static String::Pointer stringWithFormat(const char* format, ... );
        static String::Pointer stringWithUTF16Blob(const NxA::Pointer<NxA::Blob>& other);
        static String::Pointer stringWithString(const String::Pointer& other);

        #pragma mark Operators
        bool isEqualTo(String const& other) const;
        bool isEqualTo(const char* other) const;

        #pragma mark Instance Methods
        virtual const String::Pointer description(void) const;

        count length(void) const;

        const char* toCharPointer(void) const;
        NxA::Pointer<NxA::Blob> toUTF16Blob(void) const;

        void append(const String::Pointer& other);
        void append(const char* other);

        String::Array::Pointer splitBySeperator(char seperator) const;
        String::Pointer subString(count start, count end = -1);

        bool hasPrefix(String::Pointer prefix) const;
        bool hasPostfix(String::Pointer postfix) const;
        bool hasPrefix(const char* prefix) const;
        bool hasPostfix(const char* postfix) const;
    };
}
