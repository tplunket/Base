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
#import "NxAAlertViewController.h"
#import "NxAWarningList.h"
#import "NSAttributedString+NxAUtility.h"
#import "NSError+NxAUtility.h"
#import "NxAMessageLog.h"
#import "NSApplication+Utility.h"

#pragma mark Private Interface

@interface NxAAlertViewController ()

#pragma mark Properties

@property (weak, nonatomic) IBOutlet NSWindow *p_alertWindow;

@property (weak, nonatomic) IBOutlet NSTextField *p_descriptionText;

@property (weak, nonatomic) IBOutlet NSTextField *p_recoverySuggestionText;

@property (strong, nonatomic) IBOutlet NSTextView *p_moreInfoText;

@property (assign, nonatomic) NSSize p_moreInfoWindowSize;

@property (weak) IBOutlet NSImageView *p_applicationIcon;

@property (weak, nonatomic) IBOutlet NSView *p_topInfoView;

@property (weak, nonatomic) IBOutlet NSScrollView *p_moreInfoView;

@property (weak, nonatomic) IBOutlet NSButton *p_okButton;

@property (weak, nonatomic) IBOutlet NSButton *p_cancelButton;

@property (weak, nonatomic) IBOutlet NSButton *p_moreInfoButton;

@property (weak, nonatomic) IBOutlet NSButton *p_moreInfoDisclosureTriangle;

@property (weak, nonatomic) IBOutlet NSLayoutConstraint *p_moreInfoViewHeightConstraint;

@end

#pragma mark - Implementation

@implementation NxAAlertViewController

#pragma mark Accessors

- (void)setOkButtonLabel:(NSString *)okButtonLabel
{
    self.p_okButton.title = okButtonLabel;
}

- (NSString *)okButtonLabel
{
    return self.p_okButton.title;
}

- (void)setCancelButtonLabel:(NSString *)cancelButtonLabel
{
    self.p_cancelButton.title = cancelButtonLabel;

    if (cancelButtonLabel.length) {
        [self.p_cancelButton setEnabled:YES];
        [self.p_cancelButton setTransparent:NO];
    }
    else {
        [self.p_cancelButton setEnabled:NO];
        [self.p_cancelButton setTransparent:YES];
    }
}

- (NSString *)cancelButtonLabel
{
    return self.p_cancelButton.title;
}

#pragma mark Instance Methods

/*!
 * Show the 'more info' view.
 */
