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

@interface NSNumber (NxAUtility)

#pragma mark Class Methods

+ (BOOL)numbersAreEqualOrBothNil:(NSNumber *)firstNumber
                    secondNumber:(NSNumber *)secondNumber;

#pragma mark Properties

/*!
 * Get a string representing the number as a time value.
 * \details The number must be a time length, in seconds.
 *          The string is returned as HH:MM:SS or MM:SS depending on the length.
 */
@property (readonly, nonatomic) NSString *stringValueAsTimeRepresentation;

@end
