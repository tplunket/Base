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

#include <SeratoDb/Markers/CueMarker.hpp>
#include <SeratoDb/Markers/LoopMarker.hpp>
#include <SeratoDb/Markers/GridMarker.hpp>
#include <SeratoDB/Track.hpp>

#import "NxAParsedMarkerValue.h"

@class NxAMessageLog;

#pragma mark Public Interface

@interface NxAParsedMarkerValue (SeratoParsing)

#pragma mark Factory Methods

+ (instancetype)markerValueWithSeratoCueMarker:(const NxA::Serato::CueMarker&)cueMarker
                             loggingMessagesIn:(NxAMessageLog *)log;

+ (instancetype)markerValueWithSeratoLoopMarker:(const NxA::Serato::LoopMarker&)cueMarker
                              loggingMessagesIn:(NxAMessageLog *)log;

+ (instancetype)markerValueWithSeratoGridMarker:(const NxA::Serato::GridMarker&)gridMarker
                              loggingMessagesIn:(NxAMessageLog *)log;

#pragma mark Class Mathods

+ (NSArray *)parseAndSortMarkersValuesInSeratoTrack:(const NxA::Serato::Track&)track
                                  loggingMessagesIn:(NxAMessageLog *)log;

@end
