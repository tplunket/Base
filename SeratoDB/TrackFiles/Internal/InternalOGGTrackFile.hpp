//
//  Copyright (c) 2015-2016 Next Audio Labs, LLC. All rights reserved.
//
//  This file contains confidential and proprietary information of Serato
//  Inc. LLP ("Serato"). No use is permitted without express written
//  permission of Serato. If you are not a party to a Confidentiality/
//  Non-Disclosure Agreement with Serato, please immediately delete this
//  file as well as all copies in your possession. For further information,
//  please refer to the modified MIT license provided with this library,
//  or email licensing@serato.com.
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
#include "TrackFiles/Internal/InternalTrackFile.hpp"

#include <Base/Base.hpp>

#include <xiphcomment.h>

namespace NxA { namespace Serato {
    #pragma mark Constants
    static const character* oggComposerFieldName = "COMPOSER";
    static const character* oggGroupingFieldName = "GROUPING";
    static const character* oggBpmFieldName = "BPM";
    static const character* oggRecordLabelFieldName = "LABEL";
    static const character* oggRemixerFieldName = "REMIXER";
    static const character* oggDateFieldName = "DATE";
    static const character* oggYearFieldName = "YEAR";
    static const character* oggKeyFieldName = "INITIALKEY";

    struct InternalOGGTrackFile : public InternalTrackFile {
        NXA_GENERATED_DESTRUCTOR_FOR_CLASS(InternalOGGTrackFile);

        #pragma mark Constructor & Destructors
        InternalOGGTrackFile(const String& path);

        #pragma mark Class Methods
        static String::Pointer stringValueForFieldNamedInComment(const character* name, const TagLib::Ogg::XiphComment& oggComment);
        static void setStringValueForFieldNamedInComment(const String& value, const character* name, TagLib::Ogg::XiphComment& oggComment);
        static String::Pointer releaseDateInComment(const TagLib::Ogg::XiphComment& oggComment);
        static void setReleaseDateInComment(const String& date, TagLib::Ogg::XiphComment& oggComment);

        #pragma mark Instance Variables
        String::ArrayOfConst::Pointer nameOfFields;
        String::ArrayOfConst::Pointer nameOfFieldsToRemove;

        #pragma mark Instance Methods
        void parseMarkersInComment(const TagLib::Ogg::XiphComment& oggComment);
        void replaceGridMarkersFieldInComment(TagLib::Ogg::XiphComment& oggComment) const;
        void replaceMarkersV2FieldInComment(TagLib::Ogg::XiphComment& oggComment) const;
        void updateMarkersInComment(TagLib::Ogg::XiphComment& oggComment) const;
        void parseComment(TagLib::Ogg::XiphComment& oggComment);
        void updateComment(TagLib::Ogg::XiphComment& oggComment) const;

        #pragma mark Overridden TrackFile Instance Methods
        virtual void loadAndParseFile(void) override;
        virtual void updateAndSaveFile(void) const override;
    };
} }
