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
#include "Base/Internal/Blob.hpp"

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA, Blob, Object);

using namespace NxA;

#pragma mark mark Factory Methods

Blob::Pointer Blob::blob(void)
{
    return Blob::makeShared();
}

Blob::Pointer Blob::blobWithCapacity(count size)
{
    auto newBlob = Blob::makeShared();
    newBlob->internal->data = Internal::Blob::InternalVector(size);
    newBlob->fillWithZeros();

    return newBlob;
}

Blob::Pointer Blob::blobWithMemoryAndSize(const byte* other, count size)
{
    auto newBlob = Blob::makeShared();
    newBlob->internal->data = Internal::Blob::InternalVector(other, other + size);

    return newBlob;
}

Blob::Pointer Blob::blobWith(const Blob& other)
{
    auto newBlob = Blob::makeShared();
    newBlob->internal->data = other.internal->data;

    return newBlob;
}

#pragma mark Class Methods

Blob::Pointer Blob::hashFor(const byte* memory, count size)
{
    return Internal::Blob::hashFor(memory, size);
}

#pragma mark Operators

const byte& Blob::operator[] (integer index) const
{
    NXA_ASSERT_TRUE(index >= 0 && index < this->size());
    return internal->data[index];
}

bool Blob::operator==(const Blob& other) const
{
    if (this == &other) {
        return true;
    }

    return internal->data == other.internal->data;
}

#pragma mark Instance Methods

count Blob::size(void) const
{
    return internal->data.size();
}

const byte* Blob::data(void) const
{
    NXA_ASSERT_TRUE(this->size() > 0);
    return internal->data.data();
}

void Blob::fillWithZeros(void)
{
    std::memset(this->data(), 0, this->size());
}

Blob::Pointer Blob::hash(void)
{
    return Internal::Blob::hashFor(this->data(), this->size());
}

void Blob::append(const Blob& other)
{
    internal->data.insert(internal->data.end(), other.internal->data.begin(), other.internal->data.end());
}

void Blob::append(const character* other)
{
    internal->data.insert(internal->data.end(), other, other + ::strlen(other) + 1);
}

void Blob::append(const character other)
{
    internal->data.insert(internal->data.end(), 1, other);
}
