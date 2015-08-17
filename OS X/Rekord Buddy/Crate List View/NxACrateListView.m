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

#import "Crate List View/NxACrateListView.h"
#import "Track Collection/Crates/NxACrate.h"
#import "NxACrateListViewDelegate.h"
#import "NxATrackSortingBucket.h"
#import "NxACrateListViewItem.h"

#pragma mark Implementation

@implementation NxACrateListView

#pragma mark Overridden NSOutlineView Methods

- (NSMenu *)menuForEvent:(NSEvent *)event
{
    // -- If the extended delegate don't support the this part of the protocol, we just return.
    SEL selector = sel_registerName("contextualMenuForCrate:");
    if(![self.extendedDelegate respondsToSelector:selector]) {
        return nil;
    }

    NSPoint clickedPoint = [self convertPoint:event.locationInWindow fromView:nil];
    NSInteger clickedRow = [self rowAtPoint:clickedPoint];

    NxACrateListViewItem *clickedItem = [self itemAtRow:clickedRow];
    id sourceObject = clickedItem.sourceObject;
    if ([sourceObject isKindOfClass:[NxATrackSortingBucket class]]) {
        sourceObject = [sourceObject parentCrate];
    }

    if (![sourceObject isKindOfClass:[NxACrate class]]) {
        return nil;
    }

    NSIndexSet *selection = self.selectedRowIndexes;
    if (![selection containsIndex:clickedRow]) {
        // -- If the row we're right-clicking on is not selected already, select it.
        [self selectRowIndexes:[NSIndexSet indexSetWithIndex:clickedRow] byExtendingSelection:NO];
    }

    return [self.extendedDelegate contextualMenuForCrate:sourceObject];
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

            // -- If the extended delegate don't support the this part of the protocol, we just return.
            SEL selector = sel_registerName("deleteKeyPressedWithSelection");
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


@end
