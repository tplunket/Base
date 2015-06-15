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

#include "Base/Blob.hpp"
#include "Base/String.hpp"
#include "Base/Types.hpp"
#include "Base/Uncopyable.hpp"

namespace NxA {
    class File : private Uncopyable {
    public:
        #pragma mark Constructors & Destructors
        Debugger() = delete;

        #pragma mark Class Methods
        static Blob::Pointer readFileAt(String::Pointer path);
        static void writeToFile(String::Pointer Path, const Blob::Pointer content);
        static void deleteFileAt(String::Pointer path);

        static String::Pointer joinPaths(String::Pointer first, String::Pointer second);
        static String::Pointer removePrefixFromPath(String::Pointer prefix, String::Pointer path);

        static bool fileExistsAt(String::Pointer path);
        static count sizeOfFileAt(String::Pointer path);

        static timestamp modificationDateInSecondsSince1970ForFile(String::Pointer path);
    };
}
