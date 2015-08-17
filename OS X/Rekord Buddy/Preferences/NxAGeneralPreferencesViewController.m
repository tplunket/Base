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

#import "NxAGeneralPreferencesViewController.h"

#pragma mark Constants

NSString *const NxAMusicalNotationPreferenceKey = @"UserDefaultsMusicalNotation";

#pragma mark - Private Interface

@interface NxAGeneralPreferencesViewController ()

#pragma mark Properties

/*! Current Traktor directory */
@property (assign, nonatomic) IBOutlet NSPopUpButton *p_musicalKeyNotationButton;

@end

#pragma mark - Implementation

@implementation NxAGeneralPreferencesViewController

#pragma mark Accessors

@synthesize delegate;

#pragma mark Instance Methods

/*!
 * Action triggered when the musical notation preference settings changes.
 * \param sender Who sent this message.
 */
- (IBAction)p_musicalNotationChanged:(id)sender
{
    NSInteger newNotation = self.p_musicalKeyNotationButton.selectedTag;

    [[NSUserDefaults standardUserDefaults] setInteger:newNotation
                                               forKey:NxAMusicalNotationPreferenceKey];
}

- (instancetype)init
{
    self = [super initWithNibName:@"NxAGeneralPreferencesView" bundle:nil];
    if (self) {
        NSInteger notation = [[NSUserDefaults standardUserDefaults] integerForKey:NxAMusicalNotationPreferenceKey];
        [self.p_musicalKeyNotationButton selectItemWithTag:notation];
    }

    return self;
}

#pragma mark MASPreferencesViewController Protocol

/*!
 * Delegate method to provide an identifier for this view.
 * \return A string that uniquely identifies this view in the preferences dialog.
 */
- (NSString *)identifier
{
    return @"GeneralPreferences";
}

/*!
 * Delegate method to provide the image to use on the toolbar.
 * \return An image that can be used to represent this view on the toolbar.
 */
- (NSImage *)toolbarItemImage
{
    return [NSImage imageNamed:NSImageNamePreferencesGeneral];
}

/*!
 * Delegate method to provide a label to use on the toolbar.
 * \return A string that can be used to represent this view on the toolbar.
 */
- (NSString *)toolbarItemLabel
{
    return NSLocalizedString(@"General",
                             @"Toolbar item name for the General preference pane.");
}

@end
