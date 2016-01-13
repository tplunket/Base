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

#include "Markers/Internal/Marker.hpp"

using namespace NxA::Serato::Internal;

#pragma mark Constructors & Destructors

namespace NxA { namespace Serato {

    inline uinteger32 unpackUInteger32(const byte packed[5])
    {
        uinteger32 result = 0;
        result |= (packed[4] & 0x7f);
        result |= (packed[3] & 0x7f) << 7;
        result |= (packed[2] & 0x7f) << 14;
        result |= (packed[1] & 0x7f) << 21;
        result |= (packed[0] & 0x7f) << 28;
        return result;
    }
    
    inline void unpackColor(const byte packed[4], byte& r, byte& g, byte& b)
    {
        r = ((packed[1] & 0x1f) >> 2) | ((packed[0] & 0x07) << 5);
        g = ((packed[2] & 0x3f) >> 1) | ((packed[1] & 0x03) << 6);
        b = ((packed[3] & 0x7f) >> 0) | ((packed[2] & 0x01) << 7);
    }

    inline void packUInteger32(uinteger32 input, byte packed[5])
    {
        if (input == 0xFFFFFFFF)
        {
            packed[4] = packed[3] = packed[2] = packed[1] = packed[0] = 0x7f;
        }
        else
        {
            packed[4] = (input & 0x0000007f);
            packed[3] = (input & 0x00003f80) >> 7;
            packed[2] = (input & 0x001fc000) >> 14;
            packed[1] = (input & 0x0fe00000) >> 21;
            packed[0] = (input & 0xf0000000) >> 28;
        }
    }
    
    inline void packColor(byte r, byte g, byte b, byte packed[4])
    {
        packed[3] = (b & 0x7f);
        packed[2] = ((b & 0x80) >> 7) | ((g & 0x3f) << 1);
        packed[1] = ((g & 0xc0) >> 6) | ((r & 0x1f) << 2);
        packed[0] = ((r & 0xe0) >> 5);
    }
} }

Marker::Marker() {}

void Marker::rawV1TagFromEncodedV1TagStruct(SeratoRawTagV1Struct& rawTag,
                                            const SeratoEncodedTagV1Struct& encodedTag)
{
    integer32 position = unpackUInteger32(encodedTag.position);
    integer32 loopPosition = unpackUInteger32(encodedTag.loopPosition);
    integer32 loopIterations = unpackUInteger32(encodedTag.loopIterations);

    Platform::writeBigEndianUInteger32ValueAt(position, rawTag.position);
    Platform::writeBigEndianUInteger32ValueAt(loopPosition, rawTag.loopPosition);
    Platform::writeBigEndianUInteger32ValueAt(loopIterations, rawTag.loopIterations);
    rawTag.color[0] = 0;
    unpackColor(encodedTag.color, rawTag.color[1], rawTag.color[2], rawTag.color[3]);
    rawTag.locked = 0;
    rawTag.type = encodedTag.type;
    rawTag.zero = 0;
}

void Marker::addRawMarkerV1TagWithFieldsTo(MarkerType type, integer32 position,
                                           integer32 loopPosition, integer32 loopIterations,
                                           byte red, byte green, byte blue, Blob& data)
{
    SeratoRawTagV1Struct tag;
    
    Platform::writeBigEndianUInteger32ValueAt(position, tag.position);
    Platform::writeBigEndianUInteger32ValueAt(loopPosition, tag.loopPosition);
    Platform::writeBigEndianUInteger32ValueAt(loopIterations, tag.loopIterations);
    tag.color[0] = 0;
    tag.color[1] = red;
    tag.color[2] = green;
    tag.color[3] = blue;
    tag.locked = 0;
    tag.type = type;
    tag.zero = 0;
    
    auto tagData = Blob::blobWithMemoryAndSize(reinterpret_cast<const byte*>(&tag), sizeof(SeratoRawTagV1Struct));
    data.append(tagData);
}

void Marker::addEncodedMarkerV1TagWithFieldsTo(MarkerType type, integer32 position,
                                               integer32 loopPosition, integer32 loopIterations,
                                               byte red, byte green, byte blue, Blob& data)
{
    SeratoEncodedTagV1Struct tag;
    
    packUInteger32(position, tag.position);
    packUInteger32(loopPosition, tag.loopPosition);
    packUInteger32(loopIterations, tag.loopIterations);
    packColor(red, green, blue, tag.color);
    tag.locked = 0;
    tag.type = type;
    tag.zero = 0;
    
    auto tagData = Blob::blobWithMemoryAndSize(reinterpret_cast<const byte*>(&tag), sizeof(SeratoEncodedTagV1Struct));
    data.append(tagData);
}
