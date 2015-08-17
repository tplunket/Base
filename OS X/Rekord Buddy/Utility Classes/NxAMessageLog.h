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

@class NxAAlertViewController;

#import "NxAWarning.h"
#import "NxAWarningContextObject.h"
#import "NxAErrorCode.h"

#pragma mark Public Interface

@interface NxAMessageLog : NSObject

#pragma mark Properties

@property (readonly, nonatomic) BOOL hasWarnings;

@property (readonly, nonatomic) BOOL hasAnError;

@property (readonly, nonatomic) BOOL hasMessages;

@property (readonly, nonatomic) NSAttributedString *warningText;

@property (readonly, nonatomic) NSError *error;

@property (readonly, nonatomic) NSString *messagesText;

#pragma mark Instance Methods.

- (void)reset;

- (void)addMessage:(NSString *)message;

- (void)addWarning:(NxAWarning *)warning;

// -- Multiple warnings can be stored for the same warning description, as long as they relate to different object context.
// -- Description provided will be appended to "Found xx <object name>(s) " to generate the text presented to the user.
// -- so it should be written as, for example, "which have an invalid size.".
- (void)startWarningContextWith:(id <NxAWarningContextObject>)object;

- (void)addWarning:(NxAWarning *)warning
 inContextWithPerObjectWarningInfo:(NSString *)perObjectWarningInfo;

- (void)endWarningContext;

- (void)addError:(NSError *)error;

@end
