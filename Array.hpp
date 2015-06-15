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
#include "Base/Pointer.hpp"
#include "Base/WeakPointer.hpp"

#include <vector>

namespace NxA {
    template <class T> class Array {
    protected:
        struct constructor_access { };

        #pragma mark Constructors & Destructors
        Array() { }

    public:
        using Pointer = NxA::Pointer<Array>;
        using WeakPointer = NxA::WeakPointer<Array>;

        #pragma mark Constructors & Destructors
        explicit Array(const constructor_access&) : Array() { };

        #pragma mark Factory Methods
        static Array::Pointer array(void)
        {
            return std::make_shared<Array>(Array::constructor_access());
        }

        #pragma mark Operators
        const T& operator[] (integer index) const
        {
            return this->values[index];
        }
        T& operator[] (integer index)
        {
            return const_cast<T&>(static_cast<const Array<T>>(*this)[index]);
        }

        #pragma mark Instance Methods
        count length(void) const
        {
            return this->values.size();
        }

        void append(typename T::Pointer const& object)
        {
            this->values.push_back(object);
        }

    private:
        #pragma mark Instance Variables
        std::vector<typename T::Pointer> values;
    };
}
