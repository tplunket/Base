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

#import "NxAProgressViewController.h"
#import "NxAMessageLog.h"
#import "NxACollectionOperation.h"

#pragma mark Private Interface

@interface NxACollectionOperation ()

#pragma mark Properties

@property (strong, nonatomic) NSManagedObjectContext *context;

@property (strong, nonatomic) NxAProgressViewController *progress;

@property (strong, nonatomic) NxAMessageLog *log;

@end

#pragma mark Implementation

@implementation NxACollectionOperation

#pragma mark Factory Methods

+ (instancetype)operationWithContext:(NSManagedObjectContext *)context
                            progress:(NxAProgressViewController *)progress
                                 log:(NxAMessageLog *)log
{
    NxAAssertNotNil(context);
    NxAAssertNotNil(progress);
    NxAAssertNotNil(log);

    NxACollectionOperation *operation = [[[self class] alloc] initPrivate];

    operation.context = context;
    operation.progress = progress;
    operation.log = log;

    return operation;
}

#pragma mark Instance Methods

/*!
 * Private init method.
 * \details This is only called by out approved factory methods, nothing else.
 * \return An initialized instance of the class.
 */
- (instancetype)initPrivate
{
    return [super init];
}

- (instancetype)init
{
    // -- Nothing should ever call this, we only create this type of objects via our factory methods.
    ALog(@"Invalid method call.");
    return nil;
}

@end
