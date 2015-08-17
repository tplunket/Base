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
#import "NxATrackDateReleased.h"
#import "NxAParsedValue.h"
#import "Track Collection/Tags/Internal/NxATaggedObject.h"
#import "Track Collection/Categories/Parsing/Internal/NxATaggedObject+Parsing.h"

#pragma mark Implementation

@implementation NxATaggedObject (Parsing)

#pragma mark Instance Methods

- (NxAParsingResultFlags)p_replaceTagsOfClass:(Class)tagClass
                                  ifOlderThan:(NSDate *)date
                     andParsedValuesFromBlock:(NSArray *(^)(void))newValuesBlock
         areDifferentThanPreviouslyParsedWith:(id<NxAParserImplementation>)parser
          thenCallThisEveryTimeATagIsReplaced:(void(^)(void))replacedTagBlock
{
    NxAAssertNotNil(tagClass);
    NxAAssertNotNil(date);
    NxAAssertNotNil(newValuesBlock);
    NxAAssertNotNil(parser);

    BOOL forceUpdateSavedValues = [parser parseOnlyToForceUpdateSavedValues];

    NxATagEntryModificationCompareFlag result = forceUpdateSavedValues ? NxATagTypeWasLastModifiedBeforeDate :
                                                                         [self compareTagClass:tagClass toModificationDate:date];
    switch (result) {
        case NxATagTypeWasLastModifiedOnDate: {
            return NxAParsedObjectWasUpToDate;
        }
        case NxATagTypeWasLastModifiedAfterDate: {
            return NxAParsedObjectNeedsUpdating;
        }
        case NxATagTypeWasLastModifiedBeforeDate: {
            NSString *versionPrefix = [parser.parserClass versionPrefix];
            NSArray *newValues = newValuesBlock();
            NxAAssertNotNil(newValues);

            if (!forceUpdateSavedValues &&
                [self parsedValues:newValues
                       forTagClass:tagClass
               areTheSameAsVersion:versionPrefix]) {
                return NxAParsedObjectWasUpToDate;
            }

            if ([tagClass isASingleValueTagType]) {
                NxAAssertTrue(newValues.count <= 1);
            }

            [self saveValues:newValues forTagClass:tagClass andVersion:versionPrefix];

            if (forceUpdateSavedValues) {
                return NxAParsedObjectWasUpToDate;
            }

            NSMutableArray *newTags = [NSMutableArray arrayWithCapacity:newValues.count];
            for (NxAParsedValue *value in newValues) {
                if (replacedTagBlock) {
                    replacedTagBlock();
                }

                NxATag *newTag = [value tagForClass:tagClass usingContext:self.managedObjectContext];
                if (!newTag) {
                    continue;
                }
                
                [newTags addObject:newTag];
            }

            [self p_replaceTagClass:tagClass
                           withTags:newTags
                   modificationDate:date];

            return NxAParsedObjectCausedUpdating;
        }
    }
}

- (NxAParsingResultFlags)p_replaceTagsOfClass:(Class)tagClass
                                     ifNameIs:(NSString *)name
                                 andOlderThan:(NSDate *)date
                     andParsedValuesFromBlock:(NSArray *(^)(void))newValuesBlock
         areDifferentThanPreviouslyParsedWith:(id<NxAParserImplementation>)parser
{
    NxAAssertNotNil(tagClass);
    NxAAssertNotNil(name);
    NxAAssertNotNil(date);
    NxAAssertNotNil(newValuesBlock);
    NxAAssertNotNil(parser);

    BOOL forceUpdateSavedValues = [parser parseOnlyToForceUpdateSavedValues];

    NxATagEntryModificationCompareFlag result = forceUpdateSavedValues ? NxATagTypeWasLastModifiedBeforeDate :
                                                                         [self compareTagClass:tagClass withName:name toModificationDate:date];
    switch (result) {
        case NxATagTypeWasLastModifiedOnDate: {
            return NxAParsedObjectWasUpToDate;
        }
        case NxATagTypeWasLastModifiedAfterDate: {
            return NxAParsedObjectNeedsUpdating;
        }
        case NxATagTypeWasLastModifiedBeforeDate: {
            NSString *versionPrefix = [parser.parserClass versionPrefix];
            NSArray *newValues = newValuesBlock();
            NxAAssertNotNil(newValues);

            if (!forceUpdateSavedValues &&
                [self parsedValues:newValues
                       forTagClass:tagClass
                          withName:name
               areTheSameAsVersion:versionPrefix]) {
                return NxAParsedObjectWasUpToDate;
            }

            if ([tagClass isASingleValueTagType]) {
                NxAAssertTrue(newValues.count <= 1);
            }

            [self saveValues:newValues forTagClass:tagClass andName:name andVersion:versionPrefix];

            if (forceUpdateSavedValues) {
                return NxAParsedObjectWasUpToDate;
            }

            NSMutableArray *newTags = [NSMutableArray arrayWithCapacity:newValues.count];
            for (NxAParsedValue *value in newValues) {
                NxATag *newTag = [value tagForClass:tagClass usingContext:self.managedObjectContext];
                [newTags addObject:newTag];
            }

            [self p_replaceTagClass:tagClass
                            andName:name
                           withTags:newTags
                   modificationDate:date];

            return NxAParsedObjectCausedUpdating;
        }
    }
}

@end
