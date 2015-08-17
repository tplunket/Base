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

#import "NxACrateListViewController.h"
#import "Crate List View/NxACrateListView.h"
#import "Crate List View/NxACrateListViewItem.h"
#import "NxACrateListViewItemSource.h"
#import "Track Collection/NxASortingCriteria.h"
#import "Track Collection/Tags/NxATag.h"
#import "NxATrack.h"
#import "Track Collection/Crates/NxASmartPlaylist.h"
#import "NxAFilterPlaylist.h"
#import "NxACrateClone.h"
#import "NxAOtherTracksPlaylist.h"
#import "NxASmartPlaylistEditorViewController.h"
#import "NxACrateListViewItem.h"
#import "NSApplication+Utility.h"
#import "NxATrackSortingBucket.h"

#pragma mark Private Interface

@interface NxACrateListViewController ()

#pragma mark Properties

/*! Main document window. */
@property (strong, nonatomic) IBOutlet NSWindow *p_mainWindow;

/*! Top scroll view for the crate list view. */
@property (weak, nonatomic) IBOutlet NSView *p_topScrollView;

/*! Editor for smart playlist rules. */
@property (strong) IBOutlet NxASmartPlaylistEditorViewController *p_smartPlaylistEditor;

@property (readwrite, weak, nonatomic) NxACrate *selectedCrate;

@property (readwrite, weak, nonatomic) NxATrackSortingBucket *selectedSortingBucket;

@end

#pragma mark - Implementation

@implementation NxACrateListViewController

#pragma mark Accessors

- (void)setItems:(NSArray *)items
{
    NxACrateListView *crateListView = (NxACrateListView *)self.view;

    [crateListView beginUpdates];

    _items = items;

    [self refresh];
    [self expandCrateListViewItemsAndSelectAllTracks];

    [crateListView endUpdates];
}

#pragma mark Class Methods

+ (NSString *)p_nextAvailableNameForNewCrateIn:(NxACrate *)parentCrate
{
    NSUInteger newCrateIndex = 0;
    while (1) {
        NSString *newCrateName = newCrateIndex ? [NSString stringWithFormat:@"New Crate %ld", newCrateIndex] : @"New Crate";
        if (![parentCrate hasAChildCrateNamed:newCrateName]) {
            return newCrateName;
        }

        ++newCrateIndex;
    }
}

#pragma mark Instance Methods

- (void)p_tellDelegateToRefresh
{
    self.selectedCrate = nil;
    self.selectedSortingBucket = nil;

    id selectedObject = nil;

    NxACrateListView *crateListView = (NxACrateListView *)self.view;
    NSIndexSet *selection = crateListView.selectedRowIndexes;
    if (selection.count) {
        NxAAssertTrue(selection.count == 1);

        NSUInteger index = selection.firstIndex;

        NxACrateListViewItem *selectedItem = [crateListView itemAtRow:index];
        selectedObject = selectedItem.sourceObject;
        
        if ([selectedObject isKindOfClass:[NxACrate class]]) {
            self.selectedCrate = selectedObject;
        }
        else if ([selectedObject isKindOfClass:[NxATrackSortingBucket class]]) {
            self.selectedSortingBucket = selectedObject;
        }
    }

    if([self.delegate respondsToSelector:@selector(selectionDidChange:)]) {
        [self.delegate selectionDidChange:selectedObject];
    }
}

/*!
 * Add a sort cirteria to the currently selected crate.
 * \param sender Object who sent this message.
 */
- (void)p_addSortingCriteria:(id)sender
{
    NSManagedObjectContext *context = self.delegate.objectContext;
    NxAAssertNotNil(context);

    NSMenuItem *selectedMenuItem = sender;

    NxATag *tagType = selectedMenuItem.representedObject;
    if (!tagType) {
        // -- If the user selected one of the menu's seperators, we ignore it.
        return;
    }

    NxACrate *selectedCrate = self.selectedCrate;
    if (!selectedCrate) {
        NxAAssertNotNil(self.selectedSortingBucket);
        selectedCrate = self.selectedSortingBucket.parentCrate;
    }

    NxASortingCriteria *criteria = [tagType sortingCriteriaInContext:context];
    [selectedCrate addSortingCriteria:criteria];

    [self refresh];

    [self expandItemWithSourceObject:selectedCrate expandChildren:NO];
}

