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

#import "NxATaggedObject+XMLParsing.h"
#import "Track Collection/Tags/NxATag.h"
#import "NxATagEntry.h"
#import "NSDate+NxAUtility.h"
#import "NSString+NxAUtility.h"
#import "NSDecimalNumber+NxAUtility.h"
#import "NSXMLElement+NxAUtility.h"
#import "Track Collection/Tags/Properties/NxAIntegerProperty.h"
#import "NxADecimalProperty.h"
#import "Track Collection/Tags/Properties/NxADateProperty.h"
#import "Track Collection/Categories/Parsing/Internal/NxATaggedObject+Parsing.h"
#import "NSDate+NxAUtility.h"
#import "NxAParsedIntegerValue.h"
#import "NxAParsedDateValue.h"
#import "NxAParsedStringValue.h"
#import "NxAParsedDecimalValue.h"

#pragma mark Implementation

@implementation NxATaggedObject (XMLParsing)

#pragma mark Instance Methods

- (NxAParsingResultFlags)p_parseIntegerFromXMLAttributeNamed:(NSString *)name
                                            convertingItWith:(NSInteger (^)(NSInteger))convertionBlock
                                                   inElement:(NSXMLElement *)element
                                          whichWasModifiedOn:(NSDate *)date
                                               usingTagClass:(Class)tagClass
                                                   andParser:(id<NxAParserImplementation>)parser
{
    NxAAssertNotNil(name);
    NxAAssertNotNil(date);

    return [self p_replaceTagsOfClass:tagClass
                          ifOlderThan:date
             andParsedValuesFromBlock:^() {
                    NxAParsedIntegerValue *newValue;
                    NSXMLNode *attribute = [element attributeForName:name];

                    NSString *stringValue = attribute.stringValue;
                    if (stringValue.length) {
                        NSInteger intValue = stringValue.integerValue;

                        if (convertionBlock) {
                            intValue = convertionBlock(intValue);
                        }

                        newValue = [NxAParsedIntegerValue parsedValueWithInteger:intValue];
                    }
                    else {
                        newValue = [NxAParsedIntegerValue parsedValueWithNoInteger];
                    }

                    return @[ newValue ];
                }
        areDifferentThanPreviouslyParsedWith:parser
  thenCallThisEveryTimeATagIsReplaced:nil];
}

- (NxAParsingResultFlags)p_parseDateFromXMLAttributeNamed:(NSString *)name
                                            usingTimeZone:(NSString *)timeZone
                                                andFormat:(NSString *)format
                                                inElement:(NSXMLElement *)element
                                       whichWasModifiedOn:(NSDate *)date
                                            usingTagClass:(Class)tagClass
                                                andParser:(id<NxAParserImplementation>)parser
{
    NxAAssertNotNil(name);
    NxAAssertNotNil(date);
    NxAAssertNotNil(format);

    return [self p_replaceTagsOfClass:tagClass
                          ifOlderThan:date
             andParsedValuesFromBlock:^() {
                    NxAParsedDateValue *newValue;
                    NSXMLNode *attribute = [element attributeForName:name];

                    NSString *stringValue = attribute.stringValue;
                    if (stringValue.length) {
                        NSDate *newDate = nil;
                        
                        if (timeZone) {
                            newDate = [NSDate dateWithString:stringValue
                                                    timeZone:timeZone
                                                      format:format];
                        }
                        else {
                            newDate = [NSDate dateWithStringInLocalTimeZone:stringValue
                                                                     format:format];
                        }

                        newValue = [NxAParsedDateValue parsedValueWithDate:newDate];
                    }
                    else {
                        newValue = [NxAParsedDateValue parsedValueWithNoDate];
                    }

                    return @[ newValue ];
                }
        areDifferentThanPreviouslyParsedWith:parser
  thenCallThisEveryTimeATagIsReplaced:nil];
}

- (NxAParsingResultFlags)parseMultipleStringsFromXMLAttributeNamed:(NSString *)name
                                                       seperatedBy:(NSString *)preferedSeperator
                                                              orBy:(NSString *)otherSeperator
                                                         inElement:(NSXMLElement *)element
                                                whichWasModifiedOn:(NSDate *)date
                                                     usingTagClass:(Class)tagClass
                                                         andParser:(id<NxAParserImplementation>)parser
{
    NxAAssertNotNil(name);
    NxAAssertNotNil(date);

    return [self p_replaceTagsOfClass:tagClass
                          ifOlderThan:date
             andParsedValuesFromBlock:^() {
                    NSMutableArray *newValues = [NSMutableArray array];
                    NSXMLNode *attribute = [element attributeForName:name];

                    NSString *stringValue = attribute.stringValue;
                    if (stringValue.length) {
                        // -- If we have an attribute by this name and it has a value,
                        // -- we seperate all the values names from it.
                        NSArray *newStrings = [[stringValue stringByReplacingOccurrencesOfString:preferedSeperator
                                                                                     withString:otherSeperator]
                                              componentsSeparatedByString:otherSeperator];
                        for (NSString *newString in newStrings) {
                            NxAParsedStringValue *newValue = [NxAParsedStringValue parsedValueWithString:newString];
                            [newValues addObject:newValue];
                        }
                    }

                    if (!newValues.count) {
                        [newValues addObject:[NxAParsedStringValue parsedValueWithNoString]];
                    }

                    return newValues;
                }
        areDifferentThanPreviouslyParsedWith:parser
  thenCallThisEveryTimeATagIsReplaced:nil];
}

