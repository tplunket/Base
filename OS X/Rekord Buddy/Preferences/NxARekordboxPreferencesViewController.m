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

#import "NxARekordboxPreferencesViewController.h"
#import "NSFileManager+DirectoryLocations.h"
#import "NSFileManager+NxAUtility.h"
#import "NSString+NxAUtility.h"
#import "Track Collection/Parsers/NxARekordboxParser.h"

#pragma mark Implementation

@implementation NxARekordboxPreferencesViewController

#pragma mark Class Methods

+ (NSURL *)p_collectionFileURL
{
    NSFileManager *fileManager = [NSFileManager defaultManager];

    NSURL *url = [NSURL fileURLWithPath:[[fileManager applicationSupportDirectory] stringByAppendingPathComponent:@"RekordBuddy.xml"]];
    NxAAssertNotNil(url);

    return url;
}

+ (NSURL *)p_urlOfCollectionWeCanImportIfAny
{
    NSFileManager *fileManager = [NSFileManager defaultManager];

    NSURL *documentsDir = [NSURL fileURLWithPath:[NSString stringWithFormat:@"/Users/%@/Documents", NSUserName()]];
    NSArray *content = [fileManager contentsOfDirectoryAtURL:documentsDir
                                  includingPropertiesForKeys:[NSArray arrayWithObject:NSURLNameKey]
                                                     options:NSDirectoryEnumerationSkipsHiddenFiles
                                                       error:nil];
    for (NSURL *foundInDocumentsDirectory in content) {
        BOOL isDirectory = NO;
        if (![fileManager fileExistsAtPath:foundInDocumentsDirectory.path isDirectory:&isDirectory]) {
            continue;
        }

        if (isDirectory) {
            continue;
        }

        if ([[foundInDocumentsDirectory.pathExtension lowercaseString] isEqualToString:@"xml"]) {
            if ([NxARekordboxParser isValidRekordboxXML:foundInDocumentsDirectory]) {
                return foundInDocumentsDirectory;
            }
        }
    }

    return nil;
}

#pragma mark Instance Methods

/*!
 * Copy the location of the Rekordbox export file to the clipboard.
 * \param sender Who sent this action.
 */
- (IBAction)p_copyExportFileToClipboard:(id)sender {
    NSURL *url = [NxARekordboxPreferencesViewController p_collectionFileURL];
    [NxARekordboxParser makeSureCollectionFileExistsAt:url];
    
    [[NSPasteboard generalPasteboard] clearContents];
    [[NSPasteboard generalPasteboard] setString:url.path forType:NSStringPboardType];
}

- (void)p_tryToMoveImportedFile:(NSURL *)importFileURL overCollectionFile:(NSURL *)collectionFileURL
{
    NSFileManager *fileManager = [NSFileManager defaultManager];
    if (![fileManager safeMoveItemAtURL:importFileURL toOverwriteURL:collectionFileURL error:nil]) {
        NSAlert *alertForMove = [NSAlert alertWithMessageText:[NSString stringWithFormat:@"Can't move file '%@' to import its changes.",
                                                               importFileURL.pathComponents.lastObject]
                                                defaultButton:@"Ok"
                                              alternateButton:nil
                                                  otherButton:nil
                                    informativeTextWithFormat:@"Any changes the file contains will be ignored."];
        [alertForMove runModal];
        [alertForMove.window orderOut:self];
    }
}

