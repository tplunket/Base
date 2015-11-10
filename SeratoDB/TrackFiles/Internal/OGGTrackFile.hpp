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

#include "TrackFiles/OGGTrackFile.hpp"
#include "TrackFiles/Internal/TrackFile.hpp"

#include <Base/Base.hpp>

#include <xiphcomment.h>

namespace NxA { namespace Serato { namespace Internal {
    #pragma mark Constants
    static const character* oggComposerFieldName = "COMPOSER";
    static const character* oggGroupingFieldName = "GROUPING";
    static const character* oggBpmFieldName = "BPM";
    static const character* oggRecordLabelFieldName = "LABEL";
    static const character* oggRemixerFieldName = "REMIXER";
    static const character* oggDateFieldName = "DATE";
    static const character* oggYearFieldName = "YEAR";
    static const character* oggKeyFieldName = "INITIALKEY";

    struct OGGTrackFile : public TrackFile {
        NXA_GENERATED_INTERNAL_DECLARATIONS_WITHOUT_CONSTRUCTORS_FOR(NxA::Serato, OGGTrackFile);

        #pragma mark Constructor & Destructors
        OGGTrackFile(const String& path, const TagLibFilePointer& newFile);

        #pragma mark Class Methods
        static String::Pointer stringValueForFieldNamedInComment(const character* name, const TagLib::Ogg::XiphComment* oggComment);
        static void setStringValueForFieldNamedInComment(const String& value, const character* name, TagLib::Ogg::XiphComment* oggComment);
        static String::Pointer releaseDateInComment(TagLib::Ogg::XiphComment* oggComment);
        static void setReleaseDateInComment(const String& date, TagLib::Ogg::XiphComment* oggComment);

        #pragma mark Instance Variables
        TagLib::Ogg::XiphComment* oggComment;

        #pragma mark Instance Methods
        String::Pointer stringValueForFieldNamed(const character* name) const;
        void setStringValueForFieldNamed(const String& value, const character* name);

        void readMarkers(void);
        void replaceGridMarkersField(void);
        void replaceMarkersV2Field(void);
        void writeMarkers(void);
    };
} } }
