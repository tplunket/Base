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

#include <Base/Assert.hpp>

#include <memory>

namespace NxA {

template <class T> class WeakReference : protected std::weak_ptr<typename T::Internal> {

public:
    #pragma mark Constructors & Destructors
    WeakReference() = default;
    WeakReference(const WeakReference<T>& other) : std::weak_ptr<typename T::Internal>{ other } { }
    WeakReference(WeakReference<T>&&) = default;
    WeakReference(const T& other) : std::weak_ptr<typename T::Internal>{ other.internal } { }
    ~WeakReference() = default;

    #pragma mark Instance Methods
    boolean isValid(void) const
    {
        return !this->expired();
    }

    T get(void) const
    {
        NXA_ASSERT_TRUE(this->isValid());
        return { this->std::weak_ptr<typename T::Internal>::lock() };
    }

    void release(void)
    {
        NXA_ASSERT_TRUE(this->isValid());
        this->std::weak_ptr<typename T::Internal>::reset();
    }
};

}
