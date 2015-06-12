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
#include <SeratoDB/Utility.hpp>

#include <taglib/mp4file.h>
#include <taglib/mp4tag.h>
#include <taglib/tpropertymap.h>
#include <taglib/audioproperties.h>

#include <string>

namespace NxA { namespace Serato {
    #pragma mark Class Declaration
    class MP4TrackFile : public TrackFile
    {
    private:
        #pragma mark Private Instance Variable
        TagLib::MP4::ItemListMap* p_itemListMap;

        #pragma mark Private Instance Methods
        void p_readMarkers(void);
        void p_writeMarkers(void);

    public:
        #pragma mark Constructors
        explicit MP4TrackFile(const char* trackFilePath);

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
} }