- (NxAParsingResultFlags)parseStringFromXMLAttributeNamed:(NSString *)name
                                                inElement:(NSXMLElement *)element
                                       whichWasModifiedOn:(NSDate *)date
                                            usingTagClass:(Class)tagClass
                                                andParser:(id<NxAParserImplementation>)parser
{
    NxAAssertNotNil(name);
    NxAAssertNotNil(date);

    return [self p_replaceTagsOfClass:tagClass
                          ifOlderThan:date
             andParsedValuesFromBlock:^() {
                    NSXMLNode *attribute = [element attributeForName:name];
                    NSString *stringValue = attribute.stringValue;
                    if (stringValue.length) {
                        return @[ [NxAParsedStringValue parsedValueWithString:stringValue] ];
                    }
                    else {
                        return @[ [NxAParsedStringValue parsedValueWithNoString] ];
                    }
                }
        areDifferentThanPreviouslyParsedWith:parser
  thenCallThisEveryTimeATagIsReplaced:nil];
}

- (NxAParsingResultFlags)parseDecimalNumberFromXMLAttributeNamed:(NSString *)name
                                                       inElement:(NSXMLElement *)element
                                              whichWasModifiedOn:(NSDate *)date
                                                   usingTagClass:(Class)tagClass
                                                       andParser:(id<NxAParserImplementation>)parser
{
    NxAAssertNotNil(name);
    NxAAssertNotNil(date);

    return [self p_replaceTagsOfClass:tagClass
                          ifOlderThan:date
             andParsedValuesFromBlock:^() {
                    NSXMLNode *attribute = [element attributeForName:name];
                    NSString *stringValue = attribute.stringValue;
                    if (stringValue.length) {
                        NSDecimalNumber *newNumber = [NSDecimalNumber decimalNumberWithUnlocalizedString:attribute.stringValue];
                        return @[ [NxAParsedDecimalValue parsedValueWithDecimalNumber:newNumber] ];
                    }
                    else {
                        return @[ [NxAParsedDecimalValue parsedValueWithNoDecimalNumber] ];
                    }
                }
        areDifferentThanPreviouslyParsedWith:parser
  thenCallThisEveryTimeATagIsReplaced:nil];
}

- (NxAParsingResultFlags)parseIntegerFromXMLAttributeNamed:(NSString *)name
                                                 inElement:(NSXMLElement *)element
                                        whichWasModifiedOn:(NSDate *)date
                                             usingTagClass:(Class)tagClass
                                                 andParser:(id<NxAParserImplementation>)parser
{
    return [self p_parseIntegerFromXMLAttributeNamed:name
                                    convertingItWith:nil
                                           inElement:element
                                  whichWasModifiedOn:date
                                       usingTagClass:tagClass
                                           andParser:parser];
}

- (NxAParsingResultFlags)parseIntegerFromXMLAttributeNamed:(NSString *)name
                                          convertingItWith:(NSInteger (^)(NSInteger))convertionBlock
                                                 inElement:(NSXMLElement *)element
                                        whichWasModifiedOn:(NSDate *)date
                                             usingTagClass:(Class)tagClass
                                                 andParser:(id<NxAParserImplementation>)parser
{
    NxAAssertNotNil(convertionBlock);

    return [self p_parseIntegerFromXMLAttributeNamed:name
                                    convertingItWith:convertionBlock
                                           inElement:element
                                  whichWasModifiedOn:date
                                       usingTagClass:tagClass
                                           andParser:parser];
}

- (NxAParsingResultFlags)parseDateFromXMLAttributeNamed:(NSString *)name
                                            usingFormat:(NSString *)format
                                              inElement:(NSXMLElement *)element
                                     whichWasModifiedOn:(NSDate *)date
                                          usingTagClass:(Class)tagClass
                                              andParser:(id<NxAParserImplementation>)parser
{
    return [self p_parseDateFromXMLAttributeNamed:name
                                    usingTimeZone:nil
                                        andFormat:format
                                        inElement:element
                               whichWasModifiedOn:date
                                    usingTagClass:tagClass
                                        andParser:parser];
}

