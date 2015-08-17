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

#import "NxATrackEditViewControllerDelegate.h"
#import "NxATagTokenFieldDelegate.h"
#import "NxATrackListViewItem.h"

#pragma mark Public Interface

@interface NxATrackEditViewController : NSViewController <NxATagTokenFieldDelegate>

#pragma mark Properties

/*! Delegate object. */
@property (weak, nonatomic) IBOutlet id <NxATrackEditViewControllerDelegate> delegate;

#pragma mark Instance Methods

/*!
 * Display the edit dialog.
 * \param item Item to display/edit.
 * \param positioningRect The rectangle within positioningView relative to which the popover should be positioned.
 * \param positioningView The view relative to which the popover should be positioned.
 * \param preferredEdge The edge of positioningView the popover should prefer to be anchored to.
 */
- (void)displayItem:(id <NxATrackListViewItem>)item
 showRelativeToRect:(NSRect)positioningRect
             ofView:(NSView *)positioningView
      preferredEdge:(NSRectEdge)preferredEdge;

/*!
 * Close the edit dialog.
 */
- (void)close;

@end
