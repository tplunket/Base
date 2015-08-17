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

@class NxATrackSortingBucket;

#import "Track Collection/Crates/NxACrate.h"
#import "NxACrateListViewDelegate.h"
#import "NxACrateListViewControllerDelegate.h"

#pragma mark Public Interface

@interface NxACrateListViewController : NSViewController <NxACrateListViewDelegate>

#pragma mark Properties

/*! Parent where the crate list should be displayed. */
@property (weak, nonatomic) IBOutlet NSView *parentView;

/*! Delegate object. */
@property (weak, nonatomic) IBOutlet id <NxACrateListViewControllerDelegate> delegate;

/*! The currently selected crate or nil if no selection */
@property (readonly, weak, nonatomic) NxACrate *selectedCrate;

@property (readonly, weak, nonatomic) NxATrackSortingBucket *selectedSortingBucket;

/*! List of NxACrateListViewItem to display in the view. */
@property (strong, nonatomic) NSArray *items;

#pragma mark Instance Methods

- (void)refresh;

- (void)expandCrateListViewItemsAndSelectAllTracks;

- (void)expandItemWithSourceObject:(id)object
                    expandChildren:(BOOL)expandChildren;

@end
