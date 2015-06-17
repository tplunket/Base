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

#include "Base/Blob.hpp"
#include "Base/String.hpp"
#include "Base/Internal/Blob.hpp"

NXA_GENERATED_IMPLEMENTATION_FOR(NxA, Blob);

using namespace NxA;

#pragma mark mark Constructors & Destructors

Blob::Blob(Internal::Object::Pointer initial_internal) : Object(initial_internal), internal(initial_internal)
{
#if NXA_DEBUG_OBJECT_LIFECYCLE
    printf("Construct Blob at 0x%08lx.\n", (long)this);
#endif
}

#pragma mark mark Factory Methods

Blob::Pointer Blob::blob(void)
{
    return Blob::makeShared();
}

Blob::Pointer Blob::blobWithCapacity(count length)
{
    auto newBlob = Blob::makeShared();
    newBlob->internal->data = Internal::Blob::InternalVector(length);
    newBlob->clear();

    return newBlob;
}

Blob::Pointer Blob::blobWithCharPointer(const char* other, count length)
{
    auto newBlob = Blob::makeShared();
    newBlob->internal->data = Internal::Blob::InternalVector(other, other + length);

    return newBlob;
}

Blob::Pointer Blob::blobWithBlob(const Blob::Pointer& other)
{
    auto newBlob = Blob::makeShared();
    newBlob->internal->data = other->internal->data;

    return newBlob;
}

#pragma mark Operators

const uinteger8& Blob::operator[] (integer index) const
{
    return internal->data[index];
}

#pragma mark Instance Methods

count Blob::size(void) const
{
    return internal->data.size();
}

const uinteger8* Blob::data(void) const
{
    return internal->data.data();
}

bool Blob::isEqualTo(const Blob::Pointer& other) const
{
    return internal->data == other->internal->data;
}

void Blob::clear(void)
{
    std::memset(this->data(), 0, this->size());
}

void Blob::append(const Blob::Pointer& other)
{
    internal->data.insert(internal->data.end(), other->internal->data.begin(), other->internal->data.end());
}

void Blob::append(const character* other)
{
    count length = ::strlen(other);
    if (!length) {
        return;
    }

    internal->data.insert(internal->data.end(), length + 1, *other);
}
