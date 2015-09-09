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

#pragma once

#include <SeratoDb/TrackFiles/TrackFile.hpp>

#include <Base/Base.hpp>

namespace NxA { namespace Serato {
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR_CLASS(ID3TrackFile);

    class ID3TrackFile : public TrackFile {
        NXA_GENERATED_OPERATOR_EQUAL_DECLARATION_IN_NAMESPACE_FOR_CLASS(NxA::Serato, ID3TrackFile);
        NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_CLASS(NxA::Serato, ID3TrackFile);

    public:
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
        virtual void setRemixer(const String& remixer);
        virtual void setRating(integer rating);

        virtual void setArtwork(const Blob& artwork);
    };
} }
