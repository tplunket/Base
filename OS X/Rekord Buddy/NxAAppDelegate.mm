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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#import <HockeySDK/HockeySDK.h>
#pragma clang diagnostic pop

#import <Sparkle/SUUpdater.h>
#import <RNCryptor/RNEncryptor.h>
#import <RNCryptor/RNDecryptor.h>

#import "NxAAppDelegate.h"
#import "MASPreferencesWindowController.h"
#import "NxAGeneralPreferencesViewController.h"
#import "NxATraktorPreferencesViewController.h"
#import "NxASeratoPreferencesViewController.h"
#import "NxAMixedInKeyPreferencesViewController.h"
#import "NxATheOnePreferencesViewController.h"
#import "NxARekordboxPreferencesViewController.h"
#import "NxAID3Parser.h"
#import "NxAProgressViewController.h"
#import "NxAAlertViewController.h"
#import "NxACrateListViewController.h"
#import "NxATrackListViewController.h"
#import "Track Collection/Tags/Track Properties/NxATrackKey.h"
#import "NxAArtist+NxACrateListViewItem.h"
#import "NxACrateListViewItem.h"
#import "NxACrateListViewItemWithChild.h"
#import "NxACrateListViewTagClassItem.h"
#import "NxACrate+NxACrateListViewItemSource.h"
#import "NxATrackTag+NxACrateListViewItemSource.h"
#import "NxAMessageLog.h"
#import "NxASyncingOperation.h"
#import "Track Collection/Operations/NxAParsingOperation.h"
#import "NSApplication+Utility.h"
#import "NSError+NxAUtility.h"
#import "NxAAppProtection.h"
#import "NxAException.h"
#import "NSFileManager+DirectoryLocations.h"
#import "NxATrack.h"
#import "NxAButtonCell.h"

#define NXA_TRADESHOW_DEMO_MODE                             0

#pragma mark Private Interface

@interface NxAAppDelegate ()

#pragma mark Properties

/*! View controller for the progress bar. */
@property (weak, nonatomic) IBOutlet NxAProgressViewController *p_progressViewController;

/*! View controller for alerts. */
@property (weak, nonatomic) IBOutlet NxAAlertViewController *p_alertViewController;

/*! View controller for the crate list view. */
@property (weak, nonatomic) IBOutlet NxACrateListViewController *p_crateListViewController;

/*! View controller for the track list view. */
@property (weak, nonatomic) IBOutlet NxATrackListViewController *p_trackListViewController;

/*! Main window for the application. */
@property (weak, nonatomic) IBOutlet NSWindow *p_window;

/*! Control in the main window used to select which track view is displayed. */
@property (weak, nonatomic) IBOutlet NSSegmentedControl *p_trackViewSelector;

@property (weak, nonatomic) IBOutlet NSWindow *p_splashWindow;

@property (weak, nonatomic) IBOutlet NSTextField *p_splashWindowVersion;

@property (weak, nonatomic) IBOutlet NSWindow *p_aboutWindow;

@property (weak, nonatomic) IBOutlet NSWindow *p_betaWarningWindow;

@property (weak, nonatomic) IBOutlet NSWindow *p_collectionInitWindow;

@property (weak, nonatomic) IBOutlet NSPopUpButton *p_collectionInitSelector;

@property (weak, nonatomic) IBOutlet NxAButtonCell *p_syncButtonCell;

/*! Updater used to keep the executable up to date. */
@property (strong, nonatomic) SUUpdater *p_sparkleUpdater;

/*! Core data model used for the database. */
@property (strong, nonatomic) NSManagedObjectModel *p_managedObjectModel;

/*! Core data coordinator for the stored database file. */
@property (strong, nonatomic) NSPersistentStoreCoordinator *p_persistentStoreCoordinator;

/*! Our private managed object context for direct disk access. */
@property (strong, nonatomic) NSManagedObjectContext *p_masterManagedObjectContext;

/*! Core data's managed object context for the main queue. */
@property (strong, nonatomic) NSManagedObjectContext *p_uiManagedObjectContext;

@property (strong, nonatomic) NSArray *p_mainWindowTopLevelObjects;

@property (strong, nonatomic) NSArray *p_splashWindowTopLevelObjects;

@property (strong, nonatomic) NSArray *p_aboutWindowTopLevelObjects;

@property (strong, nonatomic) NSArray *p_collectionInitWindowTopLevelObjects;

@property (strong, nonatomic) NSArray *p_betaWarningWindowTopLevelObjects;

@property (strong, nonatomic) MASPreferencesWindowController *p_preferencesWindowController;

@property (strong, nonatomic) NSArray *p_preferencesViewControllers;

/*! Top level list displayed in the crate list view. */
@property (strong, nonatomic) NSArray *p_crateListItems;

@property (strong, nonatomic) NxACrate *p_allTracksCrate;

@property (strong, nonatomic) NxACrate *p_rootCrate;

/*! Queue of background operations. */
@property (strong, nonatomic) NSOperationQueue *p_operationQueue;

@end

#pragma mark - Implementation

@implementation NxAAppDelegate

#pragma mark Class Methods

+ (NSDictionary *)p_defaultPreferenceValues
{
    static NSDictionary *_defaultPreferenceValues = nil;
    static dispatch_once_t onceToken = 0;
    dispatch_once(&onceToken, ^{
        _defaultPreferenceValues = @{ NxAMusicalNotationPreferenceKey:@(NxAMusicalNotationStandard)
                                      };
    });

    NxAAssertNotNil(_defaultPreferenceValues);
    return _defaultPreferenceValues;
}

+ (NSURL *)p_applicationFilesDirectory
{
    static NSURL *_applicationFilesDirectory = nil;
    static dispatch_once_t onceToken = 0;
    dispatch_once(&onceToken, ^{
        NSFileManager *fileManager = [NSFileManager defaultManager];
        NSURL *appSupportURL = [[fileManager URLsForDirectory:NSApplicationSupportDirectory inDomains:NSUserDomainMask] lastObject];

        NSDictionary *appInfo = [[NSBundle mainBundle] infoDictionary];
        _applicationFilesDirectory = [appSupportURL URLByAppendingPathComponent:appInfo[@"CFBundleIdentifier"]];
    });

    NxAAssertNotNil(_applicationFilesDirectory);
    return _applicationFilesDirectory;
}

