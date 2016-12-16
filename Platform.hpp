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
#include <Base/Uncopyable.hpp>

namespace NxA {

class Blob;

class Platform : private Uncopyable {
public:
    // -- Constructors & Desctructors
    Platform() = delete;

    // -- Constants
    enum class Kind : uinteger32
    {
        OSX                 = 1,
        Windows             = 2,
        Unknown             = 23,
    };

    static constexpr Kind CurrentPlatform =
#ifndef _Windows
#if defined(_WIN32) || defined(WIN32) || defined(__Win32__) || defined(__WIN32__) || defined(_WINDOWS)
        Kind::Windows
#else
#if defined(__APPLE__) && defined(__MACH__)
        Kind::OSX
#else
        Kind::Unknown
#endif
#endif
#else
#if defined(__Win32__) || defined (_WIN32)
        Kind::Windows
#else
#if defined(__APPLE__) && defined(__MACH__)
        Kind::OSX
#else
        Kind::Unknown
#endif
#endif
#endif
    ;

    static constexpr uinteger32 LitleEndian   = 1;
    static constexpr uinteger32 BigEndian     = 2;
    static constexpr uinteger32 PdpEndian     = 3;
    static constexpr uinteger32 UnknownEndian = 23;

    static constexpr uinteger32 endianOrder =
          ((1 & 0xFFFFFFFF) == Platform::LitleEndian) ? Platform::LitleEndian
        : ((1 & 0xFFFFFFFF) == Platform::BigEndian) ? Platform::BigEndian
        : ((1 & 0xFFFFFFFF) == Platform::PdpEndian) ? Platform::PdpEndian
        : Platform::UnknownEndian;

    // -- Class Methods
    static float bigEndianFloatValueAt(const byte*);
    static uinteger32 bigEndianUInteger32ValueAt(const byte*);
    static uinteger16 bigEndianUInteger16ValueAt(const byte*);
    static void writeBigEndianFloatValueAt(float, byte*);
    static void writeBigEndianUInteger32ValueAt(uinteger32, byte*);
    static void writeBigEndianUInteger16ValueAt(uinteger16, byte*);

    static Blob convertEndiannessOfUInteger16From(const Blob&);
};

}
