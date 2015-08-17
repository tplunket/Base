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

#import "NxATestCase.h"
#import "NxATrack.h"
#import "Track Collection/Internal/NxATrack.h"

#pragma mark Public Interface

@interface NxATrackTests : NxATestCase

#pragma mark Class Methods

/*!
 * Create a stubbed track.
 * \details This track is a real object but some of its dependencies are stubbed.
 * \param path Path to use for the stubbed track.
 * \param context Context where to create the track.
 * \return A stubbed track for the given path.
 */
+ (NxATrack *)stubbedTrackWithPath:(NSString *)path
                           context:(NSManagedObjectContext *)context;

+ (NxATrack *)emptyTestTrackInContext:(NSManagedObjectContext *)context
                         usingKeyName:(NSString *)keyName
                       lastModifiedOn:(NSDate *)modificationDate;

/*!
 * Get a test track.
 * \details This track is a real object but some of its dependencies are stubbed.
 * \param context Context where to create the track.
 * \param keyName Name to use when providing a musical key for this track.
 * \param modificationDate Date when the track was last modified.
 * \return A stubbed test track.
 */
+ (NxATrack *)testTrack1InContext:(NSManagedObjectContext *)context
                          keyName:(NSString *)keyName
                 modificationDate:(NSDate *)modificationDate;

/*!
 * Get a test track.
 * \details This track is a real object but some of its dependencies are stubbed.
 * \param context Context where to create the track.
 * \param keyName Name to use when providing a musical key for this track.
 * \param modificationDate Date when the track was last modified.
 * \return A stubbed test track.
 */
+ (NxATrack *)testTrack2InContext:(NSManagedObjectContext *)context
                          keyName:(NSString *)keyName
                 modificationDate:(NSDate *)modificationDate;

/*!
 * Get a test track.
 * \details This track is a real object but some of its dependencies are stubbed.
 * \param context Context where to create the track.
 * \param keyName Name to use when providing a musical key for this track.
 * \param modificationDate Date when the track was last modified.
 * \return A stubbed test track.
 */
+ (NxATrack *)testTrack3InContext:(NSManagedObjectContext *)context
                          keyName:(NSString *)keyName
                 modificationDate:(NSDate *)modificationDate;

@end
