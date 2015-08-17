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

#import "NxAParserImplementation.h"
#import "Track Collection/Tags/NxATag.h"

#pragma mark Private Interface

@interface NxATaggedObject (ParsingPrivate)

#pragma mark Instance Methods

- (NxAParsingResultFlags)p_replaceTagsOfClass:(Class)tagClass
                                  ifOlderThan:(NSDate *)date
                     andParsedValuesFromBlock:(NSArray *(^)(void))newValuesBlock
         areDifferentThanPreviouslyParsedWith:(id<NxAParserImplementation>)parser
          thenCallThisEveryTimeATagIsReplaced:(void(^)(void))replacedTagBlock;

- (NxAParsingResultFlags)p_replaceTagsOfClass:(Class)tagClass
                                     ifNameIs:(NSString *)name
                                 andOlderThan:(NSDate *)date
                     andParsedValuesFromBlock:(NSArray *(^)(void))newValuesBlock
         areDifferentThanPreviouslyParsedWith:(id<NxAParserImplementation>)parser;

@end
