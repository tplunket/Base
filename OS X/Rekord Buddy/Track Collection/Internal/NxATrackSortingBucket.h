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

@class NxASortingCriteria;
@class NxATag;

#import "Track Collection/NxATrackSortingBucket.h"

#pragma mark Private Interface

@interface NxATrackSortingBucket ()

#pragma mark Properties

/*! Tag all the tracks have in common. */
@property (strong, nonatomic) NxATag *p_tag;

/*! Or class name of  tag the tracks do not have. */
@property (strong, nonatomic) NSString *p_tagClassName;

/*! Description of the tag. */
@property (strong, nonatomic) NSString *p_tagDescription;

/*! User-facing description of the bucket. */
@property (strong, nonatomic) NSString *p_bucketDescription;

/*! Tracks in the bucket. */
@property (strong, nonatomic) NSArray *p_tracks;

@property (strong, nonatomic) NxASortingCriteria *p_sortingCriteria;

/*! Crate the tracks belong to. */
@property (strong, nonatomic) NxACrate *p_parentCrate;

/*! If non-nil, parent of this sorting bucket. */
@property (strong, nonatomic) NxATrackSortingBucket *p_parentBucket;

@end
