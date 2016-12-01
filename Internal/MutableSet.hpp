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
#include <set>

#include "Base/Assert.hpp"
#include "Base/Types.hpp"
#include "Base/MutableString.hpp"
#include "Base/Internal/Object.hpp"

namespace NxA {

#pragma mark Forward Declarations

    template <class T> class MutableSetInternal;

#pragma mark Utility Methods

    // -- This is a utility function to return the description of the content of an array.
    template <class T> String descriptionOfObjectsInSet(const MutableSetInternal<T>&);

#pragma mark Class

    template <class T> struct MutableSetInternal : public Object::Internal, public std::set<T>
    {
#pragma mark Constructors/Destructors
        MutableSetInternal() = default;
        MutableSetInternal(const MutableSetInternal& other) = default;
        MutableSetInternal(MutableSetInternal&& other) = default;
        virtual ~MutableSetInternal() = default;

        MutableSetInternal(std::set<T>&& other) : std::set<T>{ std::move(other) } { }

#pragma mark Iterators
        using iterator = typename std::set<T>::iterator;
        using const_iterator = typename std::set<T>::const_iterator;

#pragma mark Instance Methods
        iterator begin() noexcept
        {
            return this->std::set<T>::begin();
        }
        const_iterator begin() const noexcept
        {
            return this->std::set<T>::begin();
        }
        iterator end() noexcept
        {
            return this->std::set<T>::end();
        }
        const_iterator end() const noexcept
        {
            return this->std::set<T>::end();
        }
        const_iterator cbegin() const noexcept
        {
            return this->std::set<T>::cbegin();
        }
        const_iterator cend() const noexcept
        {
            return this->std::set<T>::cend();
        }

        count length() const
        {
            return this->size();
        }
        void remove(const T& object)
        {
            auto position = this->find(object);
            if (position != this->cend()) {
                this->erase(position);
            }
        }
        void removeAll()
        {
            return this->clear();
        }

        void append(T object)
        {
            this->insert(object);
        }
        void append(MutableSetInternal<T> other)
        {
            for (auto object : other) {
                this->append(object);
            }
        }
        template <class... ConstructorArguments>
        void emplaceAppend(ConstructorArguments &&... arguments)
        {
            this->emplace(std::forward<ConstructorArguments>(arguments)...);
        }

        const T& anyObject() const
        {
            auto anyPos = this->cbegin();
            NXA_ASSERT_TRUE(anyPos != this->cend());
            return *anyPos;
        }
        boolean contains(const T& object) const
        {
            return this->std::set<T>::count(object) != 0;
        }
        const_iterator find(const T& object) const
        {
            return this->std::set<T>::find(object);
        }
        iterator find(const T& object)
        {
            return this->std::set<T>::find(object);
        }
        void removeObjectAt(const_iterator objectPosition)
        {
            this->erase(objectPosition);
        }

        String description(const DescriberState& state) const
        {
            auto indented = state.increaseIndent();
            auto result = MutableString::stringWithFormat(indented.indentedLine("<Set length=\"%ld\">"), this->length());
            for (auto && item : *this) {
                result.append(NxA::describe(item, indented));
            }

            result.append(indented.indentedLine("</Set>"));
            
            return { std::move(result) };
        }
        
#pragma mark Overriden Object::Internal Instance Methods
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
