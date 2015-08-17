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

#import "NxATheOnePreferencesViewController.h"
#import "NxATheOneParser.h"
#import "NSString+NxAUtility.h"

#pragma mark Constants

NSString *const NxAUserDefaultsSyncWithTheOneVersionKey = @"NxAUserDefaultsSyncWithTheOneVersion";

#pragma mark - Private Interface

@interface NxATheOnePreferencesViewController ()

@end

#pragma mark - Implementation

@implementation NxATheOnePreferencesViewController

#pragma mark Instance Methods

- (instancetype)init
{
    self = [super initWithNibName:@"NxATheOnePreferencesView" bundle:nil];
    NxAAssertNotNil(self);

    return self;
}

#pragma mark NxAPreferencesViewController protocol

@synthesize delegate;

#pragma mark NxAParser PreferencesViewController protocol

- (NSString *)nameOfTheApplication
{
    return [NxATheOneParser nameOfTheApplication];
}

- (BOOL)collectionIsAvailable
{
    return [NxATheOneParser databaseIsAvailable];
}

- (id<NxAParserImplementation>)parserImplementationUsingContext:(NSManagedObjectContext *)context
                                              loggingMessagesIn:(NxAMessageLog *)log
{
    return nil;
}

- (void)resetAll
{

}

#pragma mark MASPreferencesViewController Protocol

/*!
 * Delegate method to provide an identifier for this view.
 * \return A string that uniquely identifies this view in the preferences dialog.
 */
- (NSString *)identifier
{
    return @"TheOnePreferences";
}

/*!
 * Delegate method to provide the image to use on the toolbar.
 * \return An image that can be used to represent this view on the toolbar.
 */
- (NSImage *)toolbarItemImage
{
    return [NSImage imageNamed:@"TheOne Logo"];
}

/*!
 * Delegate method to provide a label to use on the toolbar.
 * \return A string that can be used to represent this view on the toolbar.
 */
- (NSString *)toolbarItemLabel
{
    return [self.nameOfTheApplication stringByCenteringInAStringOfAtLeastWidth:NxAPreferencesViewControllerItemMinimumWidth];
}

@end
