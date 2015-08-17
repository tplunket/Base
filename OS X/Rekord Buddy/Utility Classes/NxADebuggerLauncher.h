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

#pragma mark Constants & Defines

/*!
 * def Macro used to stop the app in the debugger if a certain condition is met, and only in DEBUG builds.
 * \details This makes it easy to inspect the call stack, local stack, etc...
 * \param condition A boolean value which, if YES, will launch the debugger.
 */
#if DEBUG
#define NxALaunchDebuggerIf(condition) do { if (condition) { DLog(@"Entering the Debugger."); [NxADebuggerLauncher launch]; }} while(0)
#else
#define NxALaunchDebuggerIf(condition, ...) do { } while (0)
#endif

#pragma mark - Public Interface

/*! Utility class to hide a breakpoint the debugger can break into if needed. */
@interface NxADebuggerLauncher : NSObject

#pragma mark Class Methods

/*!
 * Launch the debugger
 * \details This method constainis a shared user breakpoint which will halt execution when it is called.
 */
+ (BOOL)launch;

@end
