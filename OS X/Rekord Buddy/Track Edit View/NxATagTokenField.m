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

#import "NxAValidationAndCompletionQuerying.h"
#import "NxATrackTag.h"
#import "NSManagedObjectContext+NxAUtility.h"
#import "Track Edit View/Internal/NxATagTokenField.h"

#pragma mark Implementation

@implementation NxATagTokenField

#pragma mark NSNibAwaking Protocol

- (void)awakeFromNib
{
    [super awakeFromNib];
    
    self.delegate = self;
    self.p_itemsClass = [NxATrackTag class];
}

#pragma mark NSTokenFieldDelegate Protocol

- (NSTokenStyle)tokenField:(NSTokenField *)tokenField
 styleForRepresentedObject:(id)representedObject
{
    return NSRoundedTokenStyle;
}

- (BOOL)          tokenField:(NSTokenField *)tokenField
 hasMenuForRepresentedObject:(id)representedObject
{
	return NO;
}

- (NSMenu *)   tokenField:(NSTokenField *)tokenField
 menuForRepresentedObject:(id)representedObject
{
	return nil;
}

- (NSArray *)tokenField:(NSTokenField *)tokenField
       shouldAddObjects:(NSArray *)newTokens
                atIndex:(NSUInteger)index
{
    if (![self.p_itemsClass canContainMultipleValues]) {
        // -- If our property can only contain one value, we test to see if we have one already.
        NSArray *currentTokens = self.objectValue;
        if (currentTokens.count > 1) {
            return NO;
        }
    }

    NSMutableArray *validTokens = [NSMutableArray arrayWithCapacity:newTokens.count];

    // -- Go through the new tokens and only keep the valid ones.
    for (NSString *token in newTokens) {
        if ([self.p_itemsClass isAValidStringDescription:token]) {
            [validTokens addObject:token];
        }
    }

    return validTokens;
}

- (NSArray *) tokenField:(NSTokenField *)tokenField
 completionsForSubstring:(NSString *)substring
            indexOfToken:(NSInteger)tokenIndex
     indexOfSelectedItem:(NSInteger *)selectedIndex
{
    NxAAssertNotNil(self.p_itemsClass);

    NSMutableArray *matches = [[self.p_itemsClass itemsWithNamesBeginningBy:substring
                                                                    context:self.extendedDelegate.objectContext] mutableCopy];

    // -- We don't want to suggest things that are already in the token field.
    NSArray *currentValues = self.objectValue;
    [matches removeObjectsInArray:currentValues];

    // -- We don't want to select anything in the list. We let the user do that.
    *selectedIndex = -1;

    return matches;
}

#pragma mark NSTextFieldDelegate protocol

- (BOOL)control:(NSControl *)control
  isValidObject:(id)object
{
    if (![self.p_itemsClass canContainMultipleValues]) {
        // -- If our property can only contain one value, we test to see if we have one already.
        NSArray *currentTokens = self.objectValue;
        if (currentTokens.count > 1) {
            return NO;
        }

        return currentTokens.count ? [self.p_itemsClass isAValidStringDescription:currentTokens[0]] : YES;
    }

    if (![object isKindOfClass:[NSString class]]) {
        return NO;
    }

    NSString *description = object;
    NSMutableString *key = [NSMutableString stringWithFormat:@""];

    // -- We go through all the chars and find the first string of non-control characters.
    for (NSInteger index = 0; index < description.length; ++index) {
        unichar c = [description characterAtIndex:index];

        if (c != NSAttachmentCharacter) {
            [key appendString:[NSString stringWithFormat:@"%C", c]];
        }
        else if (key.length) {
            // -- If we've bumped into a NSAttachmentCharacter and we have a string then this is the
            // -- user's last inputted text which needs to be validated.
            break;
        }
    }

    if (key.length) {
        // -- If we found a user entry, we test it to make sure it's valid.
        // -- Note: There can be only one entry left as others would have been validated
        // --        by tokenField:shouldAddObjects:atIndex:
        if (![self.p_itemsClass isAValidStringDescription:key]) {
            return NO;
        }
    }
    
    return YES;
}

@end
