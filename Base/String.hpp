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

namespace NxA {
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR(String);
    
    class Blob;

    class String : public Object {
        NXA_GENERATED_DECLARATIONS_FOR(NxA, String);

    public:
        #pragma mark Factory Methods
        static String::Pointer string(void);
        static String::Pointer stringWith(const character* format, ... );
        static String::Pointer stringWith(String::ConstPointer const& other);
        static String::Pointer stringWithUTF16(NxA::ConstPointer<NxA::Blob> const& other);

        #pragma mark Operators
        bool isEqualTo(String::ConstPointer const& other) const;
        bool isEqualTo(const char* other) const;

        #pragma mark Instance Methods
        virtual String::ConstPointer description(void) const;

        count length(void) const;
        bool isEmpty(void) const
        {
            return this->length() == 0;
        };

        const character* toUTF8(void) const;
        NxA::ConstPointer<NxA::Blob> toUTF16(void) const;

        void append(String::ConstPointer const& other);
        void append(const character* other);

        String::Array::Pointer splitBySeperator(char seperator) const;
        String::Pointer subString(count start, count end = -1) const;

        bool hasPrefix(String::ConstPointer const& prefix) const;
        bool hasPostfix(String::ConstPointer const& postfix) const;
        bool hasPrefix(const character* prefix) const;
        bool hasPostfix(const character* postfix) const;

        count indexOfFirstOccurenceOf(String::ConstPointer const& other) const;
        count indexOfLastOccurenceOf(String::ConstPointer const& other) const;
        count indexOfFirstOccurenceOf(const character* other) const;
        count indexOfLastOccurenceOf(const character* other) const;
    };
}
