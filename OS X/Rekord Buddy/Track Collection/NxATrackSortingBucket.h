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

@class NxATag;
@class NxACrate;
@class NxASortingCriteria;

#pragma mark Public Interface

/*! A bucket used to sort tracks by a given criteria. */
@interface NxATrackSortingBucket : NSObject

#pragma mark Factory Methods

+ (instancetype)bucketForTracks:(NSArray *)tracks
                        withTag:(NxATag *)tag
                   fromCriteria:(NxASortingCriteria *)criteria
                    parentCrate:(NxACrate *)parentCrate;

+ (instancetype)bucketForTracks:(NSArray *)tracks
                withoutTagClass:(Class)tagClass
                 tagDescription:(NSString *)tagDescription
              bucketDescription:(NSString *)bucketDescription
                   fromCriteria:(NxASortingCriteria *)criteria
                    parentCrate:(NxACrate *)parentCrate;

#pragma mark Class Methods

/*!
 * Create sorting buckets with tracks.
 * \param tracks Tracks to put in the buckets.
 * \param criterias Sorting criterias used to sort the tracks.
 * \param index Index of the criteria used this set of tracks.
 * \param parentCrate Parent crate for this sorting bucket.
 * \return An array of sorting buckets, one for each value found for the sort criteria.
 */
+ (NSArray *)bucketsWithTracks:(NSArray *)tracks
                    sortedWith:(NSOrderedSet *)criterias
                       atIndex:(NSUInteger)index
                   parentCrate:(NxACrate *)parentCrate;

#pragma mark Properties

@property (readonly, nonatomic) NxACrate *parentCrate;

@property (strong, nonatomic) NSArray *childBuckets;

@property (readonly, nonatomic) NSString *criteriaDescription;

#pragma mark Instance Methods

- (void)destroy;

@end