- (NxAParsingResultFlags)parseDateFromXMLAttributeNamed:(NSString *)name
                                          usingTimeZone:(NSString *)timeZone
                                              andFormat:(NSString *)format
                                              inElement:(NSXMLElement *)element
                                     whichWasModifiedOn:(NSDate *)date
                                          usingTagClass:(Class)tagClass
                                              andParser:(id<NxAParserImplementation>)parser
{
    NxAAssertNotNil(timeZone);

    return [self p_parseDateFromXMLAttributeNamed:name
                                    usingTimeZone:timeZone
                                        andFormat:format
                                        inElement:element
                               whichWasModifiedOn:date
                                    usingTagClass:tagClass
                                        andParser:parser];
}

- (void)updateXMLMultipleStringAttribute:(NSString *)attributeName
                              forElement:(NSXMLElement *)element
                               seperator:(NSString *)seperator
                                tagClass:(Class)tagClass
{
    NxAAssertNotNil(attributeName);
    NxAAssertNotNil(element);

    NSMutableString *value = [NSMutableString stringWithFormat:@""];

    NSArray *tags = [tagClass tagsIn:self];
    for (NxATag *tag in tags) {
        if (value.length) {
            [value appendFormat:@"%@%@", seperator, tag.stringValueWithNameIfAny];
        }
        else {
            [value appendFormat:@"%@", tag.stringValueWithNameIfAny];
        }
    }

    NSXMLNode *attribute = [element attributeForName:attributeName];
    if (attribute) {
        [attribute setStringValue:value];
    }
    else {
        [element addAttributeWithName:attributeName stringValue:value];
    }
}

- (void)updateXMLStringAttribute:(NSString *)attributeName
                      forElement:(NSXMLElement *)element
                        tagClass:(Class)tagClass
{
    NxAAssertNotNil(attributeName);
    NxAAssertNotNil(element);

    NSString *value = @"";

    NSArray *tags = [tagClass tagsIn:self];
    if (tags.count) {
        value = [tags.firstObject stringValueWithNameIfAny];
    }

    NSXMLNode *attribute = [element attributeForName:attributeName];
    if (attribute) {
        [attribute setStringValue:value];
    }
    else {
        [element addAttributeWithName:attributeName stringValue:value];
    }
}

- (void)updateXMLDecimalNumberAttribute:(NSString *)attributeName
                             forElement:(NSXMLElement *)element
                               tagClass:(Class)tagClass
                         fractionDigits:(NSUInteger)fractionDigits
{
    NxAAssertNotNil(attributeName);
    NxAAssertNotNil(element);

    NSString *value = @"";

    NSArray *tags = [tagClass tagsIn:self];
    if (tags.count) {
        value = [tags.firstObject stringValueWithFractionDigits:fractionDigits];
    }

    NSXMLNode *attribute = [element attributeForName:attributeName];
    if (attribute) {
        [attribute setStringValue:value];
    }
    else {
        [element addAttributeWithName:attributeName stringValue:value];
    }
}

- (void)updateXMLIntegerAttribute:(NSString *)attributeName
                       forElement:(NSXMLElement *)element
                         tagClass:(Class)tagClass
                      exportBlock:(NSInteger (^)(NSInteger))exportBlock
{
    NxAAssertNotNil(attributeName);
    NxAAssertNotNil(element);

    NSString *stringValue = @"";

    NSArray *tags = [tagClass tagsIn:self];
    if (tags.count) {
        NxAIntegerProperty *property = (NxAIntegerProperty *)tags.firstObject;
        NSInteger value = exportBlock ? exportBlock(property.numberValue.integerValue) : property.numberValue.integerValue;
        stringValue = [NSString stringWithFormat:@"%ld", value];
    }

    NSXMLNode *attribute = [element attributeForName:attributeName];
    if (attribute) {
        [attribute setStringValue:stringValue];
    }
    else {
        [element addAttributeWithName:attributeName stringValue:stringValue];
    }
}

- (void)updateXMLDateAttribute:(NSString *)attributeName
                    forElement:(NSXMLElement *)element
                      tagClass:(Class)tagClass
                      timeZone:(NSString *)timeZone
                        format:(NSString *)format
{
    NxAAssertNotNil(attributeName);
    NxAAssertNotNil(element);
    NxAAssertNotNil(format);

    NSString *value = @"";

    NSArray *tags = [tagClass tagsIn:self];
    if (tags.count) {
        if (timeZone) {
            value = [tags.firstObject stringValueWithTimeZone:timeZone format:format];
        }
        else {
            value = [tags.firstObject stringValueInLocalTimeZoneWithFormat:format];
        }
    }

    NSXMLNode *attribute = [element attributeForName:attributeName];
    if (attribute) {
        [attribute setStringValue:value];
    }
    else {
        [element addAttributeWithName:attributeName stringValue:value];
    }
}

@end
