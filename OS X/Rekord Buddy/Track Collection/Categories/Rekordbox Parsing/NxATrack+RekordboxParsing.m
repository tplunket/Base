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

#import "NxATrack+RekordboxParsing.h"
#import "NxATrack+NxATrackListViewItem.h"
#import "Track Collection/Internal/NxATrack.h"
#import "Track Collection/Categories/Parsing/Internal/NxATaggedObject+Parsing.h"
#import "NxATaggedObject+XMLParsing.h"
#import "NSManagedObjectContext+NxAUtility.h"
#import "NSDate+NxAUtility.h"
#import "Track Collection/Parsers/NxARekordboxParser.h"
#import "NxADebuggerLauncher.h"
#import "NSXMLElement+NxAUtility.h"
#import "NSDecimalNumber+NxAUtility.h"
#import "Track Collection/AudioFiles/OSX/NxAOSXAudioFile.h"
#import "Track Collection/NxAArtist.h"
#import "Track Collection/Tags/NxATag.h"
#import "NxATagEntry.h"
#import "Track Collection/Categories/Rekordbox Parsing/NxAMarkerProperty+RekordboxParsing.h"
#import "Track Collection/Tags/Marker Properties/NxAGridMarkerProperty.h"
#import "NxAParsedMarkerValue+RekordboxParsing.h"
#import "NxATrackKey+RekordboxParsing.h"
#import "NxATrackRecordLabel.h"
#import "Track Collection/Tags/Track Properties/NxATrackAlbum.h"
#import "NxATrackGenre.h"
#import "NxATrackArtistCredit.h"
#import "NxATrackRemixerCredit.h"
#import "NxATrackProducerCredit.h"
#import "Track Collection/Tags/Track Properties/NxATrackKey.h"
#import "NxATrackKey+RekordboxParsing.h"
#import "NxATrackTitle.h"
#import "NxATrackComments.h"
#import "NxATrackMixName.h"
#import "NxATrackNumber.h"
#import "NxATrackRating.h"
#import "NxATrackPlayCount.h"
#import "NxATrackDateAdded.h"
#import "NxATrackDateReleased.h"
#import "NxATrackTag.h"
#import "NxAParsedIntegerValue.h"
#import "NxAParsedStringValue.h"

#pragma mark Implementation

@implementation NxATrack (RekordboxParsing)

#pragma mark Instance Methods

- (NxAParsingResultFlags)parseRekordboxKeyElement:(NSString *)elementName
                                      fromElement:(NSXMLElement *)element
                              elementModifiedDate:(NSDate *)elementModifiedDate
                                           parser:(id<NxAParserImplementation>)parser
{
    return [self p_replaceTagsOfClass:[NxATrackKey class]
                             ifNameIs:NxAKeyPropertyRekordboxKeyName
                         andOlderThan:elementModifiedDate
             andParsedValuesFromBlock:^() {
                    NSXMLNode *attribute = [element attributeForName:elementName];
                    NSString *stringValue = attribute.stringValue;
                    if (stringValue && [NxATrackKey stringIsAValidKeyValue:stringValue]) {
                        return @[ [NxAParsedStringValue parsedValueWithString:[NSString stringWithFormat:@"%@:%@", NxAKeyPropertyRekordboxKeyName, stringValue]] ];
                    }

                    return @[ [NxAParsedStringValue parsedValueWithNoString] ];
                }
        areDifferentThanPreviouslyParsedWith:parser];
}

- (NxAParsingResultFlags)parseRekordboxMarkerElementsIn:(NSXMLElement *)trackElement
                                    elementModifiedDate:(NSDate *)elementModifiedDate
                                                 parser:(NxARekordboxParser *)parser
{
    return [self p_replaceTagsOfClass:[NxAMarkerProperty class]
                          ifOlderThan:elementModifiedDate
             andParsedValuesFromBlock:^() {
                    NSArray *newValues = [NxAParsedMarkerValue parseAndSortRekordboxMarkerValuesInTrackElement:trackElement
                                                                                                           log:parser.log];
                    return newValues;
                }
        areDifferentThanPreviouslyParsedWith:parser
  thenCallThisEveryTimeATagIsReplaced:nil];
}

@end