#pragma mark Accessors

- (NSArray *)p_preferencesViewControllers
{
    if (!_p_preferencesViewControllers) {
        NSMutableArray *newControllers = [NSMutableArray array];

        NSArray *controllerClasses = @[ [NxAGeneralPreferencesViewController class],
                                        [NxARekordboxPreferencesViewController class],
                                        [NxATraktorPreferencesViewController class],
                                        [NxASeratoPreferencesViewController class],
                                        ];

        for (Class controllerClass in controllerClasses) {
            id newController = [[controllerClass alloc] init];
            NxAAssertNotNil(newController);
            [newControllers addObject:newController];
        }

        _p_preferencesViewControllers = newControllers;
    }

    return _p_preferencesViewControllers;
}

- (NSArray *)p_emptyCrateListItems
{
    NSManagedObjectContext *context = self.p_uiManagedObjectContext;

    NxACrateListViewItem *tracksItem = [NxACrateListViewTagClassItem itemWithTagClass:[NxAArtist class]
                                                                          displayedAs:@"TRACKS"
                                                                         usingContext:context];
    NxACrateListViewItem *cratesItem = [NxACrateListViewTagClassItem itemWithTagClass:[NxAArtist class]
                                                                          displayedAs:@"CRATES"
                                                                         usingContext:context];

    return @[ tracksItem, cratesItem ];
}

- (NSArray *)p_crateListItems
{
    NSManagedObjectContext *context = self.p_uiManagedObjectContext;

    if (![NxACrate canFindARootCrateInContext:context]) {
        return [self p_emptyCrateListItems];
    }

    if (!_p_crateListItems) {
        self.p_allTracksCrate = [NxACrate allTracksCrateIn:context];
        self.p_rootCrate = [NxACrate rootCrateIn:context];

        NxACrateListViewItemWithChild *tracksItem = [NxACrateListViewItemWithChild itemWithChild:self.p_allTracksCrate
                                                                                     displayedAs:@"TRACKS"];
        tracksItem.childDisplayTitle = @"All Tracks";
        NxACrateListViewItem *artistsItem = [NxACrateListViewTagClassItem itemWithTagClass:[NxAArtist class]
                                                                               displayedAs:@"ARTISTS"
                                                                              usingContext:context];
        NxACrateListViewItem *keysItem = [NxACrateListViewTagClassItem itemWithTagClass:[NxATrackKey class]
                                                                            displayedAs:@"KEYS"
                                                                           usingContext:context];
        NxACrateListViewItem *tagsItem = [NxACrateListViewTagClassItem itemWithTagClass:[NxATrackTag class]
                                                                            displayedAs:@"TAGS"
                                                                           usingContext:context];
        NxACrateListViewItem *cratesItem = [NxACrateListViewItem itemWith:self.p_rootCrate
                                                              displayedAs:@"CRATES"];
        cratesItem.shouldBeExpandedAtStart = YES;
        cratesItem.isSelectable = NO;

        _p_crateListItems = @[ tracksItem, artistsItem, keysItem, tagsItem, cratesItem ];
        NxAAssertNotNil(_p_crateListItems);
    }

    return _p_crateListItems;
}

- (NSOperationQueue *)p_operationQueue
{
    if (!_p_operationQueue) {
        _p_operationQueue = [[NSOperationQueue alloc] init];
        NxAAssertNotNil(_p_operationQueue);
    }

    return _p_operationQueue;
}

- (NSManagedObjectModel *)p_managedObjectModel
{
    if (!_p_managedObjectModel) {
        NSURL *modelURL = [[NSBundle mainBundle] URLForResource:@"RekordBuddy" withExtension:@"momd"];
        NxAAssertNotNil(modelURL);

        _p_managedObjectModel = [[NSManagedObjectModel alloc] initWithContentsOfURL:modelURL];
        NxAAssertNotNil(_p_managedObjectModel);
    }

    return _p_managedObjectModel;
}

- (NSPersistentStoreCoordinator *)p_persistentStoreCoordinator
{
    if (!_p_persistentStoreCoordinator) {
        NSManagedObjectModel *mom = self.p_managedObjectModel;
        if (!mom) {
            NSLog(@"%@:%@ No model to generate a store from", self.class, NSStringFromSelector(_cmd));
            return nil;
        }

        NSFileManager *fileManager = [NSFileManager defaultManager];
        NSURL *applicationFilesDirectory = [NxAAppDelegate p_applicationFilesDirectory];
        NSError *error = nil;

        NSDictionary *properties = [applicationFilesDirectory resourceValuesForKeys:@[ NSURLIsDirectoryKey ] error:&error];
        if (!properties) {
            if (error.code == NSFileReadNoSuchFileError) {
                if (![fileManager createDirectoryAtPath:applicationFilesDirectory.path withIntermediateDirectories:YES attributes:nil error:&error]) {
                    [[NSApplication sharedApplication] presentError:error];
                    return nil;
                }
            }
        }
        else {
            if (![properties[NSURLIsDirectoryKey] boolValue]) {
                // Customize and localize this error.
                NSDictionary *dict = @{ NSLocalizedDescriptionKey : [NSString stringWithFormat:@"Expected a folder to store application data, found a file (%@).", applicationFilesDirectory.path]
                                        };

                NSDictionary *appInfo = [[NSBundle mainBundle] infoDictionary];
                error = [NSError errorWithDomain:appInfo[@"CFBundleIdentifier"] code:101 userInfo:dict];

                [[NSApplication sharedApplication] presentError:error];
                return nil;
            }
        }

        NSURL *url = [applicationFilesDirectory URLByAppendingPathComponent:@"RekordBuddy.storedata"];
        NSPersistentStoreCoordinator *coordinator = [[NSPersistentStoreCoordinator alloc] initWithManagedObjectModel:mom];
        if (![coordinator addPersistentStoreWithType:NSSQLiteStoreType configuration:nil URL:url options:nil error:&error]) {
            // -- For now, we just assume the schema has changed and we rest the database.
            // -- In the future, some kind of migration should take place.
            [self p_resetDatabase];

            // -- TODO: Display a message for the user.

            // -- Then we try to load it again.
            if (![coordinator addPersistentStoreWithType:NSSQLiteStoreType configuration:nil URL:url options:nil error:&error]) {
                [[NSApplication sharedApplication] presentError:error];
                return nil;
            }
        }

        _p_persistentStoreCoordinator = coordinator;
        NxAAssertNotNil(_p_persistentStoreCoordinator);
    }

    return _p_persistentStoreCoordinator;
}