- (void)p_showMoreInfo
{
    // -- Show the resize mouse pointer and let the user resize the window as big as they want.
    [self.p_alertWindow setShowsResizeIndicator:YES];
    [self.p_alertWindow setMaxSize:NSMakeSize(FLT_MAX, FLT_MAX)];
    
    // -- Expand the window back to the saved expanded size.
    NSRect windowFrame = self.p_alertWindow.frame;
    windowFrame.origin.x -= (self.p_moreInfoWindowSize.width - windowFrame.size.width) / 2.0;
    windowFrame.origin.y -= self.p_moreInfoWindowSize.height - windowFrame.size.height;
    windowFrame.size = self.p_moreInfoWindowSize;
    [self.p_alertWindow setFrame:windowFrame display:YES];
    
    // -- Add the new view to the window.
    NSScrollView *moreInfoView = self.p_moreInfoView;
    [moreInfoView setTranslatesAutoresizingMaskIntoConstraints:NO];
    [self.p_alertWindow.contentView addSubview:moreInfoView];
    
    // -- Make the left and the right side of the moreInfoBox match the top view.
    NSView *topView = self.p_topInfoView;
    NSArray *leftRightConstraints = @[ [NSLayoutConstraint constraintWithItem:topView
                                                                    attribute:NSLayoutAttributeLeft
                                                                    relatedBy:NSLayoutRelationEqual
                                                                       toItem:moreInfoView
                                                                    attribute:NSLayoutAttributeLeft
                                                                   multiplier:1.0
                                                                     constant:0.0],
                                       [NSLayoutConstraint constraintWithItem:topView
                                                                    attribute:NSLayoutAttributeRight
                                                                    relatedBy:NSLayoutRelationEqual
                                                                       toItem:moreInfoView
                                                                    attribute:NSLayoutAttributeRight
                                                                   multiplier:1.0
                                                                     constant:0.0]
                                       ];
    NxAAssertNotNil(leftRightConstraints);
    [self.p_alertWindow.contentView addConstraints:leftRightConstraints];
    
    // -- Vertically fit the container between the top view and the ok button.
    NSButton *button = self.p_okButton;
    [self.p_alertWindow.contentView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:[topView][moreInfoView]-8-[button]" options:0 metrics:nil views:NSDictionaryOfVariableBindings(topView, moreInfoView, button)]];
}

/*!
 * Hide the 'more info' view.
 */
- (void)p_hideMoreInfo
{
    // -- Save off the size of the alert window while the 'more info' view is visible so we can restore it later.
    self.p_moreInfoWindowSize = self.p_alertWindow.frame.size;
    
    // -- Remove the 'more info' text view.
    [self.p_moreInfoView removeFromSuperview];
    
    // -- Shrink the window back to the minimum size.
    NSRect windowFrame = self.p_alertWindow.frame;
    windowFrame.origin.x += (windowFrame.size.width - self.p_alertWindow.minSize.width) / 2.0;

    windowFrame.origin.y += windowFrame.size.height - self.p_alertWindow.minSize.height;
    windowFrame.size = self.p_alertWindow.minSize;
    [self.p_alertWindow setFrame:windowFrame display:YES];
    
    // -- Hide the resize mouse pointer and prevent the user from resizing the window.
    [self.p_alertWindow setShowsResizeIndicator:NO];
    [self.p_alertWindow setMaxSize:self.p_alertWindow.minSize];
}

/*!
 * Action triggered when the 'OK' button is pressed.
 * \param sender Who triggered this action.
 */
- (IBAction)p_okButtonPressed:(id)sender
{
    [self.parentWindow endSheet:self.p_alertWindow returnCode:NSModalResponseOK];
}

- (IBAction)p_cancelButtonPressed:(id)sender
{
    [self.parentWindow endSheet:self.p_alertWindow returnCode:NSModalResponseCancel];
}

/*!
 * Action triggered when the disclosure triangle is pressed.
 * \param sender Who triggered this action.
 */
- (IBAction)p_disclosureToggle:(id)sender
{
    if ([sender state] == NSOnState) {
        [self p_showMoreInfo];
    }
    else {
        [self p_hideMoreInfo];
    }
}

/*!
 * Action triggered when the 'Help' button is pressed.
 * \param sender Who triggered this action.
 */
- (IBAction)p_helpButtonPressed:(id)sender
{
    [NSApp showHelp:self];
}

- (void)p_displayError:(NSError *)error
     completionHandler:(void (^)(NSModalResponse returnCode))handler
{
    NxAAssertNotNil(error);

    // -- Populate the alert window with information from the error.
    [self.p_descriptionText setStringValue:error.localizedDescription];
    [self.p_recoverySuggestionText setStringValue:error.localizedRecoverySuggestion];

    NSMutableAttributedString *moreInformationString = [[NSAttributedString stringWithString:@""] mutableCopy];
    NxAAssertNotNil(moreInformationString);

    NSString *extraErrorInformation = error.localizedAdditionalInfo;
    if (extraErrorInformation) {
        [moreInformationString appendAttributedString:[NSAttributedString stringWithString:extraErrorInformation]];
    }

    NSString *extraUrlInformation = error.additionalUrl;
    if (extraUrlInformation) {
        [moreInformationString appendAttributedString:[NSAttributedString stringWithString:NSLocalizedString(@"\n\nRelated information can be found by ",
                                                                                                             @"First part of the string to tell the user that more information is available.")]];
        [moreInformationString appendAttributedString:[NSAttributedString stringWithHyperlink:NSLocalizedString(@"clicking this link.",
                                                                                                                @"Second part of the string to tell the user that more information is available. This part is converted to a clickable link")
                                                                                          url:[NSURL URLWithString:extraUrlInformation]]];
    }

    BOOL displayMoreInfoButton = NO;

    if (moreInformationString) {
        [moreInformationString appendAttributedString:[NSAttributedString stringWithString:NSLocalizedString(@"\nIf the problem persists, email us at ",
                                                                                                             @"First part of the string to give the user a link to the Next Audio support email.")]];
        [moreInformationString appendAttributedString:[NSAttributedString stringWithHyperlink:@"support@nextaudiolabs.com."
                                                                                          url:[NSURL URLWithString:@"mailto:support@nextaudiolabs.com"]]];
        [moreInformationString appendAttributedString:[NSAttributedString stringWithString:NSLocalizedString(@".",
                                                                                                             @"Second part of the string to give the user a link to the Next Audio forums or support email..")]];

        [self.p_moreInfoText.textStorage setAttributedString:moreInformationString];

        displayMoreInfoButton = YES;
    }

    [self.p_moreInfoButton setEnabled:displayMoreInfoButton];
    [self.p_moreInfoDisclosureTriangle setEnabled:displayMoreInfoButton];
    [self.p_cancelButton setEnabled:NO];
    [self.p_cancelButton setTransparent:YES];

    NxAAssertNotNil(self.parentWindow);

    // -- Bring up the sheet and wait for a button click.
    [self.parentWindow beginSheet:self.p_alertWindow completionHandler:handler];
}

- (void)p_displayWarnings:(NSAttributedString *)warningsText
          withDescription:(NSString *)description
   withRecoverySuggestion:(NSString *)suggestion
        andWhenFinishedDo:(void (^)(NSModalResponse returnCode))handler
{
    NxAAssertNotNil(warningsText);

    // -- Populate the alert window with information from the list of warnings.
    [self.p_descriptionText setStringValue:description];
    [self.p_recoverySuggestionText setStringValue:suggestion];

    [self.p_moreInfoText.textStorage setAttributedString:warningsText];

    [self.p_moreInfoButton setEnabled:YES];
    [self.p_moreInfoDisclosureTriangle setEnabled:YES];
    [self.p_cancelButton setEnabled:YES];
    [self.p_cancelButton setTransparent:NO];

    NxAAssertNotNil(self.parentWindow);

    // -- Bring up the sheet and wait for a button click.
    [self.parentWindow beginSheet:self.p_alertWindow completionHandler:handler];
}

- (void)p_showLog:(NxAMessageLog *)log
  withDescription:(NSString *)description
 andWhenFinishedDo:(void (^)(NSModalResponse returnCode))completionHandler
{
    NxAAssertNotNil(log);
    NxAAssertNotNil(description);

    [NSApplication runBlockOnMainThreadAndWaitUntilDone:^() {
        if (log.hasAnError) {
            [self p_displayError:log.error completionHandler:completionHandler];
        }
        else {
            NSMutableAttributedString *moreInformation = [[NSMutableAttributedString alloc] initWithString:@""];

            if (log.hasWarnings) {
                NSMutableAttributedString *headerString = [[NSMutableAttributedString alloc] initWithString:@"WARNINGS:\n"];
                [headerString beginEditing];
                [headerString applyFontTraits:NSBoldFontMask
                                        range:NSMakeRange(0, headerString.length)];
                [headerString endEditing];
                [moreInformation appendAttributedString:headerString];
                [moreInformation appendAttributedString:log.warningText];
            }

            if (log.hasMessages) {
                if (log.hasWarnings) {
                    [moreInformation appendAttributedString:[NSAttributedString stringWithString:@"\n\n"]];
                }

                NSMutableAttributedString *headerString = [[NSMutableAttributedString alloc] initWithString:@"DEBUG LOG:\n"];
                [headerString beginEditing];
                [headerString applyFontTraits:NSBoldFontMask
                                        range:NSMakeRange(0, headerString.length)];
                [headerString endEditing];
                [moreInformation appendAttributedString:headerString];
                [moreInformation appendAttributedString:[NSAttributedString stringWithString:log.messagesText]];
            }

            if (moreInformation.length) {
                [self p_displayWarnings:moreInformation
                        withDescription:description
                 withRecoverySuggestion:@"Click on 'more information' for details."
                      andWhenFinishedDo:completionHandler];
            }
            else {
                NSAlert *alert = [NSAlert alertWithMessageText:description
                                                 defaultButton:@"Ok"
                                               alternateButton:@"Cancel"
                                                   otherButton:nil
                                     informativeTextWithFormat:nil];

                [alert beginSheetModalForWindow:self.p_alertWindow completionHandler:completionHandler];
            }
        }
    }];
}

- (void)showLog:(NxAMessageLog *)log
 withDescription:(NSString *)description
 andWhenFinishedDo:(void (^)(NSModalResponse returnCode))completionHandler
{
    NxAAssertNotNil(completionHandler);

    [self p_showLog:log
        withDescription:description
        andWhenFinishedDo:completionHandler];
}

- (void)showLog:(NxAMessageLog *)log
 withDescription:(NSString *)description
{
    [self p_showLog:log
        withDescription:description
        andWhenFinishedDo:nil];
}

#pragma mark NSNibAwaking Protocol

- (void)awakeFromNib
{
    [super awakeFromNib];

    // -- This forces the nib to load.
    [self.view needsDisplay];
}

#pragma mark Overridden NSViewController methods

- (void)loadView
{
    [super loadView];

    NSSize windowSize = self.p_alertWindow.frame.size;

    // -- Hide the resize mouse pointer and prevent the user from resizing the window.
    [self.p_alertWindow setShowsResizeIndicator:NO];
    [self.p_alertWindow setMinSize:windowSize];
    [self.p_alertWindow setMaxSize:windowSize];

    // -- When more info is displayed, we initially let the window be a little taller.
    windowSize.height += 140.0;
    self.p_moreInfoWindowSize = windowSize;

    // -- Set the alert icon graphics.
#if BETA_BUILD
    NSString *const iconFilename = @"Beta App Icon";
#else
    NSString *const iconFilename = @"App Icon";
#endif
    NSImage *image = [NSImage imageNamed:iconFilename];

    NxAAssertNotNil(image);
    [self.p_applicationIcon setImage:image];
}

@end
