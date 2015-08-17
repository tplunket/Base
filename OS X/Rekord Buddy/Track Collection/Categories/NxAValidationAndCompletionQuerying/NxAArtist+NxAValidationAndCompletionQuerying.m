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
#import "NSCharacterSet+NxAUtility.h"
#import "NxAArtist+NxAValidationAndCompletionQuerying.h"

#pragma mark Implementation

@implementation NxAArtist (NxAValidationAndCompletionQuerying)

#pragma mark NxAValidationAndCompletionQuerying Protocol

+ (NSArray *)itemsWithNamesBeginningBy:(NSString *)subString
                               context:(NSManagedObjectContext *)context
{
    NxAAssertNotNil(subString);
    NxAAssertNotNil(context);

    // -- Look for an existing artist.
    NSArray *results = [context fetchFromTemplate:@"ArtistsWithNameBeginningBy"
                            substitutionVariables:@{ @"TEXT" : subString }];
    if (!results || !results.count) {
        return nil;
    }

    NSMutableArray *matches = [NSMutableArray arrayWithCapacity:results.count];
    for (NxAArtist *artist in results) {
        [matches addObject:[artist description]];
    }

    NSSortDescriptor *sortDesc = [[NSSortDescriptor alloc] initWithKey:@"description"
                                                             ascending:YES
                                                              selector:@selector(localizedStandardCompare:)];
    [matches sortUsingDescriptors:@[ sortDesc ]];

    return matches;
}

+ (NSCharacterSet *)validCharactersForDescription
{
    static NSMutableCharacterSet *validCharacters = nil;
    static dispatch_once_t onceToken = 0;
    dispatch_once(&onceToken, ^{
        if (!validCharacters) {
            validCharacters = [[NSCharacterSet alphanumericCharacterSet] mutableCopy];
            [validCharacters formUnionWithCharacterSet:[NSCharacterSet symbolCharacterSet]];
            [validCharacters formUnionWithCharacterSet:[NSCharacterSet punctuationCharacterSet]];
            [validCharacters formUnionWithCharacterSet:[NSCharacterSet whitespaceCharacterSet]];
        }
    });

    return validCharacters;
}

+ (BOOL)isAValidStringDescription:(NSString *)description
{
    NSCharacterSet *validCharacters = [self validCharactersForDescription];
    return [validCharacters areTheOnlyCharactersUsedInString:description];
}

+ (BOOL)isAValidNumberDescription:(NSNumber *)description
{
    return NO;
}

+ (BOOL)canContainMultipleValues
{
    return YES;
}

@end
