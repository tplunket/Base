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

#import "NxATrack.h"
#import "Track Collection/Tags/Crate Properties/NxATrackProperty.h"
#import "NxATrackListView.h"
#import "NxATrackListCellView.h"
#import "NxATrackListRowView.h"
#import "NxATrackListViewItem.h"
#import "NxATrackEditViewController.h"
#import "NxACrate+NxACrateListViewItemSource.h"
#import "NSApplication+Utility.h"
#import "NxATrackListViewController.h"

#pragma mark Constants

CGFloat const NxATrackListViewItemHeight = 22.0;
CGFloat const NxATrackListViewGroupItemHeight = 22.0;
CGFloat const NxAArtworkTrackListViewItemHeight = 64.0;
CGFloat const NxAArtworkTrackListViewGroupItemHeight = 22.0;

#pragma mark - Private Interface

@interface NxATrackListViewController ()

#pragma mark Properties

/*! Items to display in the track list. */
@property (strong, nonatomic) NSArray *p_content;

/*! Crate to display in the track list. */
@property (weak, nonatomic) NxACrate *p_crateToDisplay;

/*! Main view of the regular track list view. */
@property (strong, nonatomic) IBOutlet NSView *p_trackListTopView;

/*! Artwork version of the main view of the new track list view. */
@property (strong, nonatomic) IBOutlet NSView *p_artworkTrackListTopView;

/*! Table view used to display the tracks. */
@property (weak, nonatomic) IBOutlet NxATrackListView *p_trackListView;

/*! Artwork version of table view used to display the tracks. */
@property (weak, nonatomic) IBOutlet NxATrackListView *p_artworkTrackListView;

/*! Controller for the track editing dialog. */
@property (strong, nonatomic) IBOutlet NxATrackEditViewController *p_trackEditController;

/*! Currently selected track list view item, if any. */
@property (readonly, nonatomic) id <NxATrackListViewItem> p_selectedItem;

@end

#pragma mark Implementation

@implementation NxATrackListViewController

#pragma mark Accessors

- (id <NxATrackListViewItem>)p_selectedItem
{
    NSOutlineView *trackListView = (NSOutlineView *)self.view;

    id <NxATrackListViewItem> item = [trackListView itemAtRow:trackListView.selectedRow];
    if (item.trackListViewItemIsGroupItem) {
        return nil;
    }

    return item;
}

#pragma mark Instance Methods

/*!
 * Deal with the user double-clicking on a track in the list.
 * \param sender Who sent this message.
 */
- (void)p_rowDoubleClicked:(id)sender
{
    if([self.delegate respondsToSelector:@selector(trackDoubleClicked:)]) {
        id <NxATrackListViewItem> selectedItem = self.p_selectedItem;

        if (!selectedItem) {
            return;
        }

        if ([selectedItem isKindOfClass:[NxATrack class]]) {
            [self.delegate trackDoubleClicked:(NxATrack *)selectedItem];
        }
    }
}

- (void)p_expandAllGroupItems
{
    NSOutlineView *trackListView = (NSOutlineView *)self.view;

    // -- Expand all the group items.
    for (NSInteger row = 0; row < trackListView.numberOfRows; ++row) {
        id <NxATrackListViewItem> item = [trackListView itemAtRow:row];

        if (item.trackListViewItemIsGroupItem) {
            [trackListView expandItem:item];
        }
    }
}

/*!
 * Set the view displayed by the controller with hugging constraints.
 * \param view The view to use as a child view.
 */
