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

#import "NxADebuggerLauncher.h"

#pragma mark Implementation

@implementation NxADebuggerLauncher

#pragma mark Class Methods

/*
 * Launch the debugger.
 * \details This method triggers a shared user breakpoint to stop execution in the debugger.
 */
+ (BOOL)launch
{
    // -- This is placeholder code which is only used to trigger a shared user breakpoint
    BOOL condition = YES;
    if (condition) {
        // -- A shared user breakpoint should be present at the line below for this class to work as advertised.
        condition = NO;
    }
    
    return condition;
}

@end
