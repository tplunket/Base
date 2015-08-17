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

#import "NxATrack.h"
#import "NxASyncingOperation.h"
#import "Track Collection/Operations/NxAParsingOperation.h"
#import "NxAProgressViewController.h"
#import "NxAMessageLog.h"
#import "Track Collection/Crates/NxACrate.h"

#pragma mark Private Interface

@interface NxASyncingOperation ()

#pragma mark Properties

/*! Array of parsing operations used to create the data we're syncing. */
@property (strong, nonatomic) NSArray *p_parsingOperations;

@end

#pragma mark - Implementation

@implementation NxASyncingOperation

#pragma mark Factory Methods

+ (instancetype)operationWithContext:(NSManagedObjectContext *)context
                   parsingOperations:(NSArray *)parsingOperations
                            progress:(NxAProgressViewController *)progress
                                 log:(NxAMessageLog *)log
{
    NxASyncingOperation *newOperation = [super operationWithContext:context
                                                        progress:progress
                                                             log:log];
    NxAAssertNotNil(newOperation);

    NSMutableArray *operationsWhichAreNotForSyncing = [NSMutableArray arrayWithCapacity:parsingOperations.count];
    
    for (NxAParsingOperation *operation in parsingOperations) {
        if ([operation isKindOfClass:[NxASyncingOperation class]]) {
            continue;
        }
        
        [operationsWhichAreNotForSyncing addObject:operation];
    }
    
    newOperation.p_parsingOperations = operationsWhichAreNotForSyncing;
        
    return newOperation;
}

#pragma mark Overridden NxACollectionOperation Class Methods

+ (instancetype)operationWithContext:(NSManagedObjectContext *)context
                            progress:(NxAProgressViewController *)progress
                                 log:(NxAMessageLog *)log
{
    // -- This should never be called.
    ALog(@"Invalid method call.");
    return nil;
}

#pragma mark Instance Methods

- (instancetype)init
{
    ALog(@"We only create this type of objects via our factory methods.");
    return nil;
}

#pragma mark Overridden NSOperation methods

- (void)main
{
    NSManagedObjectContext *context = self.context;

    __block BOOL returnCode = NO;

    [context performBlockAndWait:^() {
        @autoreleasepool {
            NSArray *operations = self.p_parsingOperations;

            NxAProgressViewController *progress = self.progress;
            progress.status = @"Syncing Collections.";
            progress.min = 0;
            progress.max = (operations.count * 2) + 1;
            progress.value = 0;

            // -- If any tracks are missing 'Date Added' tags, we add them now.
            NSDate *now = [NSDate date];

            for (NxAParsingOperation *operation in operations) {
                progress.value += 1;

                [operation setAnyMissingDateAddedOnUpdatedTracksTo:now];
            }

            // -- For any objects that need updating, tell the parsing operation to update them.
            for (NxAParsingOperation *operation in operations) {
                progress.value += 1;

                [operation updateCollection];

                if (self.log.hasAnError) {
                    return;
                }
            }

            // -- Make sure we don't have any tag entries left dangling after syncing.
            [NxATagEntry destroyOrphansIn:context andAddAnyMessagesToLog:self.log];

            returnCode = YES;
        }
    }];

    if (self.completionHandler) {
        self.completionHandler(returnCode);
    }
}

@end
