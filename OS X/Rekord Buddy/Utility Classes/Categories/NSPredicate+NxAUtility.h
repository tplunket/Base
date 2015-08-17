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

@interface NSPredicate (NxAUtility)

#pragma mark Instance Methods

/*!
 * Get a combined predicate of this predicate AND another one.
 * \param predicate Predicate to AND with this one.
 * \return A combination of both predicates appended together with AND.
 */
- (instancetype)predicateWithAppendingAnd:(NSPredicate *)predicate;

@end