- (void)p_checkForCollectionToImportAndIfNeededOverwrite:(NSURL *)collectionFileURL
{
    NSURL *importFileURL = [NxARekordboxPreferencesViewController p_urlOfCollectionWeCanImportIfAny];
    if (!importFileURL) {
        return;
    }

    __block NSAlert *alert = [NSAlert alertWithMessageText:[NSString stringWithFormat:@"Use file '%@' to import changes from Rekordbox?.",
                                                            importFileURL.pathComponents.lastObject]
                                             defaultButton:@"Yes"
                                           alternateButton:@"Choose Other"
                                               otherButton:@"No"
                                 informativeTextWithFormat:@"This file was found in your Documents folder."];

    NSInteger result = [alert runModal];
    [alert.window orderOut:self];

    switch (result) {
        case NSAlertDefaultReturn: {
            [self p_tryToMoveImportedFile:importFileURL overCollectionFile:collectionFileURL];
           break;
        }
        case NSAlertOtherReturn: {
            break;
        }
        case NSAlertAlternateReturn: {
            while (1) {
                NSOpenPanel *openDialog = [NSOpenPanel openPanel];
                [openDialog setMessage:@"Choose a file:"];
                [openDialog setCanChooseFiles:YES];
                [openDialog setCanChooseDirectories:NO];
                [openDialog setCanCreateDirectories:NO];
                [openDialog setAllowsMultipleSelection:NO];
                [openDialog setAllowedFileTypes:@[ @"xml" ]];
                [openDialog setDirectoryURL:[NSURL fileURLWithPath:@"~/Documents" isDirectory:YES]];
                [openDialog setPrompt:@"Import changes"];

                // -- Display the dialog.
                NSInteger fileDialogResult = [openDialog runModal];
                [openDialog close];

                if (fileDialogResult == NSFileHandlingPanelOKButton) {
                    if (![NxARekordboxParser isValidRekordboxXML:openDialog.URL]) {
                        NSAlert *alertForMove = [NSAlert alertWithMessageText:@"This file is not a valid Rekordbox XML file."
                                                                defaultButton:@"Ok"
                                                              alternateButton:nil
                                                                  otherButton:nil
                                                    informativeTextWithFormat:@"Please select a valid file or press cancel."];
                        [alertForMove runModal];
                        [alertForMove.window orderOut:self];
                    }
                    else {
                        [self p_tryToMoveImportedFile:openDialog.URL overCollectionFile:collectionFileURL];
                        break;
                    }
                }
            }
        }
    }
}

- (instancetype)init
{
    self = [super initWithNibName:@"NxARekordboxPreferencesView" bundle:nil];
    NxAAssertNotNil(self);

    return self;
}

#pragma mark NxAPreferencesViewController protocol

@synthesize delegate;

- (NSString *)nameOfTheApplication
{
    return [NxARekordboxParser nameOfTheApplication];
}

- (BOOL)collectionIsAvailable
{
    NSURL *collectionFileURL = [NxARekordboxPreferencesViewController p_collectionFileURL];
    return [NxARekordboxParser isValidRekordboxXML:collectionFileURL];
}

- (id<NxAParserImplementation>)parserImplementationUsingContext:(NSManagedObjectContext *)context
                                              loggingMessagesIn:(NxAMessageLog *)log
{
    NxARekordboxParser *parser = [NxARekordboxParser implementationWithContext:context
                                                             loggingMessagesIn:log];
    NxAAssertNotNil(parser);

    NSURL *collectionFileURL = [NxARekordboxPreferencesViewController p_collectionFileURL];
    NxAAssertNotNil(collectionFileURL);
    
    [self p_checkForCollectionToImportAndIfNeededOverwrite:collectionFileURL];

    [NxARekordboxParser makeSureCollectionFileExistsAt:collectionFileURL];

    parser.collectionFileURL = collectionFileURL;

    return parser;
}

- (void)resetAll
{
    NSURL *collectionFileURL = [NxARekordboxPreferencesViewController p_collectionFileURL];

    NSFileManager *fileManager = [NSFileManager defaultManager];
    [fileManager removeItemAtPath:collectionFileURL.path error:nil];
}

#pragma mark MASPreferencesViewController Protocol

/*!
 * Delegate method to provide an identifier for this view.
 * \return A string that uniquely identifies this view in the preferences dialog.
 */
- (NSString *)identifier
{
    return @"RekordboxPreferences";
}

/*!
 * Delegate method to provide the image to use on the toolbar.
 * \return An image that can be used to represent this view on the toolbar.
 */
- (NSImage *)toolbarItemImage
{
    return [NSImage imageNamed:@"Rekordbox Logo"];
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
