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

#import "NxATrackListViewDelegate.h"
#import "NxATrackEditViewControllerDelegate.h"
#import "NxATrackListViewControllerDelegate.h"

@class NxACrate;

#pragma mark Public Interface

@interface NxATrackListViewController : NSViewController <NxATrackListViewDelegate , NxATrackListViewControllerDelegate>

#pragma mark Properties

/*! Main document window. */
@property (weak, nonatomic) IBOutlet NSWindow *mainWindow;

/*! Parent where the track list should be displayed. */
@property (weak, nonatomic) IBOutlet NSView *parentView;

/*! Delegate object. */
@property (weak, nonatomic) IBOutlet id <NxATrackListViewControllerDelegate> delegate;

#pragma mark Instance Methods

/*!
 * Switch to the regular track view.
 */
- (void)useTrackView;

/*!
 * Switch to the new track view.
 */
- (void)useArtworkTrackView;

/*!
 * Display the content of a crate in the track list view.
 * \param crate Crate to display.
 */
- (void)displayCrate:(NxACrate *)crate;

/*!
 * Display some track list items in the track list view.
 * \param items Array of NxATrackListViewItem to display.
 */
- (void)displayItems:(NSArray *)items;

@end
