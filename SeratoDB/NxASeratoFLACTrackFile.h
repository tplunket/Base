//
//  NxASeratoFLACTrackFile.h
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

#ifndef __SeratoDB__NxASeratoFLACTrackFile__
#define __SeratoDB__NxASeratoFLACTrackFile__

#include <SeratoDB/NxASeratoTrackFile.h>

#include <string>

namespace NxA {
    #pragma mark Class Declaration
    class SeratoFLACTrackFile : public SeratoTrackFile
    {
        #pragma mark Private Instance Methods
        void p_readMarkersV2(void);

    public:
        #pragma mark Constructors
        explicit SeratoFLACTrackFile(const char* trackFilePath);

        #pragma mark Instance Methods
        virtual bool hasKey(void) const;
        virtual std::string key(void) const;
        virtual std::string grouping(void) const;

        virtual uint32_t lengthInMilliseconds(void) const;
        uint32_t bitDepthInBitsOrZeroIfNotApplicable(void) const;

        virtual bool hasRecordLabel(void) const;
        virtual std::string recordLabel(void) const;
        virtual bool hasRemixer(void) const;
        virtual std::string remixer(void) const;
        virtual std::string yearReleased(void) const;

        virtual CharVectorPtr artwork(void) const;
    };
}

#endif /* defined(__SeratoDB__NxASeratoFLACTrackFile__) */
