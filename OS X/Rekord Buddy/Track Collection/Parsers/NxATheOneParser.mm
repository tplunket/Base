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
#include <OneDB.h>

#import "NxAMessageLog.h"
#import "NxAProgressViewController.h"
#import "NxATagEntry.h"
#import "Track Collection/Tags/Crate Properties/NxATrackProperty.h"
#import "NxATrack+NxAOSXAudioFile.h"
#import "NxATrack+TheOneParsing.h"
#import "Track Collection/Crates/NxACrate.h"
#import "Track Collection/AudioFiles/OSX/NxAOSXAudioFile.h"
#import "NxATheOneParser.h"
#import "NSError+NxAUtility.h"

#define DISABLE_CODE_TEMPORARILY

#pragma mark Static Variables

static OneDB *_oneDB;

#pragma mark Private Interface

@interface NxATheOneParser ()

#pragma mark Properties

/*! Interface to TheOne database file parser. */
@property (readonly, nonatomic) OneDB &p_oneDB;

@end

#pragma mark - Implementation

@implementation NxATheOneParser

#pragma mark Class Methods

/*!
 * Open the database for TheOne
 * \return A handle to the opened database or nil if failed.
 */
+ (FMDatabase *)p_openDatabase
{
    NSFileManager *fileManager = [NxAOSXAudioFile fileManager];

    NSURL *databaseFileURL = [NSURL fileURLWithPath:[@"~/Library/Application Support/AudioArtery/TheOne/TheOne.db" stringByExpandingTildeInPath] isDirectory:NO];
    NSString *databaseFilePath = databaseFileURL.path;

    BOOL isDirectory = NO;
    if (![fileManager fileExistsAtPath:databaseFilePath isDirectory:&isDirectory]) {
        return nil;
    }
    else if (isDirectory) {
        return nil;
    }

    FMDatabase *db = [FMDatabase databaseWithPath:databaseFilePath];
    if (![db open]) {
        NSLog(@"Could not open db.");
        return nil;
    }

    return db;
}

+ (BOOL)databaseIsAvailable
{
    FMDatabase *db = [NxATheOneParser p_openDatabase];
    if (!db) {
        return NO;
    }

    if (![db close]) {
        return NO;
    }

    return YES;
}

#pragma mark Accessors

- (OneDB &)p_oneDB
{
    if (!_oneDB) {
        _oneDB = new OneDB();
        NxAAssertNotNil(_oneDB);
    }

    return *_oneDB;
}

#pragma mark NxAParserImplementation Protocol

+ (NSString *)nameOfTheApplication
{
    return @"The One";
}

+ (NSString *)versionPrefix
{
    return @"TH1";
}

#pragma mark Instance Methods

/*!
 * Check that the database from TheOne has the correct version.
 * \param db Handle to an open database.
 * \return YES if the version is correct, NO otherwise.
 */