- (NSManagedObjectContext *)p_masterManagedObjectContext
{
    if (!_p_masterManagedObjectContext) {
        NSPersistentStoreCoordinator *coordinator = self.p_persistentStoreCoordinator;
        if (!coordinator) {
            NSDictionary *dict = @{ NSLocalizedDescriptionKey : @"Failed to initialize the store",
                                    NSLocalizedFailureReasonErrorKey : @"There was an error building up the data file."
                                    };
            NSDictionary *appInfo = [[NSBundle mainBundle] infoDictionary];

            NSError *error = [NSError errorWithDomain:appInfo[@"CFBundleIdentifier"] code:9999 userInfo:dict];
            [[NSApplication sharedApplication] presentError:error];
            return nil;
        }

        _p_masterManagedObjectContext = [[NSManagedObjectContext alloc] initWithConcurrencyType:NSPrivateQueueConcurrencyType];

        _p_masterManagedObjectContext.persistentStoreCoordinator = coordinator;
        NxAAssertNotNil(_p_masterManagedObjectContext);
    }

    return _p_masterManagedObjectContext;
}

- (NSManagedObjectContext *)p_uiManagedObjectContext
{
    if (!_p_uiManagedObjectContext) {
        _p_uiManagedObjectContext = [[NSManagedObjectContext alloc] initWithConcurrencyType:NSMainQueueConcurrencyType];

        _p_uiManagedObjectContext.parentContext = self.p_masterManagedObjectContext;
        NxAAssertNotNil(_p_uiManagedObjectContext);
    }

    return _p_uiManagedObjectContext;
}

- (NSString *)appVersionAsString
{
    NSDictionary *appInfo = [[NSBundle mainBundle] infoDictionary];
    NxAAssertNotNil(appInfo);
    
    NSString *appVersionString = [NSString stringWithFormat:@"v%@(%@)",
                                  appInfo[@"CFBundleShortVersionString"],
                                  appInfo[@"CFBundleVersion"]];
    
    return appVersionString;
}

#pragma mark Instance Methods

- (void)p_setupPreferencesWindowController
{
    NSArray *controllers = self.p_preferencesViewControllers;

    for (id<NxAPreferencesViewController> controller in controllers) {
        NxAAssertNotNil(controller);
        controller.delegate = self;
    }

    NSString *title = NSLocalizedString(@"Preferences",
                                        @"Common title for Preferences window.");
    self.p_preferencesWindowController = [[MASPreferencesWindowController alloc] initWithViewControllers:controllers title:title];
    NxAAssertNotNil(self.p_preferencesWindowController);
}

#if BETA_BUILD && !DEBUG
- (void)p_checkForBetaExpiration
{
    NSURL *url = [NSURL URLWithString:[NSString stringWithFormat:@"https://nextaudiolabs.com/rekord-buddy-beta-updates/%@", NXA_APP_PROTECTION_KEY_FILENAME_STRING]];
    NSURLSessionDataTask *downloadTask = [[NSURLSession sharedSession]
                                          dataTaskWithURL:url completionHandler:^(NSData *data, NSURLResponse *response, NSError *error) {
                                              [NSApplication runBlockOnMainThread:^() {
                                                  if (!data.length) {
                                                      [self.p_splashWindow close];
                                                      
                                                      NSAlert *alert = [NSAlert alertWithMessageText:@"Can't authorize beta version."
                                                                                       defaultButton:@"Ok"
                                                                                     alternateButton:nil
                                                                                         otherButton:nil
                                                                           informativeTextWithFormat:@"Beta releases require an internet connection."];
                                                      [alert runModal];
                                                      return;
                                                  }
                                                  else {
                                                      NSError *localError = nil;
                                                      NSData *decryptedData = [RNDecryptor decryptData:data
                                                                                          withPassword:NXA_APP_PROTECTION_ENCRYPTION_KEY_STRING
                                                                                                 error:&localError];
                                                      if (decryptedData) {
                                                          NSArray *result = [NSKeyedUnarchiver unarchiveObjectWithData:decryptedData];
                                                          Class class = NSClassFromString(result[0]);
                                                          
                                                          if ([class isSubclassOfClass:[NxATag class]]) {
                                                              SEL selector = NSSelectorFromString(result[1]);
                                                              if (selector) {
                                                                  IMP imp = [class methodForSelector:selector];
                                                                  void (*func)(id, SEL) = (void *)imp;
                                                                  func(class, selector);
                                                                  return;
                                                              }
                                                          }
                                                      }
                                                  }
                                                  
                                                  [self.p_splashWindow close];
                                                  
                                                  NSAlert *alert = [NSAlert alertWithMessageText:@"This beta version has expired."
                                                                                   defaultButton:@"Ok"
                                                                                 alternateButton:@"Check For Update"
                                                                                     otherButton:nil
                                                                       informativeTextWithFormat:@"If a new beta version is not available then this beta period might be over."];
                                                  
                                                  if ([alert runModal] == NSAlertAlternateReturn) {
                                                      [self.p_sparkleUpdater checkForUpdates:self];
                                                  }
                                              }];
                                          }];
    
    [downloadTask resume];
}

#endif

- (void)p_setInternalDatabaseAsModifiedNowInContext:(NSManagedObjectContext *)context
                             andAddAnyMessagesToLog:(NxAMessageLog *)log
{
    NSDate *now = [NSDate date];

    [NxATagEntry setAllTagEntriesAsModified:now inContext:context andAddAnyMessagesToLog:log];

    NSArray *allTracks = [[NxACrate allTracksCrateIn:context] tracks];
    for (NxATrack *track in allTracks) {
        [track markAsModifiedOn:now];
    }
}

