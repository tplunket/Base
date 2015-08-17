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

#import "NxAPreferencesViewControllerDelegate.h"
#import "NxATrackListViewControllerDelegate.h"
#import "NxACrateListViewControllerDelegate.h"

#pragma mark Public Interface

/*! Main delegate class for the entire application. */
@interface NxAAppDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate, NxAPreferencesViewControllerDelegate, NxATrackListViewControllerDelegate, NxACrateListViewControllerDelegate>

#pragma mark Properties

@property (readonly, nonatomic) NSString *appVersionAsString;

#pragma mark Instance Methods

- (void)displayAlert:(NSAlert *)alert;

- (void)displayAlert:(NSAlert *)alert
 onceCompletedThenRun:(void (^)(NSModalResponse returnCode))handler;

@end
