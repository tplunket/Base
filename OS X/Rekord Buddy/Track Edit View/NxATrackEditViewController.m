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

#import "NxAWaveformViewController.h"
#import "NxATrackListViewItem.h"
#import "Track Collection/Tags/Crate Properties/NxATrackProperty.h"
#import "NxATrackEditViewController.h"

#pragma mark Private Interface

@interface NxATrackEditViewController ()

#pragma mark Properties

/*! The item currently being edited in the dialog. */
@property (strong, nonatomic) id <NxATrackListViewItem> p_item;

/*! Track editing popup. */
@property (weak, nonatomic) IBOutlet NSPopover *p_trackEditPopup;

/*! Controller fo the waveform view from the track edit view. */
@property (weak, nonatomic) IBOutlet NxAWaveformViewController *p_trackEditWaveformViewController;

@end

#pragma mark Implementation

@implementation NxATrackEditViewController

#pragma mark Instance Methods

- (void)displayItem:(id <NxATrackListViewItem>)item
 showRelativeToRect:(NSRect)positioningRect
             ofView:(NSView *)positioningView
      preferredEdge:(NSRectEdge)preferredEdge;
{
    NxAAssertNotNil(item);

    // -- This is more than just telling the view to display.
    // -- Accessing 'view' forces the controller to load the nib too.
    [self.view setNeedsDisplay:YES];

    self.p_item = item;

    if([item respondsToSelector:sel_registerName("displayWaveformWithController:")]) {
        [item displayWaveformWithController:self.p_trackEditWaveformViewController];
    }

    [self.p_trackEditPopup showRelativeToRect:positioningRect
                                       ofView:positioningView
                                preferredEdge:preferredEdge];
}

- (void)close
{
    if (self.p_trackEditPopup.isShown) {
        [self.p_trackEditPopup close];

        self.p_item = nil;
        
        if([self.delegate respondsToSelector:@selector(trackEditDialogWillClose)]) {
            [self.delegate trackEditDialogWillClose];
        }
    }
}

#pragma mark NxATagTokenFieldDelegate protocol

- (NSManagedObjectContext *)objectContext
{
    return self.delegate.objectContext;
}

- (id <NxATrackListViewItem>)displayedItem
{
    return self.p_item;
}

@end
