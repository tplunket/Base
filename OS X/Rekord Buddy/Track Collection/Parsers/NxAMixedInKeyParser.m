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

#import "NxAMessageLog.h"
#import "NxAProgressViewController.h"
#import "NxATagEntry.h"
#import "Track Collection/AudioFiles/OSX/NxAOSXAudioFile.h"
#import "NxATrack+MixedInKeyParsing.h"
#import "NxAMixedInKeyParser.h"
#import "NSError+NxAUtility.h"

#pragma mark Implementation

@implementation NxAMixedInKeyParser

#pragma mark NxAParserImplementation Protocol

+ (NSString *)nameOfTheApplication
{
    return @"Mixed In Key";
}

+ (NSString *)versionPrefix
{
    return @"MIK";
}

#pragma mark Instance Methods

/*!
 * Find out when the database was last modified.
 * \return The database's last modification date or nil if failed.
 */
- (NSDate *)p_databaseModificationDate
{
    NSFileManager *fileManager = [NxAOSXAudioFile fileManager];

    NSDictionary *attributes = [fileManager attributesOfItemAtPath:self.collectionFileURL.path
                                                             error:nil];
    if (!attributes) {
        return nil;
    }
    
    NSDate *modifiedDate = attributes.fileModificationDate;
    return modifiedDate;
}

/*!
 * Open the database for MixedInKey
 * \return A handle to the opened database or nil if failed.
 */
