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

#import "NxAButtonCell.h"

#pragma mark Implementation

@implementation NxAButtonCell

#pragma mark Overridden NSView Methods

- (void)drawImage:(NSImage *)image withFrame:(NSRect)frame inView:(NSView *)controlView
{
    if (self.isHighlighted) {
        [self.backgroundColor set];
        NSRect backgroundFrame = NSMakeRect(frame.origin.x, frame.origin.y - 1.0,
                                            frame.size.width, frame.size.height + 2.0f);
        NSRectFill(backgroundFrame);

        [self.clickedImage drawInRect:frame
                             fromRect:NSZeroRect
                            operation:NSCompositeSourceOver
                             fraction:1.0
                       respectFlipped:YES
                                hints:nil];
    }
    else {
        [super drawImage:image withFrame:frame inView:controlView];
    }
}

@end
