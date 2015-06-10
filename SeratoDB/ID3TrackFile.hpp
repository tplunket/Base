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

#include <SeratoDB/TrackFile.hpp>
#include <SeratoDB/DbUtility.hpp>

#include <string>

namespace NxA {
    namespace Serato {
    #pragma mark Class Declaration
    class ID3TrackFile : public TrackFile
    {
    protected:
        #pragma mark Protected Constructors
        explicit ID3TrackFile(const char* trackFilePath) : TrackFile(trackFilePath) { };

        #pragma mark Protected Instance Methods
        void p_readMarkers(void);
        void p_writeMarkers(void);
        void p_writeMarkersV2Frame(void);
        void p_writeGridMarkersFrame(void);

    public:
        #pragma mark Instance Methods
        virtual bool hasKey(void) const;
        virtual std::string key(void) const;
        virtual std::string grouping(void) const;

        virtual bool hasRecordLabel(void) const;
        virtual std::string recordLabel(void) const;
        virtual bool hasRemixer(void) const;
        virtual std::string remixer(void) const;
        virtual std::string yearReleased(void) const;

        virtual CharVectorPtr artwork(void) const;

        virtual void setKey(const char* key);
        virtual void setGrouping(const char* grouping);
        virtual void setRecordLabel(const char* recordLabel);
        virtual void setRemixer(const char* remixer);
        virtual void setYearReleased(const char* year);

        virtual void setArtwork(CharVectorPtr artwork);
    };
}
}
