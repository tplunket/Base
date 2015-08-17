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

#import "Track Collection/Crates/NxACrate.h"
#import "NxAParserImplementation.h"

#pragma mark Public Interface

@interface NxACrate (Parsing)

#pragma mark Instance Methods

- (NxAParsingResultFlags)replaceTracksIfOlderThan:(NSDate *)date
                   andParsedTrackValuesReturnedBy:(NSArray *(^)(void))newTracksBlock
             areDifferentThanPreviouslyParsedWith:(id<NxAParserImplementation>)parser
            thenCallThisEveryTimeATrackIsReplaced:(void(^)(void))replacedTrackBlock;

- (NxAParsingResultFlags)replaceCratesIfOlderThan:(NSDate *)date
                   andParsedCrateValuesReturnedBy:(NSArray *(^)(void))newCratesBlock
             areDifferentThanPreviouslyParsedWith:(id<NxAParserImplementation>)parser
            thenCallThisEveryTimeATrackIsReplaced:(void(^)(void))replacedTrackBlock;

@end
