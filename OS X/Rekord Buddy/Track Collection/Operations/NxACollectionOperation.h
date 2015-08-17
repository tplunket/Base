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

#pragma mark Typedefs

/*!
 * Completion handler type used when the operation finishes.
 */
typedef void (^NxACollectionOperationCompletionHandler)(BOOL success);

#pragma mark Public Interface

/*! An asynchronous operation for collection operations. */
@interface NxACollectionOperation : NSOperation

#pragma mark Factory Methods

/*!
 * Create a collection operation.
 * \param context Managed object context to perform the operation for.
 * \param progress View controller used to display progress.
 * \param log Message log for any errors and warnings.
 * \return An operation object if succeeded, nil otherwise.
 */
+ (instancetype)operationWithContext:(NSManagedObjectContext *)context
                            progress:(NxAProgressViewController *)progress
                                 log:(NxAMessageLog *)log;

#pragma mark Properties

/*!
 * Context used for new content produced by the operation.
 * \details This is guaranteed not to be nil.
 */
@property (readonly, nonatomic) NSManagedObjectContext *context;

/*!
 * Progress controller used to update the user on the operation's status.
 * \details This is guaranteed not to be nil.
 */
@property (readonly, nonatomic) NxAProgressViewController *progress;

/*!
 * Log used to record errors and warnings.
 * \details This is guaranteed not to be nil.
 */
@property (readonly, nonatomic) NxAMessageLog *log;

/*!
 * Handler called when the operation completes.
 * \details Passes YES to the block if completion was succesful, NO otherwise.
 */
@property (copy, nonatomic) NxACollectionOperationCompletionHandler completionHandler;

@end
