//
//  NxASeratoMP4TrackFile.h
//  SeratoDB
//
//  Created by Didier Malenfant on 2/12/15.
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

#ifndef __SeratoDB__NxASeratoMP4TrackFile__
#define __SeratoDB__NxASeratoMP4TrackFile__

#include <SeratoDB/NxASeratoTrackFile.h>
#include <SeratoDB/NxASeratoDbUtility.h>

#include <taglib/mp4file.h>
#include <taglib/mp4tag.h>
#include <taglib/tpropertymap.h>
#include <taglib/audioproperties.h>

namespace NxA {
    #pragma mark Class Declaration
    class SeratoMP4TrackFile : public SeratoTrackFile
    {
    private:
        #pragma mark Private Instance Variable
        TagLib::MP4::ItemListMap* p_itemListMap;

        #pragma mark Private Instance Methods
        void p_readMarkersV2(void);

    public:
        #pragma mark Constructors
        SeratoMP4TrackFile(const char* trackFilePath);

        #pragma mark Instance Methods
        virtual bool hasKey(void) const;
        virtual StringAutoPtr key(void) const;
        virtual StringAutoPtr grouping(void) const;

        virtual bool hasRecordLabel(void) const;
        virtual StringAutoPtr recordLabel(void) const;
        virtual bool hasRemixer(void) const;
        virtual StringAutoPtr remixer(void) const;
        virtual bool hasDateReleased(void) const;
        virtual StringAutoPtr dateReleased(void) const;
        virtual bool hasYearReleased(void) const;
        virtual StringAutoPtr yearReleased(void) const;
    };
}

#endif /* defined(__SeratoDB__NxASeratoMP4TrackFile__) */
