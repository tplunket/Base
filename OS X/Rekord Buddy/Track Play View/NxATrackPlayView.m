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

#import "NxATrackPlayView.h"

#pragma mark Implementation

@implementation NxATrackPlayView

#pragma mark Overridden NSView Methods

- (void)drawRect:(NSRect)dirtyRect
{
    NSGraphicsContext *nsGraphicsContext = [NSGraphicsContext currentContext];
    CGContextRef context = (CGContextRef)[nsGraphicsContext graphicsPort];

    NSColor *clearColor = [NSColor colorWithSRGBRed:0.0 green:0.0 blue:0.0 alpha:1.0];
    [clearColor setFill];

    CGContextSetRGBStrokeColor(context, 0.5, 0.5, 0.5, 1.0);

    CGRect rrect = self.bounds;
    CGFloat radius = 5.0;

    CGFloat minx = CGRectGetMinX(rrect), midx = CGRectGetMidX(rrect), maxx = CGRectGetMaxX(rrect);
    CGFloat miny = CGRectGetMinY(rrect), midy = CGRectGetMidY(rrect), maxy = CGRectGetMaxY(rrect);

    CGContextMoveToPoint(context, minx, midy);
    CGContextAddArcToPoint(context, minx, miny, midx, miny, radius);
    CGContextAddArcToPoint(context, maxx, miny, maxx, midy, radius);
    CGContextAddArcToPoint(context, maxx, maxy, midx, maxy, radius);
    CGContextAddArcToPoint(context, minx, maxy, minx, midy, radius);
    CGContextClosePath(context);

    CGContextDrawPath(context, kCGPathFillStroke);

    //NSRectFill(self.bounds);
}

@end
