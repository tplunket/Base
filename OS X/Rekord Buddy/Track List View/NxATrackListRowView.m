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
#import "NxATrackListCellView.h"
#import "NxATrackListRowView.h"

#pragma mark Implementation

@implementation NxATrackListRowView

#pragma mark Class Methods

+ (void)drawBackground:(NSRect)bounds
                  dark:(BOOL)dark
{
    static NSArray *colors = nil;
    static dispatch_once_t onceToken = 0;
    dispatch_once(&onceToken, ^{
        // -- These are the colors we use for the row's background.
        colors = @[ // -- Light row.
                   [NSColor colorWithRed:56.0/255.0 green:56.0/255.0 blue:56.0/255.0 alpha:1.0],

                   // -- Dark row.
                   [NSColor colorWithRed:40.0/255.0 green:40.0/255.0 blue:40.0/255.0 alpha:1.0],

                   // -- Light row bottom line.
                   [NSColor colorWithRed:46.0/255.0 green:46.0/255.0 blue:46.0/255.0 alpha:1.0],

                   // -- Dark row bottom line.
                   [NSColor colorWithRed:30.0/255.0 green:30.0/255.0 blue:30.0/255.0 alpha:1.0],

                   // -- Light row top line.
                   [NSColor colorWithRed:66.0/255.0 green:66.0/255.0 blue:66.0/255.0 alpha:1.0],

                   // -- Dark row top line.
                   [NSColor colorWithRed:50.0/255.0 green:50.0/255.0 blue:50.0/255.0 alpha:1.0]
                   ];
    });

    NSInteger index = dark ? 1 : 0;

    // -- We paint alternating rows with a different color.
    NSColor *color = colors[index];
    [color set];

    // -- First we fill the entire row with one color.
    NSRectFill(bounds);

    // -- Then when draw one line at the bottom of the the row.
    NSRect separatorRect = bounds;
    separatorRect.origin.y = NSMaxY(separatorRect) - 1;
    separatorRect.size.height = 1;

    color = colors[index + 2];
    [color set];

    NSRectFill(separatorRect);

    // -- Then when draw one line at the top of the the row.
    separatorRect.origin.y = NSMinY(bounds);

    color = colors[index + 4];
    [color set];

    NSRectFill(separatorRect);
}

+ (BOOL)rowHasDarkBackgroundAtIndex:(NSInteger)rowIndex
                        outlineView:(NSOutlineView *)outlineView
{
    NSInteger lastHeaderRowIndex = rowIndex - 1;

    while (lastHeaderRowIndex >= 0) {
        id <NxATrackListViewItem> otherItem = [outlineView itemAtRow:lastHeaderRowIndex];
        if (otherItem.trackListViewItemIsGroupItem) {
            break;
        }

        --lastHeaderRowIndex;
    }

    NSInteger backgroundIndex = rowIndex;
    if (lastHeaderRowIndex > 0) {
        backgroundIndex -= lastHeaderRowIndex;
    }

    return backgroundIndex & 1 ? YES : NO;
}

#pragma mark Overridden NSTableRowView Methods

- (void)drawBackgroundInRect:(NSRect)dirtyRect {
    // -- We find out what item is being displayed in this row by accessing the first column view.
    NSTableCellView *cellView = [self viewAtColumn:0];
    id <NxATrackListViewItem> item = cellView.objectValue;

    // -- Then we draw its background.
    if (item.trackListViewItemIsGroupItem) {
        // -- If this is a header item, we fill it with a gradient.
        NSGraphicsContext *nsGraphicsContext = [NSGraphicsContext currentContext];
        CGContextRef context = (CGContextRef)[nsGraphicsContext graphicsPort];

        CGColorSpaceRef baseSpace = CGColorSpaceCreateDeviceRGB();
        CGFloat colors[8] = { 130.0/255.0, 130.0/255.0, 130.0/255.0, 1.0,
                              50.0/255.0, 50.0/255.0, 50.0/255.0, 1.0 };

        CGFloat locations[2] = { 0.0, 1.0 };

        CGGradientRef gradient = CGGradientCreateWithColorComponents(baseSpace, colors, locations, 2);

        CGContextDrawLinearGradient(context,
                                    gradient,
                                    self.bounds.origin,
                                    CGPointMake(self.bounds.origin.x,
                                                self.bounds.origin.y + self.bounds.size.height),
                                    0);

        CGColorSpaceRelease(baseSpace);
        CGGradientRelease(gradient);
    }
    else if ([cellView isKindOfClass:[NxATrackListCellView class]]) {
        NxATrackListCellView *trackListCellView = (NxATrackListCellView *)cellView;

        // -- If this is a regular item, the background will depend on its index.
        [NxATrackListRowView drawBackground:self.bounds dark:trackListCellView.darkBackground];
    }
    else {
        ALog(@"Invalid track list cell view type.");
    }
}

@end