/*!
 * Edit the predicate for the currently selected smart playlist.
 * \param sender Object who sent this message.
 */
- (void)p_editSmartPlaylist:(id)sender
{
    NxAAssertTrue(self.selectedCrate.isASmartPlaylist);

    NxASmartPlaylist *selectedCrate = (NxASmartPlaylist *)self.selectedCrate.originalCrate;
    self.p_smartPlaylistEditor.predicate = [NSPredicate predicateWithFormat:selectedCrate.predicate];

    [self.p_smartPlaylistEditor beginAsSheetForWindow:self.p_mainWindow completionHandler:^(NSModalResponse returnCode) {
        if (returnCode != NSModalResponseContinue) {
            // -- If the user says no, just forget about it.
            return;
        }

        selectedCrate.predicate = self.p_smartPlaylistEditor.predicate.predicateFormat;

        [self refresh];
    }];
}

- (IBAction)p_createNewCrate:(id)sender
{
    NSManagedObjectContext *context = self.delegate.objectContext;
    NxAAssertNotNil(context);

    NxACrate *parentCrate = self.selectedCrate;
    if (!parentCrate || !parentCrate.isAFolder || parentCrate.isASmartPlaylist || parentCrate.isASystemCrate) {
        // -- If we don't have anything selected or if the selection can't have children crates, use the root crate instead.
        parentCrate = [NxACrate rootCrateIn:context];
    }

    NSString *newCrateName = [NxACrateListViewController p_nextAvailableNameForNewCrateIn:parentCrate];
    NxACrate *newCrate = [NxACrate folderWithName:newCrateName
                   withEmptyContentLastModifiedOn:[NSDate date]
                                        inContext:context];

    [parentCrate addCrate:newCrate modificationDate:[NSDate date]];

    [self refresh];

    [self expandItemWithSourceObject:parentCrate expandChildren:NO];
}

- (IBAction)p_createNewSmartPlaylist:(id)sender
{
    NSManagedObjectContext *context = self.delegate.objectContext;
    NxAAssertNotNil(context);

    // -- We don't have a current predicate to edit so we just leave nil to start with.
    self.p_smartPlaylistEditor.predicate = nil;

    [self.p_smartPlaylistEditor beginAsSheetForWindow:self.p_mainWindow completionHandler:^(NSModalResponse returnCode) {
        if (returnCode != NSModalResponseContinue) {
            // -- If the user says no, just forget about it.
            return;
        }

        NxASmartPlaylist *newCrate = [NxASmartPlaylist crateWithName:@"New Smart Playlist"
                                                           predicate:self.p_smartPlaylistEditor.predicate.predicateFormat
                                                             context:context];

        NxACrate *parentCrate = self.selectedCrate;
        if (!parentCrate || !parentCrate.isAFolder || parentCrate.isASmartPlaylist || parentCrate.isASystemCrate) {
            // -- If we don't have anything selected or if the selection can't have children crates, use the root crate instead.
            parentCrate = [NxACrate rootCrateIn:context];
        }

        [parentCrate addCrate:newCrate modificationDate:[NSDate date]];

        [self refresh];

        [self expandItemWithSourceObject:parentCrate expandChildren:NO];
    }];
}

