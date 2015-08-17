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

#import "NSManagedObjectContext+NxAUtility.h"
#import "NxATag+NxAValidationAndCompletionQuerying.h"
#import "NxATrackGenre+NxAValidationAndCompletionQuerying.h"

#pragma mark Implementation

@implementation NxATrackGenre (NxAValidationAndCompletionQuerying)

#pragma mark NxAValidationAndCompletionQuerying Protocol

+ (NSArray *)itemsWithNamesBeginningBy:(NSString *)subString
                               context:(NSManagedObjectContext *)context
{
    NxAAssertNotNil(subString);
    NxAAssertNotNil(context);

    NSArray *components = [subString componentsSeparatedByString:@":"];

    NSMutableArray *matches;
    // -- Look for an existing key.
    NSArray *results = [context fetchFromTemplate:@"GenresWithNameBeginningBy"
                            substitutionVariables:@{ @"TEXT" : components[0] }];
    if (!results || !results.count) {
        return nil;
    }

    NSMutableSet *matchingNames = [NSMutableSet set];
    for (NxATrackGenre *genre in results) {
        [matchingNames addObject:genre.stringValue];
    }

    matches = matchingNames.allObjects.mutableCopy;

    NSSortDescriptor *sortDesc = [[NSSortDescriptor alloc] initWithKey:@"description"
                                                             ascending:YES
                                                              selector:@selector(localizedStandardCompare:)];
    [matches sortUsingDescriptors:@[ sortDesc ]];

    return matches;
}

+ (BOOL)canContainMultipleValues
{
    return YES;
}

@end
