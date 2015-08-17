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

#import "Track Collection/Tags/NxATag.h"

#pragma mark Public Interface

/*! A crate property in a crate. */
@interface NxACrateProperty : NxATag

#pragma mark Class Methods

/*!
 * Get an array of all the crates from a list of properties.
 * \param properties List of properties to parse.
 * \return An array of all the crates contained in the crate properties.
 */
+ (NSArray *)arrayOfCratesFromProperties:(NSArray *)properties;

#pragma mark Properties

/*! Parent crate for this crate property. */
@property (readonly, nonatomic) NxACrate *parentCrate;

/*! Crate for this crate property. */
@property (readonly, nonatomic) NxACrate *crate;

@end