- (IBAction)p_createNewFilter:(id)sender
{
    NSManagedObjectContext *context = self.delegate.objectContext;
    NxAAssertNotNil(context);

    NxACrate *parentCrate = self.selectedCrate;
    if (!parentCrate || (!parentCrate.isAFilter && !parentCrate.isAPlaylist) || parentCrate.hasSortingCriterias) {
        return;
    }

    // -- We don't have a current predicate to edit so we just leave nil to start with.
    self.p_smartPlaylistEditor.predicate = nil;

    [self.p_smartPlaylistEditor beginAsSheetForWindow:self.p_mainWindow completionHandler:^(NSModalResponse returnCode) {
        if (returnCode != NSModalResponseContinue) {
            // -- If the user says no, just forget about it.
            return;
        }

        NxAFilterPlaylist *newCrate = [NxAFilterPlaylist crateWithName:@"New Filter"
                                                             predicate:self.p_smartPlaylistEditor.predicate.predicateFormat
                                                               context:context];
        [parentCrate addCrate:newCrate modificationDate:[NSDate date]];

        [self refresh];

        [self expandItemWithSourceObject:parentCrate expandChildren:NO];
    }];
}

- (IBAction)p_cloneCrate:(id)sender
{
    NxACrate *crate = self.selectedCrate;
    if (!crate) {
        return;
    }

    // -- We create a clone of the crate.
    NxACrate *newCrate = [NxACrateClone crateAsACloneOf:crate];
    NSDate *now = [NSDate date];

    if (crate.isASystemCrate) {
        // -- If we're cloning a system crate, we just add the clone to the root crate.
        NxACrate *rootCrate = [NxACrate rootCrateIn:crate.managedObjectContext];
        NxAAssertNotNil(rootCrate);

        [rootCrate addCrate:newCrate modificationDate:now];
    }
    else {
        // -- Otherwise, we add it right after the crate we've just cloned.
        NxACrate *parentCrate = crate.parentCrate;

        NSInteger index = [parentCrate indexOfSubCrate:crate];
        [parentCrate addCrate:newCrate modificationDate:now];
        [parentCrate moveCrates:@[ newCrate ] toPosition:index + 1];
    }

    [self refresh];
}

- (void)refresh
{
    NxACrateListView *crateListView = (NxACrateListView *)self.view;

    // -- We want to make sure UI stuff runs on the main thread.
    [NSApplication runBlockOnMainThreadAndWaitUntilDone:^() {
        NSMutableArray *selectedObjects = nil;
        NSIndexSet *selection = crateListView.selectedRowIndexes;
        if (selection.count) {
            selectedObjects = [NSMutableArray arrayWithCapacity:selection.count];

            [selection enumerateIndexesUsingBlock:^(NSUInteger idx, BOOL *stop) {
                id selectedItem = [crateListView itemAtRow:idx];
                [selectedObjects addObject:selectedItem];
            }];
        }

        for (NxACrateListViewItem *item in self.items) {
            [item clearCache];
        }

        [crateListView reloadData];

        NSMutableIndexSet *newIndices = [NSMutableIndexSet indexSet];
        for (id object in selectedObjects) {
            NSUInteger index = [crateListView rowForItem:object];

            if (index == NSNotFound) {
                continue;
            }

            [newIndices addIndex:index];
        }

        if (newIndices.count) {
            [crateListView selectRowIndexes:newIndices byExtendingSelection:NO];
        }
    }];

    [self p_tellDelegateToRefresh];
}

- (void)expandCrateListViewItemsAndSelectAllTracks
{
    NxACrateListView *crateListView = (NxACrateListView *)self.view;

    // -- Expand the group items.
    for (NSInteger row = 0; row < crateListView.numberOfRows; ++row) {
        NxACrateListViewItem *item = [crateListView itemAtRow:row];
        if (item.shouldBeExpandedAtStart) {
            [crateListView expandItem:item];
        }
    }

    // -- Select the first item of the first group, which is the 'All Tracks' playlist.
    [crateListView selectRowIndexes:[NSIndexSet indexSetWithIndex:1] byExtendingSelection:NO];
}

- (void)expandItemWithSourceObject:(id)object
                    expandChildren:(BOOL)expandChildren
{
    NxACrateListView *crateListView = (NxACrateListView *)self.view;

    for (NSInteger row = 0; row < crateListView.numberOfRows; ++row) {
        NxACrateListViewItem *item = [crateListView itemAtRow:row];
        if (item.sourceObject == object) {
            [crateListView expandItem:item expandChildren:expandChildren];
        }
    }
}

