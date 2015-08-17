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

#include <SeratoDb/TrackFiles/TrackFileFactory.hpp>
#include <SeratoDb/TrackFiles/TrackFile.hpp>

#import "NxATrack+SeratoParsing.h"
#import "Track Collection/Categories/Parsing/Internal/NxATaggedObject+Parsing.h"
#import "Track Collection/Internal/NxATrack.h"
#import "NxATrack+Parsing.h"
#import "NxATrack+NxAOSXAudioFile.h"
#import "Track Collection/Tags/Crate Properties/NxATrackProperty.h"
#import "NxATagEntry.h"
#import "NxATrackTag.h"
#import "NxATrackRecordLabel.h"
#import "Track Collection/Tags/Track Properties/NxATrackAlbum.h"
#import "NxATrackGenre.h"
#import "NxATrackArtistCredit.h"
#import "NxATrackRemixerCredit.h"
#import "NxATrackProducerCredit.h"
#import "Track Collection/Tags/Track Properties/NxATrackKey.h"
#import "NxATrackTitle.h"
#import "NxATrackComments.h"
#import "NxATrackMixName.h"
#import "NxATrackNumber.h"
#import "NxATrackRating.h"
#import "NxATrackPlayCount.h"
#import "NxATrackDateAdded.h"
#import "NxATrackDateReleased.h"
#import "NxATrackDateModified.h"
#import "NxATrackBitRate.h"
#import "NxATrackBitDepth.h"
#import "NxATrackSampleRate.h"
#import "NxATrackSize.h"
#import "NxATrackTitle.h"
#import "Track Collection/Tags/Track Properties/NxATrackKey.h"
#import "Track Collection/Tags/Properties/NxAArtworkProperty.h"
#import "Track Collection/Tags/Marker Properties/NxACueMarkerProperty.h"
#import "Track Collection/Tags/Marker Properties/NxAGridMarkerProperty.h"
#import "NxAParsedMarkerValue+SeratoParsing.h"
#import "NxAMarkerProperty+SeratoParsing.h"
#import "NSDecimalNumber+NxAUtility.h"
#import "NSDate+NxAUtility.h"
#import "NxAParsedIntegerValue.h"
#import "NxAParsedDateValue.h"
#import "NxAParsedStringValue.h"
#import "NxAParsedDecimalValue.h"
#import "NxAMessageLog.h"

using namespace NxA;
using namespace std;

#pragma mark Implementation

@implementation NxATrack (SeratoParsing)

#pragma mark Instance Methods

- (NxAParsingResultFlags)p_parseMarkersInSeratoTrack:(const Serato::Track&)track
                                         ifOlderThan:(NSDate *)date
 andParsedValuesAreDifferentThanPreviouslyParsedWith:(id<NxAParserImplementation>)parser
{
    return [self p_replaceTagsOfClass:[NxAMarkerProperty class]
                          ifOlderThan:date
             andParsedValuesFromBlock:^() {
                    NSArray *newValues = [NxAParsedMarkerValue parseAndSortMarkersValuesInSeratoTrack:track
                                                                                    loggingMessagesIn:parser.log];
                    return newValues;
                }
        areDifferentThanPreviouslyParsedWith:parser
  thenCallThisEveryTimeATagIsReplaced:nil];
}

- (NxAParsingResultFlags)p_parseKeyInSeratoTrack:(const Serato::Track&)track
                                     ifOlderThan:(NSDate *)date
 andParsedValuesAreDifferentThanPreviouslyParsedWith:(id<NxAParserImplementation>)parser
{
    return [self p_replaceTagsOfClass:[NxATrackKey class]
                             ifNameIs:@"Key"
                         andOlderThan:date
             andParsedValuesFromBlock:^() {
                    auto& key = track.key();
                    if (key.length()) {
                        NSString *stringValue = [NSString stringWithUTF8String:key.toUTF8()];
                        if (stringValue && [NxATrackKey stringIsAValidKeyValue:stringValue]) {
                            return @[ [NxAParsedStringValue parsedValueWithString:[NSString stringWithFormat:@"Key:%@", stringValue]] ];
                        }
                    }

                    return @[ [NxAParsedStringValue parsedValueWithNoString] ];
                }
        areDifferentThanPreviouslyParsedWith:parser];
}

