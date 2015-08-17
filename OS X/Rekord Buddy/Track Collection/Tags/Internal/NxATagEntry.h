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

#import "NxATagEntry.h"
#import "Track Collection/Tags/NxATag.h"
#import "NxATaggedObject.h"

#pragma mark Private Interface

@interface NxATagEntry ()

#pragma mark Properties

@property (strong, nonatomic) NxATag *p_tag;

/*! Date this entry was created. */
@property (strong, nonatomic) NSDate *p_dateModified;

/*! Tagged object associated with this entry. */
@property (strong, nonatomic) NxATaggedObject *p_parent;

@end
