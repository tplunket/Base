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

#import "NxABackgroundView.h"

#pragma mark Implementation

@implementation NxABackgroundView

#pragma mark Overridden NSView Methods

- (void)drawRect:(NSRect)dirtyRect {
    [[NSColor colorWithRed:56.0/255.0 green:56.0/255.0 blue:56.0/255.0 alpha:1.0] set];

    NSRect bounds = self.bounds;
    NSRectFill(bounds);

    [super drawRect:dirtyRect];
}

@end
