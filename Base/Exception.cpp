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

#include "Base/Exception.hpp"
#include "Base/Types.hpp"

#include <new>

#include <cstdlib>
#include <cstdarg>
#include <cstdio>
#include <cstring>

using namespace NxA;
using namespace std;

static const char* stringToUseInCaseOfCompleteFailure = "Complete failure. Can't recover.";

#pragma mark Constructors & Destructors

Exception::Exception(const char* format, ...) : p_reason(nullptr)
{
    constexpr size_t formatStringBufferSize = 256;
    char buffer[formatStringBufferSize];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, formatStringBufferSize, format, args);
    va_end(args);

    count length = strlen(buffer);
    this->p_reason = new(std::nothrow) char[length];
    if (this->p_reason == nullptr) {
        this->p_reason = stringToUseInCaseOfCompleteFailure;
    }
    else {
        strcpy(const_cast<char*>(this->p_reason), buffer);
    }
}

Exception::~Exception()
{
    if (this->p_reason != stringToUseInCaseOfCompleteFailure) {
        delete[] this->p_reason;
    }

    this->p_reason = nullptr;
}

#pragma mark Instance Methods

const char* Exception::what() const throw()
{
    return this->p_reason;
}
