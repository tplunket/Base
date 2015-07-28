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

using namespace NxA;

#pragma mark SBox Hash Implementation
    // SBox Hashing algorithm; source: http://floodyberry.com/noncryptohashzoo/SBox.html

const uinteger32 SBoxTable[256] = {
    0x4660c395, 0x3baba6c5, 0x27ec605b, 0xdfc1d81a, 0xaaac4406, 0x3783e9b8, 0xa4e87c68, 0x62dc1b2a,
    0xa8830d34, 0x10a56307, 0x4ba469e3, 0x54836450, 0x1b0223d4, 0x23312e32, 0xc04e13fe, 0x3b3d61fa,
    0xdab2d0ea, 0x297286b1, 0x73dbf93f, 0x6bb1158b, 0x46867fe2, 0xb7fb5313, 0x3146f063, 0x4fd4c7cb,
    0xa59780fa, 0x9fa38c24, 0x38c63986, 0xa0bac49f, 0xd47d3386, 0x49f44707, 0xa28dea30, 0xd0f30e6d,
    0xd5ca7704, 0x934698e3, 0x1a1ddd6d, 0xfa026c39, 0xd72f0fe6, 0x4d52eb70, 0xe99126df, 0xdfdaed86,
    0x4f649da8, 0x427212bb, 0xc728b983, 0x7ca5d563, 0x5e6164e5, 0xe41d3a24, 0x10018a23, 0x5a12e111,
    0x999ebc05, 0xf1383400, 0x50b92a7c, 0xa37f7577, 0x2c126291, 0x9daf79b2, 0xdea086b1, 0x85b1f03d,
    0x598ce687, 0xf3f5f6b9, 0xe55c5c74, 0x791733af, 0x39954ea8, 0xafcff761, 0x5fea64f1, 0x216d43b4,
    0xd039f8c1, 0xa6cf1125, 0xc14b7939, 0xb6ac7001, 0x138a2eff, 0x2f7875d6, 0xfe298e40, 0x4a3fad3b,
    0x066207fd, 0x8d4dd630, 0x96998973, 0xe656ac56, 0xbb2df109, 0x0ee1ec32, 0x03673d6c, 0xd20fb97d,
    0x2c09423c, 0x093eb555, 0xab77c1e2, 0x64607bf2, 0x945204bd, 0xe8819613, 0xb59de0e3, 0x5df7fc9a,
    0x82542258, 0xfb0ee357, 0xda2a4356, 0x5c97ab61, 0x8076e10d, 0x48e4b3cc, 0x7c28ec12, 0xb17986e1,
    0x01735836, 0x1b826322, 0x6602a990, 0x7c1cef68, 0xe102458e, 0xa5564a67, 0x1136b393, 0x98dc0ea1,
    0x3b6f59e5, 0x9efe981d, 0x35fafbe0, 0xc9949ec2, 0x62c765f9, 0x510cab26, 0xbe071300, 0x7ee1d449,
    0xcc71beef, 0xfbb4284e, 0xbfc02ce7, 0xdf734c93, 0x2f8cebcd, 0xfeedc6ab, 0x5476ee54, 0xbd2b5ff9,
    0xf4fd0352, 0x67f9d6ea, 0x7b70db05, 0x5a5f5310, 0x482dd7aa, 0xa0a66735, 0x321ae71f, 0x8e8ad56c,
    0x27a509c3, 0x1690b261, 0x4494b132, 0xc43a42a7, 0x3f60a7a6, 0xd63779ff, 0xe69c1659, 0xd15972c8,
    0x5f6cdb0c, 0xb9415af2, 0x1261ad8d, 0xb70a6135, 0x52ceda5e, 0xd4591dc3, 0x442b793c, 0xe50e2dee,
    0x6f90fc79, 0xd9ecc8f9, 0x063dd233, 0x6cf2e985, 0xe62cfbe9, 0x3466e821, 0x2c8377a2, 0x00b9f14e,
    0x237c4751, 0x40d4a33b, 0x919df7e8, 0xa16991a4, 0xc5295033, 0x5c507944, 0x89510e2b, 0xb5f7d902,
    0xd2d439a6, 0xc23e5216, 0xd52d9de3, 0x534a5e05, 0x762e73d4, 0x3c147760, 0x2d189706, 0x20aa0564,
    0xb07bbc3b, 0x8183e2de, 0xebc28889, 0xf839ed29, 0x532278f7, 0x41f8b31b, 0x762e89c1, 0xa1e71830,
    0xac049bfc, 0x9b7f839c, 0x8fd9208d, 0x2d2402ed, 0xf1f06670, 0x2711d695, 0x5b9e8fe4, 0xdc935762,
    0xa56b794f, 0xd8666b88, 0x6872c274, 0xbc603be2, 0x2196689b, 0x5b2b5f7a, 0x00c77076, 0x16bfa292,
    0xc2f86524, 0xdd92e83e, 0xab60a3d4, 0x92daf8bd, 0x1fe14c62, 0xf0ff82cc, 0xc0ed8d0a, 0x64356e4d,
    0x7e996b28, 0x81aad3e8, 0x05a22d56, 0xc4b25d4f, 0x5e3683e5, 0x811c2881, 0x124b1041, 0xdb1b4f02,
    0x5a72b5cc, 0x07f8d94e, 0xe5740463, 0x498632ad, 0x7357ffb1, 0x0dddd380, 0x3d095486, 0x2569b0a9,
    0xd6e054ae, 0x14a47e22, 0x73ec8dcc, 0x004968cf, 0xe0c3a853, 0xc9b50a03, 0xe1b0eb17, 0x57c6f281,
    0xc9f9377d, 0x43e03612, 0x9a0c4554, 0xbb2d83ff, 0xa818ffee, 0xf407db87, 0x175e3847, 0x5597168f,
    0xd3d547a7, 0x78f3157c, 0xfc750f20, 0x9880a1c6, 0x1af41571, 0x95d01dfc, 0xa3968d62, 0xeae03cf8,
    0x02ee4662, 0x5f1943ff, 0x252d9d1c, 0x6b718887, 0xe052f724, 0x4cefa30b, 0xdcc31a00, 0xe4d0024d,
    0xdbb4534a, 0xce01f5c8, 0x0c072b61, 0x5d59736a, 0x60291da4, 0x1fbe2c71, 0x2f11d09c, 0x9dce266a,
};
 
inline uinteger32 SBox(const byte *key, count len, uinteger32 seed) {
    uinteger32 h = seed + (uinteger32)len;
    for ( ; len & ~1; len -= 2, key += 2 )
        h = ( ( ( h ^ SBoxTable[key[0]] ) * 3 ) ^ SBoxTable[key[1]] ) * 3;
    if ( len & 1 )
        h = ( h ^ SBoxTable[key[0]] ) * 3;
    h += ( h >> 22 ) ^ ( h << 4 );
    return h;
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

String::Pointer String::stringWith(const character* other, count length)
{
    auto newString = String::makeShared();
    newString->std::string::operator=(std::string(other, length));

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

#pragma mark Class Methods
uinteger32 String::hashFor(String::Pointer str)
{
    return SBox((const byte*)str->toUTF8(), str->length(), uinteger32(-1));
}

uinteger32 String::hashFor(const character* str)
{
    return SBox((const byte*)str, lengthOf(str), uinteger32(-1));
}

count String::lengthOf(const character* str)
{
    const character* end = str;
    while (*end)
    {
        ++end;
    }

    return end - str;
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

uinteger32 String::hash(void) const
{
    return String::hashFor(this->toUTF8());
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

