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

@class NxATraktorParser;
@class NxAMessageLog;

#import "NxATrack.h"
#import "NxAParserImplementation.h"
#import "NxATaggedObject+Parsing.h"

#pragma mark Public Interface

@interface NxATrack (TraktorParsing)

#pragma mark Class Methods

+ (NSDate *)parseTraktorModifiedDateFromEntryElement:(NSXMLElement *)entryElement
                                   loggingMessagesIn:(NxAMessageLog *)log;

#pragma mark Instance Methods

/*!
 * Parse track marker elements.
 * \param entryElement Element to search for the markers in.
 * \param elementModifiedDate Date the markers were last modified.
 * \param parser Parser used to parse all this.
 * \return How the element parsed affected the collection.
 */
- (NxAParsingResultFlags)parseTraktorMarkerElementsIn:(NSXMLElement *)entryElement
                                  elementModifiedDate:(NSDate *)elementModifiedDate
                                               parser:(id<NxAParserImplementation>)parser;

/*!
 * Parse a musical key element.
 * \param elementName Name of the attribute to parse.
 * \param element Element to search for the attribute in.
 * \param elementModifiedDate Date the element was last modified.
 * \param parser Parser used to parse all this.
 * \return How the element parsed affected the collection.
 */
- (NxAParsingResultFlags)parseTraktorKeyElement:(NSString *)elementName
                                    fromElement:(NSXMLElement *)element
                            elementModifiedDate:(NSDate *)elementModifiedDate
                                         parser:(id<NxAParserImplementation>)parser;

@end
