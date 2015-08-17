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

@class NxATaggedObject;
@class NxATrack;
@class NxACrate;

#import "NxACollectionOperation.h"
#import "NxAParserImplementation.h"

#pragma mark Public Interface

/*! An asynchronous operation for parsing external collections. */
@interface NxAParsingOperation : NxACollectionOperation

#pragma mark Factory Methods

+ (instancetype)operationWithContext:(NSManagedObjectContext *)context
                 usingImplementation:(id<NxAParserImplementation>)implementation
                 showingProgressWith:(NxAProgressViewController *)progress
                   loggingMessagesIn:(NxAMessageLog *)log;

#pragma mark Properties

@property (readonly, nonatomic) id<NxAParserImplementation> implementation;

@property (readonly, nonatomic) BOOL hasChangesToSave;

@property (readonly, nonatomic) BOOL hasUpdatedInternalDatabase;

@property (nonatomic) BOOL forceParserToBeUpdatedByEverything;

#pragma mark Instance Methods

- (void)sourceFor:(NxATaggedObject *)object
               is:(id)source;

- (BOOL)hasSourceFor:(NxATaggedObject *)object;

- (id)sourceFor:(NxATaggedObject *)object;

- (BOOL)hasAlreadyParsedSource:(id)source;

- (NxATaggedObject *)objectForSource:(id)source;

- (void)hasUpdated:(NxATaggedObject *)object;

- (void)needsToUpdateSourceOf:(NxATaggedObject *)object;

- (void)needsToUpdateSourceOfObjectsUpdatedIn:(NxAParsingOperation *)parsingOperation;

- (void)setAnyMissingDateAddedOnUpdatedTracksTo:(NSDate *)date;

- (void)parseTrackSource:(id)source
                forTrack:(NxATrack *)track;

- (void)parseSourceFolderEntry:(id)entry
                forParentCrate:(NxACrate *)crate
 andReplaceContentIfDifferentThanPreviouslyParsedAndOlderThan:(NSDate *)date;

- (void)updateCollection;

- (void)saveCollection;

@end
