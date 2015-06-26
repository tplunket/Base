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

#include "Base/Pointer.hpp"

#include <memory>

namespace NxA {
    template <class T> class WeakPointer : protected std::weak_ptr<T> {
    public:
        #pragma mark Constructors & Destructors
        WeakPointer() = default;
        WeakPointer(Pointer<T> const& other) : std::weak_ptr<T>(other) { }
        WeakPointer(WeakPointer<T> const& other) : std::weak_ptr<T>(other) { }
        ~WeakPointer() = default;

        #pragma mark Instance Methods
        bool isValid(void) const {
            return !this->expired();
        }

        void release(void) const {
            this->release();
        }

        Pointer<T> toPointer(void) const
        {
            NXA_ASSERT_TRUE(this->isValid());
            return Pointer<T>(std::shared_ptr<T>(*this));
        }
    };
}
