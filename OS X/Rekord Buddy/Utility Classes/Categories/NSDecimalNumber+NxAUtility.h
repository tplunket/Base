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

@interface NSDecimalNumber (NxAUtility)

#pragma mark Factory Methods

/*!
 * Create an NSDecimalNumber.
 * \param doubleNumber Double precision floating point number to use as value.
 * \return An NSDecimalNumber representing the floating point number.
 */
+ (NSDecimalNumber *)decimalNumberWithDouble:(double)doubleNumber;

/*!
 * Create an NSDecimalNumber.
 * \param numberString String containing the number with either comma or dot as decimal seperator.
 * \return An NSDecimalNumber representing the number passed as a string or nil if failed.
 */
+ (NSDecimalNumber *)decimalNumberWithUnlocalizedString:(NSString *)numberString;

/*!
 * Create an NSDecimalNumber.
 * \param attribute XML attribute containing the number with either comma or dot as decimal seperator.
 * \return An NSDecimalNumber representing the number passed as an attribute or nil if failed.
 */
+ (NSDecimalNumber *)decimalNumberWithUnlocalizedAttribute:(NSXMLNode *)attribute;

@end
