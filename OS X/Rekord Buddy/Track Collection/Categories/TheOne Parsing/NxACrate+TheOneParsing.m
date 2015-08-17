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

#import <FMDB.h>

#import "NxATrack.h"
#import "Track Collection/Tags/Crate Properties/NxACrateProperty.h"
#import "Track Collection/Tags/Crate Properties/NxATrackProperty.h"
#import "NxATagEntry.h"
#import "NxAMessageLog.h"
#import "NxACrate+TheOneParsing.h"

#define DISABLE_CODE_TEMPORARILY

@implementation NxACrate (TheOneParsing)

#pragma mark Instance Methods

/*!
 * Parse all the tracks found in a given playlist.
 * \param id ID of the playlist to parse.
 * \param db Handle to an open database.
 * \param tracksForIDs Dictionary of tracks for given track IDs.
 * \param log Message log for any errors and warnings.
 * \return An array of tracks.
 */
- (NSArray *)p_parseTracksForPlaylistID:(NSUInteger)id
                                     db:(FMDatabase *)db
                           tracksForIDs:(NSDictionary *)tracksForIDs
                                    log:(NxAMessageLog *)log
{
    FMResultSet *results = [db executeQuery:[NSString stringWithFormat:@"SELECT * FROM collections_tracks WHERE collection_id is %ld", id]];
    NxAAssertNotNil(results);

    NSMutableArray *tracksFound = [NSMutableArray array];

    while ([results next]) {
        int trackID = [results intForColumn:@"track_id"];

        NxATrack *track = tracksForIDs[[NSNumber numberWithInt:trackID]];
        NxAAssertNotNil(track);

        [tracksFound addObject:track];
    }
    
    return tracksFound;
}

/*!
 * Parse a playlist.
 * \param tracks Tracks found in the playlist.
 * \param dateModified Date the playlist was last modified.
 * \param context Managed object context to create the new object in.
 * \param log Message log for any errors and warnings.
 * \return YES if the operation succeeded, NO otherwise.
 */
- (BOOL)p_parseTheOnePlaylist:(NSArray *)tracks
                 dateModified:(NSDate *)dateModified
                      context:(NSManagedObjectContext *)context
                          log:(NxAMessageLog *)log
{
#ifdef DISABLE_CODE_TEMPORARILY
    return NO;
#else
    for (NxATrack *track in tracks) {
        NxATrackProperty *newTag = [NxATrackProperty tagWithValue:track context:context];

        NxATagEntry *newEntry = [NxATagEntry entryWithTag:newTag
                                                versionID:NxATagEntryVersionTheOne
                                            setValueBlock:nil
                                             dateModified:dateModified
                                                  context:context];
        
        [self addAndSortTag:newTag tagEntry:newEntry];
    }

    return YES;
#endif
}

/*!
 * Parse a folder.
 * \param id ID of the folder to parse.
 * \param db Handle to an open database.
 * \param tracksForIDs Dictionary of tracks for given track IDs.
 * \param dateModified Date the playlist was last modified.
 * \param log Message log for any errors and warnings.
 * \return YES if the operation succeeded, NO otherwise.
 */
- (BOOL)parseTheOneFolder:(NSUInteger)id
                       db:(FMDatabase *)db
             tracksForIDs:(NSDictionary *)tracksForIDs
             dateModified:(NSDate *)dateModified
                  context:(NSManagedObjectContext *)context
                      log:(NxAMessageLog *)log
{
#ifdef DISABLE_CODE_TEMPORARILY
    return NO;
#else
    FMResultSet *results = [db executeQuery:[NSString stringWithFormat:@"SELECT * FROM collections WHERE parent is %ld", id]];
    NxAAssertNotNil(results);

    while ([results next]) {
        int collectionID = [results intForColumn:@"id"];
        NSString *collectionName = [results stringForColumn:@"name"];
        BOOL isTimeline = [results boolForColumn:@"is_timeline"];

        if (isTimeline) {
            // -- For now we just skip timeline (composite tracks) items.
            continue;
        }

        NSArray *containedTracks = [self p_parseTracksForPlaylistID:collectionID
                                                                 db:db
                                                       tracksForIDs:tracksForIDs
                                                                log:log];
        NxAAssertNotNil(containedTracks);

        NxACrate *crate = [self childCrateIfExistsOrNewEmptyCrateWithName:collectionName];
        NxACrateProperty *newCrateProperty = [NxACrateProperty tagWithValue:crate
                                                                    context:context];

        NxATagEntry *newTagEntry = [NxATagEntry entryWithTag:newCrateProperty
                                                   versionID:NxATagEntryVersionTheOne
                                               setValueBlock:nil
                                                dateModified:dateModified
                                                     context:context];

        [self addAndSortTag:newCrateProperty tagEntry:newTagEntry];

        if (containedTracks.count == 0) {
            // -- If we haven't found any tracks then this is a folder.
            if (![crate parseTheOneFolder:collectionID
                                       db:db
                             tracksForIDs:tracksForIDs
                             dateModified:dateModified
                                  context:context
                                      log:log]) {
                // -- We assume that the parser method has already logged an error.
                continue;
            }
        }
        else if (![crate p_parseTheOnePlaylist:containedTracks
                                  dateModified:dateModified
                                       context:context
                                           log:log]) {
            // -- We assume that the parser method has already logged an error.
            continue;
        }
    }
    
    return YES;
#endif
}

@end
