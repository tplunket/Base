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

// -- This should be included after our own includes but some names clash with its contents.
#include <map>

#include "Base/Types.hpp"
#include "Base/String.hpp"
#include "Base/Internal/Object.hpp"

namespace NxA {

#pragma mark Class

template <typename Tkey, typename Tvalue> struct MapInternal : public Object::Internal, public std::map<const Tkey, Tvalue>
{
    #pragma mark Constructors/Destructors
    MapInternal() : std::map<const Tkey, Tvalue>() { }
    MapInternal(const MapInternal& other) : std::map<const Tkey, Tvalue>{ other } { }
    MapInternal(std::map<const Tkey, Tvalue>&& other) : std::map<const Tkey, Tvalue>{ other } { }
    MapInternal(const std::map<const Tkey, Tvalue>& other) : std::map<const Tkey, Tvalue>{ std::move(other) } { }
    virtual ~MapInternal() = default;

    #pragma mark Iterators
    using iterator = typename std::map<const Tkey, Tvalue>::iterator;
    using const_iterator = typename std::map<const Tkey, Tvalue>::const_iterator;

    #pragma mark Operators
    Tvalue& operator[](const Tkey& key)
    {
        return this->std::map<const Tkey, Tvalue>::operator[](key);
    }
    Tvalue& operator[](Tkey&& key)
    {
        return this->std::map<const Tkey, Tvalue>::operator[](std::move(key));
    }

    #pragma mark Instance Methods
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

    Tvalue& valueForKey(const Tkey& key)
    {
        iterator pos = this->std::map<const Tkey, Tvalue>::find(key);
        NXA_ASSERT_TRUE(pos != this->cend());

        return pos->second;
    }
    const Tvalue& valueForKey(const Tkey& key) const
    {
        const_iterator pos = this->std::map<const Tkey, Tvalue>::find(key);
        NXA_ASSERT_TRUE(pos != this->cend());

        return pos->second;
    }
    boolean containsValueForKey(const Tkey& key) const
    {
        return this->std::map<const Tkey, Tvalue>::find(key) != this->end();
    }
    void removeAll()
    {
        this->erase();
    }
    void removeValueForKey(const Tkey& key)
    {
        this->erase(key);
    }
    void removeValueAt(const_iterator position)
    {
        this->erase(position);
    }
};

}
