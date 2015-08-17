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
@class NxATag;
@class NxATrackProperty;
@class NxACrateProperty;
@class NxASortingCriteria;

#import "NxATaggedObject.h"

#pragma mark Constants

/*!
 *  enum NxACrateUpdateSiblingsFlag
 *  Whether a crate should updats its siblings or not.
 */
typedef NS_ENUM(NSInteger, NxACrateUpdateSiblingsFlag) {
    NxACrateShouldUpdateSiblings,       /*!< The crate should update its siblings. */
    NxACrateShouldNotUpdateSiblings,    /*!< The crate should not update its siblings. */
};

#pragma mark - Public Interface

/*! Music crate, which can contains tracks and other crates. */
@interface NxACrate : NxATaggedObject

#pragma mark Factory Methods

+ (instancetype)folderWithName:(NSString *)name
 withEmptyContentLastModifiedOn:(NSDate *)modificationDate
                     inContext:(NSManagedObjectContext *)context;

+ (instancetype)playlistWithName:(NSString *)name
  withEmptyContentLastModifiedOn:(NSDate *)modificationDate
                       inContext:(NSManagedObjectContext *)context;

+ (instancetype)existingOrNewSystemFolderWithName:(NSString *)name
              ifNewWithEmptyContentLastModifiedOn:(NSDate *)modificationDate
                                        inContext:(NSManagedObjectContext *)context;

+ (instancetype)existingOrNewSystemPlaylistWithName:(NSString *)name
                ifNewWithEmptyContentLastModifiedOn:(NSDate *)modificationDate
                                          inContext:(NSManagedObjectContext *)context;

#pragma mark Class Methods

+ (BOOL)canFindARootCrateInContext:(NSManagedObjectContext *)context;

+ (BOOL)canFindAnAllTracksCrateInContext:(NSManagedObjectContext *)context;

/*!
 * Get an array of crates from the pasteboard.
 * \param pasteboard Pasteboard to read crate representations from.
 * \param context Managed object context to get the crates from.
 * \return An array of crates matching the representations or nil if failed.
 */
+ (NSArray *)cratesFromPasteboard:(NSPasteboard *)pasteboard
                          context:(NSManagedObjectContext *)context;


/*
 * Check to see if a name is valid for a crate.
 * \param name Name to check for.
 * \return YES if valid, NO otherwise.
 */
+ (BOOL)isAValidName:(NSString *)name;

/*!
 * Get the 'all tracks' crate.
 * \param context Context to look into.
 * \return The crate containing all the tracks for that context.
 */
+ (NxACrate *)allTracksCrateIn:(NSManagedObjectContext *)context;

/*!
 * Get the root crate.
 * \param context Context to look into.
 * \return The root crate containing all the other crates for that context.
 */
+ (NxACrate *)rootCrateIn:(NSManagedObjectContext *)context;

/*!
 * Get all the crate entities found in a object context.
 * \param context Managed Object context to look into.
 * \return All crate entities found in the context.
 */
+ (NSArray *)allCratesIn:(NSManagedObjectContext *)context;

#pragma mark Properties

@property (readonly, nonatomic) NSString *name;

@property (readonly, nonatomic) BOOL containsFilters;

@property (readonly, nonatomic) BOOL isEmpty;

@property (readonly, nonatomic) BOOL isAFolder;

@property (readonly, nonatomic) BOOL isAPlaylist;

@property (readonly, nonatomic) BOOL isASystemCrate;

@property (readonly, nonatomic) BOOL isASmartPlaylist;

@property (readonly, nonatomic) BOOL isAFilter;

/*! Whether this crate has sorting criterias or not. */
@property (readonly, nonatomic) BOOL hasSortingCriterias;

/*! List of tags that can be sorted by, one for each seperate tag types found in the tracks contained in this crate. */
@property (readonly, nonatomic) NSArray *tagTypesForSorting;

/*! Get all the tracks contained in the crate and any subcrates. */
@property (readonly, nonatomic) NSArray *tracks;

/*! Get all the crates contained in the crate. */
@property (readonly, nonatomic) NSArray *crates;

@property (readonly, nonatomic) NSArray *filterCrates;

@property (readonly, nonatomic) BOOL hasParentCrate;

/*! Parent crate which contains this crate. */
@property (readonly, nonatomic) NxACrate *parentCrate;

@property (readonly, nonatomic) NSString *stringValue;

@property (readonly, nonatomic) NxACrate *originalCrate;

#pragma mark Instance Methods

- (BOOL)hasAChildCrateNamed:(NSString *)name;

/*!
 * Add a track to this crate.
 * \param track Track to add.
 * \param modificationDate Date the track is to be added.
 */
- (void) addTrack:(NxATrack *)track
 modificationDate:(NSDate *)modificationDate;

/*!
 * Check if this crate contains a track.
 * \param track Track to look for.
 * \return YES if it does, NO otherwise.
 */
- (BOOL)containsTrack:(NxATrack *)track;

/*!
 * Remove a track property from this crate.
 * \param property Track property to remove.
 */
- (void)removeTrackProperty:(NxATrackProperty *)property;

/*!
 * Move tracks to a given position in the crate.
 * \param propertiesToMove Track properties to move.
 * \param index What position to move all the tracks to.
 */
- (void)moveTrackProperties:(NSArray *)propertiesToMove
                 toPosition:(NSInteger)index;

/*!
 * Add a sub crate to this crate.
 * \param crate Crate to add.
 * \param modificationDate Date the track was added.
 */
- (void) addCrate:(NxACrate *)crate
 modificationDate:(NSDate *)modificationDate;

/*!
 * Remove a sub crate from this crate.
 * \param crate Crate to remove.
 */
- (void)removeCrate:(NxACrate *)crate;

- (NxACrate *)childCrateWithName:(NSString *)name;

/*!
 * Move crates to a given position in the crate.
 * \param cratesToMove Crate properties to move.
 * \param index What position to move all the crates to.
 */
- (void)moveCrates:(NSArray *)cratesToMove
        toPosition:(NSInteger)index;

/*!
 * Find the index of a subcrate.
 * \param crate Crate to look for.
 * \return Index at which the subcrate is found or NSNotFound if not found.
 */
- (NSInteger)indexOfSubCrate:(NxACrate *)crate;

/*!
 * Add a sorting criteria to this crate.
 * \param criteria Sorting criteria to add.
 */
- (void)addSortingCriteria:(NxASortingCriteria *)criteria;

- (void)removeSortingCriteria:(NxASortingCriteria *)criteria;

/*!
 * Check if a tag is used as a sorting criteria on this crate.
 * \param tag Tag to look for.
 * \return YES if found, NO otherwise.
 */
- (BOOL)hasSortingCriteriaForTag:(NxATag *)tag;

/*!
 * Update all the automatically generated content for this crate.
 * \param updateSiblings Whether the crate should update its siblings or not.
 */
- (void)updateSmartContent:(NxACrateUpdateSiblingsFlag)updateSiblings;

@end
