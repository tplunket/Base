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

@class NxARekordboxParser;
@class NxAMessageLog;

#import "NxATrack.h"
#import "NxAParserImplementation.h"
#import "NxATaggedObject+Parsing.h"

#pragma mark Public Interface

@interface NxATrack (RekordboxParsing)

#pragma mark Instance Methods

/*!
 * Parse a musical key element.
 * \param elementName Name of the attribute to parse.
 * \param element Element to search for the attribute in.
 * \param elementModifiedDate Date the element was last modified.
 * \param parser Parser currently used to parse this.
 * \return How the element parsed affected the collection.
 */
- (NxAParsingResultFlags)parseRekordboxKeyElement:(NSString *)elementName
                                      fromElement:(NSXMLElement *)element
                              elementModifiedDate:(NSDate *)elementModifiedDate
                                           parser:(id<NxAParserImplementation>)parser;

/*!
 * Parse track marker elements.
 * \param trackElement Element to search for the markers in.
 * \param elementModifiedDate Date the markers were last modified.
 * \param parser Parser currently used to parse this.
 * \return How the element parsed affected the collection.
 */
- (NxAParsingResultFlags)parseRekordboxMarkerElementsIn:(NSXMLElement *)trackElement
                                    elementModifiedDate:(NSDate *)elementModifiedDate
                                                 parser:(NxARekordboxParser *)parser;

@end
