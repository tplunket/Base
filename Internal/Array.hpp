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

// -- This should be included after our own includes but some names clash with its contents.
#include <vector>

#include "Base/Assert.hpp"
#include "Base/Types.hpp"
#include "Base/String.hpp"
#include "Base/Internal/Object.hpp"

namespace NxA {

#pragma mark Forward Declarations

template <class T> class ArrayInternal;

#pragma mark Utility Methods

// -- This is a utility function to return the description of the content of an array.
template <class T> String descriptionOfObjectsInArrayAtAddress(const ArrayInternal<T>&);

#pragma mark Class

template <class T> struct ArrayInternal : public Object::Internal, public std::vector<T>
{
    #pragma mark Constructors/Destructors
    ArrayInternal() : std::vector<T>() { }
    ArrayInternal(const ArrayInternal& other) : std::vector<T>{ other } { }
    ArrayInternal(std::vector<T>&& other) : std::vector<T>{ std::move(other) } { }
    virtual ~ArrayInternal() = default;

    #pragma mark Iterators
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    #pragma mark Operators
    T operator[] (count index) const
    {
        NXA_ASSERT_TRUE(index >= 0 && index < this->length());
        return this->operator[](index);
    }

    #pragma mark Instance Methods
    iterator begin() noexcept
    {
        return this->begin();
    }
    const_iterator begin() const noexcept
    {
        return this->begin();
    }
    iterator end() noexcept
    {
        return this->end();
    }
    const_iterator end() const noexcept
    {
        return this->end();
    }
    const_iterator cbegin() const noexcept
    {
        return this->cbegin();
    }
    const_iterator cend() const noexcept
    {
        return this->cend();
    }

    count length() const
    {
        return this->size();
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
        this->push_back(object);
    }
    void append(const ArrayInternal<T>& other)
    {
        this->push_back(other);
    }

    T firstObject() const
    {
        NXA_ASSERT_TRUE(this->size() != 0);
        return this->operator[](0);
    }
    T lastObject() const
    {
        count length = this->size();
        NXA_ASSERT_TRUE(length != 0);
        return this->operator[](length - 1);
    }
    void insertAt(T object, const_iterator pos)
    {
        this->insert(pos, object);
    }
    boolean contains(const T& object) const
    {
        return this->find(object) != this->end();
    }
    const_iterator find(const T& object) const
    {
        return std::find(this->begin(), this->end(), object);
    }
    void removeObjectAt(const_iterator objectPosition)
    {
        this->erase(objectPosition);
    }

    String description() const
    {
        return descriptionOfObjectsInArray(*this);
    }
};

}
