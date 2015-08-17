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

#import "NxAView.h"

#pragma mark Implementation

@implementation NxAView

#pragma mark Overridden NSView Methods

- (void)drawRect:(NSRect)dirtyRect {
    // -- We let the view draw its stuff first.
    [super drawRect:dirtyRect];
    
    // -- Then we just draw over the outer edge to replace the grey with a dark grey outline instead.
    [[NSColor colorWithRed:40.0/255.0 green:40.0/255.0 blue:40.0/255.0 alpha:1.0] set];
    
    NSRect bounds = self.bounds;
    
    bounds.size.width = 1.0;
    NSRectFill(bounds);
    
    bounds.origin.x += self.bounds.size.width - 1;
    NSRectFill(bounds);
    
    bounds = self.bounds;
    bounds.size.height = 1.0;
    NSRectFill(bounds);
    
    bounds.origin.y += self.bounds.size.height - 1;
    NSRectFill(bounds);
}

@end
