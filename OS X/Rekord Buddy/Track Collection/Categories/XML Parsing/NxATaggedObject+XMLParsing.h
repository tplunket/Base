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

#import "NxATaggedObject.h"
#import "NxAParserImplementation.h"

#pragma mark Public Interface

@interface NxATaggedObject (XMLParsing)

#pragma mark Instance Methods

- (NxAParsingResultFlags)parseMultipleStringsFromXMLAttributeNamed:(NSString *)name
                                                       seperatedBy:(NSString *)preferedSeperator
                                                              orBy:(NSString *)otherSeperator
                                                         inElement:(NSXMLElement *)element
                                                whichWasModifiedOn:(NSDate *)date
                                                     usingTagClass:(Class)tagClass
                                                         andParser:(id<NxAParserImplementation>)parser;

- (NxAParsingResultFlags)parseStringFromXMLAttributeNamed:(NSString *)name
                                                inElement:(NSXMLElement *)element
                                       whichWasModifiedOn:(NSDate *)date
                                            usingTagClass:(Class)tagClass
                                                andParser:(id<NxAParserImplementation>)parser;

- (NxAParsingResultFlags)parseDecimalNumberFromXMLAttributeNamed:(NSString *)name
                                                       inElement:(NSXMLElement *)element
                                              whichWasModifiedOn:(NSDate *)date
                                                   usingTagClass:(Class)tagClass
                                                       andParser:(id<NxAParserImplementation>)parser;

- (NxAParsingResultFlags)parseIntegerFromXMLAttributeNamed:(NSString *)name
                                                 inElement:(NSXMLElement *)element
                                        whichWasModifiedOn:(NSDate *)date
                                             usingTagClass:(Class)tagClass
                                                 andParser:(id<NxAParserImplementation>)parser;

- (NxAParsingResultFlags)parseIntegerFromXMLAttributeNamed:(NSString *)name
                                          convertingItWith:(NSInteger (^)(NSInteger))convertionBlock
                                                 inElement:(NSXMLElement *)element
                                        whichWasModifiedOn:(NSDate *)date
                                             usingTagClass:(Class)tagClass
                                                 andParser:(id<NxAParserImplementation>)parser;

- (NxAParsingResultFlags)parseDateFromXMLAttributeNamed:(NSString *)name
                                            usingFormat:(NSString *)format
                                              inElement:(NSXMLElement *)element
                                     whichWasModifiedOn:(NSDate *)date
                                          usingTagClass:(Class)tagClass
                                              andParser:(id<NxAParserImplementation>)parser;

- (NxAParsingResultFlags)parseDateFromXMLAttributeNamed:(NSString *)name
                                          usingTimeZone:(NSString *)timeZone
                                              andFormat:(NSString *)format
                                              inElement:(NSXMLElement *)element
                                     whichWasModifiedOn:(NSDate *)date
                                          usingTagClass:(Class)tagClass
                                              andParser:(id<NxAParserImplementation>)parser;

/*!
 * Update a multiple string attribute.
 * \param attributeName Name of the attribute to update.
 * \param element Element to update the attribute in.
 * \param seperator Seperator string used to seperate values in the string attribute.
 * \param tagClass Class of tag to create with the attribute.
 */
- (void)updateXMLMultipleStringAttribute:(NSString *)attributeName
                              forElement:(NSXMLElement *)element
                               seperator:(NSString *)seperator
                                tagClass:(Class)tagClass;

/*!
 * Update a string attribute.
 * \param attributeName Name of the attribute to update.
 * \param element Element to update the attribute in.
 * \param tagClass Class of tag to get the value we're updating the attribute with.
 */
- (void)updateXMLStringAttribute:(NSString *)attributeName
                      forElement:(NSXMLElement *)element
                        tagClass:(Class)tagClass;

/*!
 * Update a decimal number attribute.
 * \param attributeName Name of the attribute to update.
 * \param element Element to search for the attribute in.
 * \param tagClass Class of tag to create with the attribute.
 * \param fractionDigits Number of digits to output as fraction part of the attribute value.
 */
- (void)updateXMLDecimalNumberAttribute:(NSString *)attributeName
                             forElement:(NSXMLElement *)element
                               tagClass:(Class)tagClass
                         fractionDigits:(NSUInteger)fractionDigits;

/*!
 * Update an integer attribute.
 * \param attributeName Name of the attribute to parse.
 * \param element Element to update the attribute in.
 * \param tagClass Class of tag to get the value we're updating the attribute with.
 * \param exportBlock If non-nil, block called to convert integer values before export.
 */
- (void)updateXMLIntegerAttribute:(NSString *)attributeName
                       forElement:(NSXMLElement *)element
                         tagClass:(Class)tagClass
                      exportBlock:(NSInteger (^)(NSInteger))exportBlock;

/*!
 * Update a date attribute.
 * \param attributeName Name of the attribute to update.
 * \param element Element to search for the attribute in.
 * \param tagClass Class of tag to create with the attribute.
 * \param timeZone Name of the timeZone the date will be converted to or nil for local timezone.
 * \param format Format used to convert the date to.
 */
- (void)updateXMLDateAttribute:(NSString *)attributeName
                    forElement:(NSXMLElement *)element
                      tagClass:(Class)tagClass
                      timeZone:(NSString *)timeZone
                        format:(NSString *)format;

@end
