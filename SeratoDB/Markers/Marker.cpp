//
//  Copyright (c) 2015 Next Audio Labs, LLC. All rights reserved.
//
//  This file contains confidential and proprietary information of Serato
//  Inc. LLP ("Serato"). No use is permitted without express written
//  permission of Serato. If you are not a party to a Confidentiality/
//  Non-Disclosure Agreement with Serato, please immediately delete this
//  file as well as all copies in your possession. For further information,
//  please refer to the modified MIT license provided with this library,
//  or email licensing@serato.com.
//

#include "Markers/Marker.hpp"
#include "Markers/Internal/Marker.hpp"

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA::Serato, Marker, Object);

namespace NxA { namespace Serato {
    /*
    #pragma mark Structures
    typedef struct {
        byte tag[4];
        byte size[4];
    } SeratoCueTagV2HeaderStruct;

    typedef struct {
        byte tag[4];
        byte size[4];
        byte index[2];
        byte position[4];
        byte color[4];
        byte loop_enabled;
        byte loop_locked;
        byte label[0];
    } SeratoCueTagV2Struct;

    typedef struct {
        byte position[4];
        byte loopPosition[4];
        byte zero;
        byte loopIterations[4];
        byte color[4];
        byte type;
        byte locked;
    } SeratoTagV1StructEX;

    typedef struct {
        byte position[5];
        byte loopPosition[5];
        byte zero;
        byte loopIterations[5];
        byte color[4];
        byte type;
        byte locked;
    } SeratoTagV1Struct;

    static void initializeTagV1Struct(SeratoTagV1Struct& tag)
    {
        for (int i = 0; i < 5; ++i)
        {
            tag.position[i] = 0x7f;
            tag.loopPosition[i] = 0x7f;
            tag.loopIterations[i] = 0x7f;
        }
        for (int i = 0; i < 4; ++i)
        {
            tag.color[i] = 0;
        }
        tag.zero = 0;
        tag.type = 0;
        tag.locked = 0;
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
    }*/
} }

using namespace NxA;
using namespace NxA::Serato;

