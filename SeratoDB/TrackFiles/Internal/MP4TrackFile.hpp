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

#include "TrackFiles/MP4TrackFile.hpp"
#include "TrackFiles/Internal/TrackFile.hpp"

#include <Base/Base.hpp>

#include <mp4tag.h>

namespace NxA { namespace Serato { namespace Internal {
    struct MP4TrackFile : public TrackFile {
        NXA_GENERATED_INTERNAL_DECLARATIONS_WITHOUT_CONSTRUCTORS_FOR(NxA::Serato, MP4TrackFile);

        #pragma mark Constructor & Destructors
        MP4TrackFile(const String& path, const TagLibFilePointer& newFile);

        #pragma mark Instance Variables
        TagLib::MP4::Tag* mp4Tag;

        #pragma mark Instance Methods
        integer integerValueForItemNamed(const character* name) const;
        String::Pointer stringValueForItemNamed(const character* name) const;
        void setIntegerValueForItemNamed(integer value, const character* name);
        void setStringValueForItemNamed(const String& value, const character* name);

        void readMarkers(void);
        void replaceFrameNamedWithDataAndVersion(const character* frameName, const character* frameDescription,
                                                 const Blob& frameData, int majorVersion, int minorVersion);
        void replaceMarkersV1Item(void);
        void replaceMarkersV2Item(void);
        void replaceGridMarkersItem(void);
        void writeMarkers(void);
    };
} } }