- (void)p_setView:(NSView *)view
{
    NxAAssertNotNil(view);

    NSView *parentView = self.parentView;
    NxAAssertNotNil(parentView);
    
    // -- First remove all current constraints from the parent view.
    [parentView removeConstraints:parentView.constraints];

    if (parentView.subviews.count) {
        NxAAssertTrue(parentView.subviews.count == 1);
        NSView *existingSubView = parentView.subviews[0];

        // -- If the parent has a subview, we swap it for the new one.
        [parentView replaceSubview:existingSubView with:view];
    }
    else {
        // -- Otherwise we add the new one to the view.
        [parentView addSubview:view];
    }

    // -- And set up constraints to hug the side of the superview.
    NSArray *trackListConstraints = @[ [NSLayoutConstraint constraintWithItem:parentView
                                                                    attribute:NSLayoutAttributeLeft
                                                                    relatedBy:NSLayoutRelationEqual
                                                                       toItem:view
                                                                    attribute:NSLayoutAttributeLeft
                                                                   multiplier:1.0
                                                                     constant:0.0],
                                       [NSLayoutConstraint constraintWithItem:parentView
                                                                    attribute:NSLayoutAttributeRight
                                                                    relatedBy:NSLayoutRelationEqual
                                                                       toItem:view
                                                                    attribute:NSLayoutAttributeRight
                                                                   multiplier:1.0
                                                                     constant:0.0],
                                       [NSLayoutConstraint constraintWithItem:parentView
                                                                    attribute:NSLayoutAttributeTop
                                                                    relatedBy:NSLayoutRelationEqual
                                                                       toItem:view
                                                                    attribute:NSLayoutAttributeTop
                                                                   multiplier:1.0
                                                                     constant:0.0],
                                       [NSLayoutConstraint constraintWithItem:parentView
                                                                    attribute:NSLayoutAttributeBottom
                                                                    relatedBy:NSLayoutRelationEqual
                                                                       toItem:view
                                                                    attribute:NSLayoutAttributeBottom
                                                                   multiplier:1.0
                                                                     constant:0.0]
                                       ];
    [parentView addConstraints:trackListConstraints];
}

- (void)useTrackView
{
    // -- This is more than just testing for the current view.
    // -- Accessing 'view' forces the controller to load the nib too.
    if (self.view == self.p_trackListView) {
        return;
    }
    
    // -- We want to make sure UI stuff runs on the main thread.
    [NSApplication runBlockOnMainThreadAndWaitUntilDone:^() {
        [self p_setView:self.p_trackListTopView];

        NSOutlineView *trackListView = self.p_trackListView;
        self.view = trackListView;

        [trackListView reloadData];

        [self p_expandAllGroupItems];
    }];
}

- (void)useArtworkTrackView
{
    // -- This is more than just testing for the current view.
    // -- Accessing 'view' forces the controller to load the nib too.
    if (self.view == self.p_artworkTrackListView) {
        return;
    }

    // -- We want to make sure UI stuff runs on the main thread.
    [NSApplication runBlockOnMainThreadAndWaitUntilDone:^() {
        [self p_setView:self.p_artworkTrackListTopView];

        NSOutlineView *trackListView = self.p_artworkTrackListView;
        self.view = trackListView;

        [trackListView reloadData];

        [self p_expandAllGroupItems];
    }];
}

- (void)displayCrate:(NxACrate *)crate
{
    // -- We want to make sure UI stuff runs on the main thread.
    [NSApplication runBlockOnMainThreadAndWaitUntilDone:^() {
        NSOutlineView *trackListView = (NSOutlineView *)self.view;

        self.p_crateToDisplay = crate;
        self.p_content = crate.trackListViewItems;

        [trackListView reloadData];

        [self p_expandAllGroupItems];
    }];
}

- (void)displayItems:(NSArray *)items
{
    // -- We want to make sure UI stuff runs on the main thread.
    [NSApplication runBlockOnMainThreadAndWaitUntilDone:^() {
        NSOutlineView *trackListView = (NSOutlineView *)self.view;

        self.p_crateToDisplay = nil;
        self.p_content = items;

        [trackListView reloadData];

        [self p_expandAllGroupItems];
    }];
}

#pragma mark Overridden NSViewController Methods

