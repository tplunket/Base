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

#include "SeratoDB/TrackFiles/FLACTrackFile.hpp"
#include "SeratoDB/TrackFiles/Internal/InternalTrackFile.hpp"

#include <Base/Base.hpp>

#include "id3v2tag.h"
#include "xiphcomment.h"
#include "flacproperties.h"

NXA_ENTER_NAMESPACE(NxA);
NXA_ENTER_NAMESPACE(Serato);

struct InternalFLACTrackFile : public InternalTrackFile {
    NXA_GENERATED_DESTRUCTOR_FOR_CLASS(InternalFLACTrackFile);

    #pragma mark Constructor & Destructors
    InternalFLACTrackFile(const String& path);

    #pragma mark Instance Variables
    NxA::Pointer<Array<const String>> nameOfFields;
    NxA::Pointer<Array<const String>> nameOfFieldsToRemove;
    NxA::Pointer<Array<const String>> ownersOfPrivateFrames;
    NxA::Pointer<Array<const String>> ownersOfPrivateFramesToRemove;

    boolean hasRating;

    #pragma mark Instance Methods
    void parseAudioProperties(const TagLib::FLAC::Properties& properties);
    void parseTag(const TagLib::ID3v2::Tag& id3v2Tag);
    void parseComment(const TagLib::Ogg::XiphComment& oggComment);
    void parseMarkersInComment(const TagLib::Ogg::XiphComment& oggComment);
    void updateMarkersInComment(TagLib::Ogg::XiphComment& oggComment) const;
    void updateMarkersV1ItemInComment(TagLib::Ogg::XiphComment& oggComment) const;
    void updateMarkersV2ItemInComment(TagLib::Ogg::XiphComment& oggComment) const;
    void updateGridMarkersItemInComment(TagLib::Ogg::XiphComment& oggComment) const;
    void updateTag(TagLib::ID3v2::Tag& tag) const;
    void updateComment(TagLib::Ogg::XiphComment& oggComment) const;

    #pragma mark Overridden TrackFile Instance Methods
    virtual void loadAndParseFile(void) override;
    virtual void updateAndSaveFile(void) const override;
};

NXA_EXIT_NAMESPACE;
NXA_EXIT_NAMESPACE;
