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
#include "Base/Pointer.hpp"
#include "Base/WeakPointer.hpp"

#include <map>
#include <mutex>

NXA_ENTER_NAMESPACE(NxA);

template <typename Tkey, typename Tvalue, typename = void> class Map;

// -- Implementation for built-in types, stored by value
template <typename Tkey, typename Tvalue>
class Map<Tkey, Tvalue, typename std::enable_if<std::is_fundamental<Tvalue>::value>::type> :
    public Object, private std::map<const Tkey, Tvalue> {
    NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_BASE_CLASS(NxA, Map<Tkey, Tvalue>);

public:
    using iterator = typename std::map<const Tkey, Tvalue>::iterator;
    using const_iterator = typename std::map<const Tkey, Tvalue>::const_iterator;

    #pragma mark Factory Methods
    static NxA::Pointer<Map<Tkey, Tvalue>>  map(void)
    {
        return NxA::Pointer<Map>(std::make_shared<Map>(Map::constructor_access()));
    }

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

    #pragma mark Instance Methods
    const Tvalue& valueForKey(const Tkey& key) const
    {
        return this->std::map<const Tkey, Tvalue>::at(key);
    }
    Tvalue& valueForKey(const Tkey& key)
    {
        return const_cast<Tvalue&>(static_cast<const Map<Tkey, Tvalue>*>(this)->valueForKey(key));
    }
    void setValueForKey(const Tvalue& value, const Tkey& key)
    {
        (*this)[key] = value;
    }

    iterator begin(void)
    {
        return this->std::map<const Tkey, Tvalue>::begin();
    }
    const_iterator begin(void) const
    {
        return this->std::map<const Tkey, Tvalue>::begin();
    }
    const_iterator cbegin(void) const
    {
        return this->std::map<const Tkey, Tvalue>::cbegin();
    }
    iterator end(void)
    {
        return this->std::map<const Tkey, Tvalue>::end();
    }
    const_iterator end(void) const
    {
        return this->std::map<const Tkey, Tvalue>::end();
    }
    const_iterator cend(void) const
    {
        return this->std::map<const Tkey, Tvalue>::cend();
    }
    void erase(const Tkey& key)
    {
        this->std::map<const Tkey, Tvalue>::erase(key);
    }

    count length(void) const
    {
        return this->std::map<const Tkey, Tvalue>::size();
    }

    boolean containsValueForKey(const Tkey& keyValue) const
    {
        return this->find(keyValue) != this->std::map<const Tkey, Tvalue>::end();
    }
};

// -- Implementation for NxA::Object values stored by shared_ptr
template <typename Tkey, typename Tvalue>
class Map<Tkey, Tvalue, typename std::enable_if<std::is_fundamental<Tvalue>::value == false>::type> :
    public Object, private std::map<const Tkey, std::shared_ptr<Tvalue>> {
    NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_BASE_CLASS(NxA, Map<Tkey, Tvalue>);

public:
    using iterator = typename std::map<const Tkey, std::shared_ptr<Tvalue>>::iterator;
    using const_iterator = typename std::map<const Tkey, std::shared_ptr<Tvalue>>::const_iterator;

    #pragma mark Factory Methods
    static NxA::Pointer<Map<Tkey, Tvalue>> map(void)
    {
        return Pointer<Map>(std::make_shared<Map>(Map::constructor_access()));
    }

    #pragma mark Class Methods
    static const character* staticClassName(void)
    {
        static std::mutex m;
        static std::unique_ptr<character[]> buffer;

        m.lock();

        if (!buffer.get()) {
            const character *format = "NxA::Map<%s, %s>";
            count needed = snprintf(NULL, 0, format, TypeName<Tkey>::get(), Tvalue::staticClassName()) + 1;
            buffer = std::make_unique<character[]>(needed);
            snprintf(buffer.get(), needed, format, TypeName<Tkey>::get(), Tvalue::staticClassName());
        }

        m.unlock();

        return buffer.get();
    }

    #pragma mark Instance Methods
    const Tvalue& valueForKey(const Tkey& key) const
    {
        return *(this->std::map<const Tkey, std::shared_ptr<Tvalue>>::at(key));
    }
    Tvalue& valueForKey(const Tkey& key)
    {
        return const_cast<Tvalue&>(static_cast<const Map<Tkey, Tvalue>*>(this)->valueForKey(key));
    }
    void setValueForKey(NxA::Pointer<Tvalue>& value, const Tkey& key)
    {
        (*this)[key] = value.toStdSharedPointer();
    }
    void setValueForKey(Tvalue& value, const Tkey& key)
    {
        (*this)[key] = value.pointer().toStdSharedPointer();
    }

    iterator begin(void)
    {
        return this->std::map<const Tkey, std::shared_ptr<Tvalue>>::begin();
    }
    const_iterator begin(void) const
    {
        return this->std::map<const Tkey, std::shared_ptr<Tvalue>>::begin();
    }
    const_iterator cbegin(void) const
    {
        return this->std::map<Tkey, std::shared_ptr<Tvalue>>::cbegin();
    }
    iterator end(void)
    {
        return this->std::map<const Tkey, std::shared_ptr<Tvalue>>::end();
    }
    const_iterator end(void) const
    {
        return this->std::map<const Tkey, std::shared_ptr<Tvalue>>::end();
    }
    const_iterator cend(void) const
    {
        return this->std::map<const Tkey, std::shared_ptr<Tvalue>>::cend();
    }

    using std::map<const Tkey, std::shared_ptr<Tvalue>>::erase;

    count length(void) const
    {
        return this->std::map<const Tkey, std::shared_ptr<Tvalue>>::size();
    }

    boolean containsValueForKey(const Tkey& keyValue) const
    {
        return this->find(keyValue) != this->std::map<const Tkey, std::shared_ptr<Tvalue>>::end();
    }
};

NXA_EXIT_NAMESPACE;