- (void)loadView
{
    [super loadView];

    NxATrackListView *trackListView = self.p_trackListView;

    // -- Set up a menu to allow the user to show/hide columns from the tracks table view
    NSDictionary *bindingOptions = @{ @"NSValueTransformerName" : NSNegateBooleanTransformerName };
    NSMenu *columnsMenu = [[NSMenu alloc] initWithTitle:@""];
    for (NSTableColumn *column in trackListView.tableColumns) {
        NSMenuItem *menuItem = [[NSMenuItem alloc] initWithTitle:[column.headerCell stringValue]
                                                          action:nil
                                                   keyEquivalent:@""];
        menuItem.target = self;
        menuItem.representedObject = column;

        [menuItem bind:@"value"
              toObject:column
           withKeyPath:@"hidden"
               options:bindingOptions];

        [columnsMenu addItem:menuItem];
    }

    [trackListView.headerView setMenu:columnsMenu];
    [trackListView registerForDraggedTypes:@[ (NSString *)kUTTypeURL ]];
    [trackListView setDraggingSourceOperationMask:NSDragOperationCopy
                                         forLocal:NO];
    [trackListView setTarget:self];
    [trackListView setDoubleAction:@selector(p_rowDoubleClicked:)];
    trackListView.emptyRowHeight = NxATrackListViewItemHeight;

    NxATrackListView *artworkTrackListView = self.p_artworkTrackListView;

    [artworkTrackListView registerForDraggedTypes:@[ (NSString *)kUTTypeURL ]];
    [artworkTrackListView setDraggingSourceOperationMask:NSDragOperationCopy
                                                forLocal:NO];
    [artworkTrackListView setTarget:self];
    [artworkTrackListView setDoubleAction:@selector(p_rowDoubleClicked:)];
    artworkTrackListView.emptyRowHeight = NxAArtworkTrackListViewItemHeight;
}

#pragma mark NxATrackEditViewControllerDelegate protocol

- (NSManagedObjectContext *)objectContext
{
    return self.delegate.objectContext;
}

- (void)trackEditDialogWillClose
{
    if([self.delegate respondsToSelector:@selector(trackWasEdited)]) {
        [self.delegate trackWasEdited];
    }
}

#pragma mark NxATrackTableViewDelegate protocol

- (NSMenu *)mouseRightClickedWithSelection
{
    id <NxATrackListViewItem> selectedItem = self.p_selectedItem;
    if (!selectedItem) {
        return nil;
    }

    NSOutlineView *trackListView = (NSOutlineView *)self.view;
    NSView *rowView = [trackListView rowViewAtRow:trackListView.selectedRow makeIfNecessary:NO];
    if (!rowView) {
        return nil;
    }

    [self.p_trackEditController displayItem:selectedItem
                         showRelativeToRect:rowView.bounds
                                     ofView:rowView
                              preferredEdge:NSMaxYEdge];

    return nil;
}

- (void)deleteKeyPressedWithSelection
{
    NxACrate *crateBeingDisplayed = self.p_crateToDisplay;

    if (!crateBeingDisplayed || !crateBeingDisplayed.isAPlaylist ||
        crateBeingDisplayed.isASmartPlaylist || crateBeingDisplayed.hasSortingCriterias) {
        // -- If we're not displaying a real crate then we can't delete tracks in this view.
        return;
    }

    NSOutlineView *trackListView = (NSOutlineView *)self.view;
    NSIndexSet *selectedRows = trackListView.selectedRowIndexes;
    BOOL crateIsFromCollection = crateBeingDisplayed.isASystemCrate;

    // -- Make sure the user really wants to do this.
    NSString *whatToDeleteMsg = (selectedRows.count > 1) ? [NSString stringWithFormat:@"%ld tracks", selectedRows.count] :
                                                           @"this track";
    NSString *whereToDeleteFromMsg = crateIsFromCollection ? @"the entire collection" :
                                                        [NSString stringWithFormat:@"the '%@' crate", crateBeingDisplayed.description];
    NSString *alertMsg = [NSString stringWithFormat:@"Are you sure you want to delete %@ from %@?",
                          whatToDeleteMsg,
                          whereToDeleteFromMsg];

    NSAlert *alert = [NSAlert alertWithMessageText:alertMsg
                                     defaultButton:@"Ok"
                                   alternateButton:@"Cancel"
                                       otherButton:nil
                         informativeTextWithFormat:@"This cannot be undone."];

    NxAAssertNotNil(self.mainWindow);
    [alert beginSheetModalForWindow:self.mainWindow
                  completionHandler:^(NSInteger returnCode)
     {
         if (returnCode != NSOKButton) {
             // -- If the user says no, just forget about it.
             return;
         }

         [selectedRows enumerateIndexesUsingBlock:^(NSUInteger rowIndex, BOOL *stop) {
             id <NxATrackListViewItem> selectedItem = self.p_selectedItem;
             NxAAssertNotNil(selectedItem);

             if ([selectedItem isKindOfClass:[NxATrackProperty class]]) {
                 NxATrackProperty *property = (NxATrackProperty *)selectedItem;

                 [crateBeingDisplayed removeTrackProperty:property];

                 // -- The only leaf system crate should be the all tracks crate,
                 // -- so in this case we will need to delete the track from the collection.
                 if (crateBeingDisplayed.isASystemCrate) {
                     [property.track destroy];
                 }
             }
         }];

         // -- This forces the crate item list to reload.
         [self displayCrate:crateBeingDisplayed];
     }];
}

