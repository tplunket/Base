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

#pragma once

#include "Base/Types.hpp"
#include "Base/Blob.hpp"
#include "Base/Uncopyable.hpp"

namespace NxA {
    class Platform : private Uncopyable {
    public:
        #pragma mark Constructors & Desctructors
        Platform() = delete;

        #pragma mark Constants
        static constexpr uinteger OSX       = 1;
        static constexpr uinteger Windows   = 2;
        static constexpr uinteger Unknown   = 23;

        static constexpr uinteger32 LitleEndian   = 1;
        static constexpr uinteger32 BigEndian     = 2;
        static constexpr uinteger32 PdpEndian     = 3;
        static constexpr uinteger32 UnknownEndian = 23;

        static constexpr uinteger platform =
#ifndef _Windows
#if defined(_WIN32) || defined(WIN32) || defined(__Win32__) || defined(__WIN32__) || defined(_WINDOWS)
        Platform::Windows;
#else
#if defined(__APPLE__) && defined(__MACH__)
        Platform::OSX;
#else
        Platform::Unknown;
#endif
#endif
#else
#if defined(__Win32__) || defined (_WIN32)
        Platform::Windows;
#else
#if defined(__APPLE__) && defined(__MACH__)
        Platform::OSX;
#else
        Platform::Unknown;
#endif
#endif
#endif

        static constexpr uinteger32 endianOrder =
            ((1 & 0xFFFFFFFF) == Platform::LitleEndian) ? Platform::LitleEndian
          : ((1 & 0xFFFFFFFF) == Platform::BigEndian) ? Platform::BigEndian
          : ((1 & 0xFFFFFFFF) == Platform::PdpEndian) ? Platform::PdpEndian
          : Platform::UnknownEndian;

        #pragma mark Class Methods
        static float bigEndianFloatValueAt(const byte* ptr);
        static uinteger32 bigEndianUInteger32ValueAt(const byte* ptr);
        static uinteger16 bigEndianUInteger16ValueAt(const byte* ptr);
        static void writeBigEndianFloatValueAt(float value, byte* ptr);
        static void writeBigEndianUInteger32ValueAt(uinteger32 value, byte* ptr);
        static void writeBigEndianUInteger16ValueAt(uinteger16 value, byte* ptr);

        static Blob::Pointer convertEndiannessOfUInteger16From(const NxA::Blob& other);
    };
}
