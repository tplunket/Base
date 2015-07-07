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

#include "Base/String.hpp"
#include "Base/Blob.hpp"
#include "Base/Platform.hpp"

#include <cstdio>
#include <codecvt>
#include <locale>
#include <sstream>

NXA_GENERATED_PURE_VIRTUAL_IMPLEMENTATION_FOR(NxA, String);

using namespace NxA;

NxA::Pointer<NxA::String> NxA::String::makeShared(void)
{
    return NxA::Pointer<NxA::String>(std::make_shared<NxA::String>(NxA::String::String_constructor_access()));
}

#pragma mark Factory Methods

String::Pointer String::string(void)
{
    auto newString = String::makeShared();
    newString->std::string::operator=("");

    return newString;
}

String::Pointer String::stringWith(const character* other)
{
    auto newString = String::makeShared();
    newString->std::string::operator=(other);

    return newString;
}

String::Pointer String::stringWith(const String& other)
{
    auto newString = String::makeShared();
    newString->std::string::operator=(other);

    return newString;
}

String::Pointer String::stringWithFormat(const character* format, ...)
{
    constexpr count formatStringBufferSize = 256;
    char buffer[formatStringBufferSize];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, formatStringBufferSize, format, args);
    va_end(args);

    auto newString = String::makeShared();
    newString->std::string::operator=(buffer);

    return newString;
}

String::Pointer String::stringWithUTF16(const Blob& other)
{
    auto source = other.pointer();

    if (Platform::endianOrder == Platform::LitleEndian) {
        source = Platform::convertEndiannessOfUInteger16From(other);
    }

    const integer16* characters = reinterpret_cast<const integer16*>(source->data());
    count length = source->size() / 2;

    auto newString = String::makeShared();

    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
    newString->std::string::operator=(convert.to_bytes(reinterpret_cast<const char16_t*>(characters), reinterpret_cast<const char16_t*>(characters + length)));

    return newString;
}

#pragma mark Operators

bool String::operator==(const String& other) const
{
    if (this == &other) {
        return true;
    }

    const std::string& stdStringThis = *this;
    const std::string& stdStringOther = other;
    return stdStringThis == stdStringOther;
}

bool String::operator==(const char* other) const
{
    const std::string& stdStringThis = *this;
    return stdStringThis == other;
}
bool String::operator!=(const char* other) const
{
    const std::string& stdStringThis = *this;
    return stdStringThis != other;
}

#pragma mark mark Instance Methods

String::Pointer String::description(void) const
{
    return String::stringWith(*this);
}

count String::length(void) const
{
    return this->size();
}

const character* String::toUTF8(void) const
{
    return this->c_str();
}

Blob::Pointer String::toUTF16(void) const
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

void String::append(const String& other)
{
    this->std::string::append(other);
}

void String::append(const character* other)
{
    this->std::string::append(other);
}

String::Array::Pointer String::splitBySeperator(char seperator) const
{
    auto results = String::Array::array();
    std::stringstream stream(this->toUTF8());
    std::string line;

    while(getline(stream, line, seperator)) {
        results->append(String::stringWith(line.c_str()));
    }

    return results;
}

String::Pointer String::subString(count start, count end) const
{
    NXA_ASSERT_TRUE(start <= end);

    if ((end == -1) || (end > this->length())) {
        end = this->length();
    }

    if (start >= this->length()) {
        return String::string();
    }

    auto newString = String::makeShared();
    newString->std::string::operator=(this->substr(start, end - start));
    
    return newString;
}

boolean String::hasPrefix(const String& prefix) const
{
    return this->hasPrefix(prefix.toUTF8());
}

boolean String::hasPostfix(const String& postfix) const
{
    size_t pos = this->rfind(postfix);
    if (pos == std::string::npos) {
        return false;
    }

    return pos == (this->length() - postfix.length());
}

boolean String::hasPrefix(const character* prefix) const
{
    return this->find(prefix) == 0;
}

boolean String::hasPostfix(const character* postfix) const
{
    size_t pos = this->rfind(postfix);
    if (pos == std::string::npos) {
        return false;
    }

    count length = strlen(postfix);
    return pos == (this->length() - length);
}

count String::indexOfFirstOccurenceOf(const String& other) const
{
    return this->indexOfFirstOccurenceOf(other.toUTF8());
}

count String::indexOfLastOccurenceOf(const String& other) const
{
    return this->indexOfLastOccurenceOf(other.toUTF8());
}

count String::indexOfFirstOccurenceOf(const char* other) const
{
    size_t pos = this->find(other);
    if (pos == std::string::npos) {
        return this->length();
    }

    return pos;
}

count String::indexOfLastOccurenceOf(const char*  other) const
{
    size_t pos = this->rfind(other);
    if (pos == std::string::npos) {
        return this->length();
    }

    return pos;
}
