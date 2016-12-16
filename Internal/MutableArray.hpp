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

#include "Base/Assert.hpp"
#include "Base/Describe.hpp"
#include "Base/Types.hpp"
#include "Base/MutableString.hpp"
#include "Base/Internal/Object.hpp"

#include <vector>
#include <initializer_list>

namespace NxA {

// -- Forward Declarations

template <class T> class MutableArrayInternal;

// -- Utility Methods

// -- This is a utility function to return the description of the content of an array.
template <class T> String descriptionOfObjectsInArray(const MutableArrayInternal<T>&);

// -- Class

template <class T> struct MutableArrayInternal : public Object::Internal, public std::vector<T>
{
    // -- Constructors/Destructors
    MutableArrayInternal() : std::vector<T>() { }
    MutableArrayInternal(const MutableArrayInternal& other) : std::vector<T>{ other } { }
    MutableArrayInternal(std::vector<T>&& other) : std::vector<T>{ std::move(other) } { }
    MutableArrayInternal(std::initializer_list<T> other) : std::vector<T>{other.begin(), other.end()} { }
    template<typename V, typename = std::enable_if_t<std::is_convertible<V, T>::value>>
    MutableArrayInternal(const MutableArrayInternal<V>& other) : std::vector<T>{other.begin(), other.end()} { }

    virtual ~MutableArrayInternal() = default;

    // -- Iterators
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    // -- Operators
    const T& operator[](count index) const
    {
        NXA_ASSERT_TRUE(index >= 0 && index < this->length());
        return this->std::vector<T>::operator[](index);
    }
    T& operator[](count index)
    {
        NXA_ASSERT_TRUE(index >= 0 && index < this->length());
        return this->std::vector<T>::operator[](index);
    }

    // -- Instance Methods
    iterator begin() noexcept
    {
        return this->std::vector<T>::begin();
    }
    const_iterator begin() const noexcept
    {
        return this->std::vector<T>::begin();
    }
    iterator end() noexcept
    {
        return this->std::vector<T>::end();
    }
    const_iterator end() const noexcept
    {
        return this->std::vector<T>::end();
    }
    const_iterator cbegin() const noexcept
    {
        return this->std::vector<T>::cbegin();
    }
    const_iterator cend() const noexcept
    {
        return this->std::vector<T>::cend();
    }

    count length() const
    {
        return this->size();
    }
    void reserve(count amount)
    {
        this->std::vector<T>::reserve(amount);
    }
    void remove(const T& object)
    {
        auto position = this->find(object);
        if (position != this->cend()) {
            this->erase(position);
        }
    }
    void removeAll()
    {
        return this->clear();
    }

    void append(T object)
    {
        this->emplace_back(object);
    }
    void append(MutableArrayInternal<T> other)
    {
        for (auto object : other) {
            this->emplace_back(object);
        }
    }
    template <class... ConstructorArguments>
    void emplaceAppend(ConstructorArguments &&... arguments)
    {
        this->emplace_back(std::forward<ConstructorArguments>(arguments)...);
    }

    void insertAt(T object, const_iterator pos)
    {
        this->std::vector<T>::emplace(pos, object);
    }

    const T& firstObject() const
    {
        NXA_ASSERT_TRUE(this->size() != 0);
        return this->std::vector<T>::operator[](0);
    }
    T& firstObject()
    {
        NXA_ASSERT_TRUE(this->size() != 0);
        return this->std::vector<T>::operator[](0);
    }
    const  T& lastObject() const
    {
        count length = this->size();
        NXA_ASSERT_TRUE(length != 0);
        return this->std::vector<T>::operator[](length - 1);
    }
    T& lastObject()
    {
        count length = this->size();
        NXA_ASSERT_TRUE(length != 0);
        return this->std::vector<T>::operator[](length - 1);
    }
    boolean contains(const T& object) const
    {
        return this->find(object) != this->end();
    }
    const_iterator find(const T& object) const
    {
        return std::find(this->begin(), this->end(), object);
    }
    iterator find(const T& object)
    {
        return std::find(this->begin(), this->end(), object);
    }
    void removeObjectAt(const_iterator objectPosition)
    {
        this->erase(objectPosition);
    }
    void sort()
    {
        std::sort(this->std::vector<T>::begin(), this->std::vector<T>::end());
    }

    String description(const DescriberState& state) const
    {
        auto indented = state.increaseIndent();
        if (this->length() == 0) {
            return indented.indentedLine("<Array length=\"0\" />");
        }
        auto result = MutableString::stringWithFormat(indented.indentedLine("<Array length=\"%ld\">"), this->length());
        for (auto && item : *this) {
            result.append(NxA::describe(item, indented));
        }
        result.append(indented.indentedLine("</Array>"));

        return { std::move(result) };
    }

    // -- Overriden Object::Internal Instance Methods
    uinteger32 classHash() const override
    {
        NXA_ALOG("Illegal call.");
        return 0;
    }
    const character* className() const override
    {
        NXA_ALOG("Illegal call.");
        return nullptr;
    }
};

}
