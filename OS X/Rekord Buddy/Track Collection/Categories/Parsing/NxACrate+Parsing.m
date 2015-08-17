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

#import "Track Collection/Tags/Crate Properties/NxATrackProperty.h"
#import "NxAParserImplementation.h"
#import "Track Collection/Crates/Internal/NxACrate.h"
#import "Track Collection/Categories/Parsing/Internal/NxATaggedObject+Parsing.h"
#import "NxACrate+Parsing.h"

#pragma mark Implementation

@implementation NxACrate (Parsing)

#pragma mark Instance Methods

- (NxAParsingResultFlags)replaceTracksIfOlderThan:(NSDate *)date
                   andParsedTrackValuesReturnedBy:(NSArray *(^)(void))newTracksBlock
             areDifferentThanPreviouslyParsedWith:(id<NxAParserImplementation>)parser
            thenCallThisEveryTimeATrackIsReplaced:(void(^)(void))replacedTrackBlock
{
    return [self p_replaceTagsOfClass:[NxATrackProperty class]
                          ifOlderThan:date
             andParsedValuesFromBlock:newTracksBlock
 areDifferentThanPreviouslyParsedWith:parser
  thenCallThisEveryTimeATagIsReplaced:replacedTrackBlock];
}

- (NxAParsingResultFlags)replaceCratesIfOlderThan:(NSDate *)date
                   andParsedCrateValuesReturnedBy:(NSArray *(^)(void))newCratesBlock
             areDifferentThanPreviouslyParsedWith:(id<NxAParserImplementation>)parser
            thenCallThisEveryTimeATrackIsReplaced:(void(^)(void))replacedTrackBlock
{
    return [self p_replaceTagsOfClass:[NxACrateProperty class]
                          ifOlderThan:date
             andParsedValuesFromBlock:newCratesBlock
 areDifferentThanPreviouslyParsedWith:parser
  thenCallThisEveryTimeATagIsReplaced:replacedTrackBlock];
}

@end
