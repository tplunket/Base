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
#include <memory>
#include <mutex>

namespace NxA {
    // -- This is a utility function to return the description of the content of an array.
    NxA::Pointer<NxA::String> descriptionOfObjectsInArray(const Object::ArrayOfConst& array, const void* originalArrayAddress);

    template <class T> class ArrayContainer : public Object, private std::vector<Pointer<T>> {
        NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_BASE_CLASS(NxA, ArrayContainer<T>);

    public:
        using iterator = typename std::vector<NxA::Pointer<T>>::iterator;
        using const_iterator = typename std::vector<NxA::Pointer<T>>::const_iterator;

        #pragma mark Factory Methods
        static ArrayContainer<T>::Pointer array(void)
        {
            return ArrayContainer<T>::makeShared();
        }
        static ArrayContainer<T>::Pointer arrayWith(const ArrayContainer& other)
        {
            auto result = ArrayContainer<T>::array();
            for (auto& object : other) {
                result->append(object);
            }
            return result;
        }

        #pragma mark Class Methods
        static const character* staticClassName(void)
        {
            static std::mutex m;
            static std::unique_ptr<character[]> buffer;

            m.lock();

            if (!buffer.get()) {
                const character *format = "NxA::Array<%s>";
                count needed = snprintf(NULL, 0, format, T::staticClassName()) + 1;
                buffer = std::make_unique<character[]>(needed);
                snprintf(buffer.get(), needed, format, T::staticClassName());
            }

            m.unlock();

            return buffer.get();
        }

        #pragma mark Operators
        const T& operator[] (count index) const
        {
            NXA_ASSERT_TRUE(index >= 0 && index < this->length());
            return *(this->std::vector<NxA::Pointer<T>>::operator[](index));
        }
        T& operator[] (count index)
        {
            return const_cast<T&>(static_cast<const ArrayContainer<T>*>(this)->operator[](index));
        }
        bool operator==(const ArrayContainer<T>& other) const
        {
            if (this == &other) {
                return true;
            }
            else if (this->length() != other.length()) {
                return false;
            }

            for (count i = 0; i < this->length(); ++i) {
                if (!(this->ArrayContainer<T>::operator[](i).T::operator==(other.ArrayContainer<T>::operator[](i)))) {
                    return false;
                }
            }

            return true;
        }
        bool operator!=(const ArrayContainer<T>& other) const
        {
            return !this->operator==(other);
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
        void append(const ArrayContainer<T>& other)
        {
            for (auto& object : other) {
                this->append(object);
            }
        }

        const T& firstObject(void) const
        {
            NXA_ASSERT_TRUE(this->length() != 0);
            return *(this->std::vector<NxA::Pointer<T>>::operator[](0));
        }
        T& firstObject(void)
        {
            return const_cast<T&>((static_cast<const ArrayContainer<T>*>(this))->firstObject());
        }
        const T& lastObject(void) const
        {
            count length = this->length();
            NXA_ASSERT_TRUE(length != 0);
            return *(this->std::vector<NxA::Pointer<T>>::operator[](length - 1));
        }
        T& lastObject(void)
        {
            return const_cast<T&>((static_cast<const ArrayContainer<T>*>(this))->lastObject());
        }
        void insertAt(T& object, const_iterator pos)
        {
            this->insert(pos, object.pointer());
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
        void remove(const T& object)
        {
            this->std::vector<NxA::Pointer<T>>::erase(this->find(object));
        }

        #pragma mark Overridden Object Instance Methods
        virtual NxA::Pointer<NxA::String> description(void) const override
        {
            auto tempArray = Object::ArrayOfConst::array();
            for (auto& item : *this) {
                tempArray->append(item->pointer());
            }

            const void* originalArrayAddress = static_cast<const void*>(this);
            return descriptionOfObjectsInArray(tempArray, originalArrayAddress);
        }
    };
}
