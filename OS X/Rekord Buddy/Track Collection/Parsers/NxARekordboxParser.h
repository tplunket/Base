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

@class NxAProgressViewController;
@class NxAMessageLog;

#import "NxAParserImplementation.h"

#pragma mark Public Interface

/*! Parser for a Rekordbox collection. */
@interface NxARekordboxParser : NSObject <NxAParserImplementation>

#pragma mark Class Methods

/*!
 * Make sure our internal Rekordbox collection file exists.
 * \param url URL used for the internal collection file.
 */
+ (void)makeSureCollectionFileExistsAt:(NSURL *)url;

/*!
 * Check if a file contains valid rekordbox XML.
 * \param url URL for the file to check.
 * \return YES if the file is valid, NO otherwise.
 */
+ (BOOL)isValidRekordboxXML:(NSURL *)url;

#pragma mark Properties

/*! URL of the Rekordbox collection file to sync with. */
@property (strong, nonatomic) NSURL *collectionFileURL;

#pragma mark Instance Methods

- (NSString *)rekordboxTrackIDFor:(NxATrack *)track;

- (NSString *)rekordboxTrackIDOrNextFreeTrackIDFor:(NxATrack *)track;

@end
