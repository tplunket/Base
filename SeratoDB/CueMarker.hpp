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

#include <SeratoDB/DbUtility.hpp>

#include <vector>

namespace NxA {
    namespace Serato {
    #pragma mark Forward declarations
    class CueMarker;

    #pragma mark Containers
    typedef std::unique_ptr<const CueMarker> CueMarkerPtr;
    typedef std::vector<CueMarkerPtr> CueMarkerVector;
    typedef std::unique_ptr<CueMarkerVector> CueMarkerVectorPtr;

    #pragma mark Class Declaration
    class CueMarker
    {
    private:
        #pragma mark Private Instance Variable
        uint32_t p_positionInMilliSeconds;

        uint32_t p_index;

        std::string p_label;

    public:
        #pragma mark Constructors
        explicit CueMarker(const char* id3TagStart);
        explicit CueMarker(const std::string& label,
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
}
