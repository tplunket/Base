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

#pragma mark Public Interface

/*! Artist with credits on tracks, remixes or productions. */
@interface NxAArtist : NSManagedObject

#pragma mark Factory Methods

/*!
 * Create an artist or retrieve an existing one.
 * \param name Name of the artist.
 * \param context Managed object context to create the new object in.
 * \return An artist with the given name.
 */
+ (instancetype)artistWithName:(NSString *)name
                       context:(NSManagedObjectContext *)context;

/*!
 * Retrieve an existing artist.
 * \param name Name of the artist.
 * \param context Managed object context to create look for the artist in.
 * \return An artist with the given name or nil if not found.
 */
+ (instancetype)existingArtistWithName:(NSString *)name
                               context:(NSManagedObjectContext *)context;

#pragma mark Class Methods

/*!
 * Get all the artists which name match a certain predicate.
 * \param partialPredicateFormat Test to use. Should be only be the comparaison operation.
 *                               operator and the right-side comparaison value.
 * \param context Object context to look for the artists in.
 * \return A set of all the artists which name matches the test.
 */
+ (NSArray *)artistsMatchingPredicate:(NSString *)partialPredicateFormat
                              context:(NSManagedObjectContext *)context;

#pragma mark Properties

@property (readonly, nonatomic) BOOL isAnOrphan;

#pragma mark Instance Methods

- (void)destroy;

@end
