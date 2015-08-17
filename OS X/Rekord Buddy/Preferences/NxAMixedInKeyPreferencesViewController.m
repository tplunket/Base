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

#import "NxAMixedInKeyPreferencesViewController.h"
#import "NxAAlertViewController.h"
#import "NSError+NxAUtility.h"
#import "NSString+NxAUtility.h"
#import "NxAMixedInKeyParser.h"
#import "NxAAppDelegate.h"

#pragma mark Constants

NSString *const NxAUserDefaultsSyncWithMixedInKeyVersionKey = @"NxAUserDefaultsSyncWithMixedInKeyVersion";

#pragma mark - Private Interface

@interface NxAMixedInKeyPreferencesViewController ()

#pragma mark Properties

/*! View controller used to display errors and warnings. */
@property (strong, nonatomic) IBOutlet NxAAlertViewController *p_alertViewController;

/*! Textfield used to display which version of MixedInKey we're syncing with. */
@property (weak, nonatomic) IBOutlet NSTextField *p_versionTextField;

@end

#pragma mark - Implementation

@implementation NxAMixedInKeyPreferencesViewController

#pragma mark Class Methods

/*!
 * Find the most recent MixedInKey version number.
 * \return The most recent MixedInKey version number or nil if nothing found.
 */
+ (NSString *)p_mostRecentMixedInKeyVersion
{
    NSMutableArray *versionsFound = [NSMutableArray array];

    NSURL *mixedInKeyDirectory = [NSURL fileURLWithPath:[@"~/Library/Application Support/Mixedinkey" stringByExpandingTildeInPath]];
    NSFileManager *fileManager = [NSFileManager defaultManager];

    NSArray *content = [fileManager contentsOfDirectoryAtURL:mixedInKeyDirectory
                                  includingPropertiesForKeys:[NSArray arrayWithObject:NSURLNameKey]
                                                     options:NSDirectoryEnumerationSkipsHiddenFiles
                                                       error:nil];
    for (NSURL *foundInMixedInKeyDirectory in content) {
        BOOL isDirectory = NO;
        if (![fileManager fileExistsAtPath:foundInMixedInKeyDirectory.path isDirectory:&isDirectory]) {
            continue;
        }

        if (isDirectory) {
            continue;
        }

        if (![foundInMixedInKeyDirectory.pathExtension isEqualToString:@"mikdb"]) {
            continue;
        }
        
        NSString *fileName = foundInMixedInKeyDirectory.lastPathComponent;
        if (![fileName hasPrefix:@"Collection"]) {
            continue;
        }

        NSString *versionName = [fileName substringWithRange:NSMakeRange(10, fileName.length - 10 - 6)];
        [versionsFound addObject:versionName];
    }

    NSSortDescriptor *sortDesc = [[NSSortDescriptor alloc] initWithKey:@"description"
                                                             ascending:YES
                                                              selector:@selector(localizedStandardCompare:)];
    [versionsFound sortUsingDescriptors:@[ sortDesc ]];

    return versionsFound.lastObject;
}

+ (NSURL *)collectionFileURL
{
    NSString *versionName = [[NSUserDefaults standardUserDefaults] stringForKey:NxAUserDefaultsSyncWithMixedInKeyVersionKey];
    if (!versionName) {
        return nil;
    }

    NSURL *collectionFile = [NSURL fileURLWithPath:[[NSString stringWithFormat:@"~/Library/Application Support/Mixedinkey/Collection%@.mikdb",
                                                     versionName]
                                                    stringByExpandingTildeInPath]];
    NxAAssertNotNil(collectionFile);
    
    return collectionFile;
}

#pragma mark Instance Methods

- (void)p_checkAvailableVersion
{
    NSUserDefaults *ud = [NSUserDefaults standardUserDefaults];
    NSString *previousVersion = [ud stringForKey:NxAUserDefaultsSyncWithMixedInKeyVersionKey];

    NSString *currentVersion = [NxAMixedInKeyPreferencesViewController p_mostRecentMixedInKeyVersion];
    if (!currentVersion) {
        [self.p_versionTextField setStringValue:@"No version of Mixed In Key installed."];
    }
    else {
        [self.p_versionTextField setStringValue:[NSString stringWithFormat:@"Latest MixedInKey version installed is %@", currentVersion]];
    }

    [ud setValue:currentVersion forKeyPath:NxAUserDefaultsSyncWithMixedInKeyVersionKey];

    if (previousVersion && ![previousVersion isEqualToString:currentVersion]) {
        NxAAppDelegate *appDelegate = (NxAAppDelegate *)[[NSApplication sharedApplication] delegate];
        
        if (currentVersion) {
            NSAlert *alert = [NSAlert alertWithMessageText:[NSString stringWithFormat:@"Detected version %@ of Mixed In Key's collection.", currentVersion]
                                             defaultButton:@"Ok"
                                           alternateButton:nil
                                               otherButton:nil
                                 informativeTextWithFormat:@"Check the manual to make sure this version is supported."];
            [appDelegate displayAlert:alert];
        }
        else if (previousVersion) {
            NSAlert *alert = [NSAlert alertWithMessageText:@"Can't find your Mixed In Key collection."
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
    self = [super initWithNibName:@"NxAMixedInKeyPreferencesView" bundle:nil];
    NxAAssertNotNil(self);

    [self p_forceViewToLoabNib];
    [self p_checkAvailableVersion];

    return self;
}

#pragma mark NxAPreferencesViewController protocol

@synthesize delegate;

#pragma mark NxAParser PreferencesViewController protocol

- (BOOL)collectionIsAvailable
{
    NSString *currentVersion = [[NSUserDefaults standardUserDefaults] stringForKey:NxAUserDefaultsSyncWithMixedInKeyVersionKey];
    return currentVersion != nil;
}

- (NSString *)nameOfTheApplication
{
    return [NxAMixedInKeyParser nameOfTheApplication];
}

- (id<NxAParserImplementation>)parserImplementationUsingContext:(NSManagedObjectContext *)context
                                              loggingMessagesIn:(NxAMessageLog *)log
{
    return nil;
}

- (void)resetAll
{
    NSUserDefaults *ud = [NSUserDefaults standardUserDefaults];
    [ud removeObjectForKey:NxAUserDefaultsSyncWithMixedInKeyVersionKey];

    [self p_checkAvailableVersion];
}

#pragma mark MASPreferencesViewController Protocol

/*!
 * Delegate method to provide an identifier for this view.
 * \return A string that uniquely identifies this view in the preferences dialog.
 */
- (NSString *)identifier
{
    return @"MixedInKeyPreferences";
}

/*!
 * Delegate method to provide the image to use on the toolbar.
 * \return An image that can be used to represent this view on the toolbar.
 */
- (NSImage *)toolbarItemImage
{
    return [NSImage imageNamed:@"MixedInKey Logo"];
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
