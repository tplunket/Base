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

#import "NSCharacterSet+NxAUtility.h"
#import "NSManagedObjectContext+NxAUtility.h"
#import "NxATag+NxAValidationAndCompletionQuerying.h"
#import "NxATrackTag+NxAValidationAndCompletionQuerying.h"

#pragma mark Implementation

@implementation NxATrackTag (NxAValidationAndCompletionQuerying)

#pragma mark NxAValidationAndCompletionQuerying Protocol

+ (NSArray *)itemsWithNamesBeginningBy:(NSString *)subString
                               context:(NSManagedObjectContext *)context
{
    NxAAssertNotNil(subString);
    NxAAssertNotNil(context);

    NSArray *components = [subString componentsSeparatedByString:@":"];

    // -- Look for an existing key.
    NSMutableArray *results = [[context fetchFromTemplate:@"TrackTagsWithNameBeginningBy"
                                    substitutionVariables:@{ @"TEXT" : components[0] }] mutableCopy];
    [results addObjectsFromArray:[context fetchFromTemplate:@"TrackBooleanTagsWithNameBeginningBy"
                                      substitutionVariables:@{ @"TEXT" : components[0] }]];
    if (!results.count) {
        return nil;
    }

    NSMutableSet *matchingNames = [NSMutableSet set];
    for (NxATag *tag in results) {
        [matchingNames addObject:tag.description];
    }

    NSMutableArray *matches = matchingNames.allObjects.mutableCopy;
    
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
