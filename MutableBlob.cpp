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

#include "Base/MutableBlob.hpp"
#include "Base/Blob.hpp"
#include "Base/Internal/MutableBlob.hpp"
#include "Base/String.hpp"
#include "Base/Assert.hpp"

using namespace NxA;

#pragma mark Constructors/Destructors

MutableBlob::MutableBlob() : internal{ std::make_shared<Internal>() } { }

MutableBlob::MutableBlob(const Blob &other) : internal{ std::make_shared<Internal>(*other.internal) } { }

MutableBlob::MutableBlob(const MutableBlob& other) : internal{ std::make_shared<Internal>(*other.internal) } { }

MutableBlob::MutableBlob(MutableBlob&&) = default;

MutableBlob::MutableBlob(MutableBlob&) = default;

MutableBlob::MutableBlob(std::shared_ptr<Internal>&& other) : internal{ std::move(other) } { }

MutableBlob::~MutableBlob() = default;

#pragma mark mark Factory Methods

MutableBlob MutableBlob::blobWithCapacity(count size)
{
    return { Internal::blobWithCapacity(size) };
}

MutableBlob MutableBlob::blobWithMemoryAndSize(const byte* other, count size)
{
    return { Internal::blobWithMemoryAndSize(other, size) };
}

MutableBlob MutableBlob::blobWithBase64String(const String& string)
{
    return { Internal::blobWithBase64String(string) };
}

MutableBlob MutableBlob::blobWithStringWithTerminator(const String& string)
{
    return { Internal::blobWithStringWithTerminator(string) };
}

MutableBlob MutableBlob::blobWithStringWithoutTerminator(const String& string)
{
    return { Internal::blobWithStringWithoutTerminator(string) };
}

#pragma mark Operators

MutableBlob& MutableBlob::operator=(MutableBlob&&) = default;

MutableBlob& MutableBlob::operator=(const MutableBlob&) = default;

boolean MutableBlob::operator==(const MutableBlob& other) const
{
    if (internal == other.internal) {
        return true;
    }
    return *internal == *(other.internal);
}

byte& MutableBlob::operator[] (integer index)
{
    return internal->operator[](index);
}
const byte& MutableBlob::operator[] (integer index) const
{
    return internal->operator[](index);
}

#pragma mark Instance Methods

uinteger32 MutableBlob::classHash() const
{
    return MutableBlob::staticClassHash();
}
const character* MutableBlob::className() const
{
    return MutableBlob::staticClassName();
}

bool MutableBlob::classNameIs(const character* className) const
{
    return !::strcmp(MutableBlob::staticClassName(), className);
}

count MutableBlob::size() const
{
    return internal->size();
}

byte* MutableBlob::data()
{
    return internal->data();
}

const byte* MutableBlob::data() const
{
    return internal->data();
}

Blob MutableBlob::hash()
{
    return { internal->hash() };
}

String MutableBlob::base64String()
{
    return internal->base64String();
}

void MutableBlob::fillWithZeros()
{
    return internal->fillWithZeros();
}

void MutableBlob::append(const Blob& other)
{
    return internal->append(*other.internal);
}

void MutableBlob::appendWithStringTermination(const character* other)
{
    return internal->appendWithStringTermination(other);
}

void MutableBlob::appendWithoutStringTermination(const character* other)
{
    return internal->appendWithoutStringTermination(other);
}

void MutableBlob::append(const character other)
{
    return internal->append(other);
}

void MutableBlob::removeAll()
{
    internal->removeAll();
}

void MutableBlob::padToAlignment(integer32 alignment)
{
    internal->padToAlignment(alignment);
}