- (NxAParsingResultFlags)p_parseIntegerFrom:(NSInteger)value
                           convertingItWith:(NSInteger (^)(NSInteger))convertionBlock
                          andUpdateTagClass:(Class)tagClass
                                ifOlderThan:(NSDate *)date
 andParsedValuesAreDifferentThanPreviouslyParsedWith:(id<NxAParserImplementation>)parser
{
    NxAAssertNotNil(date);

    return [self p_replaceTagsOfClass:tagClass
                          ifOlderThan:date
             andParsedValuesFromBlock:^() {
                    NSInteger intValue = value;
                    if (convertionBlock) {
                        intValue = convertionBlock(intValue);
                    }

                    return @[ [NxAParsedIntegerValue parsedValueWithInteger:intValue] ];
                }
        areDifferentThanPreviouslyParsedWith:parser
  thenCallThisEveryTimeATagIsReplaced:nil];
}

- (NxAParsingResultFlags)p_parseYearFrom:(const String&)stringValue
                       andUpdateTagClass:(Class)tagClass
                             ifOlderThan:(NSDate *)date
 andParsedValuesAreDifferentThanPreviouslyParsedWith:(id<NxAParserImplementation>)parser
{
    NxAAssertNotNil(date);

    return [self p_replaceTagsOfClass:tagClass
                          ifOlderThan:date
             andParsedValuesFromBlock:^() {
                    if (stringValue.length()) {
                        NSString *yearString = [[NSString stringWithUTF8String:stringValue.toUTF8()]  substringToIndex:4];
                        NSString *fullString = [NSString stringWithFormat:@"%@-01-01", yearString];
                        NSDate *newDate = [NSDate dateWithStringInLocalTimeZone:fullString
                                                                         format:@"yyyy-MM-dd"];
                        if (newDate) {
                            return @[ [NxAParsedDateValue parsedValueWithDate:newDate] ];
                        }
                    }

                    return @[ [NxAParsedDateValue parsedValueWithNoDate] ];
                }
        areDifferentThanPreviouslyParsedWith:parser
  thenCallThisEveryTimeATagIsReplaced:nil];
}

- (NxAParsingResultFlags)p_parseDateFromTimeIntervalSince1970:(NSTimeInterval)intervalInSeconds
                                            andUpdateTagClass:(Class)tagClass
                                                  ifOlderThan:(NSDate *)date
          andParsedValuesAreDifferentThanPreviouslyParsedWith:(id<NxAParserImplementation>)parser
{
    NxAAssertNotNil(date);

    return [self p_replaceTagsOfClass:tagClass
                          ifOlderThan:date
             andParsedValuesFromBlock:^() {
                    if (intervalInSeconds) {
                        NSDate *newDate = [NSDate dateWithTimeIntervalSince1970:intervalInSeconds];
                        NxAAssertNotNil(newDate);

                        return @[ [NxAParsedDateValue parsedValueWithDate:newDate] ];
                    }

                    return @[ [NxAParsedDateValue parsedValueWithNoDate] ];
                }
        areDifferentThanPreviouslyParsedWith:parser
  thenCallThisEveryTimeATagIsReplaced:nil];
}