- (FMDatabase *)p_openDatabase
{
    NSFileManager *fileManager = [NxAOSXAudioFile fileManager];

    NSString *databaseFilePath = self.collectionFileURL.path;
    
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

/*!
 * Check that the database from MixedInKey has the correct version.
 * \param db Handle to an open database.
 * \return YES if the version is correct, NO otherwise.
 */
- (BOOL)p_checkDbVersion:(FMDatabase *)db
{
    FMResultSet *results = [db executeQuery:@"SELECT * FROM sqlite_master"];
    NxAAssertNotNil(results);

    NSDictionary *masterCreateStatements = @{ @"ZCOLLECTION" : @"CREATE TABLE ZCOLLECTION ( Z_PK INTEGER PRIMARY KEY, Z_ENT INTEGER, Z_OPT INTEGER, ZINDEX INTEGER, ZISLIBRARY INTEGER, ZNAME VARCHAR )",
                                              @"Z_1SONGS" : @"CREATE TABLE Z_1SONGS ( Z_1COLLECTIONS INTEGER, Z_4SONGS INTEGER, PRIMARY KEY (Z_1COLLECTIONS, Z_4SONGS) )",
                                              @"ZENERGYSEGMENT" : @"CREATE TABLE ZENERGYSEGMENT ( Z_PK INTEGER PRIMARY KEY, Z_ENT INTEGER, Z_OPT INTEGER, ZSONG INTEGER, ZENERGY FLOAT, ZLENGTH FLOAT, ZSTARTTIME FLOAT, ZCOMMENT VARCHAR )",
                                              @"ZKEYSEGMENT" : @"CREATE TABLE ZKEYSEGMENT ( Z_PK INTEGER PRIMARY KEY, Z_ENT INTEGER, Z_OPT INTEGER, ZSINGLENOTE INTEGER, ZSONG INTEGER, ZCONFIDENCE FLOAT, ZENERGY FLOAT, ZLENGTH FLOAT, ZSTARTTIME FLOAT, ZVOLUME FLOAT, ZKEY VARCHAR )",
                                              @"ZWAVEFORM" : @"CREATE TABLE ZWAVEFORM ( Z_PK INTEGER PRIMARY KEY, Z_ENT INTEGER, Z_OPT INTEGER, ZSONG INTEGER, ZOBJECT BLOB )",
                                              @"Z_PRIMARYKEY" : @"CREATE TABLE Z_PRIMARYKEY (Z_ENT INTEGER PRIMARY KEY, Z_NAME VARCHAR, Z_SUPER INTEGER, Z_MAX INTEGER)",
                                              @"Z_METADATA" : @"CREATE TABLE Z_METADATA (Z_VERSION INTEGER PRIMARY KEY, Z_UUID VARCHAR(255), Z_PLIST BLOB)",
                                              @"ZSONG" : @"CREATE TABLE ZSONG ( Z_PK INTEGER PRIMARY KEY, Z_ENT INTEGER, Z_OPT INTEGER, ZCLIPPEDPEAKCOUNT INTEGER, ZISPERSONALIZED INTEGER, ZWAVEFORM INTEGER, ZANALYSISDATE TIMESTAMP, ZENERGY FLOAT, ZPITCH FLOAT, ZTEMPO FLOAT, ZVOLUME FLOAT, ZALBUM VARCHAR, ZARTIST VARCHAR, ZCOMMENT VARCHAR, ZFINGERPRINT VARCHAR, ZGENRE VARCHAR, ZKEY VARCHAR, ZNAME VARCHAR, ZSECONDKEY VARCHAR, ZBEATS BLOB, ZBOOKMARKDATA BLOB )"
                                              };
    NSSet *indicesNames = [NSSet setWithArray:@[ @"sqlite_autoindex_Z_1SONGS_1",
                                                 @"ZENERGYSEGMENT_ZSONG_INDEX",
                                                 @"ZKEYSEGMENT_ZSONG_INDEX",
                                                 @"ZWAVEFORM_ZSONG_INDEX",
                                                 @"ZSONG_ZWAVEFORM_INDEX",
                                                ]];
    
    while ([results next]) {
        NSString *itemType = [results stringForColumnIndex:0];

        if ([itemType isEqualToString:@"index"] == YES) {
            NSString *indexName = [results stringForColumnIndex:1];

            if (![indicesNames containsObject:indexName]) {
                return NO;
            }
        }
        else if ([itemType isEqualToString:@"table"]) {
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
 * Parse tracks found in MixedInKey's database.
 * \param db Handle to an open database.
 * \param modificationDate Date the database file was last modified.
 * \return YES if operation succeeded, NO otherwise.
 */
- (BOOL)p_parseTracksFrom:(FMDatabase *)db
         modificationDate:(NSDate *)modificationDate
{
    NxAProgressViewController *progress = self.progress;
    NxAMessageLog *log = self.log;
    NSManagedObjectContext *context = self.context;

    FMResultSet *results = [db executeQuery:@"SELECT COUNT(*) from ZSONG;"];
    if (!results) {
        return NO;
    }

    NSUInteger totalCount = [results next] ? [results intForColumnIndex:0] : 0;

    progress.status = @"Parsing Mixed In Key tracks...";
    progress.min = 0;
    progress.max = totalCount;
    progress.value = -1;

    results = [db executeQuery:@"SELECT * from ZSONG;"];
    if (!results) {
        return NO;
    }

    while ([results next]) {
        progress.value += 1;

        BOOL isStale = NO;
        NSData *bookmark = [results dataForColumn:@"ZBOOKMARKDATA"];
        NSURL *url = [NSURL URLByResolvingBookmarkData:bookmark
                                               options:0
                                         relativeToURL:nil
                                   bookmarkDataIsStale:&isStale
                                                 error:nil];
        
        if (isStale || !url) {
            continue;
        }
        
        NxAOSXAudioFile *audioFile = [NxAOSXAudioFile existingFileWithURL:url context:context];
        if (!audioFile) {
            continue;
        }
        
        NxATrack *track = audioFile.parentTrack;
        
        NxAParsingResultFlags result = [track parseMixedInKeyTrackFromEntry:results
                                                           modificationDate:modificationDate
                                                                     parser:self.implementation];
        if (log.hasAnError) {
            return NO;
        }
        
        if (result & NxAParsedObjectCausedUpdating) {
            [self hasUpdated:track];
        }
    }

    return YES;
}

#pragma mark Overridden NSOperation Methods

- (void)main
{
    NSManagedObjectContext *context = self.context;

    @autoreleasepool {
        [context performBlockAndWait:^() {
            FMDatabase *db = [self p_openDatabase];
            if (!db) {
                NSError *error = [NSError errorWithCode:NxACollectionParseErrorCode
                                            description:NSLocalizedString(@"Error reading your collection from MixedInKey.",
                                                                          @"Generic error message when reading the collection fails.")
                                     recoverySuggestion:NSLocalizedString(@"Can't seem to open the main db file.",
                                                                          @"Tells the user that the db file can't be found.")];
                [self.log addError:error];

                self.completionHandler(NO);

                return;
            }

            if (![self p_checkDbVersion:db]) {
                [db close];

                NSError *error = [NSError errorWithCode:NxACollectionParseErrorCode
                                            description:NSLocalizedString(@"Error reading your collection from MixedInKey.",
                                                                          @"Generic error message when reading the collection fails.")
                                     recoverySuggestion:NSLocalizedString(@"Can't seem to open the main db file.",
                                                                          @"Tells the user that the db file can't be found.")];
                [self.log addError:error];

                self.completionHandler(NO);

                return;
            }

            NSDate *modifiedDate = [self p_databaseModificationDate];
            if (!modifiedDate) {
                [db close];

                NSError *error = [NSError errorWithCode:NxACollectionParseErrorCode
                                            description:NSLocalizedString(@"Error reading your collection from MixedInKey.",
                                                                          @"Generic error message when reading the collection fails.")
                                     recoverySuggestion:NSLocalizedString(@"Can't seem to open the main db file.",
                                                                          @"Tells the user that the db file can't be found.")];
                [self.log addError:error];

                self.completionHandler(NO);

                return;
            }

            if (![self p_parseTracksFrom:db modificationDate:modifiedDate]) {
                [db close];

                NSError *error = [NSError errorWithCode:NxACollectionParseErrorCode
                                            description:NSLocalizedString(@"Error reading your collection from MixedInKey.",
                                                                          @"Generic error message when reading the collection fails.")
                                     recoverySuggestion:NSLocalizedString(@"Can't seem to open the main db file.",
                                                                          @"Tells the user that the db file can't be found.")];
                [self.log addError:error];
                
                self.completionHandler(NO);

                return;
            }

            if (![db close]) {
                NSError *error = [NSError errorWithCode:NxACollectionParseErrorCode
                                            description:NSLocalizedString(@"Error closing your collection from MixedInKey after syncing.",
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

@end