- (IBAction)p_cancelDatabaseInitWindow:(id)sender
{
    [self.p_window endSheet:self.p_collectionInitWindow];
}

- (IBAction)p_okDatabaseInitWindow:(id)sender
{
    [self.p_window endSheet:self.p_collectionInitWindow];

    NSManagedObjectContext *threadContext = [[NSManagedObjectContext alloc] initWithConcurrencyType:NSPrivateQueueConcurrencyType];
    threadContext.parentContext = self.p_uiManagedObjectContext;

    NSMenuItem *selectedItem = self.p_collectionInitSelector.selectedItem;
    id<NxAParserPreferencesViewController> parserPreferencesController = selectedItem.representedObject;

    [self p_syncAndForceUpdateToParser:parserPreferencesController];
}

- (void)p_offerToInitializeTheDatabase
{
    [self.p_collectionInitSelector.menu removeAllItems];

    NSArray *controllers = self.p_preferencesViewControllers;
    for (id<NxAPreferencesViewController> controller in controllers) {
        if (![[controller class] conformsToProtocol:@protocol(NxAParserPreferencesViewController)]) {
            continue;
        }

        id<NxAParserPreferencesViewController> parserPreferencesController = (id<NxAParserPreferencesViewController>)controller;
        if (parserPreferencesController.collectionIsAvailable) {
            NSMenuItem *newItem = [[NSMenuItem alloc] initWithTitle:parserPreferencesController.nameOfTheApplication
                                                             action:nil
                                                      keyEquivalent:@""];
            [newItem setRepresentedObject:parserPreferencesController];
            [self.p_collectionInitSelector.menu addItem:newItem];
        }
    }

    if (self.p_collectionInitSelector.menu.itemArray.count == 0) {
        NSAlert *alert = [NSAlert alertWithMessageText:@"Can't find any apps to sync with."
                                         defaultButton:@"Ok"
                                       alternateButton:nil
                                           otherButton:nil
                             informativeTextWithFormat:@"It seem like you don't have any app installed that Rekord Buddy can sync with."];
        [alert beginSheetModalForWindow:self.p_window completionHandler:^(NSInteger result) {
            [alert.window orderOut:self];
        }];
        return;
    }

    [self.p_window beginSheet:self.p_collectionInitWindow completionHandler:nil];
}

#if BETA_BUILD
- (IBAction)p_betaWarningWindowOk:(id)sender
{
    [self.p_window endSheet:self.p_betaWarningWindow];

    if (![NxACrate canFindARootCrateInContext:self.p_masterManagedObjectContext]) {
        [self p_offerToInitializeTheDatabase];
    }
}

- (IBAction)p_betaWarningWindowCancel:(id)sender
{
    [self.p_window endSheet:self.p_betaWarningWindow];

    [[NSApplication sharedApplication] terminate:self];
}

- (void)p_showBetaWarningWindow
{
    [self.p_window beginSheet:self.p_betaWarningWindow completionHandler:nil];
}
#endif

- (void)p_resetDatabase
{
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSString *dir = [fileManager applicationSupportDirectory];

    [fileManager removeItemAtPath:[dir stringByAppendingPathComponent:@"RekordBuddy.storedata"] error:nil];
    [fileManager removeItemAtPath:[dir stringByAppendingPathComponent:@"RekordBuddy.storedata-shm"] error:nil];
    [fileManager removeItemAtPath:[dir stringByAppendingPathComponent:@"RekordBuddy.storedata-wal"] error:nil];

    NSArray *controllers = self.p_preferencesViewControllers;
    for (id<NxAPreferencesViewController> controller in controllers) {
        if (![[controller class] conformsToProtocol:@protocol(NxAParserPreferencesViewController)]) {
            continue;
        }

        id<NxAParserPreferencesViewController> parserPreferencesController = (id<NxAParserPreferencesViewController>)controller;
        [parserPreferencesController resetAll];
    }
}

/*!
 * Save any changes made to our UI object context.
 */
- (void)p_saveUIChanges
{
    NSManagedObjectContext *uiContext = self.p_uiManagedObjectContext;
    if (!uiContext.commitEditing) {
        NSLog(@"%@:%@ unable to commit editing before saving", self.class, NSStringFromSelector(_cmd));
    }

#if !NXA_TRADESHOW_DEMO_MODE
    if (uiContext.hasChanges) {
        // -- If our UI context has changes, we save them.
        [uiContext performBlockAndWait:^{
            NSError *error = nil;

            if (![uiContext save:&error]) {
                [[NSApplication sharedApplication] presentError:error];
            }
        }];
    }
#endif
}

/*!
 * Open the Prefence window.
 * \param sender Who is sending the action.
 */
- (IBAction)p_openPreferences:(id)sender
{
    [NxAException runBlockAndExitOnExceptions:^() {
        [self.p_preferencesWindowController showWindow:self];
    }
                          alertViewController:self.p_alertViewController];
}

/*!
 * Open the About window.
 * \param sender Who is sending the action.
 */
- (IBAction)p_openAbout:(id)sender
{
    [NxAException runBlockAndExitOnExceptions:^() {
        NxAAssertNotNil(self.p_aboutWindow);
        [self.p_aboutWindow makeKeyAndOrderFront:self];
    }
                          alertViewController:self.p_alertViewController];
}

/*!
 * Method called when the UI object context has been modified.
 * \param notification Message data for this notification.
 */
- (void)p_uiContextModified:(NSNotification *)notification
{
    [NxAException runBlockAndExitOnExceptions:^() {
        NSDictionary *userInfo = notification.userInfo;

        // -- We grab the list of everything that was modified.
        NSMutableSet *updates = [NSMutableSet set];
        [updates unionSet:[userInfo objectForKey:@"updated"]];
        [updates unionSet:[userInfo objectForKey:@"inserted"]];

        for (id updatedObject in updates) {
            if ([updatedObject isKindOfClass:[NxATaggedObject class]]) {
                // -- And for all the tagged objects, we reset the caches.
                NxATaggedObject *taggedObject = (NxATaggedObject *)updatedObject;
                [taggedObject resetCaches];
            }
        }
    }
                          alertViewController:self.p_alertViewController];
}

