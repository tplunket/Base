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
@class NxACrateProperty;

#import "Track Collection/Crates/NxACrate.h"
#import "Track Collection/Tags/Crate Properties/NxACrateProperty.h"
#import "Track Collection/Tags/Internal/NxATaggedObject.h"

#pragma mark Constants

/*! Prefix used for system (internal) crate names. */
extern NSString *const NxACrateSystemNamePrefix;

typedef NS_ENUM(NSUInteger, NxACrateType) {
    NxACrateIsAFolder = 1,
    NxACrateIsAPlaylist,
    NxACrateIsASmartPlaylist,
    NxACrateIsAFilterPlaylist,
    NxACrateIsAClone
};

#pragma mark Private Interface

@interface NxACrate ()

#pragma mark Factory Methods

+ (instancetype)p_emptyCrateWithName:(NSString *)name
                           inContext:(NSManagedObjectContext *)context;


#pragma mark Class Methods

/*!
 * Get the tracks matching a certain predicate.
 * \param predicateFormat The predicate to use, as a formatted string.
 * \param inCrate In non-nil, only the tracks from this crate are considered.
 * \param context Object context to look for the tracks in.
 * \return An array of tracks matching the predicate.
 */
+ (NSMutableSet *)p_tracksMatchingPredicate:(NSString *)predicateFormat
                                    inCrate:(NxACrate *)inCrate
                                    context:(NSManagedObjectContext *)context;

/*!
 * Get the entity name of the current class of crate.
 * \return The entity name.
 */
+ (NSString *)p_entityName;

#pragma mark Properties

/*! Name of this crate. */
@property (strong, nonatomic) NSString *p_name;

@property (strong, nonatomic) NSNumber *p_type;

/*! Crate property tags which point to this crate. */
@property (strong, nonatomic) NxACrateProperty *p_parentCrateProperty;

/*! List of clones of this crate. */
@property (strong, nonatomic) NSSet *p_clones;

/*! List of criterias this crate is sorted by before displaying. */
@property (strong, nonatomic) NSOrderedSet *p_sortedBy;

/*! List of buckets with tracks sorted by the current criteria. (Transient) */
@property (strong, nonatomic) NSArray *p_sortingBuckets;

/*!
 * Tracks contained in this crate, if any. (Transient)
 * \details This does not recurse into any crates this crate might contains,
 *          and only returns the tracks that this crate contains. */
@property (readonly, nonatomic) NSArray *p_tracks;

/*! Full name of this crate inclusing any parent names. */
@property (readonly, nonatomic) NSString *p_fullName;

#pragma mark Instance Methods

- (void)p_addCrateWithoutUpdatingSmartContent:(NxACrate *)crate
                               lastModifiedOn:(NSDate *)modificationDate;

/*!
 * Update the sorting buckets of this crate.
 */
- (void)p_updateSortingBuckets;

/*!
 * Test to see if this crate can receive another crate.
 * \param crate Crate to receive.
 * \return YES if this crate can receive it, NO otherwise.
 */
- (BOOL)p_canReceiveCrate:(NxACrate *)crate;

@end
