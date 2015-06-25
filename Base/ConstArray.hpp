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

#pragma once

#include "Base/Types.hpp"
#include "Base/ConstPointer.hpp"
#include "Base/Pointer.hpp"
#include "Base/WeakPointer.hpp"

#include <vector>

namespace NxA {
    template <class T> class ConstArray : private std::vector<typename T::ConstPointer> {
    protected:
        struct constructor_access { };

        #pragma mark Constructors & Destructors
        ConstArray() { }

    public:
        using ConstPointer = NxA::ConstPointer<ConstArray>;
        using Pointer = NxA::Pointer<ConstArray>;
        using WeakPointer = NxA::WeakPointer<ConstArray>;
        using WeakConstPointer = NxA::WeakConstPointer<ConstArray>;
        using iterator = typename std::vector<typename T::ConstPointer>::iterator;
        using const_iterator = typename std::vector<typename T::ConstPointer>::const_iterator;

        #pragma mark Constructors & Destructors
        explicit ConstArray(const constructor_access&) : ConstArray() { };

        #pragma mark Factory Methods
        static ConstArray::Pointer array(void)
        {
            return ConstArray::Pointer(std::make_shared<ConstArray>(ConstArray::constructor_access()));
        }

        #pragma mark Operators
        const T& operator[] (integer index) const
        {
            return (*this)[index];
        }
        T& operator[] (integer index)
        {
            return const_cast<T&>(static_cast<const ConstArray<T>>(*this)[index]);
        }

        #pragma mark Instance Methods
        iterator begin() noexcept
        {
            return this->std::vector<typename T::ConstPointer>::begin();
        }
        const_iterator begin() const noexcept
        {
            return this->std::vector<typename T::ConstPointer>::begin();
        }
        iterator end() noexcept
        {
            return this->std::vector<typename T::ConstPointer>::end();
        }
        const_iterator end() const noexcept
        {
            return this->std::vector<typename T::ConstPointer>::end();
        }
        iterator cbegin() noexcept
        {
            return this->std::vector<typename T::ConstPointer>::cbegin();
        }
        const_iterator cbegin() const noexcept
        {
            return this->std::vector<typename T::ConstPointer>::cbegin();
        }
        iterator cend() noexcept
        {
            return this->std::vector<typename T::ConstPointer>::cend();
        }
        const_iterator cend() const noexcept
        {
            return this->std::vector<typename T::ConstPointer>::cend();
        }

        count length(void) const
        {
            return this->size();
        }

        void append(typename T::ConstPointer const& object)
        {
            this->push_back(object);
        }
    };
}
