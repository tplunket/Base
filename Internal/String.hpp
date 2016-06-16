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

#include <utf8rewind/utf8rewind.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <boost/algorithm/string/replace.hpp>
#pragma clang diagnostic pop

#include "Base/Assert.hpp"
#include "Base/Array.hpp"
#include "Base/MutableArray.hpp"
#include "Base/Blob.hpp"
#include "Base/Platform.hpp"
#include "Base/Internal/Object.hpp"

namespace NxA {

struct StringInternal : public Object::Internal, public std::string
{
    #pragma mark Constructors/Destructors
    StringInternal() : std::string{ "", 0 } { }
    StringInternal(const std::string& other) : std::string{ other } { }
    StringInternal(std::string&& other) : std::string{ std::move(other) } { }
    StringInternal(const character* other, count count) : std::string{ other, count }
    {
        NXA_ASSERT_NOT_NULL(other);
    }
    virtual ~StringInternal() = default;

    #pragma mark Factory Methods
    static std::shared_ptr<StringInternal> stringWithFormat(const character* format, va_list args)
    {
        NXA_ASSERT_NOT_NULL(format);

        constexpr count formatStringBufferSize = 1024;
        auto buffer = new char[formatStringBufferSize];
        NXA_SCOPE_EXIT(buffer) {
            delete [] buffer;
        } NXA_SCOPE_EXIT_END

        ::vsnprintf(buffer, formatStringBufferSize, format, args);

        return std::make_shared<StringInternal>(buffer);
    }

    static std::shared_ptr<StringInternal> stringWithUTF16(const Blob& other)
    {
        Blob local(other);

        if (Platform::endianOrder == Platform::LitleEndian) {
            local = Platform::convertEndiannessOfUInteger16From(local);
        }

        count length = local.size() / 2;
        const integer16* characters = reinterpret_cast<const integer16*>(local.data());

        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
        return std::make_shared<StringInternal>(convert.to_bytes(reinterpret_cast<const char16_t*>(characters), reinterpret_cast<const char16_t*>(characters + length)));
    }

    #pragma mark Operators
    bool operator==(const character* other) const
    {
        return this->std::string::compare(other) == 0;
    }
    bool operator==(const StringInternal& other) const
    {
        return this->std::string::compare(other) == 0;
    }

    #pragma mark Instance Methods
    count length() const
    {
        return this->size();
    }

    uinteger32 hash() const
    {
        return String::hashFor(this->asUTF8());
    }

    integer integerValue() const
    {
        return ::atoi(this->c_str());
    }

    const std::string& asStdString() const
    {
        return *this;
    }

    const character* asUTF8() const
    {
        return this->c_str();
    }

    Blob asUTF16() const
    {
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
        std::u16string u16 = convert.from_bytes(this->c_str());

        auto newBlob = Blob::blobWithMemoryAndSize(reinterpret_cast<const byte*>(reinterpret_cast<const integer16*>(u16.c_str())),
                                                   u16.length() * 2);
        if (Platform::endianOrder == Platform::LitleEndian) {
            newBlob = Platform::convertEndiannessOfUInteger16From(newBlob);
        }

        return newBlob;
    }

    void append(const StringInternal& other)
    {
        this->std::string::append(other);
    }

    void append(const character* other)
    {
        this->std::string::append(other);
    }

    std::shared_ptr<StringInternal> stringByAppending(const StringInternal& other) const
    {
        return { std::make_shared<StringInternal>(*this + other) };
    }

    Array<String> splitBySeparator(character separator) const
    {
        MutableArray<String> results;
        std::stringstream stream(this->asUTF8());
        std::string line;

        while(getline(stream, line, separator)) {
            results.append(String(line));
        }

        return { std::move(results) };
    }

