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

#import "Track Collection/Tags/NxATag.h"
#import "NxATagEntry.h"
#import "NSDate+NxAUtility.h"
#import "NSString+NxAUtility.h"
#import "NSDecimalNumber+NxAUtility.h"
#import "Track Collection/Tags/Properties/NxAIntegerProperty.h"
#import "NxADecimalProperty.h"
#import "Track Collection/Internal/NxATrack.h"
#import "Track Collection/Categories/Parsing/Internal/NxATaggedObject+Parsing.h"
#import "NxATrack+SQLiteParsing.h"
#import "NxAParsedIntegerValue.h"
#import "NxAParsedDateValue.h"
#import "NxAParsedStringValue.h"
#import "NxAParsedDecimalValue.h"

#pragma mark Implementation

@implementation NxATrack (SQLiteParsing)

#pragma mark Instance Methods

- (NxAParsingResultFlags)p_parseIntegerFromSQLiteColumnNamed:(NSString *)name
                                            convertingItWith:(NSInteger (^)(NSInteger))convertionBlock
                                                     inEntry:(FMResultSet *)entry
                                          whichWasModifiedOn:(NSDate *)date
                                               usingTagClass:(Class)tagClass
                                                   andParser:(id<NxAParserImplementation>)parser
{
    NxAAssertNotNil(name);
    NxAAssertNotNil(entry);

    return [self p_replaceTagsOfClass:tagClass
                          ifOlderThan:date
             andParsedValuesFromBlock:^() {
                    if (![entry columnIsNull:name]) {
                        NSInteger intValue = [entry intForColumn:name];

                        if (convertionBlock) {
                            intValue = convertionBlock(intValue);
                        }

                        return @[ [NxAParsedIntegerValue parsedValueWithInteger:intValue] ];
                    }

                    return @[ [NxAParsedIntegerValue parsedValueWithNoInteger] ];
                }
        areDifferentThanPreviouslyParsedWith:parser
  thenCallThisEveryTimeATagIsReplaced:nil];
}

- (NxAParsingResultFlags)parseMultipleStringsFromSQLiteColumnNamed:(NSString *)name
                                                       seperatedBy:(NSString *)preferedSeperator
                                                              orBy:(NSString *)otherSeperator
                                                           inEntry:(FMResultSet *)entry
                                                whichWasModifiedOn:(NSDate *)date
                                                     usingTagClass:(Class)tagClass
                                                         andParser:(id<NxAParserImplementation>)parser
{
    return [self p_replaceTagsOfClass:tagClass
                          ifOlderThan:date
             andParsedValuesFromBlock:^() {
                    NxAAssertNotNil(name);
                    NxAAssertNotNil(preferedSeperator);
                    NxAAssertNotNil(otherSeperator);
                    NxAAssertNotNil(entry);
                    NSMutableArray *newValues = [NSMutableArray array];

                    if (![entry columnIsNull:name]) {
                        NSString *attributeString = [entry stringForColumn:name];
                        if (attributeString.length) {
                            NSArray *newStrings = [[attributeString stringByReplacingOccurrencesOfString:preferedSeperator
                                                                                             withString:otherSeperator]
                                                  componentsSeparatedByString:otherSeperator];
                            for (NSString *newString in newStrings) {
                                NxAParsedStringValue *newValue = [NxAParsedStringValue parsedValueWithString:newString];
                                [newValues addObject:newValue];
                            }
                        }
                    }

                    if (!newValues.count) {
                        [newValues addObject:[NxAParsedStringValue parsedValueWithNoString]];
                    }

                    return newValues;
                }
        areDifferentThanPreviouslyParsedWith:parser
  thenCallThisEveryTimeATagIsReplaced:nil];
}

