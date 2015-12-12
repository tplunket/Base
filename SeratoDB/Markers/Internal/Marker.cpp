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

#include "Markers/Internal/Marker.hpp"

using namespace NxA::Serato::Internal;

#pragma mark Constructors & Destructors

namespace NxA { namespace Serato {
    typedef struct {
        byte position[4];
        byte loopPosition[4];
        byte zero;
        byte loopIterations[4];
        byte color[4];
        byte type;
        byte locked;
    } SeratoRawTagV1Struct;
    
    typedef struct {
        byte position[5];
        byte loopPosition[5];
        byte zero;
        byte loopIterations[5];
        byte color[4];
        byte type;
        byte locked;
    } SeratoEncodedTagV1Struct;
    
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


enum MarkerType
{
    eCueMarker = 1,
    eTagMarker = 3,
    eEmptyMarker = 0,
};


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
