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

namespace NxA {
    template <class T> class WeakPointer;

    template <class T> class Pointer : protected std::shared_ptr<T> {
        friend class WeakPointer<T>;

    public:
        #pragma mark Constructors & Destructors
        Pointer() = delete;
        Pointer(const std::shared_ptr<T>& other) : std::shared_ptr<T>(other) { };
        Pointer(const Pointer<T>& other) : std::shared_ptr<T>(other) { }
        ~Pointer() = default;

        #pragma mark Class Methods
        template <class Tfrom>
        static Pointer<T> dynamicCastFrom(const Pointer<Tfrom>& other)
        {
            Pointer<T> result = Pointer(std::shared_ptr<T>(std::dynamic_pointer_cast<T>(other.toStdSharedPointer())));
            NXA_ASSERT_NOT_NULL(result.get());
            return result;
        }

        #pragma mark Operators
        operator Pointer<const T>() const
        {
            return Pointer<const T>(this->toStdSharedPointer());
        }
        operator T&() const
        {
            return this->operator*();
        }
        operator T&()
        {
            return this->operator*();
        }
        T* operator->() const
        {
            return this->get();
        }
        T* operator->()
        {
            return const_cast<T*>(static_cast<const Pointer<T>>(*this).operator->());
        }
        T& operator*() const
        {
            return *this->get();
        }
        T& operator*()
        {
            return const_cast<T&>(static_cast<const Pointer<T>>(*this).operator*());
        }

        #pragma mark Instance Methods
        boolean isEqualTo(const Pointer<T>& other) const
        {
            return this->get()->isEqualTo(*other);
        }
        const std::shared_ptr<T>& toStdSharedPointer(void) const
        {
            return *this;
        }
    };
}