- (NxAParsingResultFlags)parseStringFromSQLiteColumnNamed:(NSString *)name
                                                  inEntry:(FMResultSet *)entry
                                       whichWasModifiedOn:(NSDate *)date
                                            usingTagClass:(Class)tagClass
                                                andParser:(id<NxAParserImplementation>)parser
{
    NxAAssertNotNil(name);
    NxAAssertNotNil(entry);

    return [self p_replaceTagsOfClass:tagClass
                          ifOlderThan:date
             andParsedValuesFromBlock:^() {
                    if (![entry columnIsNull:name]) {
                        NSString *newValue = [entry stringForColumn:name];
                        if (newValue.length) {
                            return @[ [NxAParsedStringValue parsedValueWithString:newValue] ];
                        }
                    }

                    return @[ [NxAParsedStringValue parsedValueWithNoString] ];
                }
        areDifferentThanPreviouslyParsedWith:parser
  thenCallThisEveryTimeATagIsReplaced:nil];
}

- (NxAParsingResultFlags)parseDecimalNumberFromSQLiteColumnNamed:(NSString *)name
                                                         inEntry:(FMResultSet *)entry
                                              whichWasModifiedOn:(NSDate *)date
                                                   usingTagClass:(Class)tagClass
                                                       andParser:(id<NxAParserImplementation>)parser;

{
    NxAAssertNotNil(name);
    NxAAssertNotNil(entry);

    return [self p_replaceTagsOfClass:tagClass
                       ifOlderThan:date
             andParsedValuesFromBlock:^() {
                    if (![entry columnIsNull:name]) {
                        NSDecimalNumber *newValue = [NSDecimalNumber decimalNumberWithString:[entry stringForColumn:name]];
                        if (newValue) {
                            return @[ [NxAParsedDecimalValue parsedValueWithDecimalNumber:newValue] ];
                        }
                    }

                    return @[ [NxAParsedDecimalValue parsedValueWithNoDecimalNumber] ];
                }
        areDifferentThanPreviouslyParsedWith:parser
  thenCallThisEveryTimeATagIsReplaced:nil];
}

- (NxAParsingResultFlags)parseIntegerFromSQLiteColumnNamed:(NSString *)name
                                          convertingItWith:(NSInteger (^)(NSInteger))convertionBlock
                                                   inEntry:(FMResultSet *)entry
                                        whichWasModifiedOn:(NSDate *)date
                                             usingTagClass:(Class)tagClass
                                                 andParser:(id<NxAParserImplementation>)parser
{
    NxAAssertNotNil(convertionBlock);

    return [self p_parseIntegerFromSQLiteColumnNamed:name
                                    convertingItWith:convertionBlock
                                             inEntry:entry
                                  whichWasModifiedOn:date
                                       usingTagClass:tagClass
                                           andParser:parser];
}

- (NxAParsingResultFlags)parseIntegerFromSQLiteColumnNamed:(NSString *)name
                                                         inEntry:(FMResultSet *)entry
                                              whichWasModifiedOn:(NSDate *)date
                                                   usingTagClass:(Class)tagClass
                                                       andParser:(id<NxAParserImplementation>)parser
{
    return [self p_parseIntegerFromSQLiteColumnNamed:name
                                    convertingItWith:nil
                                             inEntry:entry
                                  whichWasModifiedOn:date
                                       usingTagClass:tagClass
                                           andParser:parser];
}

- (NxAParsingResultFlags)parseDateFromSQLiteColumnNamed:(NSString *)name
                                                inEntry:(FMResultSet *)entry
                                     whichWasModifiedOn:(NSDate *)date
                                          usingTagClass:(Class)tagClass
                                              andParser:(id<NxAParserImplementation>)parser;
{
    NxAAssertNotNil(name);
    NxAAssertNotNil(entry);

    return [self p_replaceTagsOfClass:tagClass
                          ifOlderThan:date
             andParsedValuesFromBlock:^() {
                    if (![entry columnIsNull:name]) {
                        NSDate *newValue = [entry dateForColumn:name];
                        if (newValue) {
                            return @[ [NxAParsedDateValue parsedValueWithDate:newValue] ];
                        }
                    }

                    return @[ [NxAParsedDateValue parsedValueWithNoDate] ];
                }
        areDifferentThanPreviouslyParsedWith:parser
  thenCallThisEveryTimeATagIsReplaced:nil];
}

@end
