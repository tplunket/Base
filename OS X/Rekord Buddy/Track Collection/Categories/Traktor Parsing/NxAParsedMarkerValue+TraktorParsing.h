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

@class NxAMessageLog;

#import "NxAParsedMarkerValue.h"

#pragma mark Public Interface

@interface NxAParsedMarkerValue (TraktorParsing)

#pragma mark Class Methods

+ (NSArray *)markerValuesWithTraktorCueV2Element:(NSXMLElement *)cueV2Element
                                           tempo:(NSDecimalNumber *)tempo
                                             log:(NxAMessageLog *)log;

#pragma mark Class Methods

+ (NSArray *)parseAndSortTraktorMarkerValuesInEntryElement:(NSXMLElement *)entryElement
                                                       log:(NxAMessageLog *)log;

@end
