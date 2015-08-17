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

#import "NxATrackListView.h"
#import "NxATrackListViewItem.h"
#import "NxATrackListRowView.h"

#pragma mark Implementation

@implementation NxATrackListView

#pragma mark Overridden NSTableView methods

- (NSMenu *)menuForEvent:(NSEvent *)event
{
    SEL selector = sel_registerName("mouseRightClickedWithSelection");
    if(![self.extendedDelegate respondsToSelector:selector]) {
        // -- If the extended delegate don't support the this part of the protocol, we just return.
        return nil;
    }

	[self.window makeFirstResponder:self];

	// -- Find which row is under the cursor.
	NSPoint menuPoint = [self convertPoint:event.locationInWindow fromView:nil];
    NSInteger row = [self rowAtPoint:menuPoint];

	// -- Preserves the selection if the row under the mouse is selected (to allow for
    // -- multiple items to be selected), otherwise selects the row under the mouse.
	if (![self.selectedRowIndexes containsIndex:row]) {
        [self selectRowIndexes:[NSIndexSet indexSetWithIndex:row] byExtendingSelection:NO];
    }

	if (self.numberOfSelectedRows > 0) {
        return [self.extendedDelegate mouseRightClickedWithSelection];
    }

    return nil;
}

- (void)keyDown:(NSEvent *)theEvent
{
    NSString *keys = [theEvent characters];
    unichar key = [keys characterAtIndex:0];

    switch(key) {
        case 127: {
            // -- Delete
            if (self.numberOfSelectedRows == 0) {
                return;
            }

            SEL selector = sel_registerName("deleteKeyPressedWithSelection");

            // -- If the extended delegate don't support the this part of the protocol, we just return.
            if([self.extendedDelegate respondsToSelector:selector]) {
                [self.extendedDelegate deleteKeyPressedWithSelection];
            }

            break;
        }
        case 63232: {
            // -- Up
            if (self.numberOfSelectedRows == 0) {
                return;
            }

            // -- If the extended delegate don't support the this part of the protocol, we just return.
            SEL selector = sel_registerName("upKeyPressedWithSelection");
            if([self.extendedDelegate respondsToSelector:selector]) {
                [self.extendedDelegate upKeyPressedWithSelection];
            }

            break;
        }
        case 63233: {
            // -- Down
            if (self.numberOfSelectedRows == 0) {
                return;
            }

            // -- If the extended delegate don't support the this part of the protocol, we just return.
            SEL selector = sel_registerName("downKeyPressedWithSelection");
            if([self.extendedDelegate respondsToSelector:selector]) {
                [self.extendedDelegate downKeyPressedWithSelection];
            }

            break;
        }
    }
}

- (void)drawBackgroundInClipRect:(NSRect)clipRect
{
    // -- Find the position of the last visible row.
    // -- We only draw the alternating empty rows past this.
    NSInteger numberOfRows = self.numberOfRows;
    CGFloat yStart = 0;
    if (numberOfRows > 0) {
        yStart = NSMaxY([self rectOfRow:numberOfRows - 1]);
    }

    // -- We will start our background at the index right after the last row of the table.
    BOOL darkBackground = ![NxATrackListRowView rowHasDarkBackgroundAtIndex:(numberOfRows - 1)
                                                                outlineView:self];

    // -- This is the entire area we have to fill.
    NSRect boundsToDraw = self.bounds;

    // -- The height of each row include the spacing between rows.
    CGFloat totalRowHeight = self.emptyRowHeight + self.intercellSpacing.height;

    // -- Get the shape of one single row.
    NSRect rowRect = boundsToDraw;
    rowRect.size.height = totalRowHeight;

    // -- Draw the background for all empty rows in the table.
    while (yStart < NSMaxY(boundsToDraw)) {
        rowRect.origin.y = yStart;

        [NxATrackListRowView drawBackground:rowRect dark:darkBackground];

        yStart += totalRowHeight;

        darkBackground = !darkBackground;
    }
}

@end
