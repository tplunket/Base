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
#import "NxATag+NxAValidationAndCompletionQuerying.h"
#import "Track Collection/Tags/Track Properties/Internal/NxATrackKey.h"
#import "NxATrackKey+NxAValidationAndCompletionQuerying.h"

#pragma mark Implementation

@implementation NxATrackKey (NxAValidationAndCompletionQuerying)

#pragma mark NxAValidationAndCompletionQuerying Protocol

+ (NSArray *)itemsWithNamesBeginningBy:(NSString *)subString
                               context:(NSManagedObjectContext *)context
{
    NxAAssertNotNil(subString);
    NxAAssertNotNil(context);

    NSArray *components = [subString componentsSeparatedByString:@":"];

    NSMutableArray *matches;
    // -- Look for an existing key.
    NSArray *results = [context fetchFromTemplate:@"KeysWithNameBeginningBy"
                            substitutionVariables:@{ @"TEXT" : components[0] }];
    if (!results || !results.count) {
        return nil;
    }

    NSMutableSet *matchingNames = [NSMutableSet set];
    for (NxATrackKey *key in results) {
        [matchingNames addObject:key.description];
    }

    matches = matchingNames.allObjects.mutableCopy;

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
        validCharacters = [[NSCharacterSet alphanumericCharacterSet] mutableCopy];
        [validCharacters formUnionWithCharacterSet:[NSCharacterSet symbolCharacterSet]];
        [validCharacters formUnionWithCharacterSet:[NSCharacterSet punctuationCharacterSet]];
        [validCharacters formUnionWithCharacterSet:[NSCharacterSet whitespaceCharacterSet]];
    });

    return validCharacters;
}

+ (BOOL)isAValidStringDescription:(NSString *)description
{
    NSCharacterSet *validCharacters = [self validCharactersForDescription];
    if (![validCharacters areTheOnlyCharactersUsedInString:description]) {
        return NO;
    }

    NSString *value;

    NSArray *components = [description componentsSeparatedByString:@":"];
    if (components.count == 1) {
        value = components[0];
    }
    else {
        value = components[1];
    }

    NSDictionary *keyNames = [NxATrackKey p_keyPreferenceNames];
    NSNumber *keyValue = keyNames[value.lowercaseString];
    if (!keyValue) {
        return NO;
    }
    
    return YES;
}

+ (BOOL)canContainMultipleValues
{
    return YES;
}

@end
