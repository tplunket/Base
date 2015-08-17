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

@interface NSApplication (Utility)

#pragma mark Instance Methods.

/*!
 * Run a block on the main thread.
 * \param block Block to run.
 */
+ (void)runBlockOnMainThread:(void (^)(void))block;

/*!
 * Run a block on the main thread.
 * \details If run on a seperate thread, this waits for the block to complete.
 * \param block Block to run.
 */
+ (void)runBlockOnMainThreadAndWaitUntilDone:(void (^)(void))block;

@end
