//
//  NxASeratoOGGTrackFile.h
//  SeratoDB
//
//  Created by Didier Malenfant on 2/13/15.
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

#ifndef __SeratoDB__NxASeratoOGGTrackFile__
#define __SeratoDB__NxASeratoOGGTrackFile__

#include <SeratoDB/NxASeratoID3TrackFile.h>

namespace NxA {
    #pragma mark Class Declaration
    class SeratoOGGTrackFile : public SeratoID3TrackFile
    {
    private:
        #pragma mark Private Instance Methods
        void p_readMarkersV2(void);

    public:
        #pragma mark Constructors
        SeratoOGGTrackFile(const char* filePath);

        #pragma mark Instance Methods
        virtual bool hasKey(void) const;
        virtual StringAutoPtr key(void) const;
        virtual StringAutoPtr grouping(void) const;

        virtual bool hasRecordLabel(void) const;
        virtual StringAutoPtr recordLabel(void) const;
        virtual bool hasRemixer(void) const;
        virtual StringAutoPtr remixer(void) const;
        virtual StringAutoPtr yearReleased(void) const;

        virtual CharVectorAutoPtr artwork(void) const;
    };
}

#endif /* defined(__SeratoDB__NxASeratoOGGTrackFile__) */
