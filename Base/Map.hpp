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
#include <mutex>

namespace NxA {
    template <class Tkey, class Tvalue> class Map : public Object, private std::map<const Tkey, std::shared_ptr<Tvalue>> {
        NXA_GENERATED_OPERATOR_EQUAL_DECLARATION_IN_NAMESPACE_FOR_CLASS(NxA, Map<Tkey, Tvalue>);
        NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_BASE_CLASS(NxA, Map<Tkey, Tvalue>);

    public:
        using iterator = typename std::map<const Tkey, std::shared_ptr<Tvalue>>::iterator;
        using const_iterator = typename std::map<const Tkey, std::shared_ptr<Tvalue>>::const_iterator;
        
        #pragma mark Factory Methods
        static NxA::Pointer<Map<Tkey, Tvalue>>  map(void)
        {
            return Map::Pointer(std::make_shared<Map>(Map::constructor_access()));
        }

        #pragma mark Class Methods
        static const character* nameOfClass(void)
        {
            static std::mutex m;
            static std::unique_ptr<character[]> buffer;

            m.lock();

            if (!buffer.get()) {
                const character *format = "NxA::Map<%s, %s>";
                count needed = snprintf(NULL, 0, format, TypeName<Tkey>::get(), Tvalue::nameOfClass()) + 1;
                buffer = std::make_unique<character[]>(needed);
                snprintf(buffer.get(), needed, format, TypeName<Tkey>::get(), Tvalue::nameOfClass());
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

        iterator begin() noexcept
        {
            return this->std::map<const Tkey, std::shared_ptr<Tvalue>>::begin();
        }
        const_iterator begin() const noexcept
        {
            return this->std::map<const Tkey, std::shared_ptr<Tvalue>>::begin();
        }
        iterator end() noexcept
        {
            return this->std::map<const Tkey, std::shared_ptr<Tvalue>>::end();
        }
        const_iterator end() const noexcept
        {
            return this->std::map<const Tkey, std::shared_ptr<Tvalue>>::end();
        }
        iterator cbegin() noexcept
        {
            return this->std::map<const Tkey, std::shared_ptr<Tvalue>>::cbegin();
        }
        const_iterator cbegin() const noexcept
        {
            return this->std::map<const Tkey, std::shared_ptr<Tvalue>>::cbegin();
        }
        iterator cend() noexcept
        {
            return this->std::map<const Tkey, std::shared_ptr<Tvalue>>::cend();
        }
        const_iterator cend() const noexcept
        {
            return this->std::map<const Tkey, std::shared_ptr<Tvalue>>::cend();
        }
        void erase(iterator iter) noexcept
        {
            this->std::map<const Tkey, std::shared_ptr<Tvalue>>::erase(iter);
        }

        count length(void) const
        {
            return this->std::map<const Tkey, std::shared_ptr<Tvalue>>::size();
        }

        boolean containsValueForKey(const Tkey& keyValue) const
        {
            return this->find(keyValue) != this->std::map<const Tkey, std::shared_ptr<Tvalue>>::end();
        }
    };
}
