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

#pragma mark Protocol

/*! A delegate object for the track list view. */
@protocol NxATrackListViewDelegate <NSObject>

#pragma mark Instance Methods

@optional

/*!
 * Signal that the mouse was right-cliked on a selection of one or more items.
 * \details This delegate method allows for other things to happen when a table view is right-click on.
 *          Instead of returning a menu, the method can display something else.
 * \return A menu to display or nil for no menu.
 */
- (NSMenu *)mouseRightClickedWithSelection;

/*! Signal that the delete key was pressed. */
- (void)deleteKeyPressedWithSelection;

/*! Signal that the up key was pressed. */
- (void)upKeyPressedWithSelection;

/*! Signal that the down key was pressed. */
- (void)downKeyPressedWithSelection;

@end
