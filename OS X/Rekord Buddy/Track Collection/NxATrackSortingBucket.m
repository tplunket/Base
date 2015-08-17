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

#import "Track Collection/NxASortingCriteria.h"
#import "Track Collection/Tags/NxATag.h"
#import "Track Collection/Internal/NxATrackSortingBucket.h"
#import "Track Collection/Crates/NxACrate.h"

#pragma mark Implementation

@implementation NxATrackSortingBucket

#pragma mark Factory Methods

+ (instancetype)bucketForTracks:(NSArray *)tracks
                        withTag:(NxATag *)tag
                   fromCriteria:(NxASortingCriteria *)criteria
                    parentCrate:(NxACrate *)parentCrate
{
    NxATrackSortingBucket *bucket = [[NxATrackSortingBucket alloc] init];

    bucket.p_tracks = tracks;
    bucket.p_tag = tag;
    bucket.p_tagClassName = NSStringFromClass(tag.class);
    bucket.p_bucketDescription = tag.stringValue;
    bucket.p_tagDescription = tag.propertyDescription;
    bucket.p_sortingCriteria = criteria;
    bucket.p_parentCrate = parentCrate;

    return bucket;
}

+ (instancetype)bucketForTracks:(NSArray *)tracks
                withoutTagClass:(Class)tagClass
                 tagDescription:(NSString *)tagDescription
              bucketDescription:(NSString *)bucketDescription
                   fromCriteria:(NxASortingCriteria *)criteria
                    parentCrate:(NxACrate *)parentCrate
{
    NxATrackSortingBucket *bucket = [[NxATrackSortingBucket alloc] init];

    bucket.p_tracks = tracks;
    bucket.p_tagClassName = NSStringFromClass(tagClass);
    bucket.p_tagDescription = tagDescription;
    bucket.p_bucketDescription = bucketDescription;
    bucket.p_sortingCriteria = criteria;
    bucket.p_parentCrate = parentCrate;

    return bucket;
}

#pragma mark Class Methods

+ (NSArray *)bucketsWithTracks:(NSArray *)tracks
                    sortedWith:(NSOrderedSet *)criterias
                       atIndex:(NSUInteger)criteriaIndex
                   parentCrate:(NxACrate *)parentCrate
{
    NxAAssertNotNil(tracks);
    NxAAssertTrue(criteriaIndex < criterias.count);

    // -- First we get an array of buckets, one for each different value of the criteria.
    NxASortingCriteria *criteria = [criterias objectAtIndex:criteriaIndex];
    NSArray *sortingBuckets = [criteria sortTracks:tracks];

    ++criteriaIndex;

    if (criteriaIndex < criterias.count) {
        // -- If we have a sub-criteria, we then sort those buckets into sub-buckets.
        for (NxATrackSortingBucket *bucket in sortingBuckets) {
            bucket.childBuckets = [NxATrackSortingBucket bucketsWithTracks:bucket.p_tracks
                                                                sortedWith:criterias
                                                                   atIndex:criteriaIndex
                                                               parentCrate:parentCrate];

            // -- And all these buckets have a parent.
            for (NxATrackSortingBucket *childBucket in bucket.childBuckets) {
                childBucket.p_parentBucket = bucket;
            }
        }
    }

    return sortingBuckets;
}

#pragma mark Accessors

- (NxACrate *)parentCrate
{
    return self.p_parentCrate;
}

- (NSString *)criteriaDescription
{
    return self.p_sortingCriteria.sortingPropertyDescription;
}

#pragma mark Instance Methods

- (void)destroy
{
    [self.p_sortingCriteria destroy];

    self.p_parentCrate = nil;
}

@end
