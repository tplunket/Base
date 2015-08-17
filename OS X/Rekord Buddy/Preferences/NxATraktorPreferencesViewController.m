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

#import "NxATraktorPreferencesViewController.h"
#import "Track Collection/Parsers/NxATraktorParser.h"
#import "NxAAlertViewController.h"
#import "NSError+NxAUtility.h"
#import "NSString+NxAUtility.h"
#import "NxAAppDelegate.h"

#pragma mark Constants

NSString *const NxAUserDefaultsSyncWithTraktorVersionKey = @"NxAUserDefaultsSyncWithTraktorVersion";

#pragma mark - Private Interface

@interface NxATraktorPreferencesViewController ()

#pragma mark Properties

/*! View controller used to display errors and warnings. */
@property (strong, nonatomic) IBOutlet NxAAlertViewController *p_alertViewController;

/*! Textfield used to display which version of Traktor we're syncing with. */
@property (weak, nonatomic) IBOutlet NSTextField *p_versionTextField;

@end

#pragma mark - Implementation

@implementation NxATraktorPreferencesViewController

#pragma mark Class Methods

/*!
 * Return the full path to the Traktor collection file.
 * \param directoryURL URL pointing to the Traktor directory.
 * \return The full path if the directory is valid, nil otherwise.
 */
+ (NSURL *)p_collectionFileFromDirectory:(NSURL *)directoryURL
{
    NxAAssertNotNil(directoryURL);

    BOOL isADirectory = NO;
    NSFileManager *fileManager = [NSFileManager defaultManager];
    if ([fileManager fileExistsAtPath:directoryURL.path isDirectory:&isADirectory]) {
        if (isADirectory) {
            // -- If the directory does exist, we look for the collection file itself.
            NSURL *collectionFile = [directoryURL URLByAppendingPathComponent:@"collection.nml"];

            if ([fileManager fileExistsAtPath:collectionFile.path isDirectory:&isADirectory]) {
                if (!isADirectory) {
                    // -- If the file exists, we return the URL to the collection file.
                    return collectionFile;
                }
            }
        }
    }

    return nil;
}

/*!
 * Find the most recent Traktor version number.
 * \return The most recent Traktor version number or nil if nothing found.
 */
+ (NSString *)p_mostRecentTraktorVersion
{
    NSMutableArray *versionsFound = [NSMutableArray array];

    NSURL *traktorDirectory = [NSURL fileURLWithPath:[NSString stringWithFormat:@"/Users/%@/Documents/Native Instruments", NSUserName()]];
    NSFileManager *fileManager = [NSFileManager defaultManager];

    NSArray *content = [fileManager contentsOfDirectoryAtURL:traktorDirectory
                                  includingPropertiesForKeys:[NSArray arrayWithObject:NSURLNameKey]
                                                     options:NSDirectoryEnumerationSkipsHiddenFiles
                                                       error:nil];
    for (NSURL *foundInTraktorDirectory in content) {
        BOOL isDirectory = NO;
        if (![fileManager fileExistsAtPath:foundInTraktorDirectory.path isDirectory:&isDirectory]) {
            continue;
        }

        if (!isDirectory) {
            continue;
        }

        NSString *directoryName = foundInTraktorDirectory.lastPathComponent;
        if (![directoryName hasPrefix:@"Traktor "]) {
            continue;
        }

        NSURL *collectionFile = [NxATraktorPreferencesViewController p_collectionFileFromDirectory:foundInTraktorDirectory];
        if (collectionFile == nil) {
            // -- If the URL for the collection file is nil then the directory is not valid.
            continue;
        }
        

        NSString *versionName = [directoryName substringFromIndex:8];
        [versionsFound addObject:versionName];
    }

    NSSortDescriptor *sortDesc = [[NSSortDescriptor alloc] initWithKey:@"description"
                                                             ascending:YES
                                                              selector:@selector(localizedStandardCompare:)];
    [versionsFound sortUsingDescriptors:@[ sortDesc ]];

    return versionsFound.lastObject;
}

