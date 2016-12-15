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

#include <Base/Types.hpp>
#include <Base/String.hpp>

namespace NxA {

template <typename FlagsEnum>
class Flags
{
    #pragma mark Instance Variables
    uinteger64 flags;

    static_assert(static_cast<integer>(FlagsEnum::LastFlag) < 64, "E must be an enum class smaller than 64 values with a LastFlag entry larger than any value in the enum");

public:
    #pragma mark Constructors/Destructors
    Flags() : flags(0) { }
    Flags(FlagsEnum value) : flags(1ULL << static_cast<integer>(value)) { }

    #pragma mark Instance Methods
    bool has(FlagsEnum value) const
    {
        return this->flags & (1ULL << static_cast<integer>(value));
    }

    Flags<FlagsEnum> andAlso(FlagsEnum value) const
    {
        Flags result(*this);
        result.flags |= (1ULL << static_cast<integer>(value));
        return result;
    }
    Flags<FlagsEnum> andAlso(const Flags& other) const
    {
        Flags result(*this);
        result.flags |= other.flags;
        return result;
    }
    void set(FlagsEnum value)
    {
        this->flags |= (1ULL << static_cast<integer>(value));
    }
    void clear(FlagsEnum value)
    {
        this->flags &= ~(1ULL << static_cast<integer>(value));
    }
    void clearAll()
    {
        this->flags = 0;
    }

    String description() const
    {
        return String::stringWithFormat("%d", this->flags);
    }

    #pragma mark Operators
    operator bool() const
    {
        return this->flags != 0;
    }
};

}
