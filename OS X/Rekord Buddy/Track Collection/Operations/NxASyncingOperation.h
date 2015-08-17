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

@class NxAProgressViewController;
@class NxAMessageLog;

#import "NxACollectionOperation.h"

#pragma mark Public Interface

/*! Syncing operation for tracks and playlists. */
@interface NxASyncingOperation : NxACollectionOperation

#pragma mark Factory Methods

/*!
 * Create a syncing operation.
 * \param context Managed object context to sync content in.
 * \param parsingOperations Array of parsing operation used to create the data we're syncing.
 * \param progress View controller used to display progress.
 * \param log Message log for any errors and warnings.
 * \return YES if the parsing succeeded, NO otherwise.
 */
+ (instancetype)operationWithContext:(NSManagedObjectContext *)context
                   parsingOperations:(NSArray *)parsingOperations
                            progress:(NxAProgressViewController *)progress
                                 log:(NxAMessageLog *)log;

@end