- (void)p_processSyncingResultsFromOperations:(NSArray *)operations
                             whichUsedContext:(NSManagedObjectContext *)context
                                       andLog:(NxAMessageLog *)log
{
    if (log.hasAnError) {
        // -- If we had errors, we ignore any changes made to the local context.
        [self.p_alertViewController showLog:log withDescription:@"Syncing caused an error. Changes will be ignored."];
    }
    else {
        NSMutableArray *namesOfCollectionsWithChanges = [NSMutableArray array];
        for (NxACollectionOperation *operation in operations) {
            if (![operation isKindOfClass:[NxAParsingOperation class]]) {
                continue;
            }

            NxAParsingOperation *parsingOperation = (NxAParsingOperation *)operation;
            if (parsingOperation.hasChangesToSave) {
                [namesOfCollectionsWithChanges addObject:[parsingOperation.implementation.parserClass nameOfTheApplication]];
            }
        }

        if (context.hasChanges) {
            [namesOfCollectionsWithChanges addObject:@"Rekord Buddy"];
        }

        NSMutableString *applicationsWithChangesList = [NSMutableString string];
        if (namesOfCollectionsWithChanges.count || log.hasWarnings) {
            for (NSInteger index = 0; index < namesOfCollectionsWithChanges.count; ++index) {
                if ((index > 0) && (index < (namesOfCollectionsWithChanges.count - 1))) {
                    [applicationsWithChangesList appendString:@", "];
                }
                else if (index) {
                    [applicationsWithChangesList appendString:@" and "];
                }

                [applicationsWithChangesList appendString:namesOfCollectionsWithChanges[index]];
            }

            NSAlert *alert = nil;

            void(^handler)(NSModalResponse) = ^void(NSModalResponse response){
                if (alert) {
                    [alert.window orderOut:self];
                }

                if (response != NSModalResponseOK) {
                    return;
                }

                for (NxACollectionOperation *operation in operations) {
                    if (![operation isKindOfClass:[NxAParsingOperation class]]) {
                        continue;
                    }

                    NxAParsingOperation *parsingOperation = (NxAParsingOperation *)operation;
                    if (parsingOperation.hasChangesToSave) {
                        [parsingOperation saveCollection];
                    }
                }

#if !NXA_TRADESHOW_DEMO_MODE
                if (context.hasChanges) {
                    NSError *error = nil;
                    NSManagedObjectContext *contextToSave = context;

                    do {
                        if (![contextToSave save:&error]) {
                            NSError *errorProduced = [NSError errorWithInternalCause:error.expandedCoreDataValidationError.localizedDescription];
                            NSAlert *saveAlert = [NSAlert alertWithError:errorProduced];
                            [saveAlert beginSheetModalForWindow:self.p_window completionHandler:nil];
                        }

                        contextToSave = contextToSave.parentContext;
                    }
                    while (contextToSave);
                }
#endif
                // -- Refresh the items in the crate list view.
                self.p_crateListViewController.items = self.p_crateListItems;
                [self.p_crateListViewController refresh];
            };

            NSString *message = nil;
            if (applicationsWithChangesList.length) {
                self.p_alertViewController.okButtonLabel = @"Save Changes";
                self.p_alertViewController.cancelButtonLabel = @"Cancel Changes";
                
                if (log.hasWarnings) {
                    message = [NSString stringWithFormat:@"Syncing caused changes in %@ and some warnings.",
                               applicationsWithChangesList];
                }
                else {
                    message = [NSString stringWithFormat:@"Syncing caused changes in %@.",
                                            applicationsWithChangesList];
                }
            }
            else if (log.hasWarnings) {
                self.p_alertViewController.okButtonLabel = @"Ok";
                self.p_alertViewController.cancelButtonLabel = @"";
                
                message = [NSString stringWithFormat:@"Syncing caused some warnings but no changes."];
            }

            if (message) {
                [self.p_alertViewController showLog:log
                                    withDescription:message
                                  andWhenFinishedDo:handler];
            }
        }
    }
}

