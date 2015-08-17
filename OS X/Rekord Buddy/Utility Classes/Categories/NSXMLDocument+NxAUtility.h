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

#import "NxAMessageLog.h"

#pragma mark Public Interface

/*! Custom category of NSXMLDocument in order to easily filter out illegal characters. */
@interface NSXMLDocument (NxAUtility)

#pragma mark Factory Methods

/*!
 * Creates an NSXMLDocument that excludes any characters we deem illegal.
 * \param data Data to read the xml from.
 * \param illegalChars List of characters to filter out.
 * \param foundSomeIllegalChars Indicate to the caller whether illegal characters where found or not.
 * \param log Message log for any errors and warnings.
 * \return An NSXMLDocument which contains the input data, minus any illegal characters.
 */
- (instancetype)initWithDataAndIgnoreIllegalCharacters:(NSData *)data
                                          illegalChars:(NSCharacterSet *)illegalChars
                                     foundIllegalChars:(BOOL *)foundSomeIllegalChars
                                                   log:(NxAMessageLog *)log;

@end
