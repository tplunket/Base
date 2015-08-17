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

#pragma mark Implementation

@implementation NSCharacterSet (NxAUtility)

#pragma mark Instance Methods

- (BOOL)areTheOnlyCharactersUsedInString:(NSString *)string
{
    // -- We go through all the chars and make sure they are all valid.
    for (NSInteger index = 0; index < string.length; ++index) {
        unichar c = [string characterAtIndex:index];

        if (![self characterIsMember:c]) {
            return NO;
        }
    }

    return YES;
}

@end
