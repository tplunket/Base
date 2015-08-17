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

@class NxATrack;
@class NxACrate;
@class NxAMessageLog;

#import "NxATaggedObject.h"

#pragma mark Debug Macros

#if DEBUG
#define NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result, method, log, message) \
        { \
            NxAParsingResultFlags localResult = method; \
            if (localResult & NxAParsedObjectCausedUpdating) { \
                [log addMessage:[NSString stringWithFormat:@"  Newer %@", message]]; \
            } \
            else if (localResult & NxAParsedObjectNeedsUpdating) { \
                [log addMessage:[NSString stringWithFormat:@"  Older %@", message]]; \
            } \
            result |= localResult; \
        }
#else
#define NXA_CHECK_IF_UPDATED_AND_LOG_MESSAGE(result, method, message) \
        result |= method;
#endif

#pragma mark Public Interface

@protocol NxAParserImplementation <NSObject>

#pragma mark Factory Methods

+ (instancetype)implementationWithContext:(NSManagedObjectContext *)context
                        loggingMessagesIn:(NxAMessageLog *)log;

#pragma mark Class Methods

+ (NSString *)nameOfTheApplication;

+ (NSString *)versionPrefix;

#pragma mark Properties

@property (readonly, nonatomic) Class parserClass;

@property (readonly, nonatomic) NxAMessageLog *log;

@property (readonly, nonatomic) id rootFolderSource;

@property (readonly, nonatomic) id collectionSource;

@property (readonly, nonatomic) NSDate *collectionModificationDate;

@property (readonly, nonatomic) NSDate *rootCrateModificationDate;

@property (readwrite, nonatomic) BOOL parseOnlyToForceUpdateSavedValues;

#pragma mark Instance Methods

- (BOOL)openCollection;

- (NSString *)nameOfPlaylistSource:(id)source;

- (NSString *)nameOfFolderEntrySource:(id)source;

- (BOOL)folderEntrySourceIsAPlaylist:(id)source;

- (BOOL)folderEntrySourceIsAFolder:(id)source;

- (NSString *)locationPathForTrackSource:(id)source;

- (void)trackForLocation:(NSString *)location
                      is:(NxATrack *)track;

- (NSDate *)modificationDateForTrackSource:(id)source;

- (NxAParsingResultFlags)parseTrackSource:(id)source
                       whichWasModifiedOn:(NSDate *)modificationDate
                                 forTrack:(NxATrack *)track;

- (NxATrack *)trackForPlaylistEntrySource:(id)entry;

- (NSArray *)childrenOfCollectionSource:(id)source;

- (NSArray *)childrenOfPlaylistSource:(id)source;

- (NSArray *)childrenOfFolderSource:(id)source;

- (NSSet *)removeAndReturnValidChildrenFromCollectionSource:(id)source;

- (NSSet *)removeAndReturnValidChildrenFromPlaylistSource:(id)source;

- (NSSet *)removeAndReturnValidChildrenFromFolderSource:(id)source;

- (NSUInteger)numberOfChildrenLeftInCollectionSource:(id)source;

- (NSUInteger)numberOfChildrenLeftInFolderSource:(id)source;

- (void)updateTrackSource:(id)source
                 forTrack:(NxATrack *)track;

- (id)emptyTrackSourceFor:(NxATrack *)track;

- (id)emptyPlaylistSourceFor:(NxACrate *)playlist;

- (id)emptyFolderSourceFor:(NxACrate *)folder;

- (id)playlistEntrySourceFor:(NxATrack *)track;

- (void)addTrackSource:(id)source
             asChildOf:(id)collectionSource;

- (void)addPlaylistEntrySource:(id)source
                     asChildOf:(id)playlistSource;

- (void)addFolderEntrySource:(id)source
                   asChildOf:(id)folderSource;

- (void)setEntriesCountForCollectionSource:(id)source
                                        to:(NSNumber *)count;

- (void)setEntriesCountForPlaylistSource:(id)source
                                      to:(NSNumber *)count;

- (void)setEntriesCountForFolderSource:(id)source
                                    to:(NSNumber *)count;

- (void)deleteCollectionEntrySources:(NSSet *)objects;

- (void)deletePlaylistEntrySources:(NSSet *)objects;

- (void)deleteFolderEntrySources:(NSSet *)objects;

- (void)saveCollection;

@end
