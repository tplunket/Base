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

#include "Tags/Internal/Tag.hpp"

NXA_GENERATED_INTERNAL_IMPLEMENTATION_FOR(NxA::Serato, Tag);

using namespace NxA;
using namespace NxA::Serato::Internal;

#pragma mark Constructors & Destructors

Tag::Tag() : identifier(0) { }

#pragma mark Class Methods

void Tag::setIdentifierForTagAt(uinteger32 identifier, byte* tagAddress)
{
    TagStruct* tagStructPtr = reinterpret_cast<TagStruct*>(tagAddress);
    Platform::writeBigEndianUInt32ValueAt(identifier, tagStructPtr->identifier);
}

count Tag::dataSizeForTagAt(const byte* tagAddress)
{
    const TagStruct* tagStructPtr = reinterpret_cast<const TagStruct*>(tagAddress);
    unsigned long dataSize = Platform::bigEndianUInt32ValueAt(tagStructPtr->length);
    return dataSize;
}

void Tag::setDataSizeForTagAt(count dataSize, byte* tagAddress)
{
    TagStruct* tagStructPtr = reinterpret_cast<TagStruct*>(tagAddress);
    Platform::writeBigEndianUInt32ValueAt((uint32_t)dataSize, tagStructPtr->length);
}

count Tag::memoryNeededForTagHeader(void)
{
    return sizeof(TagStruct);
}

const byte* Tag::dataForTagAt(const byte* tagAddress)
{
    const TagStruct* tagStructPtr = reinterpret_cast<const TagStruct*>(tagAddress);
    return tagStructPtr->data;
}

byte* Tag::dataForTagAt(byte* tagAddress)
{
    return const_cast<byte*>(Tag::dataForTagAt(const_cast<const byte*>(tagAddress)));
}
