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
#include "Base/ArrayContainer.hpp"

#include <string>

namespace NxA {
    class Blob;

    class String : public Object, private std::string {
        NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_BASE_CLASS(NxA, String);
        NXA_GENERATED_CLASS_IDENTIFIER_METHODS_FOR(NxA, String);

        #pragma mark Private Class Methods
        static void p_initDefaultLocale(void);

    public:
        #pragma mark Factory Methods
        static String::Pointer string(void);
        static String::Pointer stringWith(const character* other);
        static String::Pointer stringWith(const character* other, count length);
        static String::Pointer stringWith(const String& other);
        static String::Pointer stringWithFormat(const character* format, ...);
        static String::Pointer stringWithUTF16(const Blob& other);

        #pragma mark Class Methods
        static uinteger32 hashFor(String::Pointer str);
        static uinteger32 hashFor(const character* str);
        static count lengthOf(const character* str);
        
        #pragma mark Operators
        bool operator==(const String& other) const;
        bool operator==(const char* other) const;
        bool operator!=(const char* other) const;

        integer32 compareTo(const String& other) const {
            return 0;
        }
        bool operator<(const String& other) const { return compareTo(other) < 0; }
        bool operator>(const String& other) const { return compareTo(other) > 0; }

        #pragma mark Instance Methods
        virtual String::Pointer description(void) const;

        count length(void) const;
        boolean isEmpty(void) const
        {
            return this->length() == 0;
        };

        uinteger32 hash(void) const;
        integer integerValue(void) const;

        const character* toUTF8(void) const;
        NxA::Pointer<NxA::Blob> toUTF16(void) const;

        void append(const String& other);
        void append(const character* other);

        String::Array::Pointer splitBySeperator(char seperator) const;
        String::Pointer subString(count start, count end = -1) const;
        String::Pointer lowerCaseString(void) const;
        String::Pointer uppperCaseString(void) const;

        boolean hasPrefix(const String& prefix) const;
        boolean hasPostfix(const String& postfix) const;
        boolean hasPrefix(const character* prefix) const;
        boolean hasPostfix(const character* postfix) const;

        count indexOfFirstOccurenceOf(const String& other) const;
        count indexOfLastOccurenceOf(const String& other) const;
        count indexOfFirstOccurenceOf(const character* other) const;
        count indexOfLastOccurenceOf(const character* other) const;
    };

    inline bool operator==(const char* first, const String& second)
    {
        return second == first;
    }
    inline bool operator!=(const char* first, const String& second)
    {
        return second != first;
    }
}
