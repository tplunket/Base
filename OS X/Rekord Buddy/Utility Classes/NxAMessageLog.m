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

#import "NxAMessageLog.h"
#import "NxAWarningList.h"
#import "NSError+NxAUtility.h"
#import "NSApplication+Utility.h"
#import "NxAAlertViewController.h"

#pragma mark Private Interface

@interface NxAMessageLog ()

#pragma mark Properties

@property (strong, nonatomic) NSMutableArray *p_messages;

@property (strong, nonatomic) NxAWarningList *p_warnings;

@property (strong) id <NxAWarningContextObject> p_warningContextObject;

@property (strong) NSError *p_error;

@end

@implementation NxAMessageLog

#pragma mark Accessors

- (NSMutableArray *)p_messages
{
    if (!_p_messages) {
        _p_messages = [NSMutableArray array];
    }

    return _p_messages;
}

- (NxAWarningList *)p_warnings
{
    if (!_p_warnings) {
        _p_warnings = [[NxAWarningList alloc] init];
    }

    return _p_warnings;
}

- (BOOL)hasWarnings
{
    return self.p_warnings.count != 0;
}

- (BOOL)hasAnError
{
    return self.p_error != nil;
}

- (BOOL)hasMessages
{
    return self.p_messages.count != 0;
}

- (NSAttributedString *)warningText
{
    NxAAssertTrue(self.hasWarnings);

    return self.p_warnings.warningText;
}

- (NSError *)error
{
    NxAAssertNotNil(self.p_error);

    return self.p_error;
}

- (NSString *)messagesText
{
    return [self.p_messages componentsJoinedByString:@"\n"];
}

#pragma mark Instance Methods.

- (void)reset
{
    [self.p_warnings reset];
    
    self.p_warnings = nil;
    self.p_error = nil;
}

- (void)addMessage:(NSString *)message
{
    NxAAssertNotNil(message);

    [self.p_messages addObject:message];
}

- (void)addWarning:(NxAWarning *)warning
{
    NxAAssertNotNil(warning);

    [self.p_warnings addWarning:warning];
}

- (void)startWarningContextWith:(id<NxAWarningContextObject>)object
{
    NxAAssertNotNil(object);

    self.p_warningContextObject = object;
}

- (void)addWarning:(NxAWarning *)warning
 inContextWithPerObjectWarningInfo:(NSString *)perObjectWarningInfo
{
    NxAAssertNotNil(warning);
    NxAAssertNotNil(perObjectWarningInfo);

    [self.p_warnings addWarning:warning
                      forObject:self.p_warningContextObject
                  perObjectInfo:perObjectWarningInfo];
}

- (void)endWarningContext
{
    self.p_warningContextObject = nil;
}

- (void)addError:(NSError *)error
{
    NxAAssertNotNil(error);
    NxAAssertNil(self.p_error);

    self.p_error = error;
}

@end
