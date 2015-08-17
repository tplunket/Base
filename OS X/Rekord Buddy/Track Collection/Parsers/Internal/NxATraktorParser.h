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

#import "Track Collection/Parsers/NxATraktorParser.h"
#import "Track Collection/Operations/Internal/NxAParsingOperation.h"

#pragma mark Private Interface

@interface NxATraktorParser ()

#pragma mark Properties

@property (strong, nonatomic) NSManagedObjectContext *p_context;

@property (strong, nonatomic) NSMutableDictionary *p_traktorPlaylistPathByTrackLocation;

@property (strong, nonatomic) NSMutableDictionary *p_traktorPlaylistPathByTrackID;

@property (strong, nonatomic) NSMutableDictionary *p_trackByTraktorPlaylistPath;

@property (strong, nonatomic) NSXMLDocument *p_collectionFile;

@property (readonly, nonatomic) NSXMLElement *p_trackCollectionElement;

@property (strong, nonatomic) NxAMessageLog *p_log;

@property (strong, nonatomic) NSDate *collectionModificationDate;

@property (strong, nonatomic) NSDate *rootCrateModificationDate;

@end
