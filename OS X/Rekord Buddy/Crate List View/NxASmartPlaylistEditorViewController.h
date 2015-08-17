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

#pragma mark Public Interface

@interface NxASmartPlaylistEditorViewController : NSViewController

#pragma mark Properties

/*! Predicate displayed by the editor. */
@property (strong, nonatomic) NSPredicate *predicate;

#pragma mark Instance Methods

/*!
 * Open the smart playlist editor.
 * \param window Parent window for the sheet used for the editor.
 * \param handler Block used to handle completion of the editing process.
 */
- (void)beginAsSheetForWindow:(NSWindow *)window
            completionHandler:(void (^)(NSModalResponse returnCode))handler;

@end
