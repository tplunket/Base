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

@interface NSString (NxAUtility)

#pragma mark Factory Methods

/*!
 * Create a separated string from the string values of a set of objects.
 * \param seperator String to use to seperate each objects.
 * \param set Objects to use.
 * \return A seperated string.
 */
+ (instancetype)stringSeparatedBy:(NSString *)seperator
          withStringValuesFromSet:(NSSet *)set;

/*!
 * Create a separated string from the string values of an ordered set of objects.
 * \param seperator String to use to seperate each objects.
 * \param set Objects to use.
 * \return A seperated string.
 */
+ (instancetype)stringSeparatedBy:(NSString *)seperator
   withStringValuesFromOrderedSet:(NSOrderedSet *)set;

/*!
 * Create a separated string from the string values of an array of objects.
 * \param seperator String to use to seperate each objects.
 * \param array Objects to use.
 * \return A seperated string.
 */
+ (instancetype)stringSeparatedBy:(NSString *)seperator
        withStringValuesFromArray:(NSArray *)array;

#pragma mark Class Methods

+ (BOOL)stringsAreEqualOrBothNil:(NSString *)firstString
                    secondString:(NSString *)secondString;

#pragma mark Instance Methods

- (NSString *)stringByCenteringInAStringOfAtLeastWidth:(NSUInteger)width;

@end
