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

#import "NxASeratoPreferencesViewController.h"
#import "NxAAlertViewController.h"
#import "NSError+NxAUtility.h"
#import "NSString+NxAUtility.h"
#import "Track Collection/Parsers/NxASeratoParser.h"
#import "NxAAppDelegate.h"

#pragma mark Constants

NSString *const NxAUserDefaultsSyncWithSeratoVersionKey = @"NxAUserDefaultsSyncWithSeratoVersion";

#pragma mark - Private Interface

@interface NxASeratoPreferencesViewController ()

#pragma mark Properties

@property (strong, nonatomic) IBOutlet NxAAlertViewController *p_alertViewController;

@property (weak, nonatomic) IBOutlet NSTextField *p_versionTextField;

@end

#pragma mark - Implementation

@implementation NxASeratoPreferencesViewController

#pragma mark Class Methods

+ (NSString *)p_mostRecentVersion
{
    if ([NxASeratoParser seratoIsAvailable]) {
        return [NxASeratoParser currentSeratoDatabaseVersion];
    }
    else {
        return nil;
    }
}

#pragma mark Instance Methods

- (void)p_forceViewToLoabNib
{
    [self.view needsDisplay];
}

- (void)p_checkAvailableVersion
{
    NSUserDefaults *ud = [NSUserDefaults standardUserDefaults];
    NSString *previousVersion = [ud stringForKey:NxAUserDefaultsSyncWithSeratoVersionKey];

    NSString *currentVersion = [NxASeratoPreferencesViewController p_mostRecentVersion];
    if (!currentVersion) {
        [self.p_versionTextField setStringValue:@"No version of Serato installed."];
    }
    else {
        [self.p_versionTextField setStringValue:[NSString stringWithFormat:@"Latest Serato database version installed is %@", currentVersion]];
    }

    [ud setValue:currentVersion forKeyPath:NxAUserDefaultsSyncWithSeratoVersionKey];

    if (previousVersion && ![previousVersion isEqualToString:currentVersion]) {
        NxAAppDelegate *appDelegate = (NxAAppDelegate *)[[NSApplication sharedApplication] delegate];
        
        if (currentVersion) {
            NSAlert *alert = [NSAlert alertWithMessageText:[NSString stringWithFormat:@"Detected version %@ of Serato's collection.", currentVersion]
                                             defaultButton:@"Ok"
                                           alternateButton:nil
                                               otherButton:nil
                                 informativeTextWithFormat:@"Check the manual to make sure this version is supported."];
            [appDelegate displayAlert:alert];
        }
        else if (previousVersion) {
            NSAlert *alert = [NSAlert alertWithMessageText:@"Can't find your Serato collection."
                                             defaultButton:@"Ok"
                                           alternateButton:nil
                                               otherButton:nil
                                 informativeTextWithFormat:@"Looks like the app was un-installed."];
            [appDelegate displayAlert:alert];
        }
    }
}

- (instancetype)init
{
    self = [super initWithNibName:@"NxASeratoPreferencesView" bundle:nil];
    NxAAssertNotNil(self);

    [self p_forceViewToLoabNib];
    [self p_checkAvailableVersion];

    return self;
}

#pragma mark NxAPreferencesViewController protocol

@synthesize delegate;

- (NSString *)nameOfTheApplication
{
    return [NxASeratoParser nameOfTheApplication];
}

- (BOOL)collectionIsAvailable
{
    NSString *currentVersion = [[NSUserDefaults standardUserDefaults] stringForKey:NxAUserDefaultsSyncWithSeratoVersionKey];
    return currentVersion != nil;
}

- (id<NxAParserImplementation>)parserImplementationUsingContext:(NSManagedObjectContext *)context
                                              loggingMessagesIn:(NxAMessageLog *)log
{
    NxASeratoParser *parser = [NxASeratoParser implementationWithContext:context
                                                        loggingMessagesIn:log];
    NxAAssertNotNil(parser);

    return parser;
}

- (void)resetAll
{
    NSUserDefaults *ud = [NSUserDefaults standardUserDefaults];
    [ud removeObjectForKey:NxAUserDefaultsSyncWithSeratoVersionKey];

    [self p_checkAvailableVersion];
}

#pragma mark MASPreferencesViewController Protocol

- (NSString *)identifier
{
    return @"SeratoPreferences";
}

- (NSImage *)toolbarItemImage
{
    return [NSImage imageNamed:@"Serato Logo"];
}

- (NSString *)toolbarItemLabel
{
    return [self.nameOfTheApplication stringByCenteringInAStringOfAtLeastWidth:NxAPreferencesViewControllerItemMinimumWidth];
}

@end
