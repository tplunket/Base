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

@interface NSOrderedSet (NxAUtility)

#pragma mark Factory Methods

/*!
 * Create an ordered set from the from the string values of an ordered set of objects.
 * \param set Ordered set of objects to use.
 * \return An ordered set of descriptions.
 */
+ (instancetype)orderedSetWithStringValuesFromOrderedSet:(NSOrderedSet *)set;

/*!
 * Create an ordered set from the from the descriptions of an ordered set of objects.
 * \param set Set of objects to use.
 * \return An odered set of descriptions.
 */
+ (instancetype)orderedSetWithDescriptionsFromOrderedSet:(NSOrderedSet *)set;


@end
