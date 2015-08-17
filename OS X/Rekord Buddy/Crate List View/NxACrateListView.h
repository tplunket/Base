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

#import "NxACrateListViewDelegate.h"

#pragma mark Public Interface

/*! A custom outline view to display lists of crates. */
@interface NxACrateListView : NSOutlineView

#pragma mark Properties

/*! Controller for the view. */
@property (weak, nonatomic) IBOutlet id <NxACrateListViewDelegate> extendedDelegate;

@end