- (void)p_syncAndForceUpdateToParser:(id<NxAParserPreferencesViewController> )parserControllerToForceUpdateWith
{
    [NxAException runBlockAndExitOnExceptions:^() {
        // -- Make sure we save any changes made by the UI.
        [self p_saveUIChanges];

        NxAMessageLog *log = [[NxAMessageLog alloc] init];

        NxAProgressViewController *progress = self.p_progressViewController;
        NxAAssertNotNil(progress);
        progress.status = @"";
        progress.min = 0;
        progress.max = 1;
        progress.value = 0;

        NSMutableArray *operations = [NSMutableArray array];

        // -- The first operation will be launched regardless so we start at offset 1.
        __block NSUInteger currentOperationIndex = 1;

        // -- We make a seperate context to load our stuff into so that we don't disrupt the main thread.
        NSManagedObjectContext *threadContext = [[NSManagedObjectContext alloc] initWithConcurrencyType:NSPrivateQueueConcurrencyType];

        // -- All out threads will work from our ui context.
        threadContext.parentContext = self.p_uiManagedObjectContext;

        NxACollectionOperationCompletionHandler completionHandler = ^(BOOL returnCode) {
            if ((currentOperationIndex == operations.count) || !returnCode) {
                [progress hide];

                [NSApp stopModal];

                [self p_processSyncingResultsFromOperations:operations whichUsedContext:threadContext andLog:log];
            }
            else {
                if (currentOperationIndex >= 2) {
                    NxACollectionOperation *lastOperation = operations[currentOperationIndex - 1];

                    if ([lastOperation isKindOfClass:[NxAParsingOperation class]]) {
                        NxAParsingOperation *currentParsingOperation = (NxAParsingOperation *)lastOperation;

                        if (currentParsingOperation.hasUpdatedInternalDatabase) {
                            // -- If some objects were updated and we have previous parsers, we need to
                            // -- update those parsers to make sure they get those updates too.
                            NSInteger index = currentOperationIndex - 2;

                            while (index >= 0) {
                                NxACollectionOperation *previousOperation = operations[index];

                                if ([previousOperation isKindOfClass:[NxAParsingOperation class]]) {
                                    NxAParsingOperation *previousParsingOperation = (NxAParsingOperation *)previousOperation;

#if DEBUG
                                    [log addMessage:[NSString stringWithFormat:@"%@ is forcing %@ to update objects.",
                                                     [currentParsingOperation.implementation.parserClass nameOfTheApplication],
                                                     [previousParsingOperation.implementation.parserClass nameOfTheApplication]]];
#endif

                                    [previousParsingOperation needsToUpdateSourceOfObjectsUpdatedIn:currentParsingOperation];
                                }
                                
                                --index;
                            }
                        }
                    }
                }

                [NSApplication runBlockOnMainThread:^() {
                    [self.p_operationQueue addOperation:operations[currentOperationIndex]];
                    ++currentOperationIndex;
                }];
            }
        };

        for (id<NxAPreferencesViewController> controller in self.p_preferencesViewControllers) {
            if (![[controller class] conformsToProtocol:@protocol(NxAParserPreferencesViewController)]) {
                continue;
            }

            id<NxAParserPreferencesViewController> parserPreferencesController = (id<NxAParserPreferencesViewController>)controller;
            if (!parserPreferencesController.collectionIsAvailable) {
                continue;
            }

            id<NxAParserImplementation> parser = [parserPreferencesController parserImplementationUsingContext:threadContext
                                                                                             loggingMessagesIn:log];

            NxAParsingOperation *parsingOperation = [NxAParsingOperation operationWithContext:threadContext
                                                                          usingImplementation:parser
                                                                          showingProgressWith:progress
                                                                            loggingMessagesIn:log];

            if (parserControllerToForceUpdateWith && (parserControllerToForceUpdateWith != parserPreferencesController)) {
                parsingOperation.forceParserToBeUpdatedByEverything = YES;
            }
            
            parsingOperation.completionHandler = completionHandler;

            [operations addObject:parsingOperation];
        }

        if (!operations.count) {
            NSAlert *alert = [NSAlert alertWithMessageText:@"Can't find any track collections to sync with."
                                             defaultButton:@"Ok"
                                           alternateButton:nil
                                               otherButton:nil
                                 informativeTextWithFormat:@"Syncing has been disabled."];

            [alert beginSheetModalForWindow:self.p_window completionHandler:^(NSModalResponse response) {
                [alert.window orderOut:self];
            }];

            return;
        }

        // -- If we have any parsers setup, we can also setup parsing for ID3 tags and syncing.
        NxAID3Parser *id3Parser = [NxAID3Parser operationWithContext:threadContext
                                                            progress:progress
                                                                 log:log];
        id3Parser.completionHandler = completionHandler;
        if (parserControllerToForceUpdateWith) {
            id3Parser.forceParserToBeUpdatedByEverything = YES;
        }
        [operations addObject:id3Parser];

        NxASyncingOperation *syncingOperation = [NxASyncingOperation operationWithContext:threadContext
                                                                        parsingOperations:operations
                                                                                 progress:progress
                                                                                      log:log];
        syncingOperation.completionHandler = completionHandler;
        [operations addObject:syncingOperation];
        
        [self.p_operationQueue addOperation:operations.firstObject];
        
        [progress show];
        
        [NSApp runModalForWindow:self.p_window];
    }
                          alertViewController:nil];
}

/*!
 * Action triggered when the sync button is pressed.
 * \param sender Object which triggered this action.
 */
- (IBAction)p_sync:(id)sender
{
#if NXA_TRADESHOW_DEMO_MODE
    return;
#else
    [NxAException runBlockAndExitOnExceptions:^() {
        if (![NxACrate canFindARootCrateInContext:self.p_masterManagedObjectContext]) {
            [self p_offerToInitializeTheDatabase];
        }
        else {
            [self p_syncAndForceUpdateToParser:nil];
        }
    }
                          alertViewController:nil];
#endif
}

/*!
 * Action triggered when the user wants to change the track list view.
 * \param sender Object which triggered this action.
 */
- (IBAction)p_viewSelectionChanged:(id)sender {
    [NxAException runBlockAndExitOnExceptions:^() {
        NSSegmentedControl *viewSelector = sender;

        if (viewSelector.selectedSegment) {
            [self.p_trackListViewController useArtworkTrackView];
        }
        else {
            [self.p_trackListViewController useTrackView];
        }
    }
                             alertViewController:self.p_alertViewController];
}

/*!
 * Save action for the application
 * \param sender Object which triggered this action.
 */
- (IBAction)saveAction:(id)sender
{
    [NxAException runBlockAndExitOnExceptions:^() {
        // -- Make sure we save any changes made by the UI.
        [self p_saveUIChanges];

#if !NXA_TRADESHOW_DEMO_MODE
        NSManagedObjectContext *masterContext = self.p_masterManagedObjectContext;
        if (masterContext.hasChanges) {
            // -- Then if our master context has changes, we save them to disk.
            [masterContext performBlockAndWait:^{
                NSError *error = nil;
                if (![masterContext save:&error]) {
                    [[NSApplication sharedApplication] presentError:error];
                }
            }];
        }
#endif
    }
                             alertViewController:self.p_alertViewController];
}

/*!
 * Action triggered when Check For Update menu item is selected.
 * \param sender Object which triggered this action.
 */
- (IBAction)p_checkForUpdates:(id)sender
{
    [self.p_sparkleUpdater checkForUpdates:self];
}

- (void)displayAlert:(NSAlert *)alert
{
    NxAAssertNotNil(alert);
    
    [NxAException runBlockOnMainThreadAndExitOnExceptions:^() {
        [alert runModal];
        [alert.window orderOut:self];
    }
                                      alertViewController:self.p_alertViewController];
}

- (void)displayAlert:(NSAlert *)alert
 onceCompletedThenRun:(void (^)(NSModalResponse returnCode))handler
{
    NxAAssertNotNil(alert);
    NxAAssertNotNil(handler);

    [NxAException runBlockOnMainThreadAndExitOnExceptions:^() {
        handler([alert runModal]);
        [alert.window orderOut:self];
    }
                                      alertViewController:self.p_alertViewController];
}

