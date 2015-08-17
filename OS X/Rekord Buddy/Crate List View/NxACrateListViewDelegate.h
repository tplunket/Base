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

#pragma mark Protocol

/*! A delegate object for the crate list view. */
@protocol NxACrateListViewDelegate <NSObject>

#pragma mark Instance Methods

@optional

- (NSMenu *)contextualMenuForCrate:(NxACrate *)crate;

/*! Signal that the delete key was pressed. */
- (void)deleteKeyPressedWithSelection;

/*! Signal that the up key was pressed. */
- (void)upKeyPressedWithSelection;

/*! Signal that the down key was pressed. */
- (void)downKeyPressedWithSelection;

@end