- (NxAParsingResultFlags)p_parseMultipleStringsFrom:(const String&)stringValue
                                        seperatedBy:(NSString *)preferedSeperator
                                               orBy:(NSString *)otherSeperator
                                  andUpdateTagClass:(Class)tagClass
                                        ifOlderThan:(NSDate *)date
 andParsedValuesAreDifferentThanPreviouslyParsedWith:(id<NxAParserImplementation>)parser
{
    NxAAssertNotNil(date);

    return [self p_replaceTagsOfClass:tagClass
                          ifOlderThan:date
             andParsedValuesFromBlock:^() {
                    NSMutableArray *newValues = [NSMutableArray array];

                    if (stringValue.length()) {
                        NSString *objCString = [NSString stringWithUTF8String:stringValue.toUTF8()];
                        // -- If we have an attribute by this name and it has a value,
                        // -- we seperate all the values names from it.
                        NSArray *newStrings = [[objCString stringByReplacingOccurrencesOfString:preferedSeperator
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

- (NxAParsingResultFlags)p_parseStringFrom:(const String&)stringValue
                         andUpdateTagClass:(Class)tagClass
                               ifOlderThan:(NSDate *)date
 andParsedValuesAreDifferentThanPreviouslyParsedWith:(id<NxAParserImplementation>)parser
{
    NxAAssertNotNil(date);

    return [self p_replaceTagsOfClass:tagClass
                          ifOlderThan:date
             andParsedValuesFromBlock:^() {
                    if (stringValue.length()) {
                        return @[ [NxAParsedStringValue parsedValueWithString:[NSString stringWithUTF8String:stringValue.toUTF8()]] ];
                    }
                    else {
                        return @[ [NxAParsedStringValue parsedValueWithNoString] ];
                    }
                }
        areDifferentThanPreviouslyParsedWith:parser
  thenCallThisEveryTimeATagIsReplaced:nil];
}

- (NxAParsingResultFlags)p_parseDecimalNumberFrom:(const String&)stringValue
                                andUpdateTagClass:(Class)tagClass
                                      ifOlderThan:(NSDate *)date
 andParsedValuesAreDifferentThanPreviouslyParsedWith:(id<NxAParserImplementation>)parser
{
    NxAAssertNotNil(date);

    return [self p_replaceTagsOfClass:tagClass
                          ifOlderThan:date
             andParsedValuesFromBlock:^() {
                    if (stringValue.length()) {
                        NSString *objCString = [NSString stringWithUTF8String:stringValue.toUTF8()];
                        NSDecimalNumber *newNumber = [NSDecimalNumber decimalNumberWithUnlocalizedString:objCString];
                        return @[ [NxAParsedDecimalValue parsedValueWithDecimalNumber:newNumber] ];
                    }
                    else {
                        return @[ [NxAParsedDecimalValue parsedValueWithNoDecimalNumber] ];
                    }
                }
        areDifferentThanPreviouslyParsedWith:parser
  thenCallThisEveryTimeATagIsReplaced:nil];
}

- (NxAParsingResultFlags)p_parseIntegerFrom:(NSInteger)value
                          andUpdateTagClass:(Class)tagClass
                                ifOlderThan:(NSDate *)date
 andParsedValuesAreDifferentThanPreviouslyParsedWith:(id<NxAParserImplementation>)parser
{
    return [self p_parseIntegerFrom:value
                   convertingItWith:nil
                  andUpdateTagClass:tagClass
                        ifOlderThan:date
        andParsedValuesAreDifferentThanPreviouslyParsedWith:parser];
}

- (NxAParsingResultFlags)parseIntegerFrom:(NSInteger)value
                         convertingItWith:(NSInteger (^)(NSInteger))convertionBlock
                        andUpdateTagClass:(Class)tagClass
                              ifOlderThan:(NSDate *)date
 andParsedValuesAreDifferentThanPreviouslyParsedWith:(id<NxAParserImplementation>)parser
{
    NxAAssertNotNil(convertionBlock);

    return [self p_parseIntegerFrom:value
                   convertingItWith:convertionBlock
                  andUpdateTagClass:tagClass
                        ifOlderThan:date
        andParsedValuesAreDifferentThanPreviouslyParsedWith:parser];
}

- (NSString *)p_stringValueForTagClass:(Class)tagClass
{
    NSArray *tags = [tagClass tagsIn:self];
    if (tags.count) {
        return [tags.firstObject stringValue];
    }
    else {
        return @"";
    }
}

- (NSString *)p_multipleStringsValueForTagClass:(Class)tagClass
                                    seperatedBy:(NSString *)seperator
{
    NSMutableString *result = [NSMutableString stringWithString:@""];

    NSArray *tags = [tagClass tagsIn:self];
    for (NxATag *tag in tags) {
        if (result.length) {
            [result appendString:seperator];
        }

        [result appendString:tag.stringValue];
    }

    return result;
}

- (NSInteger)p_integerValueForTagClass:(Class)tagClass
{
    NSArray *tags = [tagClass tagsIn:self];
    if (tags.count) {
        NxAIntegerProperty *property = (NxAIntegerProperty *)tags.firstObject;
        return property.numberValue.integerValue;
    }
    else {
        return 0;
    }
}

- (NSString *)p_stringValueForDateTagClass:(Class)tagClass
                               usingFormat:(NSString *)format
{
    NSArray *tags = [tagClass tagsIn:self];
    if (tags.count) {
        NxADateProperty *property = (NxADateProperty *)tags.firstObject;
        return [property stringValueInLocalTimeZoneWithFormat:format];
    }
    else {
        return @"";
    }
}

- (NSInteger)p_numberOfSecondsSince1970ForDateTagClass:(Class)tagClass
{
    NSArray *tags = [tagClass tagsIn:self];
    if (tags.count) {
        NxADateProperty *property = (NxADateProperty *)tags.firstObject;
        return property.numberOfSecondsSince1970InLocalTime;
    }
    else {
        return 0;
    }
}

- (NSString*)p_keyStringValueForTagClass:(Class)tagClass
                                 andName:(NSString *)name
                             seperatedBy:(NSString *)seperator
{
    NSMutableString *result = [NSMutableString stringWithString:@""];

    NSArray *tags = [tagClass tagsIn:self withName:name];
    for (NxATag *tag in tags) {
        if (result.length) {
            [result appendString:seperator];
        }

        NxATrackKey *key = (NxATrackKey *)tag;
        [result appendString:[key stringValueForNotation:NxAMusicalNotationStandard]];
    }

    return result;
}

- (NxAParsingResultFlags)parseSeratoPropertiesFromTrack:(const Serato::Track&)track
                                         lastModifiedOn:(NSDate *)modificationDate
                               andUpdateTagsUsingParser:(id<NxAParserImplementation>)parser
{
    NSInteger result = NxAParsedObjectWasUpToDate;

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [self p_parseStringFrom:track.title()
                                               andUpdateTagClass:[NxATrackTitle class]
                                                     ifOlderThan:modificationDate
             andParsedValuesAreDifferentThanPreviouslyParsedWith:parser],
                                         parser.log, @"Title");

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [self p_parseMultipleStringsFrom:track.artist()
                                                              seperatedBy:@", "
                                                                     orBy:@","
                                                        andUpdateTagClass:[NxATrackArtistCredit class]
                                                              ifOlderThan:modificationDate
                      andParsedValuesAreDifferentThanPreviouslyParsedWith:parser],
                                         parser.log, @"Artists");

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [self p_parseStringFrom:track.album()
                                               andUpdateTagClass:[NxATrackAlbum class]
                                                     ifOlderThan:modificationDate
             andParsedValuesAreDifferentThanPreviouslyParsedWith:parser],
                                         parser.log, @"Album");

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [self p_parseIntegerFrom:track.trackNumber()
                                                andUpdateTagClass:[NxATrackNumber class]
                                                      ifOlderThan:modificationDate
              andParsedValuesAreDifferentThanPreviouslyParsedWith:parser],
                                         parser.log, @"Album Track Number");

    if (![parser parseOnlyToForceUpdateSavedValues]) {
        NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                             [self ifNotAlreadyPresentAddNilTagsForTagClass:[NxATrackMixName class]
                                                                              createdOnDate:modificationDate],
                                             parser.log, @"Remix Name (set to nil)");
    }

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [self p_parseMultipleStringsFrom:track.genre()
                                                              seperatedBy:@" / "
                                                                     orBy:@"/"
                                                        andUpdateTagClass:[NxATrackGenre class]
                                                              ifOlderThan:modificationDate
                      andParsedValuesAreDifferentThanPreviouslyParsedWith:parser],
                                         parser.log, @"Genre");

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [self p_parseStringFrom:track.recordLabel()
                                               andUpdateTagClass:[NxATrackRecordLabel class]
                                                     ifOlderThan:modificationDate
             andParsedValuesAreDifferentThanPreviouslyParsedWith:parser],
                                         parser.log, @"Record Label");

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [self p_parseStringFrom:track.comments()
                                               andUpdateTagClass:[NxATrackComments class]
                                                     ifOlderThan:modificationDate
             andParsedValuesAreDifferentThanPreviouslyParsedWith:parser],
                                         parser.log, @"Comments");

    if (![parser parseOnlyToForceUpdateSavedValues]) {
        NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                             [self ifNotAlreadyPresentAddNilTagsForTagClass:[NxATrackTag class] createdOnDate:modificationDate],
                                             parser.log, @"Track Tag (set to nil)");
    }

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [self p_parseMultipleStringsFrom:track.remixer()
                                                              seperatedBy:@", "
                                                                     orBy:@","
                                                        andUpdateTagClass:[NxATrackRemixerCredit class]
                                                              ifOlderThan:modificationDate
                      andParsedValuesAreDifferentThanPreviouslyParsedWith:parser],
                                         parser.log, @"Remixers");

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [self p_parseMultipleStringsFrom:track.composer()
                                                              seperatedBy:@", "
                                                                     orBy:@","
                                                        andUpdateTagClass:[NxATrackProducerCredit class]
                                                              ifOlderThan:modificationDate
                      andParsedValuesAreDifferentThanPreviouslyParsedWith:parser],
                                         parser.log, @"Producers");

    if (![parser parseOnlyToForceUpdateSavedValues]) {
        NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                             [self ifNotAlreadyPresentAddNilTagsForTagClass:[NxATrackPlayCount class]
                                                                              createdOnDate:modificationDate],
                                             parser.log, @"Playcount (set to nil)");

        NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                             [self ifNotAlreadyPresentAddNilTagsForTagClass:[NxATrackRating class]
                                                                              createdOnDate:modificationDate],
                                             parser.log, @"Rating (set to nil)");
    }

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [self p_parseDateFromTimeIntervalSince1970:track.dateAddedInSecondsSinceJanuary1st1970()
                                                                  andUpdateTagClass:[NxATrackDateAdded class]
                                                                        ifOlderThan:modificationDate
                                andParsedValuesAreDifferentThanPreviouslyParsedWith:parser],
                                         parser.log, @"Date Added");

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [self p_parseKeyInSeratoTrack:track
                                                           ifOlderThan:modificationDate
                   andParsedValuesAreDifferentThanPreviouslyParsedWith:parser],
                                         parser.log, @"Key");

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [self p_parseMarkersInSeratoTrack:track
                                                               ifOlderThan:modificationDate
                       andParsedValuesAreDifferentThanPreviouslyParsedWith:parser],
                                         parser.log, @"Markers");

    NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result,
                                         [self p_parseYearFrom:track.year()
                                             andUpdateTagClass:[NxATrackDateReleased class]
                                                   ifOlderThan:modificationDate
           andParsedValuesAreDifferentThanPreviouslyParsedWith:parser],
                                         parser.log, @"Date Released");

    return (NxAParsingResultFlags)result;
}