#pragma mark NxACrateListViewDelegate protocol

- (NSMenu *)contextualMenuForCrate:(NxACrate *)crate
{
    if (crate.isAPlaylist && crate.isASystemCrate) {
        return nil;
    }

    NSMenu *newMenu = [[NSMenu alloc] initWithTitle:@"Crate List Context Menu"];

    BOOL addSeparator = NO;

    if (crate.isAFolder) {
        NSMenuItem *menuItem = [newMenu addItemWithTitle:@"New Crate"
                                                  action:@selector(p_createNewCrate:)
                                           keyEquivalent:@""];
        [menuItem setTarget:self];
        [menuItem setRepresentedObject:crate];

        menuItem = [newMenu addItemWithTitle:@"New Smart Playlist"
                                      action:@selector(p_createNewSmartPlaylist:)
                               keyEquivalent:@""];
        [menuItem setTarget:self];
        [menuItem setRepresentedObject:crate];

        addSeparator = YES;
    }
    else {
        if (!crate.hasSortingCriterias) {
            NSMenuItem *menuItem = [newMenu addItemWithTitle:@"New Filter"
                                                      action:@selector(p_createNewFilter:)
                                               keyEquivalent:@""];
            [menuItem setTarget:self];
            [menuItem setRepresentedObject:crate];

            if (crate.containsFilters) {
                return newMenu;
            }

            addSeparator = YES;
        }

        BOOL addEditCommand = crate.isASmartPlaylist && ![crate isKindOfClass:[NxAOtherTracksPlaylist class]];
        if (addEditCommand) {
            NSString *title = crate.isAFilter ? @"Edit Filter" : @"Edit Smart Playlist";
            NSMenuItem *menuItem = [newMenu addItemWithTitle:title
                                                      action:@selector(p_editSmartPlaylist:)
                                               keyEquivalent:@""];
            [menuItem setTarget:self];
            [menuItem setRepresentedObject:crate];
            
            addSeparator = YES;
        }
    }

    if (crate.isASystemCrate) {
        return newMenu;
    }
    else {
        NSMenuItem *menuItem = [newMenu addItemWithTitle:@"Clone Crate"
                                                  action:@selector(p_cloneCrate:)
                                           keyEquivalent:@""];
        [menuItem setTarget:self];
        [menuItem setRepresentedObject:crate];
    }

    NSArray *tagsForSorting = crate.tagTypesForSorting;
    if (!tagsForSorting.count) {
        return newMenu;
    }

    if (addSeparator) {
        [newMenu addItem:[NSMenuItem separatorItem]];
    }

    [newMenu addItemWithTitle:@"Sort By:"
                       action:nil
                keyEquivalent:@""];
    
    NSSortDescriptor *sortDesc = [[NSSortDescriptor alloc] initWithKey:@"propertyDescription"
                                                             ascending:YES
                                                              selector:@selector(localizedStandardCompare:)];
    NSArray *sortedTags = [tagsForSorting sortedArrayUsingDescriptors:@[ sortDesc ]];
    for (NxATag *tag in sortedTags) {
        if (![crate hasSortingCriteriaForTag:tag]) {
            NSMenuItem *menuItem = [newMenu addItemWithTitle:tag.propertyDescription
                                                      action:@selector(p_addSortingCriteria:)
                                               keyEquivalent:@""];

            [menuItem setIndentationLevel:1];
            [menuItem setTarget:self];
            [menuItem setRepresentedObject:tag];
        }
    }

    return newMenu;
}

