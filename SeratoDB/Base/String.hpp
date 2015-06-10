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

#define NXA_BASE_STRING_ALREADY_INCLUDED

#include "Base/Object.hpp"

#include <string>

namespace NxA {
    namespace Serato {
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR(String);

    class String : public Object {
        NXA_GENERATED_DECLARATIONS_FOR(String);

    public:
        #pragma mark Factory Methods
        static String::Pointer stringWithCharPointer(const char* other);
        static String::Pointer stringWithStdString(const std::string& other);
        static String::Pointer stringWithFormat(const char* format, ... );
        
        #pragma mark Instance Methods
        virtual const String::Pointer description(void) const;

        String::Pointer copy(void) const;

        const char* toCharPointer(void) const;

        void append(const String::Pointer& other);
        void append(const char* other);
    };
}
}