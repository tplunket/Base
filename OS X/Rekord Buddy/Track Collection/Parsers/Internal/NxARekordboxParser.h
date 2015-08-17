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

#import "Track Collection/Parsers/NxARekordboxParser.h"
#import "Track Collection/Operations/Internal/NxAParsingOperation.h"

#pragma mark Private Interface

@interface NxARekordboxParser ()

#pragma mark Properties

@property (strong, nonatomic) NSManagedObjectContext *p_context;

@property (strong, nonatomic) NSXMLDocument *p_collectionFile;

@property (strong, nonatomic) NSMutableDictionary *p_tracksByRekordboxTrackID;

@property (strong, nonatomic) NSMutableDictionary *p_tracksByRekordboxTrackLocation;

@property (assign, nonatomic) NSUInteger p_lastRekordboxTrackID;

@property (strong, nonatomic) NSMutableDictionary *p_rekordboxTrackIDByObjectID;

@property (readonly, nonatomic) NSString *p_nextAvailableRekordboxTrackID;

@property (strong, nonatomic) NxAMessageLog *p_log;

@property (strong, nonatomic) NSDate *collectionModificationDate;

@property (strong, nonatomic) NSDate *rootCrateModificationDate;

#pragma mark Instance Methods

- (NxATrack *)p_trackForRekordboxPlaylistIDEntryElement:(NSXMLElement *)element;

- (NxATrack *)p_trackForRekordboxPlaylistPathEntryElement:(NSXMLElement *)element;

- (void)p_trackForRekordboxTrackID:(NSString *)trackID
                                is:(NxATrack *)track;

- (NxATrack *)p_trackForRekordboxTrackID:(NSString *)trackID;

- (void)p_trackForRekordboxFileLocation:(NSString *)location
                                     is:(NxATrack *)track;

- (NxATrack *)p_trackForRekordboxFileLocation:(NSString *)location;

@end
