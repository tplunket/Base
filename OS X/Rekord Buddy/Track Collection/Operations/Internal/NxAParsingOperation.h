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

@class NxACrate;

#import "Track Collection/Operations/NxAParsingOperation.h"

#pragma mark Private Interface

@interface NxAParsingOperation ()

#pragma mark Properties

@property (strong, nonatomic) NSMutableDictionary *p_objectSourceByObjectID;

@property (strong, nonatomic) NSMutableDictionary *p_objectIDByObjectSource;

@property (strong, nonatomic) NSMutableOrderedSet *p_objectsUpdatedStoredInParsingOrder;

@property (strong, nonatomic) NSMutableOrderedSet *p_objectsToUpdateStoredInParsingOrder;

@property (strong, nonatomic) id<NxAParserImplementation> implementation;

#pragma mark Instance Properties

- (NxAParsingResultFlags)p_parseSourcePlaylist:(id)playlist
                                      forCrate:(NxACrate *)crate
    andReplaceContentIfDifferentThanPreviouslyParsedAndOlderThan:(NSDate *)date;

- (NxAParsingResultFlags)p_parseSourceFolder:(id)folder
                                    forCrate:(NxACrate *)crate
    andReplaceContentIfDifferentThanPreviouslyParsedAndOlderThan:(NSDate *)date;

- (void)p_updateCollectionSourceFor:(NxACrate *)crate;

- (void)p_updatePlaylistSourceFor:(NxACrate *)crate;

- (void)p_updateFolderSourceFor:(NxACrate *)crate;

@end
