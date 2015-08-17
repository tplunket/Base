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

#import "Track Collection/Crates/NxACrate.h"

#pragma mark Public Interface

/*! Smart playlist, which contains tracks filtered from all the tracks in the collection. */
@interface NxASmartPlaylist : NxACrate

#pragma mark Factory Methods

/*!
 * Create a smart playlist.
 * \param name Name of the crate.
 * \param predicate Predicate used to populate the crate.
 * \param context Managed object context to create the new object in.
 * \return A new crate, or nil if failed.
 */
+ (instancetype)crateWithName:(NSString *)name
                    predicate:(NSString *)predicate
                      context:(NSManagedObjectContext *)context;

#pragma mark Properties

/*! The predicate used to generate the smart playlist's content. */
@property (strong, nonatomic) NSString *predicate;

@end
