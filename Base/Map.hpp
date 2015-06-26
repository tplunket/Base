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

#include <map>

namespace NxA {
    template <class Tkey, class Tvalue> class Map : private std::map<Tkey, typename Tvalue::Pointer> {
    protected:
        struct constructor_access { };

        #pragma mark Constructors & Destructors
        Map() { }

    public:
        using Pointer = NxA::Pointer<Map>;
        using WeakPointer = NxA::WeakPointer<Map>;
        using iterator = typename std::map<Tkey, typename Tvalue::Pointer>::iterator;
        using const_iterator = typename std::map<Tkey, typename Tvalue::Pointer>::const_iterator;

        #pragma mark Constructors & Destructors
        explicit Map(const constructor_access&) : Map() { };

        #pragma mark Factory Methods
        static Map::Pointer map(void)
        {
            return Map::Pointer(std::make_shared<Map>(Map::constructor_access()));
        }

        #pragma mark Operators
        typename Tvalue::Pointer const& operator[] (Tkey const& key) const
        {
            return this->std::map<Tkey, typename Tvalue::Pointer>::at(key);
        }
        typename Tvalue::Pointer& operator[] (Tkey const& key)
        {
            return const_cast<typename Tvalue::Pointer&>(static_cast<const Map<Tkey, Tvalue>>(*this)[key]);
        }

        #pragma mark Instance Methods
        iterator begin() noexcept
        {
            return this->std::map<Tkey, typename Tvalue::Pointer>::begin();
        }
        const_iterator begin() const noexcept
        {
            return this->std::map<Tkey, typename Tvalue::Pointer>::begin();
        }
        iterator end() noexcept
        {
            return this->std::map<Tkey, typename Tvalue::Pointer>::end();
        }
        const_iterator end() const noexcept
        {
            return this->std::map<Tkey, typename Tvalue::Pointer>::end();
        }
        iterator cbegin() noexcept
        {
            return this->std::map<Tkey, typename Tvalue::Pointer>::cbegin();
        }
        const_iterator cbegin() const noexcept
        {
            return this->std::map<Tkey, typename Tvalue::Pointer>::cbegin();
        }
        iterator cend() noexcept
        {
            return this->std::map<Tkey, typename Tvalue::Pointer>::cend();
        }
        const_iterator cend() const noexcept
        {
            return this->std::map<Tkey, typename Tvalue::Pointer>::cend();
        }

        count length(void) const
        {
            return this->size();
        }

        bool containsValueForKey(Tkey const& keyValue) const
        {
            return this->find(keyValue) != this->std::map<Tkey, typename Tvalue::Pointer>::end();
        }
    };
}
