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

#include "Base/Blob.hpp"
#include "Base/MutableBlob.hpp"
#include "Base/Internal/Blob.hpp"
#include "Base/String.hpp"
#include "Base/Assert.hpp"

using namespace NxA;

NXA_GENERATED_OBJECT_METHODS_DEFINITIONS_FOR(Blob);

#pragma mark Constructors/Destructors

Blob::Blob() : internal{ std::make_shared<Internal>() } { }

Blob::Blob(MutableBlob&& other) : internal{ std::move(other.internal) }
{
    // -- If we're moving this other mutable, it can't be referred to by anyone else.
    NXA_ASSERT_TRUE(internal.use_count() == 1);
}

#pragma mark mark Factory Methods

Blob Blob::blobWithMemoryAndSize(const byte* other, count size)
{
    return { Internal::blobWithMemoryAndSize(other, size) };
}

Blob Blob::blobWithBase64String(const String& string)
{
    return { Internal::blobWithBase64String(string) };
}

Blob Blob::blobWithStringWithTerminator(const String& string)
{
    return { Internal::blobWithStringWithTerminator(string) };
}

Blob Blob::blobWithStringWithoutTerminator(const String& string)
{
    return { Internal::blobWithStringWithoutTerminator(string) };
}

#pragma mark Class Methods

Blob Blob::hashFor(const byte* memory, count size)
{
    return { Internal::hashFor(memory, size) };
}

String Blob::base64StringFor(const byte* memory, count size)
{
    return { Internal::base64StringFor(memory, size) } ;
}

#pragma mark Operators

const byte& Blob::operator[] (integer index) const
{
    return internal->operator[](index);
}

#pragma mark Instance Methods

count Blob::size() const
{
    return internal->size();
}

const byte* Blob::data() const
{
    return internal->data();
}

Blob Blob::hash()
{
    return { internal->hash() };
}

String Blob::base64String()
{
    return internal->base64String();
}
