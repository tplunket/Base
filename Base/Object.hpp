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

#include "Base/Uncopyable.hpp"
#include "Base/GeneratedObjectCode.hpp"

namespace NxA {
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR_CLASS(Object);

    class String;

    class Object : private Uncopyable, public NXA_ENABLE_SHARED_FROM_THIS(Object) {
        NXA_GENERATED_OBJECT_DECLARATIONS_WITHOUT_OVERRIDE_FOR(NxA, Object);
        NXA_GENERATED_SHARED_BASE_OPERATOR_EQUAL_WITHOUT_OVERRIDE_WITH_OBJECT_DECLARATIONS_FOR(NxA, Object);

    public:
        #pragma mark Static Methods
        static const character* nameOfClass(void)
        {
            return "NxA::Object";
        }
        static uinteger32 hashOfClassName(void);

        #pragma mark Instance Methods
        virtual NxA::Pointer<NxA::String> description(void) const;
    };
}