- (BOOL)p_checkDbVersion:(FMDatabase *)db
{
    FMResultSet *results = [db executeQuery:@"SELECT * FROM sqlite_master"];
    NxAAssertNotNil(results);

    NSDictionary *masterCreateStatements = [NSDictionary dictionaryWithObjectsAndKeys:
                                            @"CREATE TABLE library_tracks (id integer primary key default -1, url text unique, tempo real default 0, key text, play_count int default 0, rating int default 0, creation_time int, artist varchar, title varchar, album varchar, year integer, comment text, track integer, genre varchar, length integer unsigned, modified_at datetime)",
                                            @"library_tracks",
                                            @"CREATE TABLE collections_tracks (collection_id integer, track_id integer, timeline_position integer default 0, clip_duration integer default -1, start_cut integer default -1, end_cut integer default -1, stretch real default 0, left_loop_length integer default -1, right_loop_length integer default -1, timeline_track_id integer, fx_parameter_data blob)",
                                            @"collections_tracks",
                                            @"CREATE TABLE collections (id integer primary key, name text, parent int, is_timeline boolean)",
                                            @"collections",
                                            nil];

    while ([results next]) {
        NSString *itemType = [results stringForColumnIndex:0];

        if ([itemType isEqualToString:@"index"] == YES) {
            NSString *indexName = [results stringForColumnIndex:1];
            NSString *tableName = [results stringForColumnIndex:2];

            if (([indexName hasPrefix:@"sqlite_autoindex_library_tracks_"] == NO) ||
                ([tableName isEqualToString:@"library_tracks"] == NO)) {
                return NO;
            }
        }
        else if ([itemType isEqualToString:@"table"] == YES) {
            NSString *itemName = [results stringForColumnIndex:1];
            NSString *masterStatement = [masterCreateStatements objectForKey:itemName];

            if (masterStatement == nil) {
                return NO;
            }

            NSString *createStatement = [results stringForColumnIndex:4];

            if ([createStatement isEqualToString:masterStatement] == NO) {
                return NO;
            }
        }
        else {
            return NO;
        }
    }

    return YES;
}

/*!
 * Parse tracks found in TheOne's database.
 * \param db Handle to an open database.
 * \return YES if operation succeeded, NO otherwise.
 */
- (BOOL)p_parseTracksFrom:(FMDatabase *)db
{
#ifdef DISABLE_CODE_TEMPORARILY
    return NO;
#else
    NSFileManager *fileManager = [NxAOSXAudioFile fileManager];

    NxACrate *allTracksCrate = [NxACrate allTracksCrateIn:context];
    if (!allTracksCrate) {
        NSError *error = [NSError errorWithInternalCause:NSLocalizedString(@"Error getting the track collection's all tracks crate.",
                                                                           @"")];
        [self.log addError:error];
        
        return NO;
    }

    FMResultSet *results = [db executeQuery:@"SELECT COUNT(*) from library_tracks;"];
    if (!results) {
        return NO;
    }

    NSUInteger totalCount = [results next] ? [results intForColumnIndex:0] : 0;

    NxAProgressViewController *progress = self.progress;
    progress.status = @"Parsing TheOne tracks...";
    progress.min = 0;
    progress.max = totalCount;
    progress.value = -1;

    results = [db executeQuery:@"SELECT * from library_tracks;"];
    if (!results) {
        return NO;
    }

    NSMutableArray *newTracks = [NSMutableArray arrayWithCapacity:totalCount];
    NxAMessageLog *log = self.log;
    OneDB &oneDB = self.p_oneDB;

    while ([results next]) {
        progress.value += 1;

        NSString *locationPath = [results stringForColumn:@"url"];
        NSURL *locationURL = [NSURL fileURLWithPath:locationPath];

        NxATrack *track = nil;

        // -- We find out if the track already exists.
        NxAOSXAudioFile *file = [NxAOSXAudioFile existingFileWithURL:locationURL context:context];
        if (file) {
            // -- If this file was already in our collection, get its parent track.
            track = file.parentTrack;
        }
        else {
            // -- Otherwise we create a new file.
            file = [NxAOSXAudioFile fileWithURL:locationURL context:context];
            if (!file) {
                // TODO: Error
                continue;
            }

            // -- And a new track which points to that file.
            track = [NxATrack trackFromOSXAudioFile:file context:context];
        }

        if (!track) {
            // TODO: Error
            continue;
        }

        NSURL *oneDBFileURL = [[locationURL URLByDeletingPathExtension] URLByAppendingPathExtension:@"onedb"];
        NSDictionary *attributes = [fileManager attributesOfItemAtPath:oneDBFileURL.path
                                                                 error:nil];
        if (!attributes) {
            NSError *error = [NSError errorWithCode:NxACollectionParseErrorCode
                                        description:NSLocalizedString(@"Error reading your collection for TheOne.",
                                                                      @"Generic error message when reading the collection fails.")
                                 recoverySuggestion:NSLocalizedString(@"Found a track that was not analysed by TheOne.",
                                                                      @"Tells the user that we foudn a track not analysed.")];
            [self.log addError:error];

            return NO;
        }

        NSDate *modifiedDate = attributes.fileModificationDate;
        NxATrackProperty *newTag = [NxATrackProperty tagWithValue:track context:context];
        NxATagEntry *newEntry = [NxATagEntry entryWithTag:newTag
                                            versionPrefix:self.versionPrefix
                                             dateModified:modifiedDate
                                                  context:context];

        [newTracks addObject:track];
        
        NxAParsingResultFlags result = [track parseTheOneTrackAtLocation:locationPath
                                                                   oneDB:oneDB
                                                                sqlEntry:results
                                                            modifiedDate:modifiedDate
                                                                 context:context
                                                                     log:log];
        if (log.hasAnError) {
            return NO;
        }
        
        if (result & NxAParsedObjectCausedUpdating) {
            [self hasUpdated:track];
        }

        if (result & NxAParsedObjectNeedsUpdating) {
            [self needsToUpdateSourceOf:track];
        }
    }

    [self sourceFor:allTracksCrate is:collection];
    
    Class tagClass = [NxATrackProperty class];
    NxATagEntryModificationCompareFlag result = [allTracksCrate compareTagClass:tagClass toModificationDate:modificationDate];
    switch (result) {
        case NxATagTypeWasLastModifiedOnDate: {
            
            break;
        }
        case NxATagTypeWasLastModifiedAfterDate: {
            [self needToUpdateSourceOfObject:allTracksCrate];
            
            break;
        }
        case NxATagTypeWasLastModifiedBeforeDate: {
            if ([allTracksCrate parsedValues:newTracks
                                 forTagClass:tagClass
                         areTheSameAsVersion:self.versionPrefix]) {
                break;
            }
            
            [allTracksCrate replaceTagClass:tagClass withParsedValues:newTracks modificationDate:modifiedDate];
            
            [self hasUpdated:allTracksCrate];
            
            break;
        }
    }
    
    return YES;
#endif
}

