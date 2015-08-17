//
//  Copyright (c) 2013-2015 Next Audio Labs, LLC. All rights reserved.
//
//  This file contains confidential and proprietary information of Next
//  Audio Labs, LLC ("Next"). No use is permitted without express written
//  permission of Next. If you are not a party to a Confidentiality/
//  Non-Disclosure Agreement with Next, please immediately delete this
//  file as well as all copies in your possession. For further information,
//  please email info@nextaudiolabs.com.
//

#import "NxAMarkerProperty+SeratoParsing.h"
#import "NxALoopMarkerProperty+SeratoParsing.h"
#import "NxAGridMarkerProperty+SeratoParsing.h"
#import "NxACueMarkerProperty+SeratoParsing.h"
#import "NxAMessageLog.h"

using namespace NxA;
using namespace NxA::Serato;
using namespace std;

#pragma mark Implementation

@implementation NxAMarkerProperty (SeratoParsing)

#pragma mark Class Methods

+ (void)updateMarkersInSeratoTrack:(Serato::Track&)track
                      usingMarkers:(NSArray *)markers
                               log:(NxAMessageLog *)log
{
    auto cueMarkers = CueMarker::ArrayOfConst::array();
    auto loopMarkers = LoopMarker::ArrayOfConst::array();
    auto gridMarkers = GridMarker::ArrayOfConst::array();

    for (NxAMarkerProperty *marker in markers) {
        if ([marker isKindOfClass:[NxALoopMarkerProperty class]]) {
            NxALoopMarkerProperty *loopMarker = (NxALoopMarkerProperty *)marker;
            loopMarkers->append(loopMarker.seratoLoopMarker);
        }
        else if ([marker isKindOfClass:[NxAGridMarkerProperty class]]) {
            NxAGridMarkerProperty *gridMarker = (NxAGridMarkerProperty *)marker;
            gridMarkers->append(gridMarker.seratoGridMarker);
        }
        else {
            NxAAssertTrue([marker isKindOfClass:[NxAMarkerProperty class]]);
            NxACueMarkerProperty *cueMarker = (NxACueMarkerProperty *)marker;
            cueMarkers->append(cueMarker.seratoCueMarker);
        }
    }

    track.setCueMarkers(cueMarkers);
    track.setLoopMarkers(move(loopMarkers));
    track.setGridMarkers(gridMarkers);
}

@end
