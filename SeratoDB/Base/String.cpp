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
#include "Base/Internal/String.hpp"

#include <cstdio>

using namespace NxA::Serato;

NXA_GENERATED_IMPLEMENTATION_FOR(String);

#pragma mark mark Constructors & Destructors

String::String() : internal(Internal::String::makeShared()) { }

#pragma mark Factory Methods

String::Pointer String::stringWithCharPointer(const char* other)
{
    String::Pointer newString(String::makeShared());
    newString->internal->value = other;

    return newString;
}

String::Pointer String::stringWithStdString(const std::string& other)
{
    String::Pointer newString(String::makeShared());
    newString->internal->value = other;

    return newString;
}

String::Pointer String::stringWithFormat(const char* format, ...)
{
    static integer formatStringBufferSize = 256;
    char buffer[formatStringBufferSize];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, formatStringBufferSize, format, args);
    va_end(args);

    return String::stringWithCharPointer(buffer);
}

#pragma mark mark Instance Methods

String::Pointer String::copy(void) const
{
    String::Pointer newString{ String::makeShared() };
    newString->internal->value = internal->value;

    return newString;
}

const String::Pointer String::description(void) const
{
    return this->copy();
}

const char* String::toCharPointer(void) const
{
    return internal->value.c_str();
}

void String::append(const String::Pointer& other)
{
    internal->value.append(other->internal->value);
}

void String::append(const char* other)
{
    internal->value.append(other);
}
