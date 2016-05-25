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

#include "Base/Pointer.hpp"

NXA_ENTER_NAMESPACE(NxA);

// -- NOTE: This pointer type should only be used internally inside internal classes or inside methods.
// --       It should NEVER be used as argument types to methods. It should be used as little as possible
// --       and the regular Pointer should always be prefered to this solution.
template <class T> class NullablePointer : public Pointer<T> {
public:
    #pragma mark Constructors & Destructors
    NullablePointer() { }
    NullablePointer(const Pointer<T>& other) : Pointer<T>(other) { }
    NullablePointer(std::nullptr_t other) : Pointer<T>() { }
    ~NullablePointer() = default;

    #pragma mark Operators
    operator Pointer<const T>() const
    {
        NXA_ASSERT_FALSE(this->isNull());
        return Pointer<T>::operator Pointer<const T>();
    }
    explicit operator bool() const
    {
        return !this->isNull();
    }
    bool operator==(const Pointer<T>& other) const
    {
        NXA_ASSERT_FALSE(this->isNull());
        return Pointer<T>::operator==(other);
    }
    bool operator==(const T& other) const
    {
        NXA_ASSERT_FALSE(this->isNull());
        return Pointer<T>::operator==(other);
    }
    bool operator!=(const Pointer<T>& other) const
    {
        NXA_ASSERT_FALSE(this->isNull());
        return Pointer<T>::operator!=(other);
    }
    bool operator!=(const T& other) const
    {
        NXA_ASSERT_FALSE(this->isNull());
        return Pointer<T>::operator!=(other);
    }
    operator T&() const
    {
        NXA_ASSERT_FALSE(this->isNull());
        return Pointer<T>::operator T&();
    }
    operator T&()
    {
        NXA_ASSERT_FALSE(this->isNull());
        return Pointer<T>::operator T&();
    }
    T* operator->() const
    {
        NXA_ASSERT_FALSE(this->isNull());
        return Pointer<T>::operator->();
    }
    T* operator->()
    {
        NXA_ASSERT_FALSE(this->isNull());
        return Pointer<T>::operator->();
    }
    T& operator*() const
    {
        NXA_ASSERT_FALSE(this->isNull());
        return Pointer<T>::operator*();
    }
    T& operator*()
    {
        NXA_ASSERT_FALSE(this->isNull());
        return Pointer<T>::operator*();
    }

    #pragma mark Instance Methods
    boolean isNull(void) const
    {
        return this->get() == nullptr;
    }
    void setToNull(void)
    {
        this->std::shared_ptr<T>::reset();
    }

    const std::shared_ptr<T>& toStdSharedPointer(void) const
    {
        NXA_ASSERT_FALSE(this->isNull());
        return Pointer<T>::toStdSharedPointer();
    }
};

NXA_EXIT_NAMESPACE;
