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

#include <map>

namespace NxA {
    template <class Tkey, class Tvalue> class Map : private std::map<Tkey, typename Tvalue::ConstPointer> {
    protected:
        struct constructor_access { };

        #pragma mark Constructors & Destructors
        Map() { }

    public:
        using ConstPointer = NxA::ConstPointer<Map>;
        using Pointer = NxA::Pointer<Map>;
        using WeakPointer = NxA::WeakPointer<Map>;
        using WeakConstPointer = NxA::WeakConstPointer<Map>;

        #pragma mark Constructors & Destructors
        explicit Map(const constructor_access&) : Map() { };

        #pragma mark Factory Methods
        static Map::Pointer map(void)
        {
            return Map::Pointer(std::make_shared<Map>(Map::constructor_access()));
        }

        #pragma mark Operators
        typename Tvalue::ConstPointer const& operator[] (Tkey const& key) const
        {
            return (*this)[key];
        }
        typename Tvalue::ConstPointer& operator[] (Tkey const& key)
        {
            return const_cast<typename Tvalue::ConstPointer&>(static_cast<const Map<Tkey, Tvalue>>(*this)[key]);
        }

        #pragma mark Instance Methods
        typename std::map<Tkey, typename Tvalue::ConstPointer>::iterator begin() noexcept
        {
            return this->begin();
        }
        typename std::map<Tkey, typename Tvalue::ConstPointer>::const_iterator begin() const noexcept
        {
            return this->begin();
        }
        typename std::map<Tkey, typename Tvalue::ConstPointer>::iterator end() noexcept
        {
            return this->end();
        }
        typename std::map<Tkey, typename Tvalue::ConstPointer>::const_iterator end() const noexcept
        {
            return this->end();
        }
        typename std::map<Tkey, typename Tvalue::ConstPointer>::iterator cbegin() noexcept
        {
            return this->cbegin();
        }
        typename std::map<Tkey, typename Tvalue::ConstPointer>::const_iterator cbegin() const noexcept
        {
            return this->cbegin();
        }
        typename std::map<Tkey, typename Tvalue::ConstPointer>::iterator cend() noexcept
        {
            return this->cend();
        }
        typename std::map<Tkey, typename Tvalue::ConstPointer>::const_iterator cend() const noexcept
        {
            return this->cend();
        }

        count length(void) const
        {
            return this->size();
        }

        bool containsValueForKey(Tkey const& keyValue)
        {
            typename std::map<typename Tkey::ConstPointer, typename Tvalue::ConstPointer>::const_iterator it = this->find(keyValue);
            return it != this->end();
        }
    };
}