+ (NSURL *)p_collectionFileURL
{
    NSString *versionName = [[NSUserDefaults standardUserDefaults] stringForKey:NxAUserDefaultsSyncWithTraktorVersionKey];
    if (!versionName) {
        return nil;
    }

    NSURL *collectionFileURL = [NSURL fileURLWithPath:[NSString stringWithFormat:@"/Users/%@/Documents/Native Instruments/Traktor %@/collection.nml",
                                                    NSUserName(), versionName]];
    NxAAssertNotNil(collectionFileURL);

    return collectionFileURL;
}

#pragma mark Instance Methods

- (void)p_checkAvailableVersion
{
    NSUserDefaults *ud = [NSUserDefaults standardUserDefaults];
    NSString *previousVersion = [ud stringForKey:NxAUserDefaultsSyncWithTraktorVersionKey];

    NSString *currentVersion = [NxATraktorPreferencesViewController p_mostRecentTraktorVersion];
    if (!currentVersion) {
        [self.p_versionTextField setStringValue:@"No version of Traktor installed."];
    }
    else {
        [self.p_versionTextField setStringValue:[NSString stringWithFormat:@"Latest Traktor version installed is %@", currentVersion]];
    }

    [ud setValue:currentVersion forKeyPath:NxAUserDefaultsSyncWithTraktorVersionKey];

    if (previousVersion && ![previousVersion isEqualToString:currentVersion]) {
        NxAAppDelegate *appDelegate = (NxAAppDelegate *)[[NSApplication sharedApplication] delegate];
        
        if (currentVersion) {
            NSAlert *alert = [NSAlert alertWithMessageText:[NSString stringWithFormat:@"Detected version %@ of Traktor's collection.", currentVersion]
                                             defaultButton:@"Ok"
                                           alternateButton:nil
                                               otherButton:nil
                                 informativeTextWithFormat:@"Check the manual to make sure this version is supported."];
            [appDelegate displayAlert:alert];
        }
        else if (previousVersion) {
            NSAlert *alert = [NSAlert alertWithMessageText:@"Can't find your Traktor collection."
                                             defaultButton:@"Ok"
                                           alternateButton:nil
                                               otherButton:nil
                                 informativeTextWithFormat:@"Looks like the app was un-installed."];
            [appDelegate displayAlert:alert];
        }
    }
}

- (void)p_forceViewToLoabNib
{
    [self.view needsDisplay];
}

- (instancetype)init
{
    self = [super initWithNibName:@"NxATraktorPreferencesView" bundle:nil];
    NxAAssertNotNil(self);

    [self p_forceViewToLoabNib];
    [self p_checkAvailableVersion];

    return self;
}

#pragma mark NxAPreferencesViewController protocol

@synthesize delegate;

- (NSString *)nameOfTheApplication
{
    return [NxATraktorParser nameOfTheApplication];
}

- (BOOL)collectionIsAvailable
{
    NSString *currentVersion = [[NSUserDefaults standardUserDefaults] stringForKey:NxAUserDefaultsSyncWithTraktorVersionKey];
    return currentVersion != nil;
}

- (id<NxAParserImplementation>)parserImplementationUsingContext:(NSManagedObjectContext *)context
                                              loggingMessagesIn:(NxAMessageLog *)log
{
    NxATraktorParser *traktorParser = [NxATraktorParser implementationWithContext:context
                                                                loggingMessagesIn:log];
    NxAAssertNotNil(traktorParser);

    traktorParser.collectionFileURL = [NxATraktorPreferencesViewController p_collectionFileURL];

    return traktorParser;
}

- (void)resetAll
{
    NSUserDefaults *ud = [NSUserDefaults standardUserDefaults];
    [ud removeObjectForKey:NxAUserDefaultsSyncWithTraktorVersionKey];

    [self p_checkAvailableVersion];
}

#pragma mark MASPreferencesViewController Protocol

/*!
 * Delegate method to provide an identifier for this view.
 * \return A string that uniquely identifies this view in the preferences dialog.
 */
- (NSString *)identifier
{
    return @"TraktorPreferences";
}

/*!
 * Delegate method to provide the image to use on the toolbar.
 * \return An image that can be used to represent this view on the toolbar.
 */
- (NSImage *)toolbarItemImage
{
    return [NSImage imageNamed:@"Traktor Logo"];
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