- (void)deleteKeyPressedWithSelection
{
    if (self.selectedSortingBucket) {
        // -- Make sure the user really wants to do this.
        NSString *alertMsg = [NSString stringWithFormat:@"Are you sure you want to delete the sorting criteria '%@'?", self.selectedSortingBucket.criteriaDescription];
        NSAlert *alert = [NSAlert alertWithMessageText:alertMsg
                                         defaultButton:@"Ok"
                                       alternateButton:@"Cancel"
                                           otherButton:nil
                             informativeTextWithFormat:@"This cannot be undone."];

        NxAAssertNotNil(self.p_mainWindow);
        [alert beginSheetModalForWindow:self.p_mainWindow
                      completionHandler:^(NSInteger returnCode)
         {
             if (returnCode == NSOKButton) {
                 [self.selectedSortingBucket destroy];

                 [self refresh];
             }
         }];

        return;
    }

    NxACrate *selectedCrate = self.selectedCrate;
    if (!selectedCrate || selectedCrate.isASystemCrate || [selectedCrate isKindOfClass:[NxAOtherTracksPlaylist class]]) {
        return;
    }

    NSString *crateType = selectedCrate.isAFilter ? @"filter" : @"crate";

    // -- Make sure the user really wants to do this.
    NSString *alertMsg = [NSString stringWithFormat:@"Are you sure you want to delete the %@ '%@'?", crateType, selectedCrate.stringValue];
    NSAlert *alert = [NSAlert alertWithMessageText:alertMsg
                                     defaultButton:@"Ok"
                                   alternateButton:@"Cancel"
                                       otherButton:nil
                         informativeTextWithFormat:@"This cannot be undone."];

    NxAAssertNotNil(self.p_mainWindow);
    [alert beginSheetModalForWindow:self.p_mainWindow
                  completionHandler:^(NSInteger returnCode)
    {
        if (returnCode != NSOKButton) {
            // -- If the user says no, just forget about it.
            return;
        }

        NxACrate *parentCrate = selectedCrate.parentCrate;
        BOOL deletingAFilter = selectedCrate.isAFilter;

        [selectedCrate destroy];

        if (deletingAFilter) {
            NSArray *sibblings = parentCrate.crates;
            NxACrate *firstSibbling = sibblings.firstObject;
            if ((sibblings.count == 1) && [firstSibbling.name isEqualToString:NxAFilterPlaylistOtherTracksPlaylistName]) {
                [firstSibbling destroy];
            }
        }

        [self refresh];
    }];
}

- (void)upKeyPressedWithSelection
{
    NxACrateListView *crateListView = (NxACrateListView *)self.view;

    NSInteger selection = crateListView.selectedRow - 1;

    while (selection > 0) {
        NxACrateListViewItem *selectedItem = [crateListView itemAtRow:selection];

        if (selectedItem.isSelectable) {
            [crateListView selectRowIndexes:[NSIndexSet indexSetWithIndex:selection] byExtendingSelection:NO];
            return;
        }

        --selection;
    }
}

- (void)downKeyPressedWithSelection
{
    NxACrateListView *crateListView = (NxACrateListView *)self.view;

    NSInteger selection = crateListView.selectedRow + 1;

    while (selection < crateListView.numberOfRows) {
        NxACrateListViewItem *selectedItem = [crateListView itemAtRow:selection];

        if (selectedItem.isSelectable) {
            [crateListView selectRowIndexes:[NSIndexSet indexSetWithIndex:selection] byExtendingSelection:NO];
            return;
        }

        ++selection;
    }
}

#pragma mark Overridden NSViewController Methods