- (void)upKeyPressedWithSelection
{
    NxATrackListView *trackListView = (NxATrackListView *)self.view;

    NSInteger selection = trackListView.selectedRow - 1;

    while (selection >= 0) {
        id <NxATrackListViewItem> selectedObject = [trackListView itemAtRow:selection];

        if (!selectedObject.trackListViewItemIsGroupItem) {
            [trackListView selectRowIndexes:[NSIndexSet indexSetWithIndex:selection] byExtendingSelection:NO];
            return;
        }

        --selection;
    }
}

- (void)downKeyPressedWithSelection
{
    NxATrackListView *trackListView = (NxATrackListView *)self.view;

    NSInteger selection = trackListView.selectedRow + 1;

    while (selection < trackListView.numberOfRows) {
        id <NxATrackListViewItem> selectedObject = [trackListView itemAtRow:selection];

        if (!selectedObject.trackListViewItemIsGroupItem) {
            [trackListView selectRowIndexes:[NSIndexSet indexSetWithIndex:selection] byExtendingSelection:NO];
            return;
        }

        ++selection;
    }
}

#pragma mark NSOutlineViewDataSource protocol

- (id)outlineView:(NSOutlineView *)outlineView
            child:(NSInteger)index
           ofItem:(id)item
{
    if (!item) {
        return self.p_content[index];
    }
    else {
        id <NxATrackListViewItem> object = item;
        return object.trackListViewItemChildren[index];
    }
}

- (BOOL)outlineView:(NSOutlineView *)outlineView
   isItemExpandable:(id)item
{
    id <NxATrackListViewItem> object = item;
    return object.trackListViewItemIsGroupItem;
}

- (NSInteger)outlineView:(NSOutlineView *)outlineView
  numberOfChildrenOfItem:(id)item
{
    if (!item) {
        return self.p_content.count;
    }
    else {
        id <NxATrackListViewItem> object = item;
        return object.trackListViewItemChildren.count;
    }
}

- (id)         outlineView:(NSOutlineView *)outlineView
 objectValueForTableColumn:(NSTableColumn *)tableColumn
                    byItem:(id)item
{
    return item;
}

- (BOOL)outlineView:(NSOutlineView *)outlineView
         writeItems:(NSArray *)items
       toPasteboard:(NSPasteboard *)pasteboard
{
    NSMutableArray *urlsArray = [NSMutableArray array];
    NSMutableArray *pasteboardArray = [NSMutableArray array];

    for (id item in items) {
        id <NxATrackListViewItem> selectedItem = item;

        if([selectedItem respondsToSelector:sel_registerName("trackListViewItemAddURIToArray:")]) {
            [selectedItem trackListViewItemAddURIToArray:pasteboardArray];
        }

        if([selectedItem respondsToSelector:sel_registerName("trackListViewItemAddURLToArray:")]) {
            [selectedItem trackListViewItemAddURLToArray:urlsArray];
        }
    }

    [pasteboardArray addObjectsFromArray:urlsArray];

    if (!pasteboardArray.count) {
        // -- If we have nothing to add on the pasteboard then we can stop right there.
        return NO;
    }

    [pasteboard writeObjects:pasteboardArray];

	return YES;
}

