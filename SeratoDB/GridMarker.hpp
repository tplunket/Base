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
    #pragma mark Forward Declarations
    class GridMarker;

    #pragma mark Containers
    typedef std::unique_ptr<const GridMarker> GridMarkerPtr;
    typedef std::vector<GridMarkerPtr> GridMarkerVector;
    typedef std::unique_ptr<GridMarkerVector> GridMarkerVectorPtr;

    #pragma mark Class Declaration
    class GridMarker
    {
    private:
        #pragma mark Private Instance Variables
        float p_positionInSeconds;

        float p_bpm;

    public:
        #pragma mark Constructors
        explicit GridMarker(const char* markerData);
        explicit GridMarker(float positionInSeconds,
                            float bpm) :
                        p_positionInSeconds(positionInSeconds),
                        p_bpm(bpm) { }

        #pragma mark Class Methods
        static const char* nextGridMarkerAfter(const char* markerData);

        #pragma mark Instance Methods
        float positionInSeconds(void) const;
        float bpm(void) const;

        void addDataTo(CharVector& data) const;
    };
}
}