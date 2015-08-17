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
#import "NxAAlertViewController.h"
#import "NSApplication+Utility.h"
#import "NSError+NxAUtility.h"
#import "NxAException.h"

#pragma mark Implementation

@implementation NxAException

#pragma mark Class Methods

+ (void)runBlockOnMainThreadAndExitOnExceptions:(void (^)(void))block
                            alertViewController:(NxAAlertViewController *)controller
{
    [NxAException runBlockAndExitOnExceptions:^() {
        [NSApplication runBlockOnMainThread:^() {
            block();
        }];
    }
                          alertViewController:controller];
}

+ (void)runBlockAndExitOnExceptions:(void (^)(void))block
                alertViewController:(NxAAlertViewController *)controller
{
    @try {
        block();
    }
    @catch(NSException *exception) {
#if DEBUG
        DLog(@"%@", exception.callStackSymbols);
        DLog(@"%@", exception.debugDescription);
        ALog(@"Exception thrown.");
#else
        NxAMessageLog *log = [[NxAMessageLog alloc] init];
        NSError *error = [NSError errorWithCode:NxAInternalErrorCode
                                    description:@"Something wonderful has happened. You found a critical bug."
                             recoverySuggestion:@"The app will now quit to preserve your data."
                                 additionalInfo:[NSString stringWithFormat:@"Please report the following error to support:\n\n%@",
                                                 exception.debugDescription]];
        [log addError:error];

        [controller showLog:log
            withDescription:@""
          andWhenFinishedDo:^(NSModalResponse returnCode) {
                        exit(0);
                    }];
#endif
    }
}

@end
