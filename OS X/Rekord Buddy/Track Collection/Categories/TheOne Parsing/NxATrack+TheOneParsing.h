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

class OneDB;

@class FMResultSet;
@class NxAMessageLog;

#import "NxAParserImplementation.h"
#import "NxATrack.h"

#pragma mark Public Interface

@interface NxATrack (TheOneParsing)

#pragma mark Instance Methods

/*!
 * Parse a track from TheOne.
 * \param trackLocationPath Local path of the track on this computer.
 * \param oneDB Database parser used to access the file.
 * \param sqlEntry Entry in the main sql database for the track.
 * \param modifiedDate Date the entry was last modified.
 * \param parser Parser currently being used to parse this.
 * \return YES if operation succeeded, NO otherwise.
 */
- (NxAParsingResultFlags)parseTheOneTrackAtLocation:(NSString *)trackLocationPath
                                              oneDB:(OneDB &)oneDB
                                           sqlEntry:(FMResultSet *)sqlEntry
                                       modifiedDate:(NSDate *)modifiedDate
                                             parser:(id<NxAParserImplementation>)parser;

@end
