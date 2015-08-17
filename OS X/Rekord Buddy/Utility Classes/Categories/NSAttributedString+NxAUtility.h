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

/*! Custom category of the NSAttributedString class in order to easily build attributed strings. */
@interface NSAttributedString (NxAUtility)

#pragma mark Factory Methods

/*!
 * Creates an NSAttributed string from an NSString.
 * \param string String to convert to an NSAttributedString.
 * \return An NSAttributedString which contains the original string.
 */
+ (instancetype)stringWithString:(NSString *)string;

/*!
 * Creates an NSAttributed string from an NSString using specific attributes.
 * \param string String to convert to an NSAttributedString.
 * \param attributes Attributes to apply to the string.
 * \return An NSAttributedString which contains the original string with the attributes applied to it.
 */
+ (instancetype)stringWithString:(NSString *)string
                      attributes:(NSDictionary *)attributes;

/*!
 * Creates a clickable URL link as an NSAttributedString.
 * \param string String shown to the user
 * \param url URL that should be used when clicking on the string..
 * \return An NSAttributedString which contains the original string and points to the URL when clicked on.
 */
+ (instancetype)stringWithHyperlink:(NSString *)string
                                url:(NSURL *)url;

@end
