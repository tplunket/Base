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

@interface NSSet (NxAUtility)

#pragma mark Factory Methods

/*!
 * Create a set from the from the string values of a set of objects.
 * \param set Set of objects to use.
 * \return An set of string values.
 */
+ (instancetype)setWithStringValuesFromSet:(NSSet *)set;

/*!
 * Create a set from the from the descriptions of a set of objects.
 * \param set Set of objects to use.
 * \return A set of descriptions.
 */
+ (instancetype)setWithDescriptionsFromSet:(NSSet *)set;

@end
