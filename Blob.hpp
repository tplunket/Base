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
#include <Base/GeneratedObjectCode.hpp>

namespace NxA {

struct BlobInternal;
class MutableBlob;
class String;

class Blob {
    NXA_GENERATED_INTERNAL_OBJECT_FORWARD_DECLARATION_USING(BlobInternal);
    NXA_GENERATED_OBJECT_METHODS_DECLARATIONS_FOR(Blob);

    friend MutableBlob;

public:
    #pragma mark Constructors/Destructors
    Blob();
    Blob(MutableBlob&&);

    #pragma mark Factory Methods
    static Blob blobWithMemoryAndSize(const byte*, count);
    static Blob blobWithBase64String(const String&);
    static Blob blobWithStringWithTerminator(const String&);
    static Blob blobWithStringWithoutTerminator(const String&);

    #pragma mark Class Methods
    static Blob hashFor(const byte*, count);
    static String base64StringFor(const byte*, count);

    #pragma mark Operators
    const byte& operator[] (integer) const;

    #pragma mark Instance Methods
    count size(void) const;

    const byte* data(void) const;

    Blob hash(void);
    String base64String(void);
};

}