#pragma mark Overridden NSOperation Methods

- (void)main
{
    NSManagedObjectContext *context = self.context;

    @autoreleasepool {
        [context performBlockAndWait:^() {
            FMDatabase *db = [NxATheOneParser p_openDatabase];
            if (!db) {
                NSError *error = [NSError errorWithCode:NxACollectionParseErrorCode
                                            description:NSLocalizedString(@"Error reading your collection from TheOne.",
                                                                          @"Generic error message when reading the collection fails.")
                                     recoverySuggestion:NSLocalizedString(@"Can't seem to open the main db file.",
                                                                          @"Tells the user that the db file can't be found.")];
                [self.log addError:error];

                self.completionHandler(NO);

                return;
            }

            if (![self p_checkDbVersion:db]) {
                [db close];

                self.completionHandler(NO);

                return;
            }

            if (![self p_parseTracksFrom:db]) {
                [db close];

                self.completionHandler(NO);

                return;
            }

            if (![db close]) {
                NSError *error = [NSError errorWithCode:NxACollectionParseErrorCode
                                            description:NSLocalizedString(@"Error closing your collection from TheOne after syncing.",
                                                                          @"Generic error message when closing the collection fails.")
                                     recoverySuggestion:NSLocalizedString(@"Can't seem to close the main db file.",
                                                            @"Tells the user that the db file can't be closed.")];
                [self.log addError:error];

                self.completionHandler(NO);

                return;
            }

            self.completionHandler(YES);
        }];
    }
}

#pragma mark Overridden NSObject Methods

- (void)dealloc
{
    // TODO: This might never be called because of ARC. Need to look into this.
    if (_oneDB) {
        delete _oneDB;
        _oneDB = NULL;
    }
}

@end
