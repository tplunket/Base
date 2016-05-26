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

#include "Base/Object.hpp"
#include "Base/Types.hpp"
#include "Base/Array.hpp"

#include <string>

NXA_ENTER_NAMESPACE(NxA);

class Blob;

class String : public Object, private std::string {
    NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_BASE_CLASS(NxA, String);
    NXA_GENERATED_CLASS_IDENTIFIER_METHODS_FOR(NxA, String);

public:
    #pragma mark Factory Methods
    static Pointer<String> string(void);
    static Pointer<String> stringWith(const character* other);
    static Pointer<String> stringWith(const character* other, count length);
    static Pointer<String> stringWith(const String& other);
    static Pointer<String> stringWithFormat(const character* format, ...);
    static Pointer<String> stringWithUTF16(const Blob& other);

    #pragma mark Class Methods
    static uinteger32 hashFor(Pointer<String> str);
    static uinteger32 hashFor(const character* str);
    static count lengthOf(const character* str);

    #pragma mark Operators
    bool operator==(const String& other) const;
    bool operator!=(const String& other) const;
    bool operator==(const char* other) const;
    bool operator!=(const char* other) const;

    integer32 compareTo(const String& other) const
    {
        return 0;
    }
    bool operator<(const String& other) const { return compareTo(other) < 0; }
    bool operator>(const String& other) const { return compareTo(other) > 0; }

    #pragma mark Instance Methods
    count length(void) const;
    boolean isEmpty(void) const
    {
        return this->length() == 0;
    };

    uinteger32 hash(void) const;
    integer integerValue(void) const;

    const character* toUTF8(void) const;
    Pointer<NxA::Blob> toUTF16(void) const;

    void append(const String& other);
    void append(const character* other);

    Pointer<Array<String>> splitBySeperator(char seperator) const;
    Pointer<String> subString(count start, count end = -1) const;
    Pointer<String> utfSeek(count skip) const;
    Pointer<String> lowerCaseString(void) const;
    Pointer<String> upperCaseString(void) const;

    boolean hasPrefix(const String& prefix) const;
    boolean hasPostfix(const String& postfix) const;
    boolean hasPrefix(const character* prefix) const;
    boolean hasPostfix(const character* postfix) const;

    count indexOfFirstOccurenceOf(const String& other) const;
    count indexOfLastOccurenceOf(const String& other) const;
    count indexOfFirstOccurenceOf(const character* other) const;
    count indexOfLastOccurenceOf(const character* other) const;

    void replaceOccurenceOfStringWith(const character* occurence, const character* replacement);

    #pragma mark Overriden Object Instance Methods
    virtual Pointer<String> description(void) const override;
};

inline bool operator==(const char* first, const String& second)
{
    return second == first;
}
inline bool operator!=(const char* first, const String& second)
{
    return second != first;
}

NXA_EXIT_NAMESPACE;
