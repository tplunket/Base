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

#include "Base/Types.hpp"
#include "Base/String.hpp"
#include "Base/Internal/Object.hpp"

#include <map>

namespace NxA {

// -- Class

template <typename Tkey, typename Tvalue> struct MutableMapInternal : public Object::Internal, public std::map<const Tkey, Tvalue>
{
    // -- Constructors/Destructors
    MutableMapInternal() : std::map<const Tkey, Tvalue>() { }
    MutableMapInternal(const MutableMapInternal& other) : std::map<const Tkey, Tvalue>{ other } { }
    MutableMapInternal(std::map<const Tkey, Tvalue>&& other) : std::map<const Tkey, Tvalue>{ other } { }
    MutableMapInternal(const std::map<const Tkey, Tvalue>& other) : std::map<const Tkey, Tvalue>{ std::move(other) } { }
    virtual ~MutableMapInternal() = default;

    // -- Iterators
    using iterator = typename std::map<const Tkey, Tvalue>::iterator;
    using const_iterator = typename std::map<const Tkey, Tvalue>::const_iterator;

    // -- Instance Methods
    iterator begin()
    {
        return this->std::map<const Tkey, Tvalue>::begin();
    }
    const_iterator begin() const
    {
        return this->std::map<const Tkey, Tvalue>::begin();
    }
    const_iterator cbegin() const
    {
        return this->std::map<const Tkey, Tvalue>::cbegin();
    }
    iterator end()
    {
        return this->std::map<const Tkey, Tvalue>::end();
    }
    const_iterator end() const
    {
        return this->std::map<const Tkey, Tvalue>::end();
    }
    const_iterator cend() const
    {
        return this->std::map<const Tkey, Tvalue>::cend();
    }

    count length() const
    {
        return this->std::map<const Tkey, Tvalue>::size();
    }

    void setValueForKey(Tvalue value, const Tkey& key)
    {
        auto pvalue = std::pair<const Tkey, Tvalue>(key, value);
        auto result = std::map<const Tkey, Tvalue>::insert(pvalue);
        if (!result.second) {
            result.first->second = value;
        }
    }
    const Optional<Tvalue> maybeValueForKey(const Tkey& key) const
    {
        const_iterator pos = this->std::map<const Tkey, Tvalue>::find(key);
        if(pos == this->cend()) {
            return NxA::nothing;
        }
        return {pos->second};
    }
    const Tvalue& valueForKey(const Tkey& key) const
    {
        const_iterator pos = this->std::map<const Tkey, Tvalue>::find(key);
        NXA_ASSERT_TRUE(pos != this->cend());

        return pos->second;
    }
    Tvalue& valueForKey(const Tkey& key)
    {
        iterator pos = this->std::map<const Tkey, Tvalue>::find(key);
        NXA_ASSERT_TRUE(pos != this->end());

        return pos->second;
    }
    Tvalue& operator[](const Tkey& key)
    {
        return std::map<const Tkey, Tvalue>::operator[](key);
    }
    Tvalue& operator[](Tkey&& key)
    {
        return std::map<const Tkey, Tvalue>::operator[](std::move(key));
    }
    const Tvalue& operator[](const Tkey& key) const
    {
        return valueForKey(key);
    }
    boolean containsValueForKey(const Tkey& key) const
    {
        return this->std::map<const Tkey, Tvalue>::find(key) != this->std::map<const Tkey, Tvalue>::end();
    }
    void removeAll()
    {
        this->clear();
    }
    void removeValueForKey(const Tkey& key)
    {
        this->erase(key);
    }
    void removeValueAt(const_iterator position)
    {
        this->erase(position);
    }

    // -- Overriden Object::Internal Instance Methods
    uinteger32 classHash() const override
    {
        NXA_ALOG("Illegal call.");
        return 0;
    }
    const character* className() const override
    {
        NXA_ALOG("Illegal call.");
        return nullptr;
    }
};

}
