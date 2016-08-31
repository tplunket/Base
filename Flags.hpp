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

namespace NxA {

template <typename FlagsEnum> class FlagsBuilder;

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
    Flags(const FlagsBuilder<FlagsEnum>& other) : flags(other.flags) { }

    #pragma mark Instance Methods
    bool hasAny(FlagsEnum value) const
    {
        return this->flags & (1ULL << static_cast<integer>(value));
    }
    bool hasAny(Flags value) const
    {
        return this->flags & value.flags;
    }

    bool hasAll(FlagsEnum value) const
    {
        uinteger64 test = (1ULL << static_cast<integer>(value));
        return (this->flags & test) == test;
    }
    bool hasAll(Flags value) const
    {
        uinteger64 test = value.flags;
        return (this->flags & test) == test;
    }

    void clear()
    {
        this->flags = 0;
    }

    #pragma mark Operators
    operator bool() const
    {
        return this->flags != 0;
    }

    void operator|=(FlagsEnum e)
    {
        this->flags |= (1ULL << static_cast<integer>(e));
    }
};

template <typename FlagsEnum>
class FlagsBuilder
{
    friend class Flags<FlagsEnum>;

    #pragma mark Instance Variables
    mutable uinteger64 flags;

public:
    #pragma mark Constructors/Destructors
    FlagsBuilder(FlagsEnum value) : flags(1ULL << static_cast<integer>(value)) { }
    
    #pragma mark Operators
    const FlagsBuilder& operator | (FlagsEnum value) const
    {
        flags |= (1ULL << static_cast<integer>(value));
        return *this;
    }

};

}
