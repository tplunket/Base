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

// -- This should be included after our own includes but some names clash with its contents.
#include <string>
#include <cstdio>
#include <codecvt>
#include <sstream>
#include <vector>

#include <utf8rewind/utf8rewind.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <boost/algorithm/string/replace.hpp>
#pragma clang diagnostic pop

#include "Base/Assert.hpp"
#include "Base/Blob.hpp"
#include "Base/Platform.hpp"
#include "Base/Internal/Object.hpp"

extern template class std::basic_stringstream<char, std::char_traits<char>, std::allocator<char>>;
extern template class std::basic_stringbuf<char, std::char_traits<char>, std::allocator<char>>;
extern template class std::basic_string<char, std::char_traits<char>, std::allocator<char>>;


namespace NxA {

class String;
class MutableString;

struct MutableStringInternal : public Object::Internal, public std::string
{
    #pragma mark Constructors/Destructors
    MutableStringInternal() : std::string{ "", 0 } { }
    MutableStringInternal(const std::string& other) : std::string{ other } { }
    MutableStringInternal(std::string&& other) : std::string{ std::move(other) } { }
    MutableStringInternal(const character* other, count count) : std::string{ other, count }
    {
        NXA_ASSERT_NOT_NULL(other);
    }
    virtual ~MutableStringInternal() = default;

    template <typename... Args>
    static MutableStringInternal stringWithFormat(count sizeGuess, const character* format, Args&&... args)
    {
        NXA_ASSERT_NOT_NULL(format);
        NXA_ASSERT_TRUE(sizeGuess > 1 && sizeGuess < std::numeric_limits<count>::max());
        auto finalStringLength = 0;

        {
            std::string buffer;
            buffer.resize(sizeGuess, '\0');

            // -- safe to use snprintf instead of vsnprintf because buffer is unique
            finalStringLength = std::snprintf(&buffer[0], sizeGuess, format, args...);

            NXA_ASSERT_FALSE(finalStringLength < 0 || finalStringLength >= std::numeric_limits<count>::max());

            buffer.resize(finalStringLength);

            if (finalStringLength < sizeGuess) {
                return MutableStringInternal{std::move(buffer)};
            }
        }

        // -- guess was too small, redo with the exact correct size
        return stringWithFormat(finalStringLength + 1, format, std::forward<Args>(args)...);
    }

    static std::shared_ptr<MutableStringInternal> stringWithRepeatedCharacter(count number, character specificCharacter);
    static std::shared_ptr<MutableStringInternal> stringWithUTF16AtAndSize(const byte* data, count size);
    static std::shared_ptr<MutableStringInternal> stringWithUTF16(const Blob& other);
    static std::shared_ptr<MutableStringInternal> stringByFilteringNonPrintableCharactersIn(const String& other);

    #pragma mark Operators
    bool operator==(const character* other) const;
    bool operator==(const MutableStringInternal& other) const;

    #pragma mark Instance Methods
    count length() const;

    integer32 compare(const char * other) const;

    integer32 compare(const MutableStringInternal & other) const;

    uinteger32 hash() const;

    integer integerValue() const;

    decimal3 decimalValue() const;

    const std::string& asStdString() const;

    const character* asUTF8() const;

    Blob asUTF16() const;

    void append(const MutableStringInternal& other);

    void append(const character* other);

    void append(const character other);

    std::shared_ptr<MutableStringInternal> stringByAppending(const MutableStringInternal& other) const;

    std::vector<String> splitBySeparator(character separator) const;

    std::shared_ptr<MutableStringInternal> utfSeek(count skip) const;

    std::shared_ptr<MutableStringInternal> subString(count start, count end) const;

    std::shared_ptr<MutableStringInternal> lowerCaseString() const;

    std::shared_ptr<MutableStringInternal> upperCaseString() const;

    boolean hasPrefix(const MutableStringInternal& prefix) const;

    boolean hasPrefix(const character* prefix) const;
    
    boolean hasPostfix(const MutableStringInternal& postfix) const;

    boolean hasPostfix(const character* postfix) const;

    boolean contains(const MutableStringInternal& other) const;

    boolean contains(const character* other) const;

    boolean hasNonPrintableCharacters() const;

    count indexOfFirstOccurenceOf(const String& other) const;
    count indexOfFirstOccurenceOf(const character* other) const;
    count indexOfLastOccurenceOf(const String& other) const;
    count indexOfLastOccurenceOf(const character*  other) const;
    
    void replaceOccurenceOfStringWith(const character* occurence, const character* replacement);

    #pragma mark Overriden Object::Internal Instance Methods
    uinteger32 classHash() const override;
    const character* className() const override;

    template<typename T>
    static T stringArgumentAsCharacter(T && t)
    {
        return t;
    }
    static const character* stringArgumentAsCharacter(const std::string& cppstring);
    static const character* stringArgumentAsCharacter(const String& nxastring);
    static const character* stringArgumentAsCharacter(const MutableString& nxastring);
    static const character* stringArgumentAsCharacter(const MutableStringInternal& nxastring);
    static const character* stringArgumentAsCharacter(std::string& cppstring);
    static const character* stringArgumentAsCharacter(String& nxastring);
    static const character* stringArgumentAsCharacter(MutableString& nxastring);
    static const character* stringArgumentAsCharacter(MutableStringInternal& nxastring);


};

}