#pragma mark Copy Protection Callback method

/*!
 * This gets called when the copy protection or beta expiration is successfully tested against.
 */
- (BOOL)NXA_APP_PROTECTION_DELEGATE_METHOD_NAME
{
    [NxAException runBlockAndExitOnExceptions:^() {
#if DEBUG
        BOOL debugConstraints = YES;
#else
        BOOL debugConstraints = NO;
#endif
        [[NSUserDefaults standardUserDefaults] setBool:debugConstraints forKey:@"NSConstraintBasedLayoutVisualizeMutuallyExclusiveConstraints"];

        // -- Setup the default user preferences.
        [[NSUserDefaults standardUserDefaults] registerDefaults:[NxAAppDelegate p_defaultPreferenceValues]];

        // -- Load the main window nib.
        NSArray *toplevelObjects = nil;
        if (![[NSBundle mainBundle] loadNibNamed:@"MainWindow" owner:self topLevelObjects:&toplevelObjects]) {
            return;
        }

        NxAAssertNotNil(toplevelObjects);
        self.p_mainWindowTopLevelObjects = toplevelObjects;
        toplevelObjects = nil;

        // -- Load the about window nib.
        if (![[NSBundle mainBundle] loadNibNamed:@"AboutWindow" owner:self topLevelObjects:&toplevelObjects]) {
            return;
        }

        NxAAssertNotNil(toplevelObjects);
        self.p_aboutWindowTopLevelObjects = toplevelObjects;
        toplevelObjects = nil;

        // -- Load the collection init nib.
        if (![[NSBundle mainBundle] loadNibNamed:@"CollectionInitWindow" owner:self topLevelObjects:&toplevelObjects]) {
            return;
        }

        NxAAssertNotNil(toplevelObjects);
        self.p_collectionInitWindowTopLevelObjects = toplevelObjects;
        toplevelObjects = nil;

        self.p_syncButtonCell.clickedImage = [NSImage imageNamed:@"Refresh On"];
        self.p_syncButtonCell.backgroundColor = [NSColor colorWithRed:56.0/255.0 green:56.0/255.0 blue:56.0/255.0 alpha:1.0];

#if BETA_BUILD
        // -- Load the beta warning nib.
        if (![[NSBundle mainBundle] loadNibNamed:@"BetaWarningWindow" owner:self topLevelObjects:&toplevelObjects]) {
            return;
        }

        NxAAssertNotNil(toplevelObjects);
        self.p_betaWarningWindowTopLevelObjects = toplevelObjects;
        toplevelObjects = nil;
#endif

        // -- Set up the track list view.
        NxAAssertNotNil(self.p_trackListViewController);
        [self.p_trackListViewController useArtworkTrackView];
        [self.p_trackViewSelector selectSegmentWithTag:1];

        // -- Set up the crate list view.
        NxAAssertNotNil(self.p_crateListViewController);
        self.p_crateListViewController.items = self.p_crateListItems;

        [self p_setupPreferencesWindowController];

        NSManagedObjectContext *context = self.p_uiManagedObjectContext;
        if (!context) {
            // -- If we don't have a valid context then we have a problem.
            return;
        }

        // -- We want to make sure we're told of any modifications by child contexts. This is because we might
        // -- have to update some object caches after that.
        NSNotificationCenter *center = [NSNotificationCenter defaultCenter];
        [center addObserver:self
                   selector:@selector(p_uiContextModified:)
                       name:NSManagedObjectContextObjectsDidChangeNotification
                     object:self.p_uiManagedObjectContext];

        [NSThread sleepForTimeInterval:3.0f];

        NxAAssertNotNil(self.p_splashWindow);
        [self.p_splashWindow close];

        NxAAssertNotNil(self.p_window);
        [self.p_window setBackgroundColor:[NSColor colorWithRed:56.0/255.0 green:56.0/255.0 blue:56.0/255.0 alpha:1.0]];
        [self.p_window makeKeyAndOrderFront:self];

#if MAC_OS_X_VERSION_MAX_ALLOWED > MAC_OS_X_VERSION_10_9
        // -- If we hide the window's title, we need to restore its size as Cocoa currently does not
        // -- restore it correctly in this case.
        [self.p_window setTitleVisibility:NSWindowTitleHidden];

        NSString *windowFrameString = [[NSUserDefaults standardUserDefaults] stringForKey:@"WindowFrameKey"];

        if(windowFrameString != nil) {
            NSRect savedRect = NSRectFromString(windowFrameString);

            if(!NSEqualRects(self.p_window.frame, savedRect)) {
                [self.p_window setFrame:savedRect display:YES animate:NO];
            }
        }
#endif

#if BETA_BUILD
        [self p_showBetaWarningWindow];
#else
        if (![NxACrate canFindARootCrateInContext:self.p_masterManagedObjectContext]) {
            [self p_offerToInitializeTheDatabase];
        }
#endif
    }
                          alertViewController:self.p_alertViewController];

    return NO;
}
                  
#pragma mark NSApplicationDelegate Protocol

/*!
 * Delegate method called when the application has finished launching.
 * \param notification The notification message from the NSApplication object.
 */
