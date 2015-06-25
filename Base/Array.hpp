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
#include "Base/WeakConstPointer.hpp"

#include <vector>

namespace NxA {
    template <class T> class Array : private std::vector<typename T::Pointer> {
    protected:
        struct constructor_access { };

        #pragma mark Constructors & Destructors
        Array() { }

    public:
        using ConstPointer = NxA::ConstPointer<Array>;
        using Pointer = NxA::Pointer<Array>;
        using WeakPointer = NxA::WeakPointer<Array>;
        using WeakConstPointer = NxA::WeakConstPointer<Array>;
        using iterator = typename std::vector<typename T::Pointer>::iterator;
        using const_iterator = typename std::vector<typename T::Pointer>::const_iterator;

        #pragma mark Constructors & Destructors
        explicit Array(const constructor_access&) : Array() { };

        #pragma mark Factory Methods
        static Array::Pointer array(void)
        {
            return Array::Pointer(std::make_shared<Array>(Array::constructor_access()));
        }

        #pragma mark Operators
        typename T::Pointer const& operator[] (integer index) const
        {
            return this->std::vector<typename T::Pointer>::operator[](index);
        }
        typename T::Pointer& operator[] (integer index)
        {
            return const_cast<typename T::Pointer&>(static_cast<const Array<T>>(*this)[index]);
        }

        #pragma mark Instance Methods
        iterator begin() noexcept
        {
            return this->std::vector<typename T::Pointer>::begin();
        }
        const_iterator begin() const noexcept
        {
            return this->std::vector<typename T::Pointer>::begin();
        }
        iterator end() noexcept
        {
            return this->std::vector<typename T::Pointer>::end();
        }
        const_iterator end() const noexcept
        {
            return this->std::vector<typename T::Pointer>::end();
        }
        iterator cbegin() noexcept
        {
            return this->std::vector<typename T::Pointer>::cbegin();
        }
        const_iterator cbegin() const noexcept
        {
            return this->std::vector<typename T::Pointer>::cbegin();
        }
        iterator cend() noexcept
        {
            return this->std::vector<typename T::Pointer>::cend();
        }
        const_iterator cend() const noexcept
        {
            return this->std::vector<typename T::Pointer>::cend();
        }

        count length(void) const
        {
            return this->size();
        }
        void emptyAll(void)
        {
            return this->clear();
        }

        void append(typename T::Pointer const& object)
        {
            this->push_back(object);
        }
        void insertAt(typename T::Pointer const& object, iterator pos)
        {
            this->insert(pos, object);
        }
        iterator find(typename T::Pointer const& object)
        {
            for (iterator it = this->begin(); it != this->end(); ++it) {
                auto& other = *it;
                if (other.toStdSharedPointer() == object.toStdSharedPointer()) {
                    return it;
                }
            }

            return this->end();
        }
        void remove(iterator objectPosition)
        {
            this->erase(objectPosition);
        }
        void remove(typename T::Pointer const& object)
        {
            this->std::vector<typename T::Pointer>::remove(this->find(object));
        }
    };
}
