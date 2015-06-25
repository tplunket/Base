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
#include "Base/Object.hpp"

namespace NxA {
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR(Blob);

    class Blob : public Object {
        NXA_GENERATED_DECLARATIONS_FOR(NxA, Blob);

    public:
        #pragma mark Factory Methods
        static Blob::Pointer blob(void);
        static Blob::Pointer blobWithCapacity(count length);
        static Blob::Pointer blobWithMemoryAndSize(const void* other, count length);
        static Blob::Pointer blobWith(Blob::ConstPointer const& other);

        #pragma mark Operators
        const byte& operator[] (integer index) const;
        byte& operator[] (integer index)
        {
            return const_cast<uinteger8&>(static_cast<const Blob&>(*this)[index]);
        }

        #pragma mark Instance Methods
        count size(void) const;

        const byte* data(void) const;
        byte* data(void)
        {
            return const_cast<uinteger8*>(static_cast<const Blob&>(*this).data());
        }

        bool isEqualTo(Blob::ConstPointer const& other) const;

        void clear(void);

        void append(Blob::ConstPointer const& other);
        void append(const character* other);
        void append(const character other);
    };
}
