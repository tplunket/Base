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

#import "Track Collection/Tags/NxATag.h"

#pragma mark Public Interface

/*! A track property in a crate. */
@interface NxATrackProperty : NxATag

#pragma mark Class Methods

/*!
 * Get an array of all the tracks from a list of properties.
 * \param properties List of properties to parse.
 * \return An array of all the tracks contained in the track properties.
 */
+ (NSArray *)arrayOfTracksFromProperties:(NSArray *)properties;

/*!
 * Get the parent crates which contain some track properties.
 * \param properties Track properties to look into.
 * \return A set of the parent crates for these properties.
 */
+ (NSSet *)parentCratesForTrackProperties:(NSSet *)properties;

/*!
 * Get an array of track properties from the pasteboard.
 * \param pasteboard Pasteboard to read the track properties from.
 * \param context Managed object context to get the tracks from.
 * \return An array of tracks properties matching the representations or nil if failed.
 */
+ (NSArray *)trackPropertiesFromPasteboard:(NSPasteboard *)pasteboard
                                   context:(NSManagedObjectContext *)context;

#pragma mark Properties

@property (readonly, nonatomic) NxATrack *track;

@end
