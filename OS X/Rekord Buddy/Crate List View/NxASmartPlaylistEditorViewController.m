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

#import "NxASmartPlaylistEditorViewController.h"

#pragma mark Private Interface

@interface NxASmartPlaylistEditorViewController ()

#pragma mark Properties

/*! Main document window. */
@property (strong, nonatomic) NSWindow *p_mainWindow;

/*! Main document window. */
@property (strong, nonatomic) IBOutlet NSWindow *p_sheetWindow;

/*! Editor used to edit smart playlist rules. */
@property (weak) IBOutlet NSPredicateEditor *p_predicateEditor;

@end

#pragma mark - Implementation

@implementation NxASmartPlaylistEditorViewController

#pragma mark Accessors

- (void)setPredicate:(NSPredicate *)predicate
{
    // -- This is more than just telling the view to display.
    // -- Accessing 'view' forces the controller to load the nib too.
    [self.view needsDisplay];

    _predicate = predicate;
}

#pragma mark Instance Methods

/*!
 * Action triggered when the OK button is pressed in the dialog.
 * \param sender Object who sent this message.
 */
- (IBAction)p_smartPlaylistEditOk:(id)sender {
    [self.p_mainWindow endSheet:self.p_sheetWindow returnCode:NSModalResponseContinue];

    self.predicate = self.p_predicateEditor.predicate;
}

/*!
 * Action triggered when the CANCEL button is pressed in the dialog.
 * \param sender Object who sent this message.
 */
- (IBAction)p_smartPlaylistEditCancel:(id)sender {
    [self.p_mainWindow endSheet:self.p_sheetWindow returnCode:NSModalResponseStop];
}

- (void)beginAsSheetForWindow:(NSWindow *)window
            completionHandler:(void (^)(NSModalResponse returnCode))handler
{
    self.p_mainWindow = window;

    if (!self.predicate) {
        // -- If we don't have a current predicate, we initialize it with one starting rule.
        self.predicate = [NSPredicate predicateWithFormat:@"Title = ''"];
    }

    if (![self.predicate isKindOfClass:[NSCompoundPredicate class]]) {
        // -- If this predicate has only one rule, we need to add a fake 'AND' so that the editor
        // -- displays the 'ANY' or 'ALL' dialog at the top of the editor.
        NSString *newCompoundPredicate = [NSString stringWithFormat:@"%@ AND NULL == ''", self.predicate];
        self.predicate = [NSPredicate predicateWithFormat:newCompoundPredicate];
    }

    [window beginSheet:self.p_sheetWindow completionHandler:handler];
}

#pragma mark Overridden NSViewController Methods

- (void)loadView
{
    [super loadView];

    // -- Set the available rules for the user to create predicates with.
    NSArray *keyPaths = @[ [NSExpression expressionForKeyPath:@"Title"],
                           [NSExpression expressionForKeyPath:@"Artist"],
                           [NSExpression expressionForKeyPath:@"Remixer"],
                           [NSExpression expressionForKeyPath:@"Producer"],
                           [NSExpression expressionForKeyPath:@"Genre"]];
    NSArray *operators = @[ @(NSEqualToPredicateOperatorType),
                            @(NSNotEqualToPredicateOperatorType),
                            @(NSBeginsWithPredicateOperatorType),
                            @(NSEndsWithPredicateOperatorType),
                            @(NSContainsPredicateOperatorType) ];
    NSPredicateEditorRowTemplate *stringTemplates = [[NSPredicateEditorRowTemplate alloc] initWithLeftExpressions:keyPaths
                                                                                     rightExpressionAttributeType:NSStringAttributeType
                                                                                                         modifier:NSDirectPredicateModifier
                                                                                                        operators:operators
                                                                                                          options:(NSCaseInsensitivePredicateOption | NSDiacriticInsensitivePredicateOption)];
    keyPaths = @[ [NSExpression expressionForKeyPath:@"DateModified"],
                  [NSExpression expressionForKeyPath:@"DateAdded"],
                  [NSExpression expressionForKeyPath:@"DateReleased"]];
    operators = @[ @(NSLessThanPredicateOperatorType),
                   @(NSLessThanOrEqualToPredicateOperatorType),
                   @(NSGreaterThanPredicateOperatorType),
                   @(NSGreaterThanOrEqualToPredicateOperatorType),
                   @(NSEqualToPredicateOperatorType),
                   @(NSNotEqualToPredicateOperatorType)];
    NSPredicateEditorRowTemplate *dateTemplates = [[NSPredicateEditorRowTemplate alloc] initWithLeftExpressions:keyPaths
                                                                                   rightExpressionAttributeType:NSDateAttributeType
                                                                                                       modifier:NSDirectPredicateModifier
                                                                                                      operators:operators
                                                                                                        options:0];

    NSArray *compoundTypes = @[ @(NSNotPredicateType), @(NSAndPredicateType) ];
    NSPredicateEditorRowTemplate *compound = [[NSPredicateEditorRowTemplate alloc] initWithCompoundTypes:compoundTypes];

    [self.p_predicateEditor setRowTemplates:@[stringTemplates, dateTemplates, compound]];
}

@end
