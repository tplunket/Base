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

@class FMDatabase;
@class NxAMessageLog;

#import "Track Collection/Crates/NxACrate.h"

#pragma mark Public Interface

@interface NxACrate (TheOneParsing)

#pragma mark Instance Methods

/*!
 * Parse folder from TheOne into this crate.
 * \param folderIndex Index of the folder in TheOne's database.
 * \param db Handle to an open database.
 * \param tracksForIDs Dictionary of tracks for given track IDs.
 * \param dateModified Date the folder was last modified.
 * \param context Managed object context to create the new object in.
 * \param log Message log for any errors and warnings.
 * \return A crate containing the same folders and playlists as the Traktor folder, or nil if failed.
 */
- (BOOL)parseTheOneFolder:(NSUInteger)folderIndex
                       db:(FMDatabase *)db
             tracksForIDs:(NSDictionary *)tracksForIDs
             dateModified:(NSDate *)dateModified
                  context:(NSManagedObjectContext *)context
                      log:(NxAMessageLog *)log;

@end
