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

#import "NxATrackListViewItem.h"

#pragma mark Protocol

/*! A delegate object for the tag token field. */
@protocol NxATagTokenFieldDelegate <NSObject>

@required

#pragma mark Properties

/*! Context used by the objects stored in the field. */
@property (readonly, nonatomic) NSManagedObjectContext *objectContext;

/*! The item currently being edited in the dialog. */
@property (readonly, nonatomic) id <NxATrackListViewItem> displayedItem;

@end