- (void)loadView
{
    [super loadView];

    NxACrateListView *crateListTopView = (NxACrateListView *)self.p_topScrollView;
    NSView *parentView = self.parentView;

    // -- We add our view to the parent view.
    [parentView addSubview:crateListTopView];

    // -- And set up constraints to hug the side of the superview.
    NSArray *crateListConstraints = @[ [NSLayoutConstraint constraintWithItem:parentView
                                                                    attribute:NSLayoutAttributeLeft
                                                                    relatedBy:NSLayoutRelationEqual
                                                                       toItem:crateListTopView
                                                                    attribute:NSLayoutAttributeLeft
                                                                   multiplier:1.0
                                                                     constant:0.0],
                                       [NSLayoutConstraint constraintWithItem:parentView
                                                                    attribute:NSLayoutAttributeRight
                                                                    relatedBy:NSLayoutRelationEqual
                                                                       toItem:crateListTopView
                                                                    attribute:NSLayoutAttributeRight
                                                                   multiplier:1.0
                                                                     constant:0.0],
                                       [NSLayoutConstraint constraintWithItem:parentView
                                                                    attribute:NSLayoutAttributeTop
                                                                    relatedBy:NSLayoutRelationEqual
                                                                       toItem:crateListTopView
                                                                    attribute:NSLayoutAttributeTop
                                                                   multiplier:1.0
                                                                     constant:0.0],
                                       [NSLayoutConstraint constraintWithItem:parentView
                                                                    attribute:NSLayoutAttributeBottom
                                                                    relatedBy:NSLayoutRelationEqual
                                                                       toItem:crateListTopView
                                                                    attribute:NSLayoutAttributeBottom
                                                                   multiplier:1.0
                                                                     constant:0.0]
                                       ];
    [parentView addConstraints:crateListConstraints];

    NxACrateListView *crateListView = (NxACrateListView *)self.view;

    [crateListView registerForDraggedTypes:@[ (NSString *)kUTTypeURL ]];
    [crateListView setDraggingSourceOperationMask:NSDragOperationMove
                                         forLocal:NO];
}

#pragma mark NSControlTextEditingDelegate Protocol

- (BOOL)control:(NSControl *)control
  isValidObject:(id)object
{
    NxAAssertTrue([control isKindOfClass:[NSTextField class]]);
    NxAAssertTrue([object isKindOfClass:[NSString class]]);

    return [NxACrate isAValidName:object];
}

- (BOOL)       control:(NSControl *)control
 didFailToFormatString:(NSString *)string
      errorDescription:(NSString *)error
{
    NSString *alertMsg = [NSString stringWithFormat:@"The name '%@' is reserved and cannot be used for crate names.", string];
    NSAlert *alert = [NSAlert alertWithMessageText:alertMsg
                                     defaultButton:@"Ok"
                                   alternateButton:nil
                                       otherButton:nil
                         informativeTextWithFormat:@"Please choose another name."];

    NxAAssertNotNil(self.p_mainWindow);
    [alert beginSheetModalForWindow:self.p_mainWindow completionHandler:nil];

    return NO;
}

#pragma mark NSOutlineViewDataSource protocol

- (id)outlineView:(NSOutlineView *)outlineView
            child:(NSInteger)index
           ofItem:(id)item
{
    if (!item) {
        return self.items[index];
    }
    else {
        NxACrateListViewItem *viewItem = item;
        NSArray *children = viewItem.childrenItems;
        return children[index];
    }
}

- (BOOL)outlineView:(NSOutlineView *)outlineView
   isItemExpandable:(id)item
{
    NxACrateListViewItem *viewItem = item;
    return viewItem.hasChildrenItems;
}

