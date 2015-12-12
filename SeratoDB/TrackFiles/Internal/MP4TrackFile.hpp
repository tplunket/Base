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
        MP4TrackFile(const String& path);

        #pragma mark Class Methods
        static integer integerValueForItemNamedInTag(const character* name, const TagLib::MP4::Tag& tag);
        static String::Pointer stringValueForItemNamedInTag(const character* name, const TagLib::MP4::Tag& tag);
        static void setIntegerValueForItemNamedInTag(integer value, const character* name, TagLib::MP4::Tag& tag);
        static void setStringValueForItemNamedInTag(const String& value, const character* name, TagLib::MP4::Tag& tag);
        Blob::Pointer artworkInTag(const TagLib::MP4::Tag& tag);

        #pragma mark Instance Methods
        void parseMarkersInTag(const TagLib::MP4::Tag& tag);
        void replaceMarkersV2ItemInTag(TagLib::MP4::Tag& tag) const;
        void replaceGridMarkersItemInTag(TagLib::MP4::Tag& tag) const;
        void updateMarkersInTag(TagLib::MP4::Tag& tag) const;
        void parseTag(const TagLib::MP4::Tag& tag);
        void updateArtworkInTag(TagLib::MP4::Tag& tag) const;
        void updateTag(TagLib::MP4::Tag& tag) const;

        #pragma mark Overridden TrackFile Instance Methods
        virtual void loadAndParseFile(void) override;
        virtual void updateAndSaveFileIfModified(void) const override;
    };
} } }
