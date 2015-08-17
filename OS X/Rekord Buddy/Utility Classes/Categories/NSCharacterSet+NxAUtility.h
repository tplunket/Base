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

#pragma mark Public Interface

@interface NSCharacterSet (NxAUtility)

#pragma mark Instance Methods

/*!
 * Check to see if a string uses only characters from a set.
 * \param string String to test.
 * \return YES if the string only uses characters from the set, NO otherwise.
 */
- (BOOL)areTheOnlyCharactersUsedInString:(NSString *)string;

@end
