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
@class NxATag;

#pragma mark Public Interface

/*! A Criteria by which a crate gets sorted. */
@interface NxASortingCriteria : NSManagedObject <NSCopying>

#pragma mark Factory Methods

/*!
 * Create a sorting criteria.
 * \param tagClass Class of the tag to use for sorting.
 * \param description User-facing description of the tag.
 * \param context Managed object context to create the new object in.
 * \return A sorting criteria with the given key.
 */
+ (instancetype)criteriaWithTagClass:(Class)tagClass
                         description:(NSString *)description
                             context:(NSManagedObjectContext *)context;

#pragma mark Class Methods

/*!
 * Check to see if a list of sorting criterias contains one for a given tag description.
 * \param description Description of the tag to look for.
 * \param set Set of sorting criterias to look into.
 * \return YES if a matching criteria is found, NO otherwise.
 */
+ (BOOL)isCriteriaForTagDescription:(NSString *)description
                            foundIn:(NSOrderedSet *)set;

/*!
 * Check to see if a list of sorting criterias contains one for a given tag class.
 * \param tag Tag to look for.
 * \param set Set of sorting criterias to look into.
 * \return YES if a matching criteria is found, NO otherwise.
 */
+ (BOOL)isCriteriaForTag:(NxATag *)tag
                 foundIn:(NSOrderedSet *)set;

#pragma mark Properties

/*! Description of the property being sorted. */
@property (readonly, nonatomic) NSString *sortingPropertyDescription;

/* Sort descriptor used to sort items. */
@property (readonly, nonatomic) NSSortDescriptor *sortDescriptor;

#pragma mark Instance Methods

- (void)destroy;

/*!
 * Reverse the sorting order of this criteria.
 */
- (void)flipSortingOrder;

/*!
 * Sort tracks based on this sorting criteria.
 * \param tracks An array of tracks to sort.
 * \return An array of NxATrackSortingBucket, one for every tag value found for this criteria.
 */
- (NSArray *)sortTracks:(NSArray *)tracks;

@end
