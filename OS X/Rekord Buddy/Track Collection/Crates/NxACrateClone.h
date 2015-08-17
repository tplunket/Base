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

/*! Clone of a cratae, which contains the same tracks but can be sorted differently. */
@interface NxACrateClone : NxACrate

#pragma mark Factory Methods

/*!
 * Create a clone of an existing crate.
 * \param crate Crate to clone.
 * \return A clone of the crate, or nil if failed.
 */
+ (instancetype)crateAsACloneOf:(NxACrate *)crate;

@end
