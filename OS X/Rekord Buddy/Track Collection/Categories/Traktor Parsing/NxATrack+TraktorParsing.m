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

#import "NxATrack+TraktorParsing.h"
#import "NxATrack+NxATrackListViewItem.h"
#import "Track Collection/Internal/NxATrack.h"
#import "Track Collection/Categories/Parsing/Internal/NxATaggedObject+Parsing.h"
#import "NxATaggedObject+XMLParsing.h"
#import "NSManagedObjectContext+NxAUtility.h"
#import "NSDate+NxAUtility.h"
#import "Track Collection/Parsers/NxATraktorParser.h"
#import "NxADebuggerLauncher.h"
#import "NSXMLElement+NxAUtility.h"
#import "NSDecimalNumber+NxAUtility.h"
#import "Track Collection/AudioFiles/OSX/NxAOSXAudioFile.h"
#import "Track Collection/NxAArtist.h"
#import "Track Collection/Tags/NxATag.h"
#import "NxATagEntry.h"
#import "NxAMarkerProperty+TraktorParsing.h"
#import "NxAParsedMarkerValue+TraktorParsing.h"
#import "NxATrackKey+TraktorParsing.h"
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
#import "NxATrackTag.h"
#import "NSDate+NxAUtility.h"
#import "NxAParsedStringValue.h"

#pragma mark Implementation

@implementation NxATrack (TraktorParsing)

#pragma mark Class Methods

+ (NSDate *)parseTraktorModifiedDateFromEntryElement:(NSXMLElement *)entryElement
                                   loggingMessagesIn:(NxAMessageLog *)log
{
    NSXMLNode *modifierDateElement = [entryElement attributeForName:@"MODIFIED_DATE"];
    NSXMLNode *modifierTimeElement = [entryElement attributeForName:@"MODIFIED_TIME"];
    if (!modifierDateElement || !modifierTimeElement) {
        NxAWarning *warning = [NxAWarning warningWithDescription:NSLocalizedString(@"in your Traktor collection with an invalid modification date.",
                                                                                   @"")];
        [log addWarning:warning inContextWithPerObjectWarningInfo:@""];

        return nil;
    }

    NSInteger seconds = modifierTimeElement.stringValue.integerValue;

    NSInteger minutes = seconds / 60;
    seconds -= minutes * 60;

    NSInteger hours = minutes / 60;
    minutes -= hours * 60;

    // -- Traktor dates are set in the Berlin timezone.
    NSString *dateString = [NSString stringWithFormat:@"%@ %ld:%ld:%ld", modifierDateElement.stringValue, hours, minutes, seconds];
    NSDate *modifiedDate = [NSDate dateWithString:dateString
                                         timeZone:@"Europe/Berlin"
                                           format:@"y/M/d H:m:s"];
    NxAAssertNotNil(modifiedDate);

    return modifiedDate;
}

#pragma mark Instance Methods

- (NxAParsingResultFlags)parseTraktorKeyElement:(NSString *)elementName
                                    fromElement:(NSXMLElement *)element
                            elementModifiedDate:(NSDate *)elementModifiedDate
                                         parser:(id<NxAParserImplementation>)parser
{
    return [self p_replaceTagsOfClass:[NxATrackKey class]
                             ifNameIs:NxAKeyPropertyTraktorKeyName
                         andOlderThan:elementModifiedDate
             andParsedValuesFromBlock:^() {
                    NSXMLElement *musicalKeyElement = [element elementForName:elementName];
                    NSXMLNode *attribute = [musicalKeyElement attributeForName:@"VALUE"];
                    NSString *attributeString = attribute.stringValue;
                    if (attributeString) {
                        NSInteger keyIndex = attributeString.integerValue;
                        NxAAssertTrue(keyIndex >= 0 && keyIndex <= 23);

                        NSNumber *keyValue = [NxATrackKey keyValueForTraktorKeyValue:keyIndex];
                        NSString *stringValue = [NxATrackKey stringValueForKeyValue:keyValue usingNotation:NxAMusicalNotationStandard];
                        if (stringValue) {
                            return @[ [NxAParsedStringValue parsedValueWithString:[NSString stringWithFormat:@"%@:%@", NxAKeyPropertyTraktorKeyName, stringValue]] ];
                        }
                    }

                    return @[ [NxAParsedStringValue parsedValueWithNoString] ];
                }
            areDifferentThanPreviouslyParsedWith:parser];
}

- (NxAParsingResultFlags)parseTraktorMarkerElementsIn:(NSXMLElement *)entryElement
                                  elementModifiedDate:(NSDate *)elementModifiedDate
                                               parser:(id<NxAParserImplementation>)parser
{
    return [self p_replaceTagsOfClass:[NxAMarkerProperty class]
                          ifOlderThan:elementModifiedDate
             andParsedValuesFromBlock:^() {
                    NSArray *newValues = [NxAParsedMarkerValue parseAndSortTraktorMarkerValuesInEntryElement:entryElement
                                                                                                         log:parser.log];
                    return newValues;
                }
        areDifferentThanPreviouslyParsedWith:parser
  thenCallThisEveryTimeATagIsReplaced:nil];
}

@end
