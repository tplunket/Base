//
//  NxASeratoMarker.h
//  SeratoDB
//
//  Created by Didier Malenfant on 2/11/15.
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

#ifndef __SeratoDB__NxASeratoLoopMarker__
#define __SeratoDB__NxASeratoLoopMarker__

#include <SeratoDB/NxASeratoDbUtility.h>

#include <vector>

namespace NxA {
    #pragma mark Forward declarations
    class SeratoLoopMarker;

    #pragma mark Containers
    typedef std::auto_ptr<const SeratoLoopMarker> SeratoLoopMarkerAutoPtr;
    typedef std::vector<SeratoLoopMarkerAutoPtr> SeratoLoopMarkerVector;
    typedef std::auto_ptr<SeratoLoopMarkerVector> SeratoLoopMarkerVectorAutoPtr;

    #pragma mark Class Declaration
    class SeratoLoopMarker
    {
    private:
        #pragma mark Private Instance Variable
        uint32_t p_startPositionInMilliSeconds;
        uint32_t p_endPositionInMilliSeconds;

        uint32_t p_index;

        StringAutoPtr p_label;

    public:
        #pragma mark Constructors
        SeratoLoopMarker(const char* id3TagStart);

        #pragma mark Instance Methods
        uint32_t startPositionInMilliseconds(void);
        uint32_t endPositionInMilliseconds(void);
        uint16_t index(void);
        const std::string& label(void);
    };
}

#endif /* defined(__SeratoDB__NxASeratoLoopMarker__) */
