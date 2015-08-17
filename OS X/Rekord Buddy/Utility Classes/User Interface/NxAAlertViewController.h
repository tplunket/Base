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

@class NxAWarningList;
@class NxAMessageLog;

#pragma mark Public Interface

@interface NxAAlertViewController : NSViewController <NSAnimationDelegate>

#pragma mark Properties

@property (strong, nonatomic) IBOutlet NSWindow *parentWindow;

@property (strong, nonatomic) NSString *okButtonLabel;

@property (strong, nonatomic) NSString *cancelButtonLabel;

#pragma mark Instance Methods

- (void)showLog:(NxAMessageLog *)log
 withDescription:(NSString *)description
 andWhenFinishedDo:(void (^)(NSModalResponse returnCode))completionHandler;

- (void)showLog:(NxAMessageLog *)log
 withDescription:(NSString *)description;

@end
