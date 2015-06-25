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

#include <SeratoDb/TrackFiles/ID3TrackFile.hpp>

#include <Base/Base.hpp>

namespace NxA { namespace Serato {
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR(OGGTrackFile);

    class OGGTrackFile : public ID3TrackFile {
        NXA_GENERATED_DECLARATIONS_FOR(NxA::Serato, OGGTrackFile);

    public:
        #pragma mark Factory Methods
        static OGGTrackFile::Pointer fileWithFileAt(String::ConstPointer const& path);

        #pragma mark Instance Methods
        virtual bool hasKey(void) const;
        virtual String::Pointer key(void) const;
        virtual String::Pointer grouping(void) const;

        virtual bool hasRecordLabel(void) const;
        virtual String::Pointer recordLabel(void) const;
        virtual bool hasRemixer(void) const;
        virtual String::Pointer remixer(void) const;
        virtual String::Pointer yearReleased(void) const;

        virtual Blob::Pointer artwork(void) const;

        virtual void setKey(String::ConstPointer const& key);
        virtual void setGrouping(String::ConstPointer const& grouping);
        virtual void setRecordLabel(String::ConstPointer const& recordLabel);
        virtual void setRemixer(String::ConstPointer const& remixer);
        virtual void setYearReleased(String::ConstPointer const& year);

        virtual void setArtwork(Blob::ConstPointer const& artwork);
    };
} }
