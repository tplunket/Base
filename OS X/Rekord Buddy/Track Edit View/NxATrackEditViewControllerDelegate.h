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

/*! A delegate object for the track edit view controller. */
@protocol NxATrackEditViewControllerDelegate <NSObject>

@required

#pragma mark Properties

/*! Context used by the tracks stored in the view. */
@property (readonly, nonatomic) NSManagedObjectContext *objectContext;

@optional

#pragma mark Instance Methods

/*!
 * Delegate method called when the dialog is about to close.
 */
- (void)trackEditDialogWillClose;

@end
