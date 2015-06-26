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

#include "TrackFiles/ID3TrackFile.hpp"
#include "TrackFiles/Internal/TrackFile.hpp"

#include <Base/Base.hpp>

#include <id3v2tag.h>
#include <generalencapsulatedobjectframe.h>

namespace NxA { namespace Serato { namespace Internal {
    struct ID3TrackFile : public TrackFile {
        NXA_GENERATED_INTERNAL_DECLARATIONS_WITHOUT_CONSTRUCTOR_FOR(NxA::Serato, ID3TrackFile);

        #pragma mark Constructor & Destructors
        ID3TrackFile(const String& path, TagLibFilePointer const& newFile);

        #pragma mark Class Methods
        static bool isAValidGeobFrame(const TagLib::ID3v2::GeneralEncapsulatedObjectFrame* frame);
        static TagLib::ID3v2::FrameList::Iterator frameInListWithDescription(TagLib::ID3v2::FrameList& list,
                                                                             const String& description);
        
        #pragma mark Instance Methods
        void readMarkers(void);
        void writeMarkers(void);
        void writeMarkersV2Frame(void);
        void writeGridMarkersFrame(void);
    };
} } }
