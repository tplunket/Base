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

#ifndef __SeratoDB__NxASeratoCueMarker__
#define __SeratoDB__NxASeratoCueMarker__

#include <SeratoDB/NxASeratoDbUtility.h>

#include <vector>

namespace NxA {
    #pragma mark Forward declarations
    class SeratoCueMarker;

    #pragma mark Containers
    typedef std::unique_ptr<const SeratoCueMarker> SeratoCueMarkerPtr;
    typedef std::vector<SeratoCueMarkerPtr> SeratoCueMarkerVector;
    typedef std::unique_ptr<SeratoCueMarkerVector> SeratoCueMarkerVectorPtr;

    #pragma mark Class Declaration
    class SeratoCueMarker
    {
    private:
        #pragma mark Private Instance Variable
        uint32_t p_positionInMilliSeconds;

        uint32_t p_index;

        std::string p_label;

    public:
        #pragma mark Constructors
        explicit SeratoCueMarker(const char* id3TagStart);
        explicit SeratoCueMarker(const std::string& label,
                                 uint32_t positionInMilliseconds,
                                 uint16_t index) :
                                    p_positionInMilliSeconds(positionInMilliseconds),
                                    p_index(index),
                                    p_label(label) { }

        #pragma mark Instance Methods
        uint32_t positionInMilliseconds(void) const;
        uint16_t index(void) const;
        const std::string& label(void) const;

        void addId3TagTo(CharVector& data) const;
    };
}

#endif /* defined(__SeratoDB__NxASeratoCueMarker__) */
