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

#include "Base/String.hpp"

namespace NxA {

class DescriberState
{
    int indent_ = 0;

    DescriberState(int indent) : indent_{indent} {}

public:

    DescriberState() : indent_{0} {}

    DescriberState increaseIndent() const
    {
        return DescriberState{indent_ + 1};
    }

    String indentedLine(String line) const
    {
        return String::stringWithFormat("%s%s\n", String::stringWithRepeatedCharacter(indent_ * 4, ' '), line);
    }
};

template<typename T>
class HasDescriptionMember
{
    static const DescriberState& state;
    template<typename U> static constexpr auto test(int) -> decltype(std::declval<U>().description(state), std::true_type{});
    template<typename> static constexpr std::false_type test(...);

public:
    using type = decltype(test<T>(0));
    static constexpr bool value = type::value;
};


// -- Template used to find the description for a type
template<typename T, typename Enable = void>
struct Describer; // -- trying to use this template means T isn't supported 

template<typename T>
struct Describer<T, std::enable_if_t<HasDescriptionMember<T>::value>>
{
    static String describeWithState(const T & item, const DescriberState& state)
    {
        return item.description(state);
    }
};

template<typename T>
struct Describer<T, std::enable_if_t<std::is_integral<typename std::decay_t<T>>::value | std::is_floating_point<typename std::decay_t<T>>::value>>
{
    static String describeWithState(const T & item, const DescriberState& state)
    {
        return String{std::to_string(item)};
    }
};

template<typename T>
struct Describer<T, std::enable_if_t<std::is_pointer<T>::value>>
{
    static String describeWithState(const T & item, const DescriberState& state)
    {
        return String{std::to_string(static_cast<uinteger64>(item))};
    }
};

template <typename T>
struct Describer<std::shared_ptr<T>>
{
    static String describeWithState(std::shared_ptr<T> item, const DescriberState& state)
    {
        auto innerState = state.increaseIndent();
        if (!item) {
            return innerState.indentedLine("<shared_ptr::nullptr />");
        }
        return Describer<T>::describeWithState(*item, innerState);
    }
};

template <int i>
struct Describer<dec::decimal<i>>
{
    static String describeWithState(dec::decimal<i> item, const DescriberState& state)
    {
        return String{dec::toString(item)};
    }
};

template <typename T>
struct Describer<Optional<T>>
{
    static String describeWithState(Optional<T> item, const DescriberState& state)
    {
        auto innerState = state.increaseIndent();
        if (!item) {
            return innerState.indentedLine("<Optional::Empty />");
        }
        return Describer<T>::describeWithState(*item, innerState);
    }
};

template <typename T>
String describe(T item, const DescriberState& state)
{
    return Describer<T>::describeWithState(item, state);
}

template <typename T>
String describe(T item)
{
    const DescriberState& state{};
    return Describer<T>::describeWithState(item, state);
}

}