- (NSInteger)outlineView:(NSOutlineView *)outlineView
  numberOfChildrenOfItem:(id)item
{
    if (!item) {
        return self.items.count;
    }
    else {
        NxACrateListViewItem *viewItem = item;
        return viewItem.childrenItems.count;
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
    NSMutableArray *urisArray = [NSMutableArray array];

    for (NxACrateListViewItem *item in items) {
        id <NxACrateListViewItemSource> selectedObject = item.sourceObject;

        if([selectedObject respondsToSelector:@selector(addURIToArray:)]) {
            [selectedObject addURIToArray:urisArray];
        }
    }

    // -- If we have nothing on the pasteboard then we can stop right there.
    if (!urisArray.count) {
        return NO;
    }

    [pasteboard writeObjects:urisArray];

	return YES;
}

- (NSDragOperation)outlineView:(NSOutlineView *)outlineView
                  validateDrop:(id <NSDraggingInfo>)info
                  proposedItem:(id)item
            proposedChildIndex:(NSInteger)index
{
    if (![item respondsToSelector:@selector(validateDropForItems:index:)]) {
        return NSDragOperationNone;
    }

    NSManagedObjectContext *context = self.delegate.objectContext;
    if (!context) {
        // -- We don't support dropping anything if the context hasn't been set yet.
        return NSDragOperationNone;
    }

    // -- First check to see if we have tracks on the pasteboard.
    NSArray *items = [NxATrack tracksFromPasteboard:info.draggingPasteboard
                                            context:context];
    if (!items.count) {
        // -- If we didn't have tracks, check to see if we have crates on the pasteboard.
        items = [NxACrate cratesFromPasteboard:info.draggingPasteboard
                                       context:context];

        if (!items.count) {
            // -- If not, we have nothing to drop.
            return NSDragOperationNone;
        }
    }

    return [item validateDropForItems:items index:index];
}

- (BOOL)outlineView:(NSOutlineView *)outlineView
         acceptDrop:(id <NSDraggingInfo>)info
               item:(id)item
         childIndex:(NSInteger)index
{
    if (![item respondsToSelector:@selector(acceptDropForItems:index:)]) {
        return NO;
    }

    NSManagedObjectContext *context = self.delegate.objectContext;
    if (!context) {
        // -- We don't support dropping if the context hasn't been set yet.
        return NO;
    }

    // -- First check to see if we have tracks on the pasteboard.
    NSArray *items = [NxATrack tracksFromPasteboard:info.draggingPasteboard
                                            context:context];
    if (!items.count) {
        // -- If we didn't have tracks, check to see if we have crates on the pasteboard.
        items = [NxACrate cratesFromPasteboard:info.draggingPasteboard
                                       context:context];

        if (!items.count) {
            // -- If not, we have nothing to drop.
            return NO;
        }
    }

    if (![item acceptDropForItems:items index:index]) {
        return NO;
    }

    // -- We've modifed some stuff so let's refresh ourselves.
    [self refresh];

    NSInteger selectedCrateRow = [outlineView rowForItem:item];
    if (selectedCrateRow >= 0) {
        [outlineView expandItem:[outlineView itemAtRow:selectedCrateRow]];
    }

    return YES;
}

#pragma mark NSOutlineView delegate

- (BOOL)outlineView:(NSOutlineView *)outlineView
        isGroupItem:(id)item
{
    NxAAssertNotNil(item);
    NxAAssertTrue([item isKindOfClass:[NxACrateListViewItem class]]);

    NxACrateListViewItem *viewItem = item;
    return [viewItem.cellIdentifier isEqualToString:@"HeaderCell"];
}

- (BOOL)         outlineView:(NSOutlineView *)outlineView
 shouldShowOutlineCellForItem:(id)item
{
    NxAAssertNotNil(item);
    NxAAssertTrue([item isKindOfClass:[NxACrateListViewItem class]]);

    NxACrateListViewItem *viewItem = item;
    return [self outlineView:outlineView isGroupItem:viewItem] || viewItem.hasChildrenItems;
}

- (BOOL)outlineView:(NSOutlineView *)outlineView
   shouldSelectItem:(id)item
{
    NxAAssertNotNil(item);
    NxAAssertTrue([item isKindOfClass:[NxACrateListViewItem class]]);

    NxACrateListViewItem *viewItem = item;
    return viewItem.isSelectable;
}

- (NSView *)outlineView:(NSOutlineView *)outlineView
     viewForTableColumn:(NSTableColumn *)tableColumn
                   item:(id)item
{
    NxAAssertNotNil(item);
    NxAAssertTrue([item isKindOfClass:[NxACrateListViewItem class]]);

    NxACrateListViewItem *viewItem = item;
    NSTableCellView *cellView = [outlineView makeViewWithIdentifier:viewItem.cellIdentifier owner:self];

    return cellView;
}

- (void)outlineViewSelectionDidChange:(NSNotification *)notification
{
    [self p_tellDelegateToRefresh];
}

@end