- (void)applicationDidFinishLaunching:(NSNotification *)notification
{
    [NxAException runBlockAndExitOnExceptions:^() {
#if !DEBUG
#if BETA_BUILD
        [[BITHockeyManager sharedHockeyManager] configureWithIdentifier:@"1eb42852f0320951d95537a1be92b05e"];
#else
        [[BITHockeyManager sharedHockeyManager] configureWithIdentifier:@"9a16d7d4775dd419a629e39e3bf8c63d"];
#endif

        [[BITHockeyManager sharedHockeyManager] startManager];

        self.p_sparkleUpdater = [[SUUpdater alloc] init];
        NxAAssertNotNil(self.p_sparkleUpdater);
#endif

        if (([NSEvent modifierFlags] & NSAlternateKeyMask) == NSAlternateKeyMask) {
            /*! If the command key is down, offer to delete the current database. */
            NSAlert *alert = [NSAlert alertWithMessageText:@"Do you really want to reset the database?."
                                             defaultButton:@"Yes"
                                           alternateButton:@"No"
                                               otherButton:nil
                                 informativeTextWithFormat:@"You will loose all your current information."];

            if ([alert runModal] == NSAlertDefaultReturn) {
                [self p_resetDatabase];
            }

            [alert.window orderOut:self];
        }

        // -- Load the splash window nib.
        NSArray *toplevelObjects = nil;
        if (![[NSBundle mainBundle] loadNibNamed:@"SplashWindow" owner:self topLevelObjects:&toplevelObjects]) {
            return;
        }

        NxAAssertNotNil(toplevelObjects);
        self.p_splashWindowTopLevelObjects = toplevelObjects;

        // -- Make sure the splash window shows up
        NxAAssertNotNil(self.p_splashWindow);
        [self.p_splashWindow makeKeyAndOrderFront:self];

#if BETA_BUILD && !DEBUG
        [self p_checkForBetaExpiration];
#else
        [self NXA_APP_PROTECTION_DELEGATE_METHOD_NAME];
#endif
    }
                             alertViewController:self.p_alertViewController];
}

/*!
 * Delegate method called to determine if closing the last window should cause the application to terminate.
 * \param sender The application making the request.
 * \return YES if the application should quit, NO otherwise.
 */
- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender
{
    return YES;
}

/*!
 * Delegate method called when the application wants to terminate.
 * \param sender The object who sent this message.
 * \return A reply telling the application if it's ok to terminate.
 */
- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender
{
    __block NSApplicationTerminateReply reply = NSTerminateCancel;

    [NxAException runBlockAndExitOnExceptions:^() {
        // -- Make sure we save any changes made by the UI.
        [self p_saveUIChanges];

        NSManagedObjectContext *masterContext = self.p_masterManagedObjectContext;
        if (!masterContext) {
            // -- If we don't even have an object context, we can just exit now.
            reply = NSTerminateNow;
            return;
        }

        // -- We commit any editing to the context.
        if (![masterContext commitEditing]) {
            NSLog(@"%@:%@ unable to commit editing to terminate", self.class, NSStringFromSelector(_cmd));
            reply = NSTerminateCancel;
            return;
        }

#if !NXA_TRADESHOW_DEMO_MODE
        if (!masterContext.hasChanges) {
            // -- If we don't have any changes, we can exit.
            reply = NSTerminateNow;
            return;
        }

        // -- If we do have changes, we save them.
        NSError *error = nil;
        if (![masterContext save:&error]) {
            BOOL result = [sender presentError:error];
            if (result) {
                reply = NSTerminateCancel;
                return;
            }

            NSString *question = NSLocalizedString(@"Could not save changes while quitting. Quit anyway?", @"Quit without saves error question message");
            NSString *info = NSLocalizedString(@"Quitting now will lose any changes you have made since the last successful save", @"Quit without saves error question info");
            NSString *quitButton = NSLocalizedString(@"Quit anyway", @"Quit anyway button title");
            NSString *cancelButton = NSLocalizedString(@"Cancel", @"Cancel button title");
            NSAlert *alert = [[NSAlert alloc] init];

            alert.messageText = question;
            alert.informativeText = info;

            [alert addButtonWithTitle:quitButton];
            [alert addButtonWithTitle:cancelButton];
            
            NSInteger answer = [alert runModal];
            
            if (answer == NSAlertAlternateReturn) {
                reply = NSTerminateCancel;
                return;
            }
        }
#endif

        // -- Every seems to have gone fine, we can terminate now.
        reply = NSTerminateNow;

    }
                             alertViewController:self.p_alertViewController];

    return reply;
}

- (void)applicationWillTerminate:(NSNotification *)notification
{
#if MAC_OS_X_VERSION_MAX_ALLOWED > MAC_OS_X_VERSION_10_9
    // -- If we hide the window's title, we need to save the window's size as Cocoa currently does not
    // -- restore it correctly in this case.
    [[NSUserDefaults standardUserDefaults] setObject:NSStringFromRect(self.p_window.frame) forKey:@"WindowFrameKey"];
#endif
}

#pragma mark NSWindowDelegate Protocol

- (NSUndoManager *)windowWillReturnUndoManager:(NSWindow *)window
{
    return self.p_uiManagedObjectContext.undoManager;
}

#pragma mark NxAPreferencesViewControllerDelegate Protocol

- (NSWindow *)parentWindow
{
    __block NSWindow *window = nil;

    [NxAException runBlockAndExitOnExceptions:^() {
        window = self.p_preferencesWindowController.window;
    }
                          alertViewController:self.p_alertViewController];

    return window;
}

#pragma mark NxATrackListViewControllerDelegate protocol

- (void)trackWasEdited
{
    [NxAException runBlockAndExitOnExceptions:^() {
        // -- Editing might have involved new tags, keys or artists so we update these.
        [self.p_crateListViewController refresh];
    }
                          alertViewController:self.p_alertViewController];
}

#pragma mark NxACrateListViewControllerDelegate protocol

- (void)selectionDidChange:(id)selection
{
    [NxAException runBlockAndExitOnExceptions:^() {
        if (selection) {
            if ([selection isKindOfClass:[NxACrate class]]) {
                NxACrate *crate = (NxACrate *)selection;
                [self.p_trackListViewController displayCrate:crate];
            }
            else {
                if ([selection respondsToSelector:@selector(trackListViewItems)]) {
                    id <NxACrateListViewItemSource> object = selection;
                    [self.p_trackListViewController displayItems:object.trackListViewItems];
                }
                else {
                    [self.p_trackListViewController displayItems:@[ ]];
                }
            }
        }
        else {
            [self.p_trackListViewController displayItems:@[ ]];
        }
    }
                          alertViewController:self.p_alertViewController];
}

#pragma mark NxATrackListViewControllerDelegate & NxACrateListViewControllerDelegate protocols

- (NSManagedObjectContext *)objectContext
{
    return self.p_uiManagedObjectContext;
}

@end