- (NSDragOperation)outlineView:(NSOutlineView *)outlineView
                  validateDrop:(id <NSDraggingInfo>)info
                  proposedItem:(id)item
            proposedChildIndex:(NSInteger)index
{
    NSManagedObjectContext *context = self.delegate.objectContext;
    if (!context) {
        // -- We don't support dropping anything if the context hasn't been set yet.
        return NSDragOperationNone;
    }

    if (!self.p_crateToDisplay) {
        return NSDragOperationNone;
    }

    // -- We don't support moving tracks in sorted crates ot moving tracks on top of tracks.
    if (index < 0 || self.p_crateToDisplay.hasSortingCriterias) {
        return NSDragOperationNone;
    }

    NSArray *tracks = [NxATrack tracksFromPasteboard:info.draggingPasteboard
                                             context:context];
    if (!tracks.count) {
        return NSDragOperationNone;
    }

    return NSDragOperationMove;
}

- (BOOL)outlineView:(NSOutlineView *)outlineView
         acceptDrop:(id <NSDraggingInfo>)info
               item:(id)item
         childIndex:(NSInteger)index
{
    NSManagedObjectContext *context = self.delegate.objectContext;
    if (!context || !self.p_crateToDisplay) {
        // -- We don't support dropping anything if the context hasn't been set yet.
        // -- Or if there isn't a current crate being displayed (i.e. if this is a smart playlist being displayed).
        return NO;
    }

    if (!([info draggingSourceOperationMask] & NSDragOperationMove)) {
        // -- For now we only support moving tracks inside crates.
        return NO;
    }

    NSArray *properties = [NxATrackProperty trackPropertiesFromPasteboard:[info draggingPasteboard]
                                                                  context:context];
    [self.p_crateToDisplay moveTrackProperties:properties
                                    toPosition:index];

    // -- This forces the crate item list to reload.
    [self displayCrate:self.p_crateToDisplay];

    return YES;
}

#pragma mark NSOutlineView delegate

- (BOOL)outlineView:(NSOutlineView *)outlineView
        isGroupItem:(id)item
{
    id <NxATrackListViewItem> object = item;
    NxAAssertTrue([object respondsToSelector:@selector(trackListViewItemIsGroupItem)]);
    return object.trackListViewItemIsGroupItem;
}

- (BOOL)         outlineView:(NSOutlineView *)outlineView
shouldShowOutlineCellForItem:(id)item
{
    id <NxATrackListViewItem> object = item;
    return object.trackListViewItemIsGroupItem;
}

- (BOOL)outlineView:(NSOutlineView*)outlineView
   shouldSelectItem:(id)item
{
    id <NxATrackListViewItem> object = item;
    return !object.trackListViewItemIsGroupItem;
}

- (NSView *)outlineView:(NSOutlineView *)outlineView
     viewForTableColumn:(NSTableColumn *)tableColumn
                   item:(id)item
{
    id <NxATrackListViewItem> object = item;
    if (object.trackListViewItemIsGroupItem) {
        // -- If it's a group item we use a HeaderCell.
        NSView *result = [outlineView makeViewWithIdentifier:object.trackListViewItemCellIdentifier owner:self];
        return result;
    }

    // -- Otherwise we use the identifier of the column which is set to the same as the view.
    NSView *result = [outlineView makeViewWithIdentifier:[tableColumn identifier] owner:self];
    if (![result isKindOfClass:[NxATrackListCellView class]]) {
        return result;
    }

    NSInteger itemRowIndex = [outlineView rowForItem:item];
    BOOL darkBackground = [NxATrackListRowView rowHasDarkBackgroundAtIndex:itemRowIndex
                                                               outlineView:outlineView];

    NxATrackListCellView *rowView = (NxATrackListCellView *)result;
    rowView.darkBackground = darkBackground;

    return result;
}

#pragma mark NSOutlineViewDelegate Protocol

- (CGFloat)outlineView:(NSOutlineView *)outlineView
     heightOfRowByItem:(id)item
{
    id <NxATrackListViewItem> object = item;

    if (self.view == self.p_trackListView) {
        if (object.trackListViewItemIsGroupItem) {
            return NxATrackListViewGroupItemHeight;
        }
        else {
            return NxATrackListViewItemHeight;
        }
    }
    else {
        if (object.trackListViewItemIsGroupItem) {
            return NxAArtworkTrackListViewGroupItemHeight;
        }
        else {
            return NxAArtworkTrackListViewItemHeight;
        }
    }
}

- (void)outlineViewSelectionIsChanging:(NSNotification *)aNotification
{
    [self.p_trackEditController close];
}

@end
