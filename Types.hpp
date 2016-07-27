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

#include <ctime>
#include <cstdint>
#include <memory>
#include <typeinfo>
#include <experimental/optional>

#include <Base/Internal/decimal.h>

namespace NxA {

// -- Types used by the codebase.
using boolean = bool;

using uinteger = unsigned int;
using integer = int;

using uinteger8 = uint8_t;
using integer8 = int8_t;
using uinteger16 = uint16_t;
using integer16 = int16_t;
using uinteger32 = uint32_t;
using integer32 = int32_t;
using uinteger64 = uint64_t;
using integer64 = int64_t;

using byte = uint8_t;
using character = char;

using count = std::size_t;

using timestamp = std::time_t;

using decimal2 = dec::decimal<2>;
using decimal3 = dec::decimal<3>;
using decimal = decimal3;

// -- Provide an optional type based on std::experimental::optional. TODO: change to std::optional in C++1y
template <typename T>
using Optional = std::experimental::optional<T>;
using NullOptional = std::experimental::nullopt_t;
constexpr NullOptional nothing{0};
template <typename T>
inline constexpr Optional<typename std::decay<T>::type>
makeOptional(T&& v) {
    return Optional<typename std::decay<T>::type>(std::forward<T>(v));
}

// -- prevent derived types from being copied
class NoCopy
{
protected:
    NoCopy() = default;
    ~NoCopy() = default;

    NoCopy(NoCopy&&) = default;
    NoCopy& operator=(NoCopy&&) = default;

    NoCopy(NoCopy const&) = delete;
    NoCopy& operator=(NoCopy const&) = delete;
};

// -- Template used to find the description for a type
template <typename T>
struct Describe {
    static const character * describe(const T& item) {
        return item.description().asUTF8();
    }
};

template <typename T>
struct Describe<std::shared_ptr<T>> {
    static const character * describe(const std::shared_ptr<T>& item) {
        if (!item) {
            return "-empty shared_ptr-";
        }
        return Describe<T>::describe(*item);
    }
};

template <typename T>
struct Describe<Optional<T>> {
    static const character * describe(const Optional<T>& item) {
        if (!item) {
            return "-empty optional-";
        }
        return Describe<T>::describe(*item);
    }
};

// -- Template used by default to produce the name of unknown types.
template <typename T>
struct TypeName {
    static const character* get() {
        return T::staticClassName();
    }
};

template <typename T>
struct TypeName<std::shared_ptr<T>> {
    static const character* get() {
        return TypeName<T>::get();
    }
};
    
template <typename T>
struct TypeName<std::experimental::optional<T>> {
    static const character* get() {
        return TypeName<T>::get();
    }
};

// -- Specialization for each type we support.
#define NXA_STR_VALUE_FOR_TYPE(arg...) #arg

#define NXA_SPECIALIZE_TYPENAME_FOR_TYPE(name) \
template <> struct TypeName<name> \
{ \
    static const character* get() \
    { \
        return NXA_STR_VALUE_FOR_TYPE(name); \
    } \
};

NXA_SPECIALIZE_TYPENAME_FOR_TYPE(boolean);

// -- Placeholder for NXA_SPECIALIZE_TYPENAME_FOR_TYPE(uinteger) which is the same specialization as uinteger32 on OSX;
// -- Placeholder for NXA_SPECIALIZE_TYPENAME_FOR_TYPE(integer) which is the same specialization as uinteger32 on OSX;

NXA_SPECIALIZE_TYPENAME_FOR_TYPE(integer8);
NXA_SPECIALIZE_TYPENAME_FOR_TYPE(uinteger16);
NXA_SPECIALIZE_TYPENAME_FOR_TYPE(integer16);
NXA_SPECIALIZE_TYPENAME_FOR_TYPE(uinteger32);
NXA_SPECIALIZE_TYPENAME_FOR_TYPE(integer32);
NXA_SPECIALIZE_TYPENAME_FOR_TYPE(uinteger64);
NXA_SPECIALIZE_TYPENAME_FOR_TYPE(integer64);

NXA_SPECIALIZE_TYPENAME_FOR_TYPE(byte);
NXA_SPECIALIZE_TYPENAME_FOR_TYPE(character);

NXA_SPECIALIZE_TYPENAME_FOR_TYPE(count);

NXA_SPECIALIZE_TYPENAME_FOR_TYPE(timestamp);

NXA_SPECIALIZE_TYPENAME_FOR_TYPE(decimal);

}