    std::shared_ptr<StringInternal> utfSeek(count skip) const
    {
        NXA_ASSERT_TRUE(skip >= 0);

        if (skip > this->length()) {
            return { std::make_shared<StringInternal>() };
        }

        auto rawPointer = this->c_str();
        auto startPointer = utf8seek(rawPointer, this->length(), rawPointer, skip, SEEK_SET);

        return { std::make_shared<StringInternal>(startPointer) };
    }

    std::shared_ptr<StringInternal> subString(count start, count end) const
    {
        NXA_ASSERT_TRUE(start <= end);

        if ((end == -1) || (end > this->length())) {
            end = this->length();
        }

        if (start >= this->length()) {
            return { std::make_shared<StringInternal>() };
        }

        return { std::make_shared<StringInternal>(this->substr(start, end - start)) };
    }

    std::shared_ptr<StringInternal> lowerCaseString() const
    {
        auto input = this->c_str();
        auto inputLength = this->length();
        auto worstCaseOutputLength = inputLength * 2;
        character output[worstCaseOutputLength];
        memset(output, 0, worstCaseOutputLength);

        int32_t errors;
        auto convertedSize = utf8tolower(input, inputLength, output, worstCaseOutputLength, &errors);
        NXA_ASSERT_TRUE(errors == UTF8_ERR_NONE);

        if (convertedSize == 0) {
            return { std::make_shared<StringInternal>() };
        }

        return { std::make_shared<StringInternal>(const_cast<const character*>(output), convertedSize) };
    }

    std::shared_ptr<StringInternal> upperCaseString() const
    {
        auto input = this->c_str();
        auto inputLength = this->length();
        auto worstCaseOutputLength = inputLength * 2;
        character output[worstCaseOutputLength];
        memset(output, 0, worstCaseOutputLength);

        int32_t errors;
        auto convertedSize = utf8toupper(input, inputLength, output, worstCaseOutputLength, &errors);
        NXA_ASSERT_TRUE(errors == UTF8_ERR_NONE);

        if (convertedSize == 0) {
            return { std::make_shared<StringInternal>() };
        }

        auto tst = StringInternal(output, convertedSize);
        return { std::make_shared<StringInternal>(const_cast<const character*>(output), convertedSize) };
    }

    boolean hasPrefix(const StringInternal& prefix) const
    {
        return this->find(prefix) == 0;
    }

    boolean hasPostfix(const StringInternal& postfix) const
    {
        size_t pos = this->find(postfix);
        if (pos == std::string::npos) {
            return false;
        }

        return pos == (this->length() - postfix.length());
    }

    boolean hasPrefix(const character* prefix) const
    {
        NXA_ASSERT_NOT_NULL(prefix);

        return this->find(prefix) == 0;
    }

    boolean hasPostfix(const character* postfix) const
    {
        NXA_ASSERT_NOT_NULL(postfix);

        size_t pos = this->rfind(postfix);
        if (pos == std::string::npos) {
            return false;
        }

        count length = strlen(postfix);
        return pos == (this->length() - length);
    }

    count indexOfFirstOccurenceOf(const String& other) const
    {
        return this->indexOfFirstOccurenceOf(other.asUTF8());
    }

    count indexOfLastOccurenceOf(const String& other) const
    {
        return this->indexOfLastOccurenceOf(other.asUTF8());
    }
    
    count indexOfFirstOccurenceOf(const character* other) const
    {
        NXA_ASSERT_NOT_NULL(other);
        
        size_t pos = this->find(other);
        if (pos == std::string::npos) {
            return this->length();
        }
        
        return pos;
    }
    
    count indexOfLastOccurenceOf(const character*  other) const
    {
        NXA_ASSERT_NOT_NULL(other);
        
        size_t pos = this->rfind(other);
        if (pos == std::string::npos) {
            return this->length();
        }
        
        return pos;
    }
    
    void replaceOccurenceOfStringWith(const character* occurence, const character* replacement)
    {
        NXA_ASSERT_NOT_NULL(occurence);
        NXA_ASSERT_NOT_NULL(replacement);

        boost::replace_all(*static_cast<std::string*>(this), occurence, replacement);
     }
};

}
