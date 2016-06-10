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
#include "Base/Internal/String.hpp"
#include "Base/Exception.hpp"
#include "Base/Platform.hpp"
#include "Base/Assert.hpp"

using namespace NxA;

NXA_GENERATED_OBJECT_METHODS_DEFINITIONS_FOR(MutableString);

#pragma mark Constructors/Destructors

MutableString::MutableString() : internal{ std::make_shared<Internal>() } { }

MutableString::MutableString(const String& other) : internal{ std::make_shared<Internal>(*other.internal) } { }

MutableString::MutableString(const std::string& other) : internal{ std::make_shared<Internal>(other) } { }

MutableString::MutableString(const character* other, size_t size) : internal{ std::make_shared<Internal>(other, size) } { }

#pragma mark Factory Methods

MutableString MutableString::stringWithFormat(const character* format, ...)
{
    va_list args;
    va_start(args, format);
    auto result = Internal::stringWithFormat(format, args);
    va_end(args);

    return { std::move(result) };
}

MutableString MutableString::stringWithUTF16(Blob other)
{
    return { Internal::stringWithUTF16(other) };
}

#pragma mark Instance Methods

String MutableString::description(void) const
{
    return String(*internal);
}

count MutableString::length(void) const
{
    return internal->length();
}

uinteger32 MutableString::hash(void) const
{
    return internal->hash();
}

integer MutableString::integerValue(void) const
{
    return internal->integerValue();
}

const std::string& MutableString::asStdString(void) const
{
    return internal->asStdString();
}

const character* MutableString::asUTF8() const
{
    return internal->asUTF8();
}

Blob MutableString::asUTF16(void) const
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

MutableString MutableString::lowerCaseString(void) const
{
    return { internal->lowerCaseString() };
}

MutableString MutableString::upperCaseString(void) const
{
    return { internal->upperCaseString() };
}

boolean MutableString::hasPrefix(const String& prefix) const
{
    return internal->hasPrefix(*prefix.internal);
}

boolean MutableString::hasPostfix(const String& postfix) const
{
    return internal->hasPostfix(*postfix.internal);
}

boolean MutableString::hasPrefix(const character* prefix) const
{
    return internal->hasPrefix(prefix);
}

boolean MutableString::hasPostfix(const character* postfix) const
{
    return internal->hasPostfix(postfix);
}

count MutableString::indexOfFirstOccurenceOf(const String& other) const
{
    return internal->indexOfFirstOccurenceOf(other);
}

count MutableString::indexOfLastOccurenceOf(const String& other) const
{
    return internal->indexOfLastOccurenceOf(other);
}

count MutableString::indexOfFirstOccurenceOf(const character* other) const
{
    return internal->indexOfFirstOccurenceOf(other);
}

count MutableString::indexOfLastOccurenceOf(const character*  other) const
{
    return internal->indexOfLastOccurenceOf(other);
}

void MutableString::replaceOccurenceOfStringWith(const character* occurence, const character* replacement)
{
    return internal->replaceOccurenceOfStringWith(occurence, replacement);
}
