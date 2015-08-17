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
#import "NxATrackRecordLabel.h"
#import "Track Collection/Tags/Track Properties/NxATrackAlbum.h"
#import "NxATrackGenre.h"
#import "NxATrackArtistCredit.h"
#import "NxATrackRemixerCredit.h"
#import "NxATrackProducerCredit.h"
#import "Track Collection/Tags/Track Properties/NxATrackKey.h"
#import "NxATrackTitle.h"
#import "NxATrackComments.h"
#import "NxATrackMixName.h"
#import "NxATrackNumber.h"
#import "NxATrackRating.h"
#import "NxATrackTag.h"
#import "NxATrackPlayCount.h"
#import "NxATrackDateAdded.h"
#import "NxATrackDateReleased.h"
#import "NxATrack+SQLiteParsing.h"
#import "NxATrack+TheOneParsing.h"
#import "NxATheOneParser.h"

#define DISABLE_CODE_TEMPORARILY

#pragma mark Implementation

@implementation NxATrack (TheOneParsing)

#pragma mark Instance Methods

- (NxAParsingResultFlags)parseTheOneTrackAtLocation:(NSString *)trackLocationPath
                                              oneDB:(OneDB &)oneDB
                                           sqlEntry:(FMResultSet *)sqlEntry
                                       modifiedDate:(NSDate *)modifiedDate
                                             parser:(id<NxAParserImplementation>)parser
{
    // -- The C++ compiler doesn't like OR-ing into the enum name. We use an NSInteger for now.
    NSUInteger result = NxAParsedObjectWasUpToDate;
    
    result |= [self parseStringFromSQLiteColumnNamed:@"title"
                                             inEntry:sqlEntry
                                  whichWasModifiedOn:modifiedDate
                                       usingTagClass:[NxATrackTitle class]
                                           andParser:parser];

    result |= [self parseMultipleStringsFromSQLiteColumnNamed:@"artist"
                                                  seperatedBy:@", "
                                                         orBy:@","
                                                      inEntry:sqlEntry
                                           whichWasModifiedOn:modifiedDate
                                                usingTagClass:[NxATrackArtistCredit class]
                                                    andParser:parser];

    result |= [self parseMultipleStringsFromSQLiteColumnNamed:@"remixer"
                                                  seperatedBy:@", "
                                                         orBy:@","
                                                      inEntry:sqlEntry
                                           whichWasModifiedOn:modifiedDate
                                                usingTagClass:[NxATrackRemixerCredit class]
                                                    andParser:parser];

    result |= [self parseStringFromSQLiteColumnNamed:@"comment"
                                             inEntry:sqlEntry
                                  whichWasModifiedOn:modifiedDate
                                       usingTagClass:[NxATrackComments class]
                                           andParser:parser];

    result |= [self parseMultipleStringsFromSQLiteColumnNamed:@"genre"
                                                  seperatedBy:@", "
                                                         orBy:@","
                                                      inEntry:sqlEntry
                                           whichWasModifiedOn:modifiedDate
                                                usingTagClass:[NxATrackGenre class]
                                                    andParser:parser];

    result |= [self parseStringFromSQLiteColumnNamed:@"label"
                                             inEntry:sqlEntry
                                  whichWasModifiedOn:modifiedDate
                                       usingTagClass:[NxATrackRecordLabel class]
                                           andParser:parser];

    result |= [self parseIntegerFromSQLiteColumnNamed:@"rating"
                                              inEntry:sqlEntry
                                   whichWasModifiedOn:modifiedDate
                                        usingTagClass:[NxATrackRating class]
                                            andParser:parser];

    result |= [self parseIntegerFromSQLiteColumnNamed:@"play_count"
                                              inEntry:sqlEntry
                                   whichWasModifiedOn:modifiedDate
                                        usingTagClass:[NxATrackPlayCount class]
                                            andParser:parser];

    result |= [self parseStringFromSQLiteColumnNamed:@"album"
                                             inEntry:sqlEntry
                                  whichWasModifiedOn:modifiedDate
                                       usingTagClass:[NxATrackAlbum class]
                                           andParser:parser];

    result |= [self parseIntegerFromSQLiteColumnNamed:@"track"
                                              inEntry:sqlEntry
                                   whichWasModifiedOn:modifiedDate
                                        usingTagClass:[NxATrackNumber class]
                                            andParser:parser];

#ifndef DISABLE_CODE_TEMPORARILY
    NxATrackKey *newKey = nil;
    if (![sqlEntry columnIsNull:@"key"]) {
        NSString *keyText = [sqlEntry stringForColumn:@"key"];
        NSNumber *keyValue = [NxATrackKey keyValueForString:keyText];
        if (keyValue) {
            newKey = [NxATrackKey tagWithName:@"TheOneKey"
                                        value:keyValue
                                      context:context];
            NxAAssertNotNil(keyValue);
        }
    }

    // -- Then we create an entry for the current value of this attribute,
    // -- and give the syncing engine a way to mofidy it if this is out of sync with
    // -- another collection.
    NxATagEntry *newEntry = [NxATagEntry entryWithTag:newKey
                                            versionID:NxATagEntryVersionTheOne
                                        setValueBlock:nil
                                         dateModified:modifiedDate
                                              context:context];
    NxAAssertNotNil(newEntry);

    [self addAndSortTag:newKey tagEntry:newEntry];

    // year integer

    OneDBFileHandle fileHandle = oneDB.open(trackLocationPath.UTF8String);

    if(oneDB.getIsCueSet(fileHandle)) {
        printf("Cue is set to: %lld\n", oneDB.getCuePosition(fileHandle));
    }

    // tempo real
    for(int i = 0; i < 10; ++i) {
        if(oneDB.getIsHotCueSet(fileHandle, i)) {
            printf("HotCue %i is set to: %lld\n", i, oneDB.getHotCuePosition(fileHandle, i));
        }

    }

    std::vector<Int64> beats;
    oneDB.getBeats(fileHandle, beats);
    printf("Found %ld beats\n", beats.size());
    
    oneDB.close(fileHandle);
#endif

    return (NxAParsingResultFlags)result;
}

@end
