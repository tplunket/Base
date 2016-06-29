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

struct MutableBlobInternal;
class String;
class Blob;

class MutableBlob {
    NXA_GENERATED_INTERNAL_OBJECT_FORWARD_DECLARATION_USING(MutableBlobInternal);
    NXA_GENERATED_OBJECT_METHODS_DECLARATIONS_FOR(MutableBlob);

    friend Blob;
    
public:
    #pragma mark Constructors/Destructors
    MutableBlob();
    MutableBlob(const Blob&);

    #pragma mark Factory Methods
    static MutableBlob blobWithCapacity(count);
    static MutableBlob blobWithMemoryAndSize(const byte*, count);
    static MutableBlob blobWithBase64String(const String&);
    static MutableBlob blobWithStringWithTerminator(const String&);
    static MutableBlob blobWithStringWithoutTerminator(const String&);

    #pragma mark Operators
    const byte& operator[] (integer) const;
    byte& operator[] (integer index)
    {
        return const_cast<byte&>(static_cast<const MutableBlob*>(this)->operator[](index));
    }

    #pragma mark Instance Methods
    count size() const;

    const byte* data() const;
    byte* data()
    {
        return const_cast<byte*>(static_cast<const MutableBlob*>(this)->data());
    }

    void fillWithZeros();

    Blob hash();
    String base64String();

    void append(const Blob&);
    void appendWithStringTermination(const character*);
    void appendWithoutStringTermination(const character*);
    void append(const character);

    void removeAll();

    void padToAlignment(integer32);
};

}
