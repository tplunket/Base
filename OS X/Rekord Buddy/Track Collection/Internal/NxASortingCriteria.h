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
#import "Track Collection/Crates/NxACrate.h"

#pragma mark Private Interface

@interface NxASortingCriteria ()

#pragma mark Properties

/*! Name of the class of the tag this criteria sorts by. */
@property (strong, nonatomic) NSString *p_sortTagClassName;

/*! User-facing description of the tag this criteria sorts by. */
@property (strong, nonatomic) NSString *p_sortTagDescription;

/*! Whether the sorting is ascending or descending. */
@property (strong, nonatomic) NSNumber *p_ascending;

/*! Crate this criteria belongs to. */
@property (strong, nonatomic) NxACrate *p_parentCrate;

@end
