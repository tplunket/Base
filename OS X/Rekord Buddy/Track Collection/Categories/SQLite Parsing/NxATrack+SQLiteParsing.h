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

#import "NxAParserImplementation.h"
#import "NxATrack.h"
#import "NxATagEntry.h"

#pragma mark Private Interface

@interface NxATrack (SQLiteParsing)

- (NxAParsingResultFlags)parseMultipleStringsFromSQLiteColumnNamed:(NSString *)name
                                                       seperatedBy:(NSString *)preferedSeperator
                                                              orBy:(NSString *)otherSeperator
                                                           inEntry:(FMResultSet *)entry
                                                whichWasModifiedOn:(NSDate *)date
                                                     usingTagClass:(Class)tagClass
                                                         andParser:(id<NxAParserImplementation>)parser;

- (NxAParsingResultFlags)parseStringFromSQLiteColumnNamed:(NSString *)name
                                                  inEntry:(FMResultSet *)entry
                                       whichWasModifiedOn:(NSDate *)date
                                            usingTagClass:(Class)tagClass
                                                andParser:(id<NxAParserImplementation>)parser;

- (NxAParsingResultFlags)parseDecimalNumberFromSQLiteColumnNamed:(NSString *)name
                                                         inEntry:(FMResultSet *)entry
                                              whichWasModifiedOn:(NSDate *)date
                                                   usingTagClass:(Class)tagClass
                                                       andParser:(id<NxAParserImplementation>)parser;

- (NxAParsingResultFlags)parseIntegerFromSQLiteColumnNamed:(NSString *)name
                                          convertingItWith:(NSInteger (^)(NSInteger))convertionBlock
                                                   inEntry:(FMResultSet *)entry
                                        whichWasModifiedOn:(NSDate *)date
                                             usingTagClass:(Class)tagClass
                                                 andParser:(id<NxAParserImplementation>)parser;

- (NxAParsingResultFlags)parseIntegerFromSQLiteColumnNamed:(NSString *)name
                                                   inEntry:(FMResultSet *)entry
                                        whichWasModifiedOn:(NSDate *)date
                                             usingTagClass:(Class)tagClass
                                                 andParser:(id<NxAParserImplementation>)parser;

- (NxAParsingResultFlags)parseDateFromSQLiteColumnNamed:(NSString *)name
                                                inEntry:(FMResultSet *)entry
                                     whichWasModifiedOn:(NSDate *)date
                                          usingTagClass:(Class)tagClass
                                              andParser:(id<NxAParserImplementation>)parser;

@end
