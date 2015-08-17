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

#import <Foundation/Foundation.h>

#pragma mark Protocol

/*! Class Methods to assist validating and completing tag token field inputs. */
@protocol NxAValidationAndCompletionQuerying <NSObject>

@required

#pragma mark Class Methods

/*!
 * Items which names starts by a certain string.
 * \param subString First characters of the name we're looking for.
 * \param context Managed object context to look into.
 * \return An array of matching items, if any.
 */
+ (NSArray *)itemsWithNamesBeginningBy:(NSString *)subString
                               context:(NSManagedObjectContext *)context;

/*!
 * List of valid characters for the string description of the property.
 * \return A character set.
 */
+ (NSCharacterSet *)validCharactersForDescription;

/*!
 * Check to see if a item's string description is valid.
 * \param description Description to validate.
 * \return YES if the description is valid, NO otherwise.
 */
+ (BOOL)isAValidStringDescription:(NSString *)description;

/*!
 * Check to see if a item's number description is valid.
 * \param description Description to validate.
 * \return YES if the description is valid, NO otherwise.
 */
+ (BOOL)isAValidNumberDescription:(NSNumber *)description;

/*!
 * Whether this property can contain multiple values.
 * \return YES if the property can contain multiple values, NO otherwise.
 */
+ (BOOL)canContainMultipleValues;

@end
