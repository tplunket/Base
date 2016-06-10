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

#include <Base/Types.hpp>
#include <Base/String.hpp<>
#include <Base/GeneratedObjectCode.hpp>
#include <Base/Internal/Map.hpp>

#include <map>
#include <mutex>

namespace NxA {

template <typename Tkey, typename Tvalue> class Map;

#pragma mark Class

template <typename Tkey, typename Tvalue> class MutableMap {
    NXA_GENERATED_INTERNAL_OBJECT_FORWARD_DECLARATION_USING(MapInternal<const Tkey, Tvalue>);

    std::shared_ptr<MapInternal<const Tkey, Tvalue>> internal;

    friend Map<const Tkey, Tvalue>;

public:
    #pragma mark Constructors/Destructors
    MutableMap() : internal{ std::make_shared<MapInternal<const Tkey, Tvalue>>() } { }
    MutableMap(const MutableMap&) = default;
    MutableMap(MutableMap&&) = default;
    ~MutableMap() = default;
        
    #pragma mark Class Methods
    static const character* staticClassName(void)
    {
        static std::mutex m;
        static std::unique_ptr<character[]> buffer;

        m.lock();

        if (!buffer.get()) {
            const character *format = "NxA::Map<%s, %s>";
            count needed = snprintf(NULL, 0, format, TypeName<Tkey>::get(), TypeName<Tvalue>::get()) + 1;
            buffer = std::make_unique<character[]>(needed);
            snprintf(buffer.get(), needed, format, TypeName<Tkey>::get(), TypeName<Tvalue>::get());
        }

        m.unlock();

        return buffer.get();
    }

    #pragma mark Operators
    using iterator = typename MapInternal<const Tkey, Tvalue>::iterator;
    using const_iterator = typename MapInternal<const Tkey, Tvalue>::const_iterator;

    #pragma mark Operators
    Tvalue& operator[](Tkey&& key)
    {
        return internal->operator[](std::move(key));
    }
    Tvalue& operator[](const Tkey& key)
    {
        return internal->operator[](key);
    }

    #pragma mark Instance Methods
    iterator begin(void)
    {
        return internal->begin();
    }
    const_iterator begin(void) const
    {
        return internal->begin();
    }
    const_iterator cbegin(void) const
    {
        return internal->cbegin();
    }
    iterator end(void)
    {
        return internal->end();
    }
    const_iterator end(void) const
    {
        return internal->end();
    }
    const_iterator cend(void) const
    {
        return internal->cend();
    }
    void erase(const Tkey& key)
    {
        return internal->erase();
    }

    count length(void) const
    {
        return internal->length();
    }

    Tvalue& valueForKey(const Tkey& key) const
    {
        return internal->valueForKey(key);
    }
    boolean containsValueForKey(const Tkey& key) const
    {
        return internal->containsValueForKey(key);
    }
};

}
