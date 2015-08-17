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

#import <FMDB.h>

#import "NxAMessageLog.h"
#import "Track Collection/Tags/Track Properties/NxATrackKey.h"
#import "NxATrackTag.h"
#import "NxATrack+SQLiteParsing.h"
#import "NxATrack+MixedInKeyParsing.h"
#import "Track Collection/Categories/Parsing/Internal/NxATaggedObject+Parsing.h"
#import "Track Collection/Internal/NxATrack.h"
#import "NxAMixedInKeyParser.h"
#import "NxAParsedIntegerValue.h"
#import "NxAParsedStringValue.h"

#pragma mark Implementation

@implementation NxATrack (MixedInKeyParsing)

#pragma mark Instance Methods

- (NxAParsingResultFlags)parseMixedInKeyTrackFromEntry:(FMResultSet *)sqlEntry
                                      modificationDate:(NSDate *)modificationDate
                                                parser:(id<NxAParserImplementation>)parser
{
    NxAParsingResultFlags results;
    results = [self p_replaceTagsOfClass:[NxATrackKey class]
                                ifNameIs:@"MixedInKey"
                            andOlderThan:modificationDate
                andParsedValuesFromBlock:^() {
                       NSString *firstKeyString = [sqlEntry stringForColumn:@"ZKEY"];
                       NSNumber *firstKey = [NxATrackKey keyValueForString:firstKeyString];
                       if (firstKey) {
                           NSString *secondKeyString = [sqlEntry stringForColumn:@"ZSECONDKEY"];
                           NSNumber *secondKey = [NxATrackKey keyValueForString:secondKeyString];
                           if (!secondKey || [firstKey isEqualTo:secondKey]) {
                               return @[ [NxAParsedIntegerValue parsedValueWithInteger:firstKey.integerValue] ];
                           }
                           else {
                               return @[ [NxAParsedIntegerValue parsedValueWithInteger:firstKey.integerValue],
                                         [NxAParsedIntegerValue parsedValueWithInteger:secondKey.integerValue] ];
                           }
                       }

                       return @[ [NxAParsedIntegerValue parsedValueWithNoInteger] ];
                   }
        areDifferentThanPreviouslyParsedWith:parser];

    results |= [self p_replaceTagsOfClass:[NxATrackTag class]
                                 ifNameIs:@"Energy"
                             andOlderThan:modificationDate
                 andParsedValuesFromBlock:^() {
                        NSString *energy = [NSString stringWithFormat:@"%ld", (NSInteger)[sqlEntry doubleForColumn:@"ZENERGY"]];
                        if (energy) {
                            return @[ [NxAParsedStringValue parsedValueWithString:energy] ];
                        }
                        else {
                            return @[ [NxAParsedStringValue parsedValueWithNoString] ];
                        }
                    }
        areDifferentThanPreviouslyParsedWith:parser];

    return results;
}

@end
