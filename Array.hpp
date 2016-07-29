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
#include <Base/MutableArray.hpp>
#include <Base/GeneratedObjectCode.hpp>
#include <Base/Internal/MutableArray.hpp>

#include <algorithm>
#include <vector>
#include <mutex>
#include <memory>

namespace NxA {

#pragma mark Class

template <class T> class Array {
    NXA_GENERATED_INTERNAL_OBJECT_FORWARD_DECLARATION_USING(MutableArrayInternal<T>);

    std::shared_ptr<MutableArrayInternal<T>> internal;

    friend class MutableArray<T>;

public:
    #pragma mark Constructors/Destructors
    Array() : internal{ std::make_shared<MutableArrayInternal<T>>() } { }
    Array(const Array& other) : internal{ other.internal } {}
    Array(Array&& other) : internal{ std::move(other.internal) } {}
    Array(const MutableArray<T>& other) : internal{ std::make_shared<MutableArrayInternal<T>>(*other.internal) } { }
    Array(MutableArray<T>&& other) : internal{ std::move(other.internal) } { }
    template<class InputIt>
    Array(InputIt first, InputIt last) : internal{ std::make_shared<MutableArrayInternal<T>>(first, last) } { }
    ~Array() {}

    #pragma mark Class Methods
    static const character* staticClassName()
    {
        static std::unique_ptr<character[]> buffer;
        auto className = buffer.get();
        if (className) {
            // This is the fast lock-free path for the common case (unique_ptr engaged)
            return className;
        }

        static std::mutex m;
        std::lock_guard<std::mutex> guard(m);

        // now under guard, this is the slow-and-safe path. We have to re-check get() in case we lost a race to the lock.
        if (!buffer.get()) {
            const character* format = "Array<%s>";
            const character* valueTypeName = TypeName<T>::get();
            count needed = snprintf(NULL, 0, format, valueTypeName) + 1;
            buffer = std::make_unique<character[]>(needed);
            snprintf(buffer.get(), needed, format, valueTypeName);
        }

        return buffer.get();
    }

    static uinteger32 staticClassHash()
    {
        static uinteger32 result = String::hashFor(Array::staticClassName());
        return result;
    }

    #pragma mark Iterators
    using iterator = typename MutableArrayInternal<T>::iterator;
    using const_iterator = typename MutableArrayInternal<T>::const_iterator;

    #pragma mark Operators
    Array& operator=(Array&& other) { internal = std::move(other.internal); return *this; }
    Array& operator=(const Array& other) { internal = other.internal; return *this; }
    bool operator==(const Array& other) const
    {
        if (internal == other.internal) {
            return true;
        }

        return *internal == *(other.internal);
    }
    bool operator!=(const Array& other) const
    {
        return !this->operator==(other);
    }
    bool operator==(const MutableArray<T>& other) const
    {
        if (internal == other.internal) {
            return true;
        }

        return *internal == *(other.internal);
    }
    bool operator!=(const MutableArray<T>& other) const
    {
        return !this->operator==(other);
    }
    const T& operator[](count index) const
    {
        return internal->operator[](index);
    }
    T& operator[](count index)
    {
        return internal->operator[](index);
    }

    #pragma mark Instance Methods
    uinteger32 classHash() const
    {
        return Array::staticClassHash();
    }
    const character* className() const
    {
        return Array::staticClassName();
    }
    boolean classNameIs(const character* className) const
    {
        return !::strcmp(Array::staticClassName(), className);
    }
    iterator begin() noexcept
    {
        return internal->begin();
    }
    const_iterator begin() const noexcept
    {
        return internal->begin();
    }
    iterator end() noexcept
    {
        return internal->end();
    }
    const_iterator end() const noexcept
    {
        return internal->end();
    }
    const_iterator cbegin() const noexcept
    {
        return internal->cbegin();
    }
    const_iterator cend() const noexcept
    {
        return internal->cend();
    }

    count length() const
    {
        return internal->length();
    }

    const T& firstObject() const
    {
        return internal->firstObject();
    }
    T& firstObject()
    {
        return internal->firstObject();
    }
    const T& lastObject() const
    {
        return internal->lastObject();
    }
    T& lastObject()
    {
        return internal->lastObject();
    }
    boolean contains(const T& object) const
    {
        return internal->contains(object);
    }
    iterator find(const T& object)
    {
        return internal->find(object);
    }
    const_iterator find(const T& object) const
    {
        return internal->find(object);
    }

    String description() const
    {
        return internal->description();
    }
};

}
