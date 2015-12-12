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

#include <SeratoDb/TrackFiles/TrackFile.hpp>

#include <Base/Base.hpp>

namespace NxA { namespace Serato {
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR_CLASS(MP4TrackFile);

    class MP4TrackFile : public TrackFile {
        NXA_GENERATED_OPERATOR_EQUAL_DECLARATION_IN_NAMESPACE_FOR_CLASS(NxA::Serato, MP4TrackFile);
        NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_CLASS(NxA::Serato, MP4TrackFile);

    public:
        #pragma mark Factory Methods
        static MP4TrackFile::Pointer fileWithFileAt(const String& path, TrackFile::Flags flags);

        #pragma mark Instance Methods
        virtual boolean hasKey(void) const;
        virtual String::Pointer key(void) const;
        virtual String::Pointer composer(void) const;
        virtual String::Pointer grouping(void) const;
        virtual String::Pointer bpm(void) const;

        virtual boolean hasRecordLabel(void) const;
        virtual String::Pointer recordLabel(void) const;
        virtual boolean hasRemixer(void) const;
        virtual String::Pointer remixer(void) const;
        virtual boolean hasRating(void) const;
        virtual integer rating(void) const;

        virtual Blob::Pointer artwork(void) const;

        virtual void setKey(const String& key);
        virtual void setComposer(const String& composer);
        virtual void setGrouping(const String& grouping);
        virtual void setBpm(const String& bpm);

        virtual void setRecordLabel(const String& recordLabel);
        virtual void setRemixer(const String& emixer);
        virtual void setRating(integer rating);

        virtual void setArtwork(const Blob& artwork);
    };
} }
