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

#import "NxAProgressViewController.h"
#import "NSApplication+Utility.h"

@interface NxAProgressViewController ()

/*! Main progress window. */
@property (strong, nonatomic) IBOutlet NSWindow *p_window;

/*! Status text field in the progress view. */
@property (weak, nonatomic) IBOutlet NSTextField *p_statusField;

/*! Progress indicator in the progress view. */
@property (weak, nonatomic) IBOutlet NSProgressIndicator *p_progressBar;

@end

@implementation NxAProgressViewController

#pragma mark Accessors

- (NSString *)status
{
    return self.p_statusField.stringValue;
}

- (void)setStatus:(NSString *)status
{
    [NSApplication runBlockOnMainThread:^() {
        self.p_statusField.stringValue = status;
    }];
}

- (double)min
{
    return self.p_progressBar.minValue;
}

- (void)setMin:(double)min
{
    [NSApplication runBlockOnMainThread:^() {
        self.p_progressBar.minValue = min;
    }];
}

- (double)max
{
    return self.p_progressBar.maxValue;
}

- (void)setMax:(double)max
{
    [NSApplication runBlockOnMainThread:^() {
        self.p_progressBar.maxValue = max;
    }];
}

- (double)value
{
    return self.p_progressBar.doubleValue;
}

- (void)setValue:(double)value
{
    [NSApplication runBlockOnMainThread:^() {
        self.p_progressBar.doubleValue = value;
    }];
}

#pragma mark Instance Methods

- (void)sheetDidEnd:(NSWindow *)sheet
         returnCode:(NSInteger)returnCode contextInfo:(void *)contextInfo
{
    [self.p_window orderOut:self];
}

- (void)show
{
    NxAAssertNotNil(self.parentWindow);

    [NSApplication runBlockOnMainThread:^() {
        [NSApp beginSheet:self.p_window
           modalForWindow:self.parentWindow
            modalDelegate:self
           didEndSelector:@selector(sheetDidEnd:returnCode:contextInfo:)
              contextInfo:nil];
    }];
}

- (void)hide
{
    [NSApplication runBlockOnMainThread:^() {
        [NSApp endSheet:self.p_window];
    }];
}

#pragma mark NSNibAwaking Protocol

- (void)awakeFromNib
{
    [super awakeFromNib];

    // -- This forces the nib file to load for the view.
    [self.view needsDisplay];
}

@end
