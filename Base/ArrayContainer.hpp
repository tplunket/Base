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

#include "Base/Object.hpp"

#include <vector>
#include <algorithm>

namespace NxA {
    template <class T> class ArrayContainer : public Object, private std::vector<Pointer<T>> {
        NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_BASE_CLASS(NxA, ArrayContainer<T>);

    public:
        using iterator = typename std::vector<NxA::Pointer<T>>::iterator;
        using const_iterator = typename std::vector<NxA::Pointer<T>>::const_iterator;

        #pragma mark Factory Methods
        static ArrayContainer::Pointer array(void)
        {
            return ArrayContainer<T>::makeShared();
        }

        #pragma mark Operators
        const T& operator[] (integer index) const
        {
            NXA_ASSERT_TRUE(index >= 0 && index < this->length());
            return *(this->std::vector<NxA::Pointer<T>>::operator[](index));
        }
        T& operator[] (integer index)
        {
            return const_cast<T&>((static_cast<const ArrayContainer<T>&>(*this))[index]);
        }
        bool operator==(const ArrayContainer<T>& other) const
        {
            if (this == &other) {
                return true;
            }
            else if (this->length() != other.length()) {
                return false;
            }

            for (integer i = 0; i < this->length(); ++i) {
                if ((*this)[i] != other[i]) {
                    return false;
                }
            }

            return true;
        }

        #pragma mark Instance Methods
        iterator begin() noexcept
        {
            return this->std::vector<NxA::Pointer<T>>::begin();
        }
        const_iterator begin() const noexcept
        {
            return this->std::vector<NxA::Pointer<T>>::begin();
        }
        iterator end() noexcept
        {
            return this->std::vector<NxA::Pointer<T>>::end();
        }
        const_iterator end() const noexcept
        {
            return this->std::vector<NxA::Pointer<T>>::end();
        }
        iterator cbegin() noexcept
        {
            return this->std::vector<NxA::Pointer<T>>::cbegin();
        }
        const_iterator cbegin() const noexcept
        {
            return this->std::vector<NxA::Pointer<T>>::cbegin();
        }
        iterator cend() noexcept
        {
            return this->std::vector<NxA::Pointer<T>>::cend();
        }
        const_iterator cend() const noexcept
        {
            return this->std::vector<NxA::Pointer<T>>::cend();
        }

        count length(void) const
        {
            return this->size();
        }
        void emptyAll(void)
        {
            return this->clear();
        }

        void append(T& object)
        {
            this->push_back(object.pointer());
        }

        void insertAt(T& object, const_iterator pos)
        {
            this->insert(pos, object.pointer());
        }
        const_iterator find(const T& object) const
        {
            return std::find(this->begin(), this->end(), object);
        }
        void remove(const_iterator objectPosition)
        {
            this->erase(objectPosition);
        }
        void remove(const T& object)
        {
            this->std::vector<NxA::Pointer<T>>::erase(this->find(object));
        }
    };
}
