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

#include "Base/Types.hpp"
#include "Base/MutableString.hpp"
#include "Base/String.hpp"
#include "Base/Internal/MutableString.hpp"
#include "Base/Exception.hpp"
#include "Base/Platform.hpp"
#include "Base/Assert.hpp"
#include "Base/Array.hpp"

using namespace NxA;

#pragma mark Constructors/Destructors

MutableString::MutableString() : internal{ std::make_shared<Internal>() } { }

MutableString::MutableString(const String& other) : internal{ std::make_shared<Internal>(*other.internal) } { }

MutableString::MutableString(const std::string& other) : internal{ std::make_shared<Internal>(other) } { }

MutableString::MutableString(std::string&& other) : internal{ std::make_shared<Internal>(std::move(other)) } { }

MutableString::MutableString(const character* other, size_t size) : internal{ std::make_shared<Internal>(other, size) } { }

MutableString::MutableString(std::shared_ptr<Internal>&& other) : internal{ std::move(other) } { }

MutableString::~MutableString() = default;

#pragma mark Factory Methods

MutableString MutableString::stringWithUTF16(const Blob& other)
{
    return { Internal::stringWithUTF16(other) };
}


MutableString MutableString::stringWithRepeatedCharacter(count number, character specificChar)
{
    return { Internal::stringWithRepeatedCharacter(number, specificChar) };
}

#pragma mark Operators

bool MutableString::operator==(const String& other) const
{
    return internal->operator==(*other.internal);
}

bool MutableString::operator==(const character* other) const
{
    return internal->operator==(other);
}

boolean MutableString::operator==(const MutableString& other) const
{
    if (internal == other.internal) {
        return true;
    }
    return *internal == *(other.internal);
}

#pragma mark Instance Methods

uinteger32 MutableString::classHash() const
{
    return MutableString::staticClassHash();
}

const character* MutableString::className() const
{
    return MutableString::staticClassName();
}

boolean MutableString::classNameIs(const character* className) const
{
    return !::strcmp(MutableString::staticClassName(), className);
}

uinteger32 MutableString::staticClassHash()
{
    static uinteger32 result = String::hashFor(MutableString::staticClassName());
    return result;
}

String MutableString::description(const DescriberState& state) const
{
    return String(*internal);
}

count MutableString::length() const
{
    return internal->length();
}

uinteger32 MutableString::hash() const
{
    return internal->hash();
}

integer MutableString::integerValue() const
{
    return internal->integerValue();
}

decimal3 MutableString::decimalValue() const
{
    return internal->decimalValue();
}

const std::string& MutableString::asStdString() const
{
    return internal->asStdString();
}

const character* MutableString::asUTF8() const
{
    return internal->asUTF8();
}

Blob MutableString::asUTF16() const
{
    return internal->asUTF16();
}

void MutableString::append(const String& other)
{
    internal->append(*other.internal);
}

void MutableString::append(const character* other)
{
    internal->append(other);
}

void MutableString::append(const character other)
{
    internal->append(other);
}

Array<String> MutableString::splitBySeparator(character separator) const
{
    return internal->splitBySeparator(separator);
}

MutableString MutableString::utfSeek(count skip) const
{
    return { internal->utfSeek(skip) };
}

MutableString MutableString::subString(count start, count end) const
{
    return { internal->subString(start, end) };
}

MutableString MutableString::lowerCaseString() const
{
    return { internal->lowerCaseString() };
}

MutableString MutableString::upperCaseString() const
{
    return { internal->upperCaseString() };
}

boolean MutableString::hasPrefix(const String& prefix) const
{
    return internal->hasPrefix(*prefix.internal);
}

boolean MutableString::hasPrefix(const character* prefix) const
{
    return internal->hasPrefix(prefix);
}

boolean MutableString::hasPostfix(const String& postfix) const
{
    return internal->hasPostfix(*postfix.internal);
}

boolean MutableString::hasPostfix(const character* postfix) const
{
    return internal->hasPostfix(postfix);
}

boolean MutableString::contains(const String& other) const
{
    return internal->contains(*other.internal);
}

boolean MutableString::contains(const character* other) const
{
    return internal->contains(other);
}

boolean MutableString::hasNonPrintableCharacters() const
{
    return internal->hasNonPrintableCharacters();
}

count MutableString::indexOfFirstOccurenceOf(const String& other) const
{
    return internal->indexOfFirstOccurenceOf(other);
}

count MutableString::indexOfFirstOccurenceOf(const character* other) const
{
    return internal->indexOfFirstOccurenceOf(other);
}

count MutableString::indexOfLastOccurenceOf(const String& other) const
{
    return internal->indexOfLastOccurenceOf(other);
}

count MutableString::indexOfLastOccurenceOf(const character*  other) const
{
    return internal->indexOfLastOccurenceOf(other);
}

void MutableString::replaceOccurenceOfStringWith(const character* occurence, const character* replacement)
{
    return internal->replaceOccurenceOfStringWith(occurence, replacement);
}
