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

#import "Track Collection/Parsers/NxASeratoParser.h"
#import "NxAMessageLog.h"
#import "Track Collection/Operations/Internal/NxAParsingOperation.h"

#include <SeratoDB/Database.hpp>
#include <SeratoDB/Crate.hpp>
#include <SeratoDB/Track.hpp>
#include <SeratoDB/TrackEntry.hpp>
#include "Base/Object.hpp"

#pragma mark Private Interface

@interface NxASeratoParser ()
{
    NxA::Serato::Database::Pointer* p_database;
}

#pragma mark Properties

@property (strong, nonatomic) NSManagedObjectContext *p_context;

@property (strong, nonatomic) NSMutableDictionary *p_tracksBySeratoTrackLocation;

@property (strong, nonatomic) NxAMessageLog *p_log;

@property (strong, nonatomic) NSDate *collectionModificationDate;

@property (strong, nonatomic) NSDate *rootCrateModificationDate;

@end

@interface NxASeratoDatabaseSource : NSObject
{
    NxA::Serato::Database::Pointer* value;
}

+ (NxASeratoDatabaseSource*)sourceWithSeratoDatabasePointer:(NxA::Serato::Database&)other;

- (NxA::Serato::Database&)getSeratoDatabase;

@end

@interface NxASeratoCrateSource : NSObject
{
    NxA::Serato::Crate::Pointer* value;
}

+ (NxASeratoCrateSource *)sourceWithSeratoCratePointer:(NxA::Serato::Crate&)other;

- (NxA::Serato::Crate&)getSeratoCrate;

@end

@interface NxASeratoTrackSource : NSObject
{
    NxA::Serato::Track::Pointer* value;
}

+ (NxASeratoTrackSource *)sourceWithSeratoTrackPointer:(NxA::Serato::Track&)other;

- (NxA::Serato::Track&)getSeratoTrack;

@end

@interface NxASeratoTrackEntrySource: NSObject
{
    NxA::Serato::TrackEntry::Pointer* value;
}

+ (NxASeratoTrackEntrySource *)sourceWithSeratoTrackEntryPointer:(NxA::Serato::TrackEntry&)other;

- (NxA::Serato::TrackEntry&)getSeratoTrackEntry;

@end
