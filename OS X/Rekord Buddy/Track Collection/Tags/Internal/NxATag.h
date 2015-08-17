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

#import "Track Collection/Tags/NxATag.h"

#pragma mark Private Interface

@interface NxATag ()

#pragma mark Class Methods

/*!
 * Entity name used by this type of tags.
 * \return A managed entity name.
 */
+ (NSString *)p_managedEntityName;

/*!
 * Class for the type of values held by this tag.
 * \return The class of the values held by the tag.
 */
+ (Class)p_valueClass;

#pragma mark Properties

/*! Name of this tag. */
@property (strong, nonatomic) NSString *p_tagName;

/*! Tag entries for this tag. */
@property (strong, nonatomic) NSSet *p_parentTagEntries;

@end