- (void)updateSeratoTrack:(Serato::Track&)track
{
    track.setTitle(String::stringWith([[self p_stringValueForTagClass:[NxATrackTitle class]] UTF8String]));
    track.setArtist(String::stringWith([[self p_multipleStringsValueForTagClass:[NxATrackArtistCredit class]
                                                                    seperatedBy:@", "] UTF8String]));
    track.setAlbum(String::stringWith([[self p_stringValueForTagClass:[NxATrackAlbum class]] UTF8String]));
    track.setGenre(String::stringWith([[self p_multipleStringsValueForTagClass:[NxATrackGenre class]
                                                                   seperatedBy:@", "] UTF8String]));
    track.setComments(String::stringWith([[self p_stringValueForTagClass:[NxATrackComments class]] UTF8String]));
    track.setRemixer(String::stringWith([[self p_multipleStringsValueForTagClass:[NxATrackRemixerCredit class]
                                                                     seperatedBy:@", "] UTF8String]));
    track.setRecordLabel(String::stringWith([[self p_stringValueForTagClass:[NxATrackRecordLabel class]] UTF8String]));
    track.setComposer(String::stringWith([[self p_multipleStringsValueForTagClass:[NxATrackProducerCredit class]
                                                                      seperatedBy:@", "] UTF8String]));
    track.setKey(String::stringWith([[self p_keyStringValueForTagClass:[NxATrackKey class] andName:@"Key" seperatedBy:@", "] UTF8String]));

    NSArray *markers = [NxAMarkerProperty tagsIn:self];
    if (markers.count) {
        NSDecimalNumber *averageBPM = [NxAGridMarkerProperty averageBpmForMarkers:markers];
        track.setBpm(String::stringWith([[NSString stringWithFormat:@"%3.2f", averageBPM.floatValue] UTF8String]));
    }
    else {
        track.setBpm(String::stringWith("0"));
    }

    track.setYear(String::stringWith([[self p_stringValueForDateTagClass:[NxATrackDateReleased class] usingFormat:@"yyyy"] UTF8String]));
    track.setTrackNumber((uinteger32)[self p_integerValueForTagClass:[NxATrackNumber class]]);
    track.setDateAddedInSecondsSinceJanuary1st1970((timestamp)[self p_numberOfSecondsSince1970ForDateTagClass:[NxATrackDateAdded class]]);
    track.setDateModifiedInSecondsSinceJanuary1st1970((timestamp)[self p_numberOfSecondsSince1970ForDateTagClass:[NxATrackDateModified class]]);

    [NxAMarkerProperty updateMarkersInSeratoTrack:track usingMarkers:markers log:nil];
}

@end